
package com.example.handshack

import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.hardware.usb.UsbManager
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.handshack.ui.theme.HandshackTheme
import com.hoho.android.usbserial.driver.UsbSerialPort
import com.hoho.android.usbserial.driver.UsbSerialProber
import com.hoho.android.usbserial.util.SerialInputOutputManager
import kotlinx.coroutines.*
import kotlinx.coroutines.channels.awaitClose
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
                    MainScreen()
                }
            }
        }
    }
}

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

//
// Implementation of a Bucket class
//
data class Bucket(val name: String)

//
// Implementation of the MC class
//
object Mc {

    //
    // Implementation of a BucketDao class
    //
    class BucketDao {

        private val events = MutableSharedFlow<List<Bucket>>(replay = 0)

        init {

            // Collect incoming events from the MC
            val scope = CoroutineScope(Dispatchers.IO)
            scope.launch {
                Mc.events.collect { event ->
                    when(event) {
                        is Event.OnNewData -> {

                            // Signal the Flow that the list is available
                            val names = event.data.split(",")
                            val list = emptyList<Bucket>().toMutableList()
                            for (name in names) list.add(Bucket(name = name))
                            events.emit(list)
                        }
                        is Event.OnRunError -> Unit
                    }
                }
            }
        }

        fun listAll() : Flow<List<Bucket>> {
            sendMsg("<BucketDao::listAll>")
            return flow {
                events.collect { list ->
                    emit(list)
                }
            }
        }
    }

    sealed class Event {
        data class OnNewData(val data: String) : Event()
        data class OnRunError(val error: Exception?) : Event()
    }

    private lateinit var usbApi: UsbApi
    private val events: Flow<Event> by lazy {
        callbackFlow {
            val callback = object : SerialInputOutputManager.Listener {
                override fun onNewData(data: ByteArray?) {
                    trySend(Event.OnNewData(String(data!!)))
                }
                override fun onRunError(e: Exception?) {
                    trySend(Event.OnRunError(e))
                }
            }
            usbApi.register(callback)
            awaitClose {
                usbApi.unregister()
            }
        }
    }

    val bucketDao = BucketDao()

    fun connect(context: Context) {
        usbApi = UsbApi()
        usbApi.connect(context)
    }
    fun sendMsg(msg: String) {
        usbApi.write(msg)
    }

}

//
// Implementation of the BucketRepository class
//
class BucketRepository{

    private val dao: Mc.BucketDao = Mc.bucketDao

    fun listAll() = dao.listAll()
}

//
// Implementation of the MainViewModel class
//
class MainViewModel : ViewModel() {

    private val repo = BucketRepository()

    var msg by mutableStateOf("<BucketDao::listAll>")
        private set

    var listFlow = repo.listAll()
        private set

    fun onMsgChange(value: String) {
        msg = value
    }
    fun onSendMsgClick() {
        Mc.sendMsg(msg)
    }
}

//
// Implementation of the MainScreen composable
//
@Composable
fun MainScreen() {

    val viewModel = viewModel<MainViewModel>()
    val list = viewModel.listFlow.collectAsState(initial = emptyList())

    Column(
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally,
        modifier = Modifier
            .fillMaxSize()
            .padding(all = 16.dp)
    ) {

        // Show a text field and a button the send commands
        OutlinedTextField(
            value = viewModel.msg,
            onValueChange = viewModel::onMsgChange,
            label = {
                Text(text = "MC message")
            },
            modifier = Modifier.fillMaxWidth()
        )
        Spacer(modifier = Modifier.height(16.dp))
        Button(onClick = viewModel::onSendMsgClick) {
            Text(text = "Send Msg")
        }

        // Show the content if the list
        Spacer(modifier = Modifier.height(16.dp))
        LazyColumn(modifier = Modifier
            .weight(1f)
            .fillMaxWidth()
        ) {
            items(items = list.value) { item ->
                Text(
                    modifier = Modifier.fillMaxWidth(),
                    text = item.name
                )
            }
        }
    }
}
