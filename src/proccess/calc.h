#include "Generic.h"

void Generic::setCalc(const char *o, const char *prop)
{
    String sA = "";
    String sB = "";
    char sC = '0';

    size_t length = strlen(prop);
    int x = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (prop[i] == '/')
            x++;
        else if (x == 0)
            sC = prop[i];
        else if (x == 1)
            sA += prop[i];
        else if (x == 2)
            sB += prop[i];
    }

    float a = 0;
    float b = 0;

    if (isalpha(sA[0]))
    {
        a = checkStatus(sA.c_str());
    }
    else
    {
        a = sA.toFloat();
    }

    if (isalpha(sB[0]))
    {
        b = checkStatus(sB.c_str());
    }
    else
    {
        b = sB.toFloat();
    }

    int m = atoi(o + 1);

    switch (sC)
    {
    case '*':
        CT_CT[m] = (a * b);
        break;
    case '!':
        CT_CT[m] = (a / b);
        break;
    case '-':
        CT_CT[m] = (a - b);
        break;
    case '+':
        CT_CT[m] = (a + b);
        break;
    default:
        CT_CT[m] = 0.00;
    }
}