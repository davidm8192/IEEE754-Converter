#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen failed");
        return 0;
    }

    // first, read the number
    signed int input;
    fscanf(fp, "%d", &input);

    // print bits
    for ( int bit=3; 0<=bit; bit-- ) {
        size_t bit_val = ((1<<4)-1) & input>>bit*4; // shift and mask
        //char character = bit_val ? '1' : '0';
        char character;
        switch(bit_val) {
            case 10 : 
                character = 'A';
                printf("%c",character);
                break; 
            case 11 : 
                character = 'B';
                printf("%c",character);
                break;
            case 12 : 
                character = 'C';
                printf("%c",character);
                break; 
            case 13 : 
                character = 'D';
                printf("%c",character);
                break;
            case 14 : 
                character = 'E';
                printf("%c",character);
                break; 
            case 15 : 
                character = 'F';
                printf("%c",character);
                break;
            default : 
                printf("%ld",bit_val);
                break;
        }
    }
    printf("\n");

    return EXIT_SUCCESS;

}
