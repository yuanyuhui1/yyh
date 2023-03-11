/* 7zAlloc.c -- Allocation functions
2017-04-03 : Igor Pavlov : Public domain */

#include "Services/SoftwareUpdate/Decompression/Interface/Precomp.h"
#include <OSPL_memory.h>
#include "FreeRTOS.h"

#include <stdlib.h>

#include "Services/SoftwareUpdate/Decompression/Interface/7zAlloc.h"

#ifndef NDEBUG
// #include "system_dbug.h"
/* use _SZ_ALLOC_DEBUG to debug alloc/free operations */
// #define _SZ_ALLOC_DEBUG
#endif


#ifdef _SZ_ALLOC_DEBUG
#include <stdio.h>
int g_allocCount = 0;
int g_allocCountTemp = 0;
#endif

void *SzAlloc(ISzAllocPtr p, size_t size)
{
  UNUSED_VAR(p);
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  DBUG_LOG_INFO( "\nAlloc %10u bytes; count = %10d", (unsigned)size, g_allocCount);
  //fprintf(stderr, "\nAlloc %10u bytes; count = %10d", (unsigned)size, g_allocCount);
  
  g_allocCount++;
  #endif
  
  return pvPortMallocSlow(size);
}

void SzFree(ISzAllocPtr p, void *address)
{
  UNUSED_VAR(p);
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
  {
    g_allocCount--;
    DBUG_LOG_INFO("\nFree; count = %10d", g_allocCount);
    //fprintf(stderr, "\nFree; count = %10d", g_allocCount
  }
  #endif
  vPortFreeSlow(address);
}

void *SzAllocTemp(ISzAllocPtr p, size_t size)
{
  UNUSED_VAR(p);
  if (size == 0)
    return 0;
  #ifdef _SZ_ALLOC_DEBUG
  //fprintf(stderr, "\nAlloc_temp %10u bytes;  count = %10d", (unsigned)size, g_allocCountTemp);
  DBUG_LOG_INFO("\nAlloc_temp %10u bytes;  count = %10d", (unsigned)size, g_allocCountTemp);
  g_allocCountTemp++;
  #endif
  return pvPortMallocSlow(size);
}

void SzFreeTemp(ISzAllocPtr p, void *address)
{
  UNUSED_VAR(p);
  #ifdef _SZ_ALLOC_DEBUG
  if (address != 0)
  {
    g_allocCountTemp--;
    DBUG_LOG_INFO("\nFree_temp; count = %10d", g_allocCountTemp);
  }
  #endif
  vPortFreeSlow(address);
}
