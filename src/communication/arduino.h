#include "SmartLadderArduino.h"

void SmartLadderArduino::startTypeRequest(char e)
{
    _TPR = e;
    if (e == 0x23 || e == 0x40)
        P_P = "";
    else if (e == 0x24)
        F_F = "";
    else if (e == 0x7C || e == 0x2A)
        R_R = "";
}

void SmartLadderArduino::receiver(char e)
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
#if defined(ESP32)
        TEMP_ += e;
#endif
    }
    else
        startTypeRequest(e);
}

void SmartLadderArduino::eR()
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

    Serial.print(F(">"));

    if (_TPR == 0x23 || _TPR == 0x40)
    {
        Serial.print(String(P_P.length()));
        if (_TPR == 0x40)
            setConfigProgram();
    }
    else if (_TPR == 0x24)
    {
        upForce();
        Serial.print(String(F_F.length()));
    }
    else if (_TPR == 0x7C || _TPR == 0x2A)
    {
        typePort();
        Serial.print(String(R_R.length()));
        if (_TPR == 0x2A)
            setConfigPort();
    }
    else if (_TPR == 0x63)
    {
        Serial.print(VERSION);
    }
    else if (_TPR == 0x7A)
    {
    }
    else if (_TPR == 0x78)
    {
    }

    Serial.print(F("<"));
    _TPR = 0x0;
}

void SmartLadderArduino::emitDigitalRead()
{
    String p = "";
    String v = String(F(">s"));
    String va = "";
    for (size_t x = 0; x < R_R.length(); x++)
    {
        if (R_R[x] != 'i' && R_R[x] != 'o' && R_R[x] != 'a' && R_R[x] != 'd')
            p += R_R[x];
        else if (R_R[x] == 'a')
        {
            va += String(atvIADCPort(p.toInt())) + ':';
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
    Serial.print(v + '-');
    Serial.print(va + '-');
}

void SmartLadderArduino::setStatusResource()
{
    _ST = 0;
    emitDigitalRead();

    for (int x = 0; x < 10; x++)
        Serial.print(String(getTemp(x)) + ':');

    Serial.print(F("-"));
    for (int x = 0; x < 10; x++)
        Serial.print(String(getCount(x)) + ':');

    Serial.print(F("-"));
    for (int x = 0; x < 20; x++)
        Serial.print(String(B_B[x])+':');
        
    Serial.print(F("<"));
}
