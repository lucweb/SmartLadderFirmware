#include "Generic.h"

int Generic::checkStatus(const char* port)
{
  if (isdigit(port[0]))
  {
    int resource = atoi(port);
    if (resource)
      return digitalRead(resource);
  }
  else if (port[0] == 'b')
  {
    int t = atoi(port + 1); 
    return B_B[t];
  }
  else if (port[0] == 'T')
  {
    int t = atoi(port + 1); 
    return getTemp(t);
  }
  else if (port[0] == 'c')
  {
    int t = atoi(port + 1); 
    return getCount(t);
  }
  return 0;
}

bool Generic::setEqual(const char* tipoCond, const char* sA, const char* sB)
{
    const char* slashA = strchr(sA, '/');
    const char* slashB = strchr(sB, '/');
    int a = 0;
    int b = 0;

    if (slashA != nullptr)
    {
        a = checkStatus(sA + 1);
    }
    else
    {
        a = atoi(sA);
    }

    if (slashB != nullptr)
    {
        b = checkStatus(sB + 1);
    }
    else
    {
        b = atoi(sB);
    }

    switch (tipoCond[0])
    {
    case '=':
        return (a == b);
    case '!':
        return (a != b);
    case '-':
        return (a < b);
    case '+':
        return (a > b);
    default:
        return false;
    }
}