#include "Generic.h"

void Generic::start()
{
  // P_P = "l6:T0:2000;9:T0;l-:1000:/T0;3:12;3:7:0/37/0;13:1|o12:0;";
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