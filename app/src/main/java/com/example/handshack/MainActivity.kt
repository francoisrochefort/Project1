package com.example.handshack

import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.hardware.usb.UsbManager
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.compose.ui.window.Dialog
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.handshack.ui.theme.HandshackTheme
import com.hoho.android.usbserial.driver.UsbSerialPort
import com.hoho.android.usbserial.driver.UsbSerialProber
import com.hoho.android.usbserial.util.SerialInputOutputManager
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.Channel
import kotlinx.coroutines.flow.*

private const val TAG = "there is no spoon"

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        // Connect to the MC
        Mc.connect(applicationContext)

        setContent {
            HandshackTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colors.background
                ) {
                    // Show the main screen
                    BucketsScreen()
                }
            }
        }
    }
}

// ================================================================
// ViewModel events
// ================================================================
sealed class ViewModelEvents {
    object OnDismiss : ViewModelEvents()
}

// ================================================================
// Implementation of an USB class
// ================================================================
class UsbApi {

    class NoUsbDriverFound : Exception()

    companion object {
        const val REQUEST_PERMISSION_TIMEOUT = 5000L
        const val WRITE_TIMEOUT = 100
        const val BAUD_RATE = 115_200
    }

    private lateinit var usbSerialPort: UsbSerialPort
    private lateinit var serialInputOutputManager: SerialInputOutputManager

    fun connect(context: Context) {
        val manager: UsbManager = context.getSystemService(Context.USB_SERVICE) as UsbManager
        val availableDrivers = UsbSerialProber.getDefaultProber().findAllDrivers(manager)
        if (availableDrivers.isEmpty())
            throw NoUsbDriverFound()
        val driver = availableDrivers.first()
        while (!manager.hasPermission(driver.device)) {
            manager.requestPermission(
                driver.device,
                PendingIntent.getBroadcast(
                    context,
                    0,
                    Intent("com.example.handshake.USB_PERMISSION"),
                    PendingIntent.FLAG_IMMUTABLE
                )
            )
            Thread.sleep(REQUEST_PERMISSION_TIMEOUT)
        }
        val connection = manager.openDevice(driver.device)
        usbSerialPort = driver.ports.first()
        usbSerialPort.open(connection)
        usbSerialPort.setParameters(
            BAUD_RATE,
            8,
            UsbSerialPort.STOPBITS_1,
            UsbSerialPort.PARITY_NONE
        )
    }

    fun register(callback: SerialInputOutputManager.Listener) {
        serialInputOutputManager = SerialInputOutputManager(usbSerialPort, callback)
        serialInputOutputManager.start()
    }

    fun unregister() {
        serialInputOutputManager.stop()
    }

    fun write(message: String) {
        usbSerialPort.write(message.toByteArray(), WRITE_TIMEOUT)
    }
}

// ================================================================
// Implementation of a Bucket class
// ================================================================
data class Bucket(var name: String)

// ================================================================
// Implementation of the MC class
// ================================================================
object Mc : SerialInputOutputManager.Listener {

    sealed class Events {
        data class OnBucketList(val buckets: List<Bucket>) : Events()
        data class OnNewBucket(val bucketId: Int) : Events()
    }

    private lateinit var usbApi: UsbApi
    private val events = MutableSharedFlow<Events>(replay = 0)

    private var buffer: String = ""

    fun connect(context: Context) {
        usbApi = UsbApi()
        usbApi.connect(context)
        usbApi.register(this)
    }

    fun listBuckets(): Flow<List<Bucket>> {

        // Query the list from the MC
        sendMsg("{listBuckets()}")

        return flow {

            // Wait for onBucketList to broadcast the list
            events.collect { event ->
                when(event) {
                    is Events.OnBucketList -> emit(event.buckets)
                    else -> Unit
                }
            }
        }
    }

    fun add(bucket: Bucket) {

        // Send the command to the MC
        sendMsg("{addBucket(\"${bucket.name}\")}")
    }

    private fun sendMsg(msg: String) {
        usbApi.write(msg)
    }

    override fun onNewData(data: ByteArray?) {
        data?.let {

            // Append the new data to the buffer
            buffer += String(it)

            // Check if the sent event is complete
            val leftCurly = buffer.indexOf('{')
            val rightCurly = buffer.indexOf('}', leftCurly + 1)
            if (rightCurly != -1) {
                val expr = buffer.subSequence(leftCurly + 1, rightCurly)

                // Parse the event parameters
                val leftParenthesis = expr.indexOf('(')
                val rightParenthesis = expr.indexOf(')', leftParenthesis + 1)
                val params = expr.subSequence(leftParenthesis + 1, rightParenthesis) as String

                // Parse the event name
                when (expr.subSequence(0, leftParenthesis)) {
                    "onBucketList" -> onBucketList(params)
                }

                // Remove the parsed text from the buffer
                buffer = buffer.subSequence(rightCurly, buffer.lastIndex) as String
            }
        }
    }

    override fun onRunError(e: Exception?) {
        Log.d(TAG, "onRunError(e=$e)")
    }

    private fun onBucketList(params: String) {

        // Parse the name array parameter
        val leftBracket = params.indexOf('[')
        val rightBracket = params.indexOf(']')
        val names = params.subSequence(leftBracket + 1, rightBracket).split(',')

        // Convert the name array to a bucket list
        val list = emptyList<Bucket>().toMutableList()
        for (name in names) list.add(Bucket(name = name))

        // Broadcast the list to subscribers
        val scope = CoroutineScope(Dispatchers.IO)
        scope.launch {
            events.emit(Events.OnBucketList(list))
        }
    }
}

// ================================================================
// Implementation of the BucketRepository class
// ================================================================
class BucketsRepository {
    fun listAll() = Mc.listBuckets()
    fun add(bucket: Bucket) = Mc.add(bucket = bucket)
}

// ================================================================
// Implementation of the BucketsViewModel class
// ================================================================
class BucketsViewModel : ViewModel() {
    private val repo = BucketsRepository()

    private val _events = Channel<ViewModelEvents>()
    val events = _events.receiveAsFlow()

    var buckets = repo.listAll()
        private set

    var name by mutableStateOf("")
        private set
    fun onNameChange(name: String) {
        this.name = name
    }
    fun onSubmitClick() {
        viewModelScope.launch(Dispatchers.IO) {
            repo.add(Bucket(name = name))
            _events.send(ViewModelEvents.OnDismiss)
        }
    }
}

// ================================================================
// Implementation of the BucketsScreen composable
// ================================================================
@Composable
fun BucketsScreen() {

    // Local variables
    val viewModel = viewModel<BucketsViewModel>()
    val buckets by viewModel.buckets.collectAsState(initial = emptyList())
    var showDialog by rememberSaveable { mutableStateOf(false) }

    // Collect events from the ViewModel
    LaunchedEffect(key1 = true) {
        viewModel.events.collect { event ->
            when (event) {
                is ViewModelEvents.OnDismiss -> showDialog = false
            }
        }
    }

    // Composes the content of the screen
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(all = 16.dp)
    ) {
        // Bucket list
        LazyColumn(
            modifier = Modifier.weight(1f)
        ) {
            items(items = buckets) { bucket ->
                Text(
                    modifier = Modifier.fillMaxWidth(),
                    text = bucket.name
                )
            }
        }
        Spacer(modifier = Modifier.height(16.dp))

        // Compose the command bar
        Row(
            modifier = Modifier.fillMaxWidth(),
            horizontalArrangement = Arrangement.Center
        ) {
            Button(
                onClick = { showDialog = true }
            ) {
                Text(text = "Add..")
            }
        }

        // Compose the bucket dialog
        if (showDialog) {
            Dialog(
                onDismissRequest = { showDialog = false }
            ) {
                Column(
                    verticalArrangement = Arrangement.Center,
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {

                    // Name field
                    OutlinedTextField(
                        value = viewModel.name,
                        onValueChange = viewModel::onNameChange,
                        label = {
                            Text(text = "Name")
                        }
                    )
                    Spacer(modifier = Modifier.height(16.dp))

                    // Submit button
                    Button(
                        onClick = viewModel::onSubmitClick
                    ) {
                        Text(text = "Submit")
                    }
                }
            }
        }
    }
}
