// compile with gcc -o rs rs.c -lfec
#include <stdio.h>
#include <stdlib.h>
#include <fec.h>
#include <string.h>

int main()
{
    unsigned char *data,*parity, *encoded;
    int i;

    data=(char*) calloc(223,sizeof(char));
    if (data == NULL) {
        printf("Cannot allocate %zu bytes for data\n", 223);
        exit(EXIT_FAILURE);
    }

    parity=(char*) calloc(32,sizeof(char));
    if (parity == NULL) {
        printf("Cannot allocate %zu bytes for parity\n", 32);
        exit(EXIT_FAILURE);
    }

    encoded=(char*) calloc(255,sizeof(char));
    if (encoded == NULL) {
        printf("Cannot allocate %zu bytes for the encoded message\n", 255);
        exit(EXIT_FAILURE);
    }

    strncpy(data,"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",64);
    printf ("%s\n",data);

    encode_rs_8(data,parity,0);

    for (i=0; i<32; i++) 
        printf("%02X",parity[i]);
    printf("\n");  

    // corrupt the message
    memcpy(encoded,"bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",64);
    memcpy(encoded+223,parity,32);

    printf("%s\n",encoded);

    i=decode_rs_8(encoded,NULL,0,0);
    if ( i == -1)
        printf("The message cannot be corrected\n");
  
    if (i > 0 && i <= 16) {
        printf("The message has %d corupted byte(s) and has been corrected.\n",i);
        printf("%s\n",encoded);
    };  

    free(data);
    free(parity);
    free(encoded);

    data = "Test real data with padding!\0";
    parity=(char*) calloc(32,sizeof(char));
    if (parity == NULL) {
        printf("Cannot allocate %zu bytes for parity\n", 32);
        exit(EXIT_FAILURE);
    }
    
    encoded=(char*) calloc(strlen(data)+32,sizeof(char));
    if (encoded == NULL) {
        printf("Cannot allocate %zu bytes for the encoded message\n", 255);
        exit(EXIT_FAILURE);
    }
 

    encode_rs_8(data,parity,195);

    for (i=0; i<32; i++)
        printf("%02X",parity[i]);
    printf("\n");

    // corrupt the message
    memcpy(encoded,"Test FAKE data with padding!",28);
    printf("%s\n",encoded);
    memcpy(encoded+28,parity,32);

    i=decode_rs_8(encoded,NULL,0,195);
    if ( i == -1)
        printf("The message cannot be corrected\n");

    if (i > 0 && i <= 16) {
        printf("The message has %d corupted byte(s) and has been corrected.\n",i);
        printf("%s\n",encoded);
    };

    free(parity);
    free(encoded);
    
    
}
