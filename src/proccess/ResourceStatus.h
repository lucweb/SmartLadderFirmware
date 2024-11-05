#include "Generic.h"

bool Generic::atv(String tipoCond, String port, String prop, int s)
{
    int tCond = tipoCond.toInt();
    int resource = port.toInt();

    switch (tCond)
    {
    case 10:
#if defined(ESP32)
        atvIADCPort(port);
#else
        atvIADCPort(resource);
#endif
        return s;
    case 11:
        atvIDACPort(resource, prop.c_str());
        return s;
    case 12:
        return atvCountDown(port.c_str(), prop.c_str(), s);
    case 14:
        if (s)
            return stepper(resource, prop);
        return s;
    case 15:
        if (s)
            atvSet(port.c_str(), prop.c_str());
        return s;
    case 16:
    case 17:
        if (s)
            return atvPUpDown(tCond, port.c_str(), prop.c_str());
        return s;
    case 18:
        if (s)
            setCalc(port.c_str(), prop.c_str());
        return s;
    case 13:
#if defined(ESP32)
        int t = prop.toInt();
        if (s && !DT_SV_S[t] && !DT_SV.length())
        {
            DT_SV = port;
            DT_SV_S[t] = 1;
        }
        else if (!s && DT_SV_S[t])
        {
            DT_SV_S[t] = 0;
        }
#endif
        return s;
    }

    if ((resource || port == "0") && tCond != 0)
    {
        if (prop != "")
        {
#if USE_I2C
            return atvIOPortI2c(resource, tCond, prop.c_str(), s);
#else
            return s;
#endif
        }
        else
            return atvIOPort(resource, tCond, s);
    }
    else if (port[0] == 'b')
    {
        return atvIOBit(port.c_str(), tCond, s);
    }
    else if (port[0] == 'T')
    {
        return atvTime(port.c_str(), tCond, prop.c_str(), s);
    }
    else if (tCond > 0 && port[0] == 'c')
    {
        return atvCount(port.c_str(), tCond, prop.c_str(), s);
    }
    else if (tCond == 0)
    {
        if (s)
            return setEqual(tipoCond.c_str(), port.c_str(), prop.c_str());
        return s;
    }

    return s;
}