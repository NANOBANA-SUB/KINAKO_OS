#include "kstring.h"

void *memset(void *buf, char c, size_t n)
{
    uint8_t *p = (uint8_t *) buf;

    while (n--)
    {
        *p++ = c;
    }
    
    return buf;
}

void *memcpy(void *dst, const void *src, size_t n)
{
    uint8_t *dstPtr = (uint8_t *) dst;
    const uint8_t *srcPtr = (const uint8_t *) src;

    while (n--)
    {
        *dstPtr++ = *srcPtr++;
    }
    
    return dstPtr;
}

char *strcpy(char *dst, const char *src)
{
    char *dstPtr = (char *) dst;
    const char *srcPtr = (char *) src;

    while (*srcPtr)
    {
        *dstPtr++ = *srcPtr++;
    }

    *dstPtr = '\0';

    return dstPtr;
}

int strcmp(const char *s1, const char *s2);
{
    while (*s1 && *s2)
    {
        if (*s1 != *s2)
            break;
        
        *s1++;
        *s2++;
    }
    
    return *(unsigned char *)s1 - *(unsigned char *)s2; // POSIXの仕様に合わせる.
}