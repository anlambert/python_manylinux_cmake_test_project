#include <stdlib.h>
#include <stdio.h>
#include <string.h>  // for strlen

#include <zlib.h>

#include "zliblinkage.h"

// adapted from: http://stackoverflow.com/questions/7540259/deflate-and-inflate-zlib-h-in-c

char* compressString(const char *str) {

    char *ret = malloc(strlen(str)+1);

    printf("Uncompressed size is: %lu\n", strlen(str));
    printf("Uncompressed string is: %s\n", str);


    printf("\n----------\n\n");

    // STEP 1.
    // deflate a into b. (that is, compress a into b)

    // zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)strlen(str)+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)str; // input char array
    defstream.avail_out = (uInt)sizeof(ret); // size of output
    defstream.next_out = (Bytef *)ret; // output char array

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);

    // This is one way of getting the size of the output
    printf("Compressed size is: %lu\n", strlen(ret));
    printf("Compressed string is: %s\n", ret);

    return ret;
}
