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
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.handshack.ui.theme.HandshackTheme
import com.hoho.android.usbserial.driver.UsbSerialPort
import com.hoho.android.usbserial.driver.UsbSerialProber
import com.hoho.android.usbserial.util.SerialInputOutputManager
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.flow.*
import kotlinx.coroutines.launch
import kotlin.math.exp

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
data class Bucket(val name: String)

// ================================================================
// Implementation of the MC class
// ================================================================
object Mc : SerialInputOutputManager.Listener {

    private lateinit var usbApi: UsbApi
    private val events = MutableSharedFlow<List<Bucket>>(replay = 0)

    var buffer: String = ""

    fun connect(context: Context) {
        usbApi = UsbApi()
        usbApi.connect(context)
        usbApi.register(this)
    }

    fun listBuckets(): Flow<List<Bucket>> {
        sendMsg("{listBuckets()}")
        return flow {
            events.collect { list ->
                emit(list)
            }
        }
    }

    private fun sendMsg(msg: String) {
        usbApi.write(msg)
    }

    override fun onNewData(data: ByteArray?) {
        data?.let {
            buffer += String(it)
            val startCurly = buffer.indexOf('{')
            val endCurly = buffer.indexOf('}', startCurly + 1)
            if (endCurly != -1) {
                val expr = buffer.subSequence(startCurly + 1, endCurly)
                val leftParenthesis = expr.indexOf('(')
                val rightParenthesis = expr.indexOf(')', leftParenthesis + 1)
                val cmd = expr.subSequence(0, leftParenthesis)
                val params = expr.subSequence(leftParenthesis + 1, rightParenthesis) as String
                when(cmd) {
                    "onBucketList" -> onBucketList(params)
                }
                buffer = buffer.subSequence(endCurly, buffer.lastIndex) as String
            }
        }
    }

    override fun onRunError(e: java.lang.Exception?) {
        Log.d(TAG, "onRunError(e=$e)")
    }

    private fun onBucketList(params: String) {
        val leftBracket = params.indexOf('[')
        val rightBracket = params.indexOf(']')
        val names = params.subSequence(leftBracket + 1, rightBracket).split(',')
        val list = emptyList<Bucket>().toMutableList()
        for (name in names) list.add(Bucket(name = name))
        val scope = CoroutineScope(Dispatchers.IO)
        scope.launch {
            events.emit(list)
        }
    }
}

// ================================================================
// Implementation of the BucketRepository class
// ================================================================
class BucketRepository {
    fun listAll() = Mc.listBuckets()
}

// ================================================================
// Implementation of the MainViewModel class
// ================================================================
class MainViewModel : ViewModel() {
    private val repo = BucketRepository()
    var listFlow = repo.listAll()
        private set
}

// ================================================================
// Implementation of the MainScreen composable
// ================================================================
@Composable
fun MainScreen() {
    val viewModel = viewModel<MainViewModel>()
    val list = viewModel.listFlow.collectAsState(initial = emptyList())
    LazyColumn(
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally,
        modifier = Modifier
            .padding(all = 16.dp)
            .fillMaxSize()
    ) {
        items(items = list.value) { item ->
            Text(
                modifier = Modifier.fillMaxWidth(),
                text = item.name
            )
        }
    }
}
