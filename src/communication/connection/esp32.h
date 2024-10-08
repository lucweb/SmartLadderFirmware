#include "SmartLadderEsp32.h"

#if USE_ETH8720
#include <ETH.h>
#endif

#if USE_WIFI
#include <WiFi.h>
#endif

#if USE_ETH8720 || USE_WIFI
#include <PubSubClient.h>
const char *BROKER_MQTT = "mqtt.smartladder.io";
int BROKER_PORT = 1883;
#endif

#if USE_WIFI
WiFiClient client;
#elif USE_ETH8720
NetworkClient client;
#endif

PubSubClient MQTT(client);

long int msWf = 0;
byte T_C_ = 0;

#if USE_WIFI
void SmartLadderEsp32::getScanNetworks()
{
    String networks = "";
    int n = WiFi.scanNetworks();
    for (size_t i = 0; i < n; ++i)
        networks += WiFi.SSID(i) + '|';
    emit(networks);
}

void SmartLadderEsp32::checkStatusWifi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        if (!SSID_.equals("") && !PASW_.equals(""))
        {
            if (WiFi.begin(SSID_.c_str(), PASW_.c_str()))
                delay(500);
        }
    }
}
#endif

#if USE_ETH8720
void SmartLadderEsp32::connectEth()
{
    if (!ETH.begin(ETH_TYPE, ETH_ADDR, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_POWER_PIN, ETH_CLK_MODE))
        Serial.println(F("eETH_0"));
    else
        Serial.println(F("eETH_1"));

    delay(100);
}
#endif

void SmartLadderEsp32::vStatusAll()
{
    bool connected = false;
#if USE_ETH8720
    if (!ETH.linkUp())
        connectEth();
    else
        connected = true;
#endif

#if USE_WIFI
    checkStatusWifi();
    if (WiFi.status() == WL_CONNECTED)
        connected = true;
#endif

    if (connected && !MQTT.connected())
        rMQTT();
}

#if USE_ETH8720 || USE_WIFI
void SmartLadderEsp32::emitDtSv(String v)
{
    MQTT.publish((CODE_ + String(F("_dt"))).c_str(), v.c_str());
}

void SmartLadderEsp32::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    ORIGIN_ = 0;
    for (size_t i = 0; i < length; i++)
        receiver((char)payload[i]);
}

void SmartLadderEsp32::rMQTT()
{
    if (!MQTT.connected())
    {
        const char *codeStr = CODE_.c_str();
        if (MQTT.connect(codeStr, codeStr, codeStr))
        {
            delay(100);
            Serial.println(F("smqtt"));
            MQTT.setCallback([this](char *topic, byte *payload, unsigned int length)
                             { this->mqttCallback(topic, payload, length); });
            MQTT.subscribe(CODE_.c_str());
        }
        else
            Serial.println(F("emqtt"));
    }
}

void SmartLadderEsp32::setupMqttConnect()
{
    MQTT.setKeepAlive(600);
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void SmartLadderEsp32::dataConfig(bool bSave = false)
{
    SSID_ = "";
    PASW_ = "";
    CODE_ = "";
    TEMP_.trim();
    int i = 0;
    for (size_t x = 0; x < TEMP_.length(); x++)
    {
        if (TEMP_[x] == '|')
            i++;
        else if (i == 0)
            SSID_ += TEMP_[x];
        else if (i == 1)
            PASW_ += TEMP_[x];
        else if (i == 2)
            CODE_ += TEMP_[x];
    }

    if (bSave)
        setConfig();
    TEMP_ = "";
}

void SmartLadderEsp32::TempStatusConnect()
{
    if ((millis() - msWf) >= (T_C_ <= 3 ? (T_C_ * 2000) : 60000))
    {
        T_C_ += 1;
        msWf = 0;
        vStatusAll();
    }
    if (msWf == 0)
        msWf = millis();
}

void SmartLadderEsp32::connectLoop()
{
    TempStatusConnect();
    MQTT.loop();
}

#endif