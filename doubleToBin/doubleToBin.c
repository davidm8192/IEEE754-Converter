#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#define EXP_SZ 11
#define FRAC_SZ 52

int main(int argc, char *argv[]) {

    // SETUP

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    double value;
    fscanf(fp, "%lf", &value);

    // the reference solution
    unsigned long int ref_bits = *(unsigned long int*) &value;

    // THE SIGN BIT
    bool sign = value<0.0;
    if(1/value > 0) {
        sign = false; 
    }
    else {
        sign = true; 
    }
    printf("%d_",sign);
    //assert (sign == (1&ref_bits>>(EXP_SZ+FRAC_SZ))); // validate your result against the reference

    // THE EXP FIELD
    double fraction = sign ? -value : value;

    long double trial_exp=(1<<(EXP_SZ-1))-1; // start by assuming largest possible exp (before infinity)
    // do trial division 
    int e = 0; 
    
    if(fabs(fraction) >= 1.0) {
        for(int i = 0; i < trial_exp; i++) {
            if(fabs(fraction) >= 1.0 && fabs(fraction) < 2.0) {                
                fraction--;
                break; 
            }
            fraction /= 2.0; 
            e = i + 1; 
        }
    }
    else if (fabs(fraction) < 1.0) {
        for(int i = 0; i < trial_exp; i++) {
            if(fabs(fraction) >= 1.0 && fabs(fraction) < 2.0) {                
                fraction--;
                break; 
            }
            fraction *= 2.0; 
            e--; 
        }
    }
    
    unsigned short bias = (1<<(EXP_SZ-1))-1;
    signed short exp = e + bias;

    for ( int exp_index=EXP_SZ-1; 0<=exp_index; exp_index-- ) {
        bool exp_bit = 1&exp>>exp_index;
        printf("%d",exp_bit);
        assert (exp_bit == (1&ref_bits>>(exp_index+FRAC_SZ))); // validate your result against the reference
    }
    printf("_");

    // THE FRAC FIELD
    bool frac_array[FRAC_SZ+1]; // one extra LSB bit for rounding

    if(exp == 0) {
        if(fraction >= 1) {
            frac_array[FRAC_SZ] = 1;
        }
        else {
            frac_array[FRAC_SZ] = 0;
        }
        for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {            
            fraction -= (unsigned int)fraction;
            fraction *= 2.0;
            if(fraction < 1.0) {
                fraction -= (unsigned int)fraction;
                frac_array[frac_index] = false; 
            }
            else {
                fraction -= (int)fraction;
                
                frac_array[frac_index] = true; 
            }
        }
    }
    else {
        for ( int frac_index=FRAC_SZ; 0<=frac_index; frac_index-- ) {            
            fraction -= (unsigned int)fraction;
            fraction *= 2.0;
            if(fraction < 1.0) {
                fraction -= (unsigned int)fraction;
                frac_array[frac_index] = false; 
            }
            else {
                fraction -= (int)fraction;
                frac_array[frac_index] = true; 
            }
        }
    }

    for ( int frac_index=FRAC_SZ-1; 0<=frac_index; frac_index-- ) {
        bool frac_bit = frac_array[frac_index+1]; // skipping the extra LSB bit for rounding
        printf("%d", frac_bit);
        assert (frac_bit == (1&ref_bits>>frac_index)); // validate your result against the reference
    }

}
