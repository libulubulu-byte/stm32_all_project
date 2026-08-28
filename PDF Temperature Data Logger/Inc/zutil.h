/* zutil.h -- internal interface and configuration of the compression library
 * Copyright (C) 1995-2005 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */


/* $Id: zutil.h,v 1.7.2.3 2008/02/28 11:02:06 rjs Exp $ */
/* @(#) $Id: zutil.h,v 1.7.2.3 2008/02/28 11:02:06 rjs Exp $ */

#ifndef ZUTIL_H
#define ZUTIL_H

#define ZLIB_INTERNAL
#include "zlib.h"


#  include <string.h>
#  include <stdlib.h>


#ifndef local
#  define local static
#endif
/* compile with -Dlocal if your debugger can't find static symbols */

typedef unsigned char  uch;
typedef uch FAR uchf;
typedef unsigned short ush;
typedef ush FAR ushf;
typedef unsigned long  ulg;


/* (size given to avoid silly warnings with Visual C++) */




#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif
/* default memLevel */

#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2
/* The three kinds of block type */

#define MIN_MATCH  3
#define MAX_MATCH  128
/* The minimum and maximum match lengths */

#define PRESET_DICT 0x20 /* preset dictionary flag in zlib header */

        /* target dependencies */


#ifdef HAVE_MEMCPY
#  ifdef SMALL_MEDIUM /* MSDOS small or medium model */
#    define zmemcpy _fmemcpy
#    define zmemcmp _fmemcmp
#    define zmemzero(dest, len) _fmemset(dest, 0, len)
#  else
#    define zmemcpy memcpy
#    define zmemcmp memcmp
#    define zmemzero(dest, len) memset(dest, 0, len)
#  endif
#else
   extern void zmemcpy  OF((Bytef* dest, const Bytef* source, uInt len));
   extern int  zmemcmp  OF((const Bytef* s1, const Bytef* s2, uInt len));
   extern void zmemzero OF((Bytef* dest, uInt len));
#endif



#endif /* ZUTIL_H */
