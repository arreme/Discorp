#include <core/core_func.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int core::function1(int b) 
{
    int a = 5;
    if (b > 2) 
    {
        auto res = std::rand();
        res + 5;
    }
    return a + b;
}