#include "SmartLadderEsp32.h"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <PubSubClient.h>

WebServer server(80);
IPAddress local_IP(192, 168, 4, 5);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char *BROKER_MQTT = "smartladder.com.br";
int BROKER_PORT = 9000;

WiFiClient espClient;
PubSubClient MQTT(espClient);

long int msWf = 0;
byte T_C_ = 0;
byte ORIGIN_ = 0;

const char MAIN_page[] PROGMEM = R"=====(<!doctypehtml><meta content='text/html; charset=utf-8'http-equiv=Content-Type><title>SMARTLadder</title><style>body{color:#434343;font-family:'Helvetica Neue',Helvetica,Arial,sans-serif;font-size:14px;background-color:#eee;margin-top:100px}.container{margin:0 auto;max-width:400px;padding:30px;box-shadow:0 10px 20px rgba(0,0,0,.19),0 6px 6px rgba(0,0,0,.23);background-color:#fff;border-radius:10px}h2{text-align:center;margin-bottom:20px;margin-top:0;color:#0ee6b1;font-size:35px}#titleGreen{color:#fff;font-weight:700;background:#3f51b5;padding:0 5px;border-radius:5px;margin-right:3px}#titleBlack{color:#8999ef}h3{text-align:center;margin-bottom:40px;margin-top:0;color:#8999ef;font-size:35px}form .field-group{box-sizing:border-box;clear:both;padding:4px 0;position:relative;margin:1px 0;width:100%}.text-field{font-size:15px;margin-bottom:4%;-webkit-appearance:none;display:block;background:#fafafa;color:#3f51b5;width:100%;padding:15px 0 15px 0;text-indent:10px;border-radius:5px;border:1px solid #e6e6e6;background-color:transparent}.text-field:focus{border-color:#8999ef;outline:0}.button-container{box-sizing:border-box;clear:both;margin:1px 0 0;padding:4px 0;position:relative;width:100%}.button{background:#3f51b5;border:none;border-radius:5px;color:#fff;cursor:pointer;display:block;font-weight:700;font-size:16px;padding:15px 0;text-align:center;text-transform:uppercase;width:100%;-webkit-transition:background 250ms ease;-moz-transition:background 250ms ease;-o-transition:background 250ms ease;transition:background 250ms ease}a{text-decoration:none;color:#fff;margin-top:0}#status{text-align:center;text-decoration:none;color:#336859;font-size:14px}</style><script>function v() {const ssid = document.forms['myForm']['ssid'].value;const password = document.forms['myForm']['password'].value;var status = document.getElementById('statusDiv'); if (ssid == '' && password == '') {    status.innerHTML = `<p id='status' style='color:red;'>Selecione uma rede e digite uma senha.</p>`;    return false;} else if (!ssid) {    status.innerHTML = `<p id='status' style='color:red;'>Selecione uma rede</p>`; return false;}else if (!password) {    status.innerHTML = `<p id='status' style='color:red;'>Digite uma senha</p>`;    return false;} else if (!document.forms['myForm']['codeProgram'].value) {    status.innerHTML = `<p id='status' style='color:red;'>Informe o código do programa desejado</p>`; return false;} else {    status.innerHTML = `<p id='status'>Conectando...</p>`;    return true;}
        }</script><div class=container><h3><span id=titleGreen>SMARTL</span><span id=titleBlack>adder</span></h3><form action=/n_c method=post name=myForm onsubmit='return v()'><b>Selecione uma rede*</b><div class=field-group><select class='text-field' name='ssid'></select></div><br><b>Senha*</b><div class=field-group><input class=text-field length=64 name=password placeholder=****** type=password></div><br><b>Código do programa*</b><div class=field-group><input class=text-field length=64 name=codeProgram placeholder='Código do programa'value=#code></div><div id=statusDiv></div><br><br><div class=button-container><input class=button type=submit value=Conectar></div></form><br><br><p>REDE: #rede<p>STATUS: #status<p>IP: #ip<p>DNS: #dns</div>)=====";

String SmartLadderEsp32::getScanNetworks()
{
    String networks = "";
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i)
        networks += WiFi.SSID(i) + F("|");
    return networks;
}

String SmartLadderEsp32::listSSID()
{
    String index = (const __FlashStringHelper *)MAIN_page;

    String networks = "";
    int n = WiFi.scanNetworks();

    if (n == 0)
    {
        index.replace(F("<select class='text-field' name='ssid'></select>"), F("<select class='text-field' name='ssid'><option value='' disabled selected>No networks found</option></select>"));
        index.replace(F("<br><br>"), F("<p id='status' style='color:red;'>Rede não encontrada.</p>"));
        return index;
    }
    else
    {
        networks += F("<select class='text-field' name='ssid'><option value='' disabled selected>SSID</option>");
        for (int i = 0; i < n; ++i)
        {
            networks += F("<option ");
            if (SSID_ == WiFi.SSID(i))
                networks += F(" selected ");
            networks += " value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
        }
        networks += F("</select>");
    }
    index.replace(F("<select class='text-field' name='ssid'></select>"), networks);

    index.replace(F("#rede"), SSID_);
    if (WiFi.status() == WL_CONNECTED)
        index.replace(F("#status"), "<span color='green'>CONECTADO</span>");
    if (WiFi.status() == WL_CONNECTED)
        index.replace(F("#status"), "<span color='red'>DESCONECTADO</span>");

    index.replace(F("#code"), CODE_);
    String ip = WiFi.localIP().toString();
    index.replace(F("#ip"), ip);

    index.replace(F("#dns"), getDNS() + F(".local"));
    return index;
}

String SmartLadderEsp32::getDNS()
{

    String serverDNS = "device";
    if (CODE_ && CODE_.length() > 8)
    {
        serverDNS = "";
        for (size_t i = 0; i < 8; i++)
            serverDNS += CODE_[i];
    }
    return "smartladder-" + serverDNS;
}

void SmartLadderEsp32::setDNS()
{
    String serverDNS = getDNS();
    if (!MDNS.begin(serverDNS.c_str()))
    {
        Serial.println(F("Error setting up MDNS!"));
    }
    else
    {
        Serial.println(F("start dns: smartladder-device!"));
    }
    MDNS.addService("http", "tcp", 80);
}

void SmartLadderEsp32::wifiInit()
{
    connectEeprom();
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP("SMARTLadder", "12345678");
    Serial.println(WiFi.softAPIP());

    delay(2000);

    server.on("/", [this]()
              { this->hRoot(); });
    server.on(F("/info"), [this]()
              { this->getInfo(); });
    server.on(F("/n_c"), [this]()
              { this->hForm(); });

    server.begin();

    setDNS();
}

void SmartLadderEsp32::emitDtSv(String v)
{
    MQTT.publish((CODE_ + String(F("_dt"))).c_str(), v.c_str());
}

void SmartLadderEsp32::mqttCallback(char *topic, byte *payload, unsigned int length)
{
    ORIGIN_ = 0;
    for (int i = 0; i < length; i++)
        receiver((char)payload[i]);
}

void SmartLadderEsp32::rMQTT()
{
    if (!MQTT.connected())
    {
        if (MQTT.connect(CODE_.c_str(), CODE_.c_str(), CODE_.c_str()))
        {
            MQTT.setCallback([this](char *topic, byte *payload, unsigned int length)
                             { this->mqttCallback(topic, payload, length); });
            emit(String(1));
            MQTT.subscribe(CODE_.c_str());
        }
        else
        {
            Serial.println("Error mqtt");
        }
    }
}

void SmartLadderEsp32::connectClientWifi()
{
    if (!SSID_.equals("") && !PASW_.equals(""))
    {
        WiFi.begin(SSID_.c_str(), PASW_.c_str());
        delay(500);
        isForm = true;
        checkStatusWifi();
    }
}

void SmartLadderEsp32::reloadWifi()
{
    SSID_ = "";
    PASW_ = "";
    CODE_ = "";
    int i = 0;
    for (int x = 0; x < TEMP_.length(); x++)
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
    connectClientWifi();
}

void SmartLadderEsp32::setupConnect()
{
    wifiInit();

    MQTT.setKeepAlive(600);
    MQTT.setCallback([this](char *topic, byte *payload, unsigned int length)
                     { this->mqttCallback(topic, payload, length); });
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
}

void SmartLadderEsp32::hRoot()
{
    String index = listSSID();
    server.send(200, "text/html", index);
}

void SmartLadderEsp32::getInfo()
{
    String index = WiFi.localIP().toString();
    server.send(200, "text/html", index);
}

void SmartLadderEsp32::hForm()
{
    SSID_ = server.arg(F("ssid"));
    PASW_ = server.arg(F("password"));
    CODE_ = server.arg(F("codeProgram"));

    Serial.println(SSID_);
    Serial.println(PASW_);
    Serial.println(CODE_);

    connectClientWifi();
    server.send(200, "text/html", "<script>setTimeout(() => { window.location.href='/' },5000);</script> <center><br /><br />CONECTANDO...</center>");
}

void SmartLadderEsp32::TempStatusWifi()
{
    if ((millis() - msWf) >= (T_C_ <= 3 ? 5000 : 60000))
    {
        T_C_ += 1;
        msWf = 0;
        vStatusAll();
    }
    if (msWf == 0)
        msWf = millis();
}

void SmartLadderEsp32::vStatusAll()
{
    if (WiFi.status() != WL_CONNECTED || isForm)
        checkStatusWifi();
    else if (!AP_DISCONNECT && WiFi.status() == WL_CONNECTED)
    {
        WiFi.softAPdisconnect(true);
        AP_DISCONNECT = true;
    }
    else if (!MQTT.connected() && WiFi.status() == WL_CONNECTED)
    {
        Serial.println("mqtt disconnected!");
        rMQTT();
    }
}

void SmartLadderEsp32::checkStatusWifi()
{
    if (WiFi.status() == WL_CONNECTED && isForm)
    {
        isForm = false;
        setEEPROM();
        MQTT.disconnect();
        rMQTT();
    }
    else if (WiFi.status() == WL_CONNECT_FAILED)
    {
        if (!SSID_.equals("") && !PASW_.equals(""))
        {
            WiFi.begin(SSID_.c_str(), PASW_.c_str());
            delay(1000);
        }
    }
}

void SmartLadderEsp32::conectLoop()
{
    server.handleClient();
    TempStatusWifi();

    MQTT.loop();
}