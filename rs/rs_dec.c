// compile with gcc rs_dec.c -o rs_dec -lfec
// This tool adds Reed-Solomon parity to random file
// It implements rs(255:239) with 16 bytes parity for each 239 bytes block
// Reed-Solomon parameter are
// symbol size: 8 bits
// nroots: 8
// prim: 11
// fcr: 121
// poly: 1 + x + x^2 + x^7 + x^8
// polyval: 0x187
// (N,K) = (255,239)
// based on https://berthub.eu/articles/posts/reed-solomon-for-programmers/
// and libfec https://github.com/quiet/libfec

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <fec.h>

int main(int argc, char *argv[])
{
    unsigned char *data;
    char input_f[64], output_f[64];
    int fi = 0, fo = 1;		// defaults to stdin and stdout
    int i, err = 0;
    ssize_t len;
    void *rs, *rs_pad;

    data = (unsigned char *) calloc(255, sizeof(unsigned char));
    if (data == NULL) {
	printf("Cannot allocate %d bytes for data\n", 223);
	exit(EXIT_FAILURE);
    }

    if (argc > 1) {
	while ((i = getopt(argc, argv, "i:o:")) != -1) {
	    switch (i) {
	    case 'i':
		strncpy(input_f, optarg, sizeof(input_f));
		fi = open(input_f, O_RDONLY);
		if (fi == -1) {
		    fprintf(stderr, "Error opening file input %d\n", errno);
		    exit(EXIT_FAILURE);
		}
		break;
	    case 'o':
		strncpy(output_f, optarg, sizeof(output_f));
		fo = open(output_f, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fo == -1) {
		    fprintf(stderr, "Error opening file output %d\n", errno);
		    exit(EXIT_FAILURE);
		}
		break;
	    }
	}
    }

    rs = init_rs_char(8, 0x187, 121, 11, 16, 0);

    while ((len = read(fi, data, 255)) > 0 && err != -1) {
	if (len == 255) {
	    err = decode_rs_char(rs, data, NULL, 0);
	} else {
	    rs_pad = init_rs_char(8, 0x187, 121, 11, 16, 255 - len);
	    err = decode_rs_char(rs_pad, data, NULL, 0);
	    free_rs_char(rs_pad);
	}
	if (err != -1)
	    write(fo, data, (size_t) (len - 16));
    }

    free_rs_char(rs);

    if (fi != 0)
	close(fi);
    if (fo != 1)
	close(fo);
    free(data);

    if (err == -1) {
	fprintf(stderr, "Unrecoverable corruption found. Cleaning up\n");
	remove((const char *) &output_f);
	exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
