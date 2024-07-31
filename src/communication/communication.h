#include "SmartLadderArduino.h"

void SmartLadderArduino::startTypeRequest(char e)
{
    _TPR = e;
    if (e == 0x23 || e == 0x40)
        P_P = "";
    else if (e == 0x24)
        _F = "";
    else if (e == 0x7C || e == 0x2A)
        R_R = "";
}

void SmartLadderArduino::receiver(char e)
{
    if (e == 0x2E) //.
        eR();
    else if (_TPR == 0x23 || _TPR == 0x40)
        P_P += e;
    else if (_TPR == 0x24)
        _F += e;
    else if (_TPR == 0x7C || _TPR == 0x2A)
        R_R += e;
    else if (_TPR == 0x26)
        _TD += e;
    else
        startTypeRequest(e);
}

void SmartLadderArduino::eR()
{
    if (!_TPR) return;

    if (_TPR == 0x26)
    {
        _ST = _TD.toInt();
        _TD = "";
        _TPR = 0x0;
        return;
    }

    Serial.print('>');

    if (_TPR == 0x23 || _TPR == 0x40){
        Serial.print(String(P_P.length()));
        if (_TPR == 0x40)
            setEEPROMProgram();
    } else if (_TPR == 0x24)
    {
        upForce();
        Serial.print(String(_F.length()));
    }
    else if (_TPR == 0x7C || _TPR == 0x2A)
    {
        typePort();
        Serial.print(String(R_R.length()));
        if (_TPR == 0x2A)
            setEEPROMPort();
    }
    else if (_TPR == 0x63)
    {
        Serial.print(SMARTLADDER_ARDUINO_VERSION);
    }

    Serial.print('<');
    _TPR = 0x0;
}

void SmartLadderArduino::emitDigitalRead()
{
    String p = "";
    String v = ">s";
    String va = "";
    for (int x = 0; x < R_R.length(); x++)
    {
        if (R_R[x] != 'i' && R_R[x] != 'o' && R_R[x] != 'a')
            p += R_R[x];
        else if (R_R[x] == 'a')
        {
            va += String(atvIADCPort(p.toInt())) + ':';
            p = "";
        }
        else
        {
            v += String(digitalRead(p.toInt()));
            p = "";
        }
    }
    Serial.print(v + '-' + va + '-');
}

void SmartLadderArduino::setStatusResource()
{
    _ST = 0;
    emitDigitalRead();

    for (int x = 0; x < 10; x++)
        Serial.print(String(getTemp(x)) + ':');

    Serial.print('-');
    for (int x = 0; x < 10; x++)
        Serial.print(String(getCount(x)) + ':');

    Serial.print('-');
    for (int x = 0; x < 20; x++)
        Serial.print(B_B[x] ? 1 : 0);

    Serial.print('<');
}