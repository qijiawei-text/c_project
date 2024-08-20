
/**
 * `ieee_float.h' - ieee_float
 *
 */

#ifndef IEEE_FLOAT_H
#define IEEE_FLOAT_H 1

#define B64_STR_SIZE  8
#define B64_BITS_SIZE  6
#define BINARY_STR_SIZE 32

#ifdef __cplusplus
extern "C" {
#endif

/****MAIN APIs****/
/* Encode `float *' source, */
/* Returns a `char *' ieee float base64 encoded string. */
int ieee_float_encode(const float, char *);

/* Dencode `char *' ieee float base64 encode source */
/* Returns a `float' */
int ieee_float_decode(const char *, float *);

#ifdef __cplusplus
}
#endif

#endif
