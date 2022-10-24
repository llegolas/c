#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int min_i (unsigned int a, unsigned int b, unsigned int c) {
    if (a <= b && a <= c)
        return a;
    else if (b < a && b < c)
        return b;
    else 
        return c;
}

int levenstein (char *token1, char *token2) {
    unsigned int len_token1, len_token2, x, y, cur, prev;

    if (strcmp(token1,token2) == 0)
        return 0;    

    len_token1 = strlen(token1);
    len_token2 = strlen(token2);
    unsigned int column[len_token1 + 1];

    for (y = 1; y <= len_token1; y++)
        column[y] = y;

    for (x = 1; x <= len_token2; x++) {
        column[0] = x;
        cur = x - 1;
        for (y = 1; y <= len_token1; y++) {
            prev = column[y];
            column[y] = min_i(column[y] + 1, column[y - 1] + 1, cur + (token1[y-1] == token2[x - 1] ? 0 : 1));
            cur = prev;
        }
    }

    return column[len_token1];
}

int main (int argc, char *argv[]) {
    char token1[64], token2[64];

    if (argc !=3 ){
       fprintf(stderr,"Provide two tokens as command line parameters!\n");
       exit(EXIT_FAILURE);
    }
    
    strncpy(token1,argv[1],sizeof(token1));
    strncpy(token2,argv[2],sizeof(token2));

    printf ("%d\n",levenstein(token1,token2));

}
