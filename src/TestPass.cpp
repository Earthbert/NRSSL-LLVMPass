#include "NRSSL.h"
#include <cstdint>
#include <iostream>

extern float a;

int main(int argc, char const *argv[]) {
    NRSSL nrssl;

    uint32_t value = *(uint32_t *)&a;

    std::cout << nrssl.convert32TypeToDouble(value, POSIT32) << std::endl;

    return 0;
}
