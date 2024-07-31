#if defined(ESP32)
    #include "SmartLadderEsp32.h"
#elif defined(ESP8266)
    #include "SmartLadderEsp8266.h"
#else
    #include "SmartLadderArduino.h"
#endif

bool SmartLadderArduino::isForce(int i, char tipo = 0)
{
  String fMsg = "";
  for (int x = 0; x < _F.length(); x++)
  {
    if (_F[x] != ';')
      fMsg += _F[x];
    else
    {
      fMsg.remove(0, 1);
      String tp = (tipo == 0) ? "" : String(tipo);
      if (fMsg == "/" + tp + String(i))
        return 1;
      fMsg = "";
    }
  }
  return 0;
}

void SmartLadderArduino::setForce(String m)
{
  String b = "";
  for (int i = 2; i < m.length(); i++)
    b += m[i];
  int isBit = b.indexOf('b');
  if (isBit >= 0)
  {
    b.remove(0, 1);
    B_B[b.toInt()] = m[0];
  }
  else
    digitalWrite(b.toInt(), String(m[0]).toInt());
}

void SmartLadderArduino::upForce()
{
  String _FMsg = "";
  for (int x = 0; x < _F.length(); x++)
  {
    if (_F[x] != ';')
      _FMsg += _F[x];
    else
    {
      setForce(_FMsg);
      _FMsg = "";
    }
  }
}