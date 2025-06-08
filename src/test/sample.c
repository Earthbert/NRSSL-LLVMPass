
float float_var = 1.0;
float float_array[10] = {1.0, 2.0, 3.0, 4.0, 5.0};

double double_var = 5.12345;    
double double_array[10] = {1.0, 2.0, 3.0, 4.0, 5.0};

float multi_dim_float_array[2][5] = {
    {1.0, 2.0, 3.0, 4.0, 5.0},
    {6.0, 7.0, 8.0, 9.0, 10.0}
};

float store_float_and_return() {
    float a = 77777.44444;
    return a;
}

float store_array_and_return() {
    float b[10] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};
    return b[5];
}