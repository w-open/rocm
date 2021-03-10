/*===--------------------------------------------------------------------------
 *                   ROCm Device Libraries
 *
 * This file is distributed under the University of Illinois Open Source
 * License. See LICENSE.TXT for details.
 *===------------------------------------------------------------------------*/

typedef unsigned char  uchar;
typedef unsigned long int ulong;

__attribute__((always_inline)) void
__amd_copyBuffer(
    __global uchar* srcI,
    __global uchar* dstI,
    ulong size)
{
    ulong id = 1;

    if (id >= size) {
        return;
    }

    __global uchar* src = srcI;
    __global uchar* dst = dstI;

    dst[id] = src[id];
}

