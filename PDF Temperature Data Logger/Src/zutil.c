/* zutil.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995-2005 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* $Id: zutil.c,v 1.6 2005/10/11 21:31:11 rjs Exp $ */
/* @(#) $Id: zutil.c,v 1.6 2005/10/11 21:31:11 rjs Exp $ */

#include "zutil.h"


#ifndef HAVE_MEMCPY

void zmemcpy(
    Bytef* dest,
    const Bytef* source,
    uInt  len)
{
    if (len == 0) return;
    do {
        *dest++ = *source++; /* ??? to be unrolled */
    } while (--len != 0);
}

int zmemcmp(
    const Bytef* s1,
    const Bytef* s2,
    uInt  len)
{
    uInt j;

    for (j = 0; j < len; j++) {
        if (s1[j] != s2[j]) return 2*(s1[j] > s2[j])-1;
    }
    return 0;
}

void zmemzero(
    Bytef* dest,
    uInt  len)
{
    if (len == 0) return;
    do {
        *dest++ = 0;  /* ??? to be unrolled */
    } while (--len != 0);
}
#endif



