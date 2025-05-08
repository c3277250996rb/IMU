#include "stm32f10x.h"                  // Device header
#include "my_tool.h"

//限幅函数，把val限制在[min, max]区间内
float my_constrain(float val, float min, float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}


