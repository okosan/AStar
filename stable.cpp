#include "stable.h"


void xfBeep(int freq, int duration)
{
    std::printf ("beeping!!!");
}

// TODO: outdated convention
bool XFAIL(int ret)
{
    return (ret != 0);
}

// TODO: need proper template
float SQR(float val)
{
    return val*val;
}

bool xfBetween(double val, double val1, double val2)
{
    if (val1<val2)
    {
        if ((val >= val1) && (val <= val2))
            return true;
        return false;
    }
    else
    {
        if ((val >= val2) && (val <= val1))
            return true;
        return false;
    }
}

bool xfBetweenExcl(double val, double val1, double val2)
{
    if (val1<val2)
    {
        if ((val > val1) && (val < val2))
            return true;
        return false;
    }
    else
    {
        if ((val > val2) && (val < val1))
            return true;
        return false;
    }
}
