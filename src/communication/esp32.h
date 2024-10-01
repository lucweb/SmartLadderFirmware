#include "SmartLadderEsp32.h"

void SmartLadderEsp32::emit(String v)
{
    if (!ORIGIN_)
    {

#if USE_ETH8720 || USE_WIFI
        MQTT.publish((CODE_ + "_").c_str(), v.c_str());
#endif
        return;
    }
    else
        Serial.print(v);
}

void SmartLadderEsp32::startTypeRequest(char e)
{
    _TPR = e;
    if (e == 0x23 || e == 0x40)
        P_P = "";
    else if (e == 0x24)
        F_F = "";
    else if (e == 0x7C || e == 0x2A)
        R_R = "";
}

void SmartLadderEsp32::receiver(char e)
{
    if (e == 0x2E)
        eR();
    else if (_TPR == 0x23 || _TPR == 0x40)
        P_P += e;
    else if (_TPR == 0x24)
        F_F += e;
    else if (_TPR == 0x7C || _TPR == 0x2A)
        R_R += e;
    else if (_TPR == 0x26)
        TD_TD += e;
    else if (_TPR == 0x78)
    {
        TEMP_ += e;
    }
    else
        startTypeRequest(e);
}

void SmartLadderEsp32::eR()
{
    if (!_TPR)
        return;

    if (_TPR == 0x26)
    {
        _ST = TD_TD.toInt();
        TD_TD = "";
        _TPR = 0x0;
        return;
    }

    emit(F(">"));

    if (_TPR == 0x23 || _TPR == 0x40)
    {
        emit(String(P_P.length()));
        if (_TPR == 0x40)
            setConfigProgram();
    }
    else if (_TPR == 0x24)
    {
        upForce();
        emit(String(F_F.length()));
    }
    else if (_TPR == 0x7C || _TPR == 0x2A)
    {
        typePort();
        emit(String(R_R.length()));
        if (_TPR == 0x2A)
        {
            setConfigPort();
        }
    }
    else if (_TPR == 0x63)
    {
        emit(VERSION);
        emit(String(F("|")) + String(USE_WIFI) + String(F("|")) + SSID_);
    }
    else if (_TPR == 0x7A)
    {
        emit(String(F("_")) + CODE_ + String(F("|")));
#if USE_WIFI
        getScanNetworks();
#endif
    }
    else if (_TPR == 0x78)
    {
#if USE_ETH8720 || USE_WIFI
        dataConfig(true);
#endif
        emit(String(F("x")));
    }

    emit(F("<"));
    _TPR = 0x0;
}

void SmartLadderEsp32::emitDigitalRead()
{
    String p = "";
    String v = ">s";
    String va = "";
    for (int x = 0; x < R_R.length(); x++)
    {
        if (R_R[x] != 'i' && R_R[x] != 'o' && R_R[x] != 'a' && R_R[x] != 'd')
            p += R_R[x];
        else if (R_R[x] == 'a')
        {
            va += String(atvIADCPort(p)) + ':';
            p = "";
        }
        else if (R_R[x] == 'd')
        {
            p = "";
        }
        else
        {
            v += String(digitalRead(p.toInt()));
            p = "";
        }
    }
    emit(v + '-');
    emit(va + '-');
}

void SmartLadderEsp32::setStatusResource()
{
    _ST = 0;
    emitDigitalRead();

    for (int x = 0; x < 10; x++)
        emit(String(getTemp(x)) + ':');

    emit(F("-"));
    for (int x = 0; x < 10; x++)
        emit(String(getCount(x)) + ':');

    emit(F("-"));
    for (int x = 0; x < 30; x++)
        emit(String(B_B[x])+':');

    emit(F("<"));
}

void SmartLadderEsp32::ctrlDtSv()
{
    String v = "";
    String t = "";
    bool c = false;
    DT_SV += '|';
    for (int x = 0; x < DT_SV.length(); x++)
    {
        if (!c)
        {
            if (DT_SV[x] == '|')
                c = true;
            else
                v += DT_SV[x];
        }
        else if (DT_SV[x] != '|')
        {
            t += DT_SV[x];
        }
        else
        {
            v += ':';
            v += getVDS(t);
            t = "";
        }
    }
#if USE_ETH8720 || USE_WIFI
    emitDtSv(v);
#endif
    DT_SV = "";
}

String SmartLadderEsp32::getVDS(String port)
{
    int x = port[0];
    port.remove(0, 1);
    int n = port.toInt();
    switch (x)
    {
    case 'o':
    case 'i':
        return String(digitalRead(n));
    case 'a':
        return String(atvIADCPort(port));
    case 'b':
        return String(B_B[n]);
    case 'T':
        return String(getTemp(n));
    case 'c':
        return String(getCount(n));
    }
    return "";
}