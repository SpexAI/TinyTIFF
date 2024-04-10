/*
    Copyright (c) 2008-2020 Jan W. Krieger (<jan@jkrieger.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


*/
#ifndef TIFF_DEFINITIONS_INTERNAL_H
#define TIFF_DEFINITIONS_INTERNAL_H

#include <assert.h>

#define TIFF_ORDER_UNKNOWN 0
#define TIFF_ORDER_BIGENDIAN 1
#define TIFF_ORDER_LITTLEENDIAN 2


#define TIFF_FIELD_IMAGEWIDTH 256
#define TIFF_FIELD_IMAGELENGTH 257
#define TIFF_FIELD_BITSPERSAMPLE 258
#define TIFF_FIELD_COMPRESSION 259
#define TIFF_FIELD_PHOTOMETRICINTERPRETATION 262
#define TIFF_FIELD_FILLORDER 266
#define TIFF_FIELD_IMAGEDESCRIPTION 270
#define TIFF_FIELD_STRIPOFFSETS 273
#define TIFF_FIELD_ORIENTATION 274
#define TIFF_FIELD_SAMPLESPERPIXEL 277
#define TIFF_FIELD_ROWSPERSTRIP 278
#define TIFF_FIELD_STRIPBYTECOUNTS 279
#define TIFF_FIELD_XRESOLUTION 282
#define TIFF_FIELD_YRESOLUTION 283
#define TIFF_FIELD_PLANARCONFIG 284
#define TIFF_FIELD_RESOLUTIONUNIT 296
#define TIFF_FIELD_TILE_WIDTH 322
#define TIFF_FIELD_TILE_LENGTH 323
#define TIFF_FIELD_TILE_OFFSETS 324
#define TIFF_FIELD_TILE_BYTECOUNTS 325
#define TIFF_FIELD_EXTRASAMPLES 338
#define TIFF_FIELD_SAMPLEFORMAT 339

#define TIFF_TYPE_BYTE 1
#define TIFF_TYPE_ASCII 2
#define TIFF_TYPE_SHORT 3
#define TIFF_TYPE_LONG 4
#define TIFF_TYPE_RATIONAL 5

#define TIFF_COMPRESSION_NONE 1
#define TIFF_COMPRESSION_CCITT 2
#define TIFF_COMPRESSION_PACKBITS 32773

#define TIFF_PLANARCONFIG_CHUNKY 1
#define TIFF_PLANARCONFIG_PLANAR 2

#define TIFF_PHOTOMETRICINTERPRETATION_WHITEISZERO 0
#define TIFF_PHOTOMETRICINTERPRETATION_BLACKISZERO 1
#define TIFF_PHOTOMETRICINTERPRETATION_RGB 2
#define TIFF_PHOTOMETRICINTERPRETATION_PALETTE 3
#define TIFF_PHOTOMETRICINTERPRETATION_TRANSPARENCY 4
#define TIFF_PHOTOMETRICINTERPRETATION_CMYK 5
#define TIFF_PHOTOMETRICINTERPRETATION_YCBCR 6
#define TIFF_PHOTOMETRICINTERPRETATION_CIELAB 8


#define TIFF_ORIENTATION_STANDARD 1

#define TIFF_FILLORDER_DEFAULT 1
#define TIFF_FILLORDER_REVERSE 2

#define TIFF_EXTRASAMPLES_UNSPECIFIED 0
#define TIFF_EXTRASAMPLES_ASSOCIATEDALPHA 1
#define TIFF_EXTRASAMPLES_UNASSOCIATEDALPHA 2

#define TIFF_RESOLUTIONUNIT_NONE 1
#define TIFF_RESOLUTIONUNIT_INCH 2
#define TIFF_RESOLUTIONUNIT_CENTIMETER 3

#define TIFF_SAMPLEFORMAT_UINT  1
#define TIFF_SAMPLEFORMAT_INT  2
#define TIFF_SAMPLEFORMAT_IEEEFP  3
#define TIFF_SAMPLEFORMAT_VOID  4

#define TIFF_RESOLUTION_UNIT_NONE 1
#define TIFF_RESOLUTION_UNIT_INCH 2
#define TIFF_RESOLUTION_UNIT_CENTIMETER 3

/* BOS checks */
#define BOS_UNKNOWN ((size_t)-1)
#define _BOS_KNOWN(dest) ((size_t)BOS(dest) != BOS_UNKNOWN)
#if defined __has_builtin
#  if __has_builtin (__builtin_object_size)
#    if defined(_FORTIFY_SOURCE) && _FORTIFY_SOURCE > 1
#      define BOS(dest) __builtin_object_size((dest), 1)
#    else
#      define BOS(dest) __builtin_object_size((dest), 0)
#    endif
#  else
#    define BOS(dest) BOS_UNKNOWN
#  endif
#else
#  define BOS(dest) BOS_UNKNOWN
#endif

#ifdef HAVE_SPRINTF_S
#define SPRINTF_S(dest, destsz, ...) sprintf_s(dest, destsz, __VA_ARGS__)
#else
#define SPRINTF_S(dest, destsz, ...) \
    if (_BOS_KNOWN(dest) && BOS(dest) < destsz) \
        assert(0 && "memcpy_s: destsz too big"); \
    sprintf(dest, __VA_ARGS__)
#endif

#ifdef HAVE_MEMCPY_S
#define MEMCPY_S(dest, destsz, src, count) memcpy_s(dest, destsz, src, count)
#else
#define MEMCPY_S(dest, destsz, src, count)         \
    { if ((!dest || !src) && count)                \
        assert(0 && "memcpy_s: NULL dest or src"); \
      if (_BOS_KNOWN(dest) && (BOS(dest) < destsz || BOS(dest) < count)) \
        assert(0 && "memcpy_s: dest not big enough"); \
      if (_BOS_KNOWN(src) && BOS(src) < count)       \
        assert(0 && "memcpy_s: src not big enough"); \
    }                                                \
    memcpy(dest, src, count)
#endif

#ifdef HAVE_STRCPY_S
#define STRCPY_S(dest, destsz, src) strcpy_s(dest, destsz, src)
#else
#define STRCPY_S(dest, destsz, src)                   \
    if (!dest || !src)                                \
        assert(0 && "strcpy_s: NULL dest or src");    \
    if (_BOS_KNOWN(dest) && (BOS(dest) < destsz))     \
        assert(0 && "strcpy_s: destsz too big"); \
    if (_BOS_KNOWN(src) && BOS(src) > destsz)         \
        assert(0 && "strcpy_s: dest not big enough"); \
    strcpy(dest, src)
#endif

#ifdef HAVE_STRCAT_S
#define STRCAT_S(dest, destsz, src) strcat_s(dest, destsz, src)
#else
#define STRCAT_S(dest, destsz, src)                   \
    if (!dest || !src)                                \
        assert(0 && "strcat_s: NULL dest or src");    \
    if (_BOS_KNOWN(dest) && BOS(dest) < destsz)       \
        assert(0 && "strcat_s: destsz too big"); \
    if (_BOS_KNOWN(src) && BOS(src) > destsz)         \
        assert(0 && "strcat_s: dest not big enough"); \
    strcat(dest, src)
#endif


#ifdef HAVE_STRCPY_S
#define TINYTIFF_SET_LAST_ERROR(tiff, message) strcpy_s(tiff->lastError, TIFF_LAST_ERROR_SIZE, message);
#else
#define TINYTIFF_SET_LAST_ERROR(tiff, message) strcpy(tiff->lastError, message);
#endif



#endif // TIFF_DEFINITIONS_INTERNAL_H
