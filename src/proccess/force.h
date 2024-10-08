#include "Generic.h"

bool Generic::isForce(int i, char tipo)
{
  String fMsg = "";
  for (size_t x = 0; x < F_F.length(); x++)
  {
    if (F_F[x] != ';')
      fMsg += F_F[x];
    else
    {
      fMsg.remove(0, 1);
      String tp = (tipo == '0') ? "" : String(tipo);
      if (fMsg == "/" + tp + String(i))
        return 1;
      fMsg = "";
    }
  }
  return 0;
}

void Generic::setForce(String m)
{
  String b = "";
  for (size_t i = 2; i < m.length(); i++)
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

void Generic::upForce()
{
  String F_FMsg = "";
  for (size_t x = 0; x < F_F.length(); x++)
  {
    if (F_F[x] != ';')
      F_FMsg += F_F[x];
    else
    {
      setForce(F_FMsg);
      F_FMsg = "";
    }
  }
}