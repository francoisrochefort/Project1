package com.example.handshack

import android.app.Application
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
import dagger.Module
import dagger.Provides
import dagger.hilt.InstallIn
import dagger.hilt.android.AndroidEntryPoint
import dagger.hilt.android.HiltAndroidApp
import dagger.hilt.android.lifecycle.HiltViewModel
import dagger.hilt.android.qualifiers.ApplicationContext
import dagger.hilt.components.SingletonComponent
import kotlinx.coroutines.channels.awaitClose
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.callbackFlow
import kotlinx.coroutines.flow.collect
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltAndroidApp
class HandshakeApp: Application()

@Module
@InstallIn(SingletonComponent::class)
class AppModule {

    @Provides
    fun provideUsbApi(
        @ApplicationContext context: Context
    ) = UsbApi().also { it.connect(context) }

    @Provides
    fun provideMcRepository(usbApi: UsbApi) : McRepository = McRepositoryImpl(usbApi)
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

interface McRepository {

    sealed class Event {
        data class OnNewData(val data: String) : Event()
        data class OnRunError(val error: Exception?) : Event()
    }

    val events: Flow<Event>
    fun sendMsg(msg: String)
}

class McRepositoryImpl(private val usbApi: UsbApi) : McRepository {

    override val events: Flow<McRepository.Event> by lazy {
        callbackFlow {
            val callback = object : SerialInputOutputManager.Listener {
                override fun onNewData(data: ByteArray?) {
                    trySend(McRepository.Event.OnNewData(data.toString()))
                }

                override fun onRunError(e: Exception?) {
                    trySend(McRepository.Event.OnRunError(e))
                }
            }
            usbApi.register(callback)
            awaitClose {
                usbApi.unregister()
            }
        }
    }

    override fun sendMsg(msg: String) {
        usbApi.write(msg)
    }
}

@HiltViewModel
class MainViewModel @Inject constructor(
    private val mcRepository: McRepository
) : ViewModel() {

    var msg by mutableStateOf("")
    private set

    init {
        viewModelScope.launch {
            mcRepository.events.collect {
                when(it) {
                    is McRepository.Event.OnNewData -> Log.d("francois", it.data)
                    is McRepository.Event.OnRunError -> Unit
                }
            }
        }
    }

    fun onMsgChange(value: String) {
        msg = value
    }
    fun onSendMsgClick() {
        mcRepository.sendMsg(msg)
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

@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
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