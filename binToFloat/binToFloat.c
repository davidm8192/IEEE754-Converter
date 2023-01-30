#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define EXP_SZ 8
#define FRAC_SZ 23

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the binary number representation of float point number
    //char buff;
    unsigned int binary[32];

    for (int i=EXP_SZ+FRAC_SZ; 0<=i; i--) { // read MSB first as that is what comes first in the file
        unsigned int bit; 
        if (!fscanf(fp, "%1d", &bit)) {
            perror("reading the bit failed");
            exit(EXIT_FAILURE);
        }
        binary[i] = bit; 
    }
    // float number = *(float *)&binary; // you are not allowed to do this.

    bool sign = binary[31]; 

    // E
    
    double expNum = 0; 
    unsigned int e; 
    int counter = 0; 
    for(int i = FRAC_SZ; i < FRAC_SZ+EXP_SZ; i++) {
        expNum += ldexp(binary[i], counter);
        counter++; 
    }
    e = expNum - 127; 
    counter = -1;

    // M
    double m = 0; 
    for(int i = FRAC_SZ-1; i >= 0; i--) {
        m += ldexp(binary[i], counter);
        counter--; 
    }
    m += 1; 

    double number = ldexp ( m, e );
    number = sign ? -number : number;
    printf("%f\n", number);

    return EXIT_SUCCESS;

}
