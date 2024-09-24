#include "Generic.h"

void Generic::atvSet(const char *port, const char *prop)
{
    int i = atoi(port + 1);
    int value = atoi(prop);

    switch (port[0])
    {
    case 'T':
        TL_TL[i] = value;
        break;
    case 'b':
        B_B[i] = value;
        break;
    case 'c':
        CT_CT[i] = value;
        break;
    }
}