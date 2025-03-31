#include "NRSSL.h"
#include <cstdint>
#include <cstdio>
#include <iostream>

extern float float_var;
extern float float_array[10];

extern double double_var;
extern double double_array[10];

extern "C" {
float store_float_and_return();
}

using namespace std;

int main(int argc, char const *argv[]) {
    NRSSL nrssl;

    {
        uint32_t value = *(uint32_t *)&float_var;
        cout << nrssl.convertUintToDouble(value, NRSSL::POSIT) << endl;
    }

    {
        for (int i = 0; i < 10; i++) {
            uint32_t value = *(uint32_t *)&float_array[i];
            cout << nrssl.convertUintToDouble(value, NRSSL::POSIT) << " ";
        }
        cout << endl;
    }

    {
        uint64_t value = *(uint64_t *)&double_var;
        cout << nrssl.convertUintToDouble(value, NRSSL::POSIT) << endl;
    }

    {
        for (int i = 0; i < 10; i++) {
            uint64_t value = *(uint64_t *)&double_array[i];
            cout << nrssl.convertUintToDouble(value, NRSSL::POSIT) << " ";
        }
        cout << endl;
    }

    {
        float value = store_float_and_return();
        uint32_t ivalue = *(uint32_t *)&value;
        cout << fixed << nrssl.convertUintToDouble(ivalue, NRSSL::POSIT) << endl;
    }

    return 0;
}
