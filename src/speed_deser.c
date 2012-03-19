#include <time.h>
#include <string.h>
#include <stdio.h>

#include "deserialize.h"

double duration_as_ms(clock_t start, clock_t end)
{
    return ((double)(end - start) * 1000) / CLOCKS_PER_SEC;
}


int main(int argc, char *argv[])
{
    int repeats = 10000000;
    char *buf = "A test string";
    uint32_t dum;
    clock_t st1, end1;
    clock_t st2, end2;

    int i;
    st1 = clock();
    for (i=0; i<repeats; i++) {
        dum = read_uint32(buf, 0, 1);
    }
    end1 = clock();

    int j;
    st2 = clock();
    for (j=0; j<repeats; j++) {
        dum = read_uint32_2(buf, 0, 1);
    }
    end2 = clock();

    printf("read_uint32  : %fms\n", duration_as_ms(st1, end1)/repeats);
    printf("read_uint32_2: %fms\n", duration_as_ms(st2, end2)/repeats);    
    return 0;
}
    
