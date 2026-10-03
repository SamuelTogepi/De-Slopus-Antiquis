/*
 *  ibex - pseudo-library
 *  Copyright (c) 2015 xerub
 *  Boyer-Moore-Horspool algorithm from Wikipedia
 *  Adapted for 64-bit ARM iBoot Patcher Suite.
 */

#include <libc.h>

#define UCHAR_MAX 255

unsigned char *
boyermoore_horspool_memmem(const unsigned char* haystack, size_t hlen,
                           const unsigned char* needle,   size_t nlen)
{
    size_t last, scan = 0;
    size_t bad_char_skip[UCHAR_MAX + 1];

    /* Sanity checks on the parameters */
    if (nlen <= 0 || !haystack || !needle)
        return NULL;

    /* ---- Preprocess ---- */
    for (scan = 0; scan <= UCHAR_MAX; scan = scan + 1)
        bad_char_skip[scan] = nlen;

    last = nlen - 1;

    for (scan = 0; scan < last; scan = scan + 1)
        bad_char_skip[needle[scan]] = last - scan;

    /* ---- Do the matching ---- */
    while (hlen >= nlen)
    {
        for (scan = last; haystack[scan] == needle[scan]; scan = scan - 1)
            if (scan == 0)
                return (void *)haystack;

        hlen     -= bad_char_skip[haystack[last]];
        haystack += bad_char_skip[haystack[last]];
    }

    return NULL;
}

void *
memmem(const void *haystack, size_t hlen, const void *needle, size_t nlen)
{
    const unsigned char *h;
    const unsigned char *n;
    if (!nlen) {
        return (void *)haystack;
    }
    if (nlen > hlen) {
        return NULL;
    }
    if (nlen >= 4 && hlen >= 256) {
        return boyermoore_horspool_memmem(haystack, hlen, needle, nlen);
    }
    for (h = haystack, n = needle; hlen >= nlen; hlen--, h++) {
        if (*h == *n && !memcmp(h + 1, n + 1, nlen - 1)) {
            return (char *)h;
        }
    }
    return NULL;
}
