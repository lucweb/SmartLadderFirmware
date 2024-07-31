#if defined(ESP32)
    #include "SmartLadderEsp32.h"
#elif defined(ESP8266)
    #include "SmartLadderEsp8266.h"
#else
    #include "SmartLadderArduino.h"
#endif

bool SmartLadderArduino::atv(String tipoCond, String port, String prop, int s) {
    int tCond = tipoCond.toInt();
    int resource = port.toInt();

    switch (tCond) {
        case 10:
            atvIADCPort(resource);
            return s;
        case 11:
            atvIDACPort(resource, prop);
            return s;
        case 12:
            return atvCountDown(port, tCond, prop, s);
        case 14:
            return stepper(resource, prop);
        case 13:
            /*
            int t = prop.toInt();
            if (s && !DT_SV_S[t] && !DT_SV.length()) {
                DT_SV = port;
                DT_SV_S[t] = 1;
            } else if (!s && DT_SV_S[t]) {
                DT_SV_S[t] = 0;
            }
            */
            return s;  
        default:
            if (resource && tCond != 0) {
                return atvIOPort(resource, tCond, s);
            } else if (port[0] == 'b') {
                return atvIOBit(port, tCond, s);
            } else if (port[0] == 'T') {
                return atvTime(port, tCond, prop, s);
            } else if (tCond > 0 && port[0] == 'c') {
                return atvCount(port, tCond, prop, s);
            } else if (tCond == 0) {
                return setEqual(tipoCond, port, prop);
            }
        break;
    }

    //if (port[0] == 'A' || port[0] == 'a') {}

    return true;  
}