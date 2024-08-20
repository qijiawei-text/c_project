/* C program to convert a real value */
/* to IEEE 754 floating point representaion in Base64 encoding */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ieee_float.h"

/**
 * Base64 index table.
 */

static const char b64_table[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};


typedef union {
    float f;
    struct
    {

        /* Order is important. */
        /* Here the members of the union data structure */
        /* use the same memory (32 bits). */
        /* The ordering is taken */
        /* from the LSB to the MSB. */
        unsigned int mantissa : 23;
        unsigned int exponent : 8;
        unsigned int sign : 1;

    } raw;
} ieeefloat;


/**
 * convert interger to binary
 * value - input
 * length -bits to be converted
 * position - position of bits updated in binary
 */
static void
int_to_binary(int value, int length, int position, char *binary)
{
    /* update the binary representation */
    /* of a number n up to i-bits. */
    int k, p = position;
    for (k = length - 1; k >= 0; k--) {

        if ((value >> k) & 1)
            binary[p] = '1';
        else
            binary[p] = '0';
        p++;
    }
}

/* Function to convert a binary array */
/* to the corresponding integer */
static int
binary_to_int(const char *arr, int low, int high)
{
    int f = 0, i, v;
    for (i = high; i >= low; i--) {
        if (arr[i] == '1')
            v = 1;
        else
            v = 0;
        f = f + v *pow(2, high - i);
    }
    return f;
}


/* Function to convert real value */
/* to IEEE foating point representation (32 bits) in binary string */
static void
float_to_binary(float var, char *binary)
{
    ieeefloat v;

    v.f = var;
    if (v.raw.sign == 1)
        binary[0] = '1';
    else
        binary[0] = '0';

    int_to_binary(v.raw.exponent, 8, 1, binary);
    int_to_binary(v.raw.mantissa, 23, 9, binary);
/*    printf("float in:%f\n", var); */
/*    printf("binary string out:%s\n", binary); */
}

/* convert IEEE foating point representation (32 bits) */
/* to ieeefloat type */
static void
binary_to_float(char *binary, float *var)
{

    ieeefloat *v = (ieeefloat *)var;

    /* Convert the least significant */
    /* mantissa part (23 bits) */
    /* to corresponding decimal integer */
    unsigned f = binary_to_int(binary, 9, 31);

    /* Assign integer representation of mantissa */
    v->raw.mantissa = f;

    /* Convert the exponent part (8 bits) */
    /* to a corresponding decimal integer */
    f = binary_to_int(binary, 1, 8);

    /* Assign integer representation */
    /* of the exponent */
    v->raw.exponent = f;

    /* Assign sign bit */
    v->raw.sign = binary[0];
/*    printf("binary string in:%s\n", binary); */
/*   printf("float out:%f\n", *var); */
}

static void
binary_to_b64(const char *binary, char *b64)
{
    int i = 0;
    int low = 0;
    int indx;

    while (low < BINARY_STR_SIZE) {
        if (low < BINARY_STR_SIZE - B64_BITS_SIZE - 1)
            /* convert 6 bits to integer */
            indx = binary_to_int(binary, low, low + B64_BITS_SIZE - 1);
        else
            /* remaider */
            indx = binary_to_int(binary, low, BINARY_STR_SIZE - 1);

        /* set b64 encode */
        b64[i++] = b64_table[indx];
        low += B64_BITS_SIZE;
    }

    /* append `=' */
    while ((i < B64_STR_SIZE)) {
        b64[i++] = '=';
    }
}

static int
b64_to_binary(const char *b64, char *binary)
{
    int i = 0;
    int indx = 0;
    int p = 0;
    int len;

    while (i < B64_STR_SIZE) {
        /* break if char is `=' */
        if ('=' == b64[i]) {
            break;
        }

        /* find translation char in `b64_table' */
        for (indx = 0; indx < 64; indx++) {
            if (b64[i] == b64_table[indx]) {
                break;
            }
        }

        if (indx == 64) {
            printf("invalide base64 character: %c\n", b64[i]);
            return -1;
        }

        /* check remaider */
        if (p + B64_BITS_SIZE > BINARY_STR_SIZE)
            len = BINARY_STR_SIZE - p;
        else
            len = B64_BITS_SIZE;

        int_to_binary(indx, len, p, binary);
        p += B64_BITS_SIZE;
        i++;
    }

    /* printf("b64 in:%s\n", b64); */
    /* printf("binary string out from b64:%s\n", binary); */
    return 0;
}

int
ieee_float_encode(const float f, char *b64)
{
    char binary [BINARY_STR_SIZE];

    /* convert to binary */
    float_to_binary(f, binary);

    /* b64 encoding (8bytes) */
    binary_to_b64((const char *)binary, b64);

    return 0;
}

int
ieee_float_decode(const char *b64, float *f)
{
    char binary[BINARY_STR_SIZE];
    int rc = -1;

    if ((int)strlen(b64) < B64_STR_SIZE)
        return rc;

    /* decode b64 to binary char */
    rc = b64_to_binary(b64, binary);
    if (rc < 0)
        return rc;

    /* covert back to float */
    binary_to_float(binary, f);

    return 0;
}
