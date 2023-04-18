
// (float)(((double)rand() / ((double)RAND_MAX + 1) * N))

// %[flags][width][.precision][size]type

#include "main.h"

int main()
{
    int N = 1000;
    alignas(64) float F32_One[16];
    alignas(64) float F32_Two[16];

    for (int i = 0; i < 16; ++i)
    {
        F32_One[i] = (float)(((double)rand() / ((double)RAND_MAX + 1) * N));
    }
    for (int i = 0; i < 16; ++i)
    {
        F32_Two[i] = (float)(((double)rand() / ((double)RAND_MAX + 1) * N));
    }

    // warmup m512
    __m512 One_vals = _mm512_load_ps(F32_One);
    __m512 Two_vals = _mm512_load_ps(F32_Two);
    __m512 Sum_vals = _mm512_add_ps(One_vals, Two_vals);
    alignas(64) float F32_Sum[16];
    _mm512_store_ps(F32_Sum, Sum_vals);

    // start m512

    std::chrono::duration<double> m512_time(0);
    auto m512_startTime = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000000; ++i)
    {
        One_vals = _mm512_load_ps(F32_One);
        Two_vals = _mm512_load_ps(F32_Two);
        Sum_vals = _mm512_add_ps(One_vals, Two_vals);
        _mm512_store_ps(F32_Sum, Sum_vals);
    }
    m512_time += std::chrono::high_resolution_clock::now() - m512_startTime;

    // end m512

    // warmup standard

    alignas(64) float F32_Standard_Sum[16];
    for (int i = 0; i < 16; ++i)
    {
        F32_Standard_Sum[i] = F32_One[i] + F32_Two[i];
    }

    // start standard

    std::chrono::duration<double> standard_time(0);
    auto standard_startTime = std::chrono::high_resolution_clock::now();
    // #pragma loop( hint_parallel( 16 ) )
    for (int i = 0; i < 100000000; ++i)
    {
        F32_Standard_Sum[0] = F32_One[0] + F32_Two[0];
        F32_Standard_Sum[1] = F32_One[1] + F32_Two[1];
        F32_Standard_Sum[2] = F32_One[2] + F32_Two[2];
        F32_Standard_Sum[3] = F32_One[3] + F32_Two[3];
        F32_Standard_Sum[4] = F32_One[4] + F32_Two[4];
        F32_Standard_Sum[5] = F32_One[5] + F32_Two[5];
        F32_Standard_Sum[6] = F32_One[6] + F32_Two[6];
        F32_Standard_Sum[7] = F32_One[7] + F32_Two[7];
        F32_Standard_Sum[8] = F32_One[8] + F32_Two[8];
        F32_Standard_Sum[9] = F32_One[9] + F32_Two[9];
        F32_Standard_Sum[10] = F32_One[10] + F32_Two[10];
        F32_Standard_Sum[11] = F32_One[11] + F32_Two[11];
        F32_Standard_Sum[12] = F32_One[12] + F32_Two[12];
        F32_Standard_Sum[13] = F32_One[13] + F32_Two[13];
        F32_Standard_Sum[14] = F32_One[14] + F32_Two[14];
        F32_Standard_Sum[15] = F32_One[15] + F32_Two[15];
    }

    standard_time += std::chrono::high_resolution_clock::now() - standard_startTime;

    // end standard

    printf("m512 Time: %.6f\n", m512_time.count());
    printf("standard Time: %.6f\n\n", standard_time.count());

    for (int i = 0; i < 16; ++i)
    {
        printf("%10f + %10f = %10f (%10f)\n", F32_One[i], F32_Two[i], F32_Sum[i], F32_Standard_Sum[i]);
    }
}
