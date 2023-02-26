
#include <mc.h>

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void MQTTStub::setup()
{
    // Init. WiFi client
    Serial.print("Connecting to WiFi");
    // WiFi.mode(WIFI_STA);
    // WiFi.hostname("hostname");
    WiFi.begin("VIDEOTRON6590", "A9UAYAHH9947N");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.print("Connected to WiFi: ");
    Serial.print("Videotron6590_5GHz");
    Serial.print(" using password: ");
    Serial.println("A9UAYAHH9947N");

    // Init. MQTT client
    Serial.print("Connecting to MQTT");
    client.setServer("public.mqtthq.com", 1883);
    client.setCallback([](char* topic, byte* payload, unsigned int length)
        {
            Serial.println("Tiens tiens, une commande MQTT vient d'arriver");
        }
    );
    while (!client.connect("francois rochefort")) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("");
    Serial.print("Connected to MQTT: ");
    Serial.print("public.mqtthq.com");
    Serial.print(":");
    Serial.println("1883");

    // Subscribe to topics
    client.subscribe("COMMANDS");

    // Send debug signal 'READY' to mosquitto_sub
    client.publish("EVENTS", "I am ready to come");
}