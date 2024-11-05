#include "Generic.h"

bool Generic::atvPUpDown(int tCond, const char *port, const char *prop)
{
    String _b = "";
    char sw = '0';
    char t = '0';
    String a = "";
    String b = "";
    String c = "";
    String d = "";

    size_t l = strlen(prop);
    int x = 0;
    for (size_t i = 0; i < l; i++)
    {
        if (prop[i] == '/')
            x++;
        else if (x == 0)
            _b += prop[i];
        else if (x == 1)
            sw = prop[i];
        else if (x == 2)
            t = prop[i];
        else if (x == 3)
            a += prop[i];
        else if (x == 4)
            b += prop[i];
        else if (x == 5)
            c += prop[i];
        else if (x == 6)
            d += prop[i];
    }

    int i_p = atoi(port);
    int i_b = _b.toInt();
    bool st = false;

    if (t == '0')
        st = digitalRead(i_p);
    else if (t == 'b')
        st = B_B[i_p];
    else if (t == 'i')
    {
        const size_t MAX_LENGTH = c.length() + 1;
        char bytes[MAX_LENGTH];
        strncpy(bytes, c.c_str(), MAX_LENGTH - 1);
        bytes[MAX_LENGTH - 1] = '\0';
        st = checkPinStateI2c((a == F("0") ? I2C_1 : I2C_2), b.toInt(), i_p, bytes);
        if (d == F("1"))
            st = !st;
    }

    if (tCond == 16)
    {
        if (sw == '1')
            return toggleSetUp(i_b, st);
        else
            return toggleUp(i_b, st);
    }
    else if (tCond == 17)
    {
        if (sw == '1')
            return toggleSetDown(i_b, st);
        else
            return toggleDown(i_b, st);
    }
    return false;
}

bool Generic::toggleUp(int i, bool st)
{
    if (B_B[i] == 0 && st == 1)
    {
        B_B[i] = 1;
        return true;
    }
    B_B[i] = st;
    return false;
}

bool Generic::toggleSetUp(int i, bool st)
{
    if ((st && B_B[i] != 1))
        B_B[i] = 1;
    return B_B[i] == 1;
}

bool Generic::toggleDown(int i, bool st)
{
    if (B_B[i] == 1 && st == 0)
    {
        B_B[i] = 0;
        return true;
    }
    B_B[i] = st;
    return false;
}

bool Generic::toggleSetDown(int i, bool st)
{
    if ((st && B_B[i] == 0) || (!st && B_B[i] == 1))
        B_B[i] = B_B[i] + 1;
    return B_B[i] == 2;
}