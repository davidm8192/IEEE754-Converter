#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return EXIT_FAILURE;
    }

    // SETUP

    // first, read the minuend (number to be subtracted from)
    //char buff;
    bool minuend[8]; // suggested that you store bits as array of bools; minuend[0] is the LSB
    for (int i=7; 0<=i; i--) { // read MSB first as that is what comes first in the file
        int bit; 
        if (!fscanf(fp, "%1d", &bit)) {
            perror("reading the bit failed");
            exit(EXIT_FAILURE);
        }
        minuend[i] = bit;
    }

    // second, read the subtrahend (number to subtract)
    bool subtrahend[8]; // suggested that you store bits as array of bools; subtrahend[0] is the LSB
    for (int i=7; 0<=i; i--) { // read MSB first as that is what comes first in the file
        int b; 
        if (!fscanf(fp, "%1d", &b)) {
            perror("reading the bit failed");
            exit(EXIT_FAILURE);
        }
        subtrahend[i] = b; 
    }

    // Negate the subtrahend
    // flip all bits
    for (int i=7; 0<=i; i--) {
        if(subtrahend[i] == 0) {
            subtrahend[i] = 1;
        }
        else {
            subtrahend[i] = 0; 
        }
    }

    // add one
    bool carry = true; // to implement the 'add one' logic, we do binary addition logic with carry set to true at the beginning
    for (int i=0; i<8; i++) { // iterate from LSB to MSB
        if(subtrahend[i] == 1) {
            subtrahend[i] = 0; 
            continue; 
        } 
        else {
            subtrahend[i] = 1; 
            carry = false; 
            break;
        }
    }

    // Add the minuend and the negated subtrahend
    carry = false; 
    bool difference[8];
    for (int i=0; i<8; i++) {
        int temp = subtrahend[i] + minuend[i];
        if(carry) {
            temp += 1; 
        }

        if(temp == 1) {
            carry = false;
            difference[i] = 1; 
        }
        else if (temp == 0){
            carry = false; 
            difference[i] = 0; 
        }
        else {
            carry = true; 
            if(temp == 2) {
                difference[i] = 0; 
            }
            else {
                difference[i] = 1; 
            }
        }
    }

    // print the difference bit string
    for (int i=7; 0<=i; i--)
        printf("%d",difference[i]);

    return EXIT_SUCCESS;

}
