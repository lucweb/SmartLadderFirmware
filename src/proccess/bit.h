#include "Generic.h"

bool Generic::atvIOBit(const char *port, int tCond, int s)
{
    int b = atoi(port + 1);

    switch (tCond)
    {
    case 1:
        if (s)
            return B_B[b];
        break;
    case 2:
        if (s)
            return !B_B[b];
        break;
    case 3:
        if (!isForce(b, 'b'))
        {
            B_B[b] = s;
        }
        break;
    case 4:
        if (!isForce(b, 'b'))
        {
            if (s)
                B_B[b] = 1;
        }
        break;
    case 5:
        if (!isForce(b, 'b'))
        {
            if (s)
                B_B[b] = 0;
        }
        break;
    case 9:
        if (s)
            B_B[b] = 0;
        break;
    }

    return s;
}
