#if defined(ESP32)
    #include "SmartLadderEsp32.h"
#elif defined(ESP8266)
    #include "SmartLadderEsp8266.h"
#else
    #include "SmartLadderArduino.h"
#endif

void SmartLadderArduino::start()
{
  byte state = 0;
  String param1 = "";
  String param2 = "";
  String param3 = "";
  int delimiterCount = 0;
  bool isLoop = false;

  for (int i = 0; i < P_P.length(); i++) {
    char currentChar = P_P[i];

    if (currentChar == '>') {
      continue;
    }

    if (currentChar == 'l') {
      isLoop = true;
    } else if (currentChar == ':') {
      delimiterCount++;
    } else if (currentChar != ';' && currentChar != '_') {
      switch (delimiterCount) {
        case 0:
          param1 += currentChar;
          break;
        case 1:
          param2 += currentChar;
          break;
        case 2:
          param3 += currentChar;
          break;
      }
    } else {
      if (state != 0 && currentChar == ';') {
        if (!isLoop && state == 2) {
          isLoop = true;
        } else if (isLoop && state == 1) {
          isLoop = false;
        }
        state = 0;
      }

      if (param1 != "") {
        bool success = atv(param1, param2, param3, isLoop);
        if (isLoop && currentChar != '_') {
          isLoop = success;
        }

        if (isLoop && currentChar == '_' && state < 2) {
          state = (success) ? 2 : 1;
        }
      }

      param1 = "";
      param2 = "";
      param3 = "";
      delimiterCount = 0;
    }
  }
}