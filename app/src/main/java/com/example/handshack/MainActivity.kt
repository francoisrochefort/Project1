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
import androidx.compose.material.*
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import androidx.lifecycle.viewmodel.compose.viewModel
import com.example.handshack.ui.theme.HandshackTheme
import com.hoho.android.usbserial.driver.UsbSerialPort
import com.hoho.android.usbserial.driver.UsbSerialProber
import com.hoho.android.usbserial.util.SerialInputOutputManager
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.launch

private const val TAG = "there is no spoon"

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

object Mc {

    sealed class Event {
        data class OnNewData(val data: String) : Event()
        data class OnRunError(val error: Exception?) : Event()
    }

    private lateinit var usbApi: UsbApi

    fun connect(context: Context) {
        usbApi = UsbApi()
        usbApi.connect(context)
    }

    val events: Flow<Event> by lazy {
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

    fun sendMsg(msg: String) {
        usbApi.write(msg)
    }
}

class MainViewModel : ViewModel() {

    var msg by mutableStateOf("")
    private set

    init {
        viewModelScope.launch {
            Mc.events.collect {
                when(it) {
                    is Mc.Event.OnNewData -> Log.d(TAG, it.data)
                    is Mc.Event.OnRunError -> Unit
                }
            }
        }
    }

    fun onMsgChange(value: String) {
        msg = value
    }
    fun onSendMsgClick() {
        Mc.sendMsg(msg)
    }
}

@Composable
fun MainScreen() {
    val viewModel = viewModel<MainViewModel>()
    Column(
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally,
        modifier = Modifier
            .fillMaxSize()
            .padding(all = 16.dp)
    ) {
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
    }
}

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
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