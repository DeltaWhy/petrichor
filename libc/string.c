#include <string.h>
#include <stdbool.h>

/**
 * Copies no more than n bytes from src to dest, stopping when the character c
 * is found
 * @return A pointer to the next character in dest after c, or NULL if c was not
 * found in the first n characters of src.
 */
void *memccpy(void *dest, const void *src, int c, size_t n)
{
    const unsigned char *s = (const unsigned char *)src;
    unsigned char *d = (unsigned char *)dest;
    while (n != 0 && *s != (unsigned char)c) {
        *d = *s;
        d++;
        s++;
        n--;
    }
    if (n != 0 && *s == (unsigned char)c) {
        *d = *s;
        return d+1;
    } else {
        return NULL;
    }
}

/**
 * Scans the initial n bytes of s for the first instance of c, interpreted as
 * unsigned char.
 * @return A pointer to the matching byte or NULL
 */
void *memchr(const void *s, int c, size_t n)
{
    const unsigned char *ss = (const unsigned char *)s;
    while (n != 0 && *ss != (unsigned char)c) {
        ss++;
        n--;
    }
    if (n != 0 && *ss == (unsigned char)c) {
        return (void *)ss;
    } else {
        return NULL;
    }
}

/**
 * Compares the first n bytes of s1 and s2.
 * @return An integer less than, equal to, or greater than zero if the first n
 * bytes of s1 is found, respectively, to be less than, to match, or be greater
 * than the first n bytes of s2.
 */
int memcmp(const void *s1, const void *s2, size_t n)
{
    const unsigned char *ss1 = (const unsigned char *)s1;
    const unsigned char *ss2 = (const unsigned char *)s2;
    while (n != 0) {
        if (*ss1 < *ss2) return -1;
        if (*ss1 > *ss2) return 1;
        ss1++;
        ss2++;
        n--;
    }
    return 0;
}

/**
 * Copies n bytes from src to dest. Memory areas must not overlap.
 * @return A pointer to dest.
 */
void *memcpy(void *dest, const void *src, size_t n)
{
    const unsigned char *s = (const unsigned char *)src;
    unsigned char *d = (unsigned char *)dest;
    while (n != 0) {
        *d = *s;
        d++;
        s++;
        n--;
    }
    return dest;
}

/**
 * Copies n bytes from src to dest, memory areas may overlap.
 */
void *memmove(void *dest, const void *src, size_t n)
{
    // This is probably a bad implementation - it can most likely be done
    // without allocating a temp buffer, and that temp buffer really doesn't
    // belong on the stack.
    unsigned char tmp[n]; //apparently you can do this in C99
    memcpy(tmp, src, n);
    memcpy(dest, tmp, n);
    return dest;
}

/**
 * Fills the first n bytes of s with the constant byte c.
 * @return pointer to s
 */
void *memset(void *s, int c, size_t n)
{
    unsigned char *ss = (unsigned char *)s;
    while (n != 0) {
        *ss = (unsigned char)c;
        ss++;
        n--;
    }
    return s;
}

/**
 * Appends src to dest, overwriting the terminating null byte at the end of
 * dest, and then adds a terminating null byte.
 * @return A pointer to dest.
 */
char *strcat(char *dest, const char *src)
{
    char *p = dest;
    while (*p) p++;
    while (*src) {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';
    return dest;
}

/**
 * Appends at most n bytes from src to dest.
 */
char *strncat(char *dest, const char *src, size_t n)
{
    char *p = dest;
    while (*p) p++;
    while (*src && n != 0) {
        *p = *src;
        p++;
        src++;
        n--;
    }
    *p = '\0';
    return dest;
}

/**
 * Returns a pointer to the first occurence of the character c in s.
 */
char *strchr(const char *s, int c)
{
    while (*s != (char)c && *s != '\0') s++;
    if (*s == c) return (char *)s;
    return NULL;
}

/**
 * Returns a pointer to the last occurence of the character c in s.
 */
char *strrchr(const char *s, int c)
{
    const char *p = s;
    while (*p) p++;
    while (*p != (char)c && p >= s) p--;
    if (p < s) return NULL;
    return (char *)p;
}

/**
 * Compares s1 and s2.
 */
int strcmp(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        if (*s1 < *s2) return -1;
        if (*s1 > *s2) return 1;
        s1++;
        s2++;
    }
    if (*s1 < *s2) return -1;
    if (*s1 > *s2) return 1;
    return 0;
}

/**
 * Compares at most the first n bytes of s1 and s2.
 */
int strncmp(const char *s1, const char *s2, size_t n)
{
    while (*s1 && *s2 && n != 0) {
        if (*s1 < *s2) return -1;
        if (*s1 > *s2) return 1;
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    if (*s1 < *s2) return -1;
    if (*s1 > *s2) return 1;
    return 0;
}

/**
 * Compares s1 and s2 in the current locale.
 */
int strcoll(const char *s1, const char *s2)
{
    // STUB - we don't have locales
    return strcmp(s1, s2);
}

/**
 * Copies src, including the terminating null byte, to dest. Strings may not
 * overlap.
 */
char *strcpy(char *dest, const char *src)
{
    char *p = dest;
    while (*src) {
        *p = *src;
        p++;
        src++;
    }
    *p = '\0';
    return dest;
}

/**
 * Copies at most n bytes of src to dest.
 * If there is no null byte among the first n bytes of src, dest will not be
 * null-terminated.
 * If the length of src is less than n, writes additional null bytes to dest to
 * ensure that a total of n bytes are written.
 */
char *strncpy(char *dest, const char *src, size_t n)
{
    char *p = dest;
    while (*src && n != 0) {
        *p = *src;
        p++;
        src++;
        n--;
    }
    while (n != 0) {
        *p = '\0';
        p++;
        n--;
    }
    return dest;
}

/**
 * Returns the number of bytes in the initial segment of s which are in the
 * string accept.
 */
size_t strspn(const char *s, const char *accept)
{
    size_t n = 0;
    size_t len = strlen(accept);
    while (*s) {
        bool ok = false;
        for (size_t i=0; i < len; i++) {
            if (*s == accept[i]) {
                ok = true;
                break;
            }
        }
        if (!ok) return n;
        s++;
        n++;
    }
    return n;
}

/**
 * Returns the number of bytes in the initial segment of s which are not in the
 * string reject.
 */
size_t strcspn(const char *s, const char *reject)
{
    size_t n = 0;
    size_t len = strlen(reject);
    while (*s) {
        for (size_t i=0; i < len; i++) {
            if (*s == reject[i]) return n;
        }
        s++;
        n++;
    }
    return n;
}

/**
 * Calculate the length of a string, excluding the terminating null byte.
 */
size_t strlen(const char *s)
{
    size_t n = 0;
    while (*s) {
        s++;
        n++;
    }
    return n;
}

/**
 * Locates the first occurrence in s of any of the bytes in the string accept.
 */
char *strpbrk(const char *s, const char *accept)
{
    size_t len = strlen(accept);
    while (*s) {
        for (size_t i=0; i < len; i++) {
            if (*s == accept[i]) {
                return (char *)s;
            }
        }
        s++;
    }
    return NULL;
}

/**
 * Finds the first occurrence of needle in haystack.
 */
char *strstr(const char *haystack, const char *needle)
{
    size_t len = strlen(needle);
    if (len == 0) return (char *)haystack;
    while (*haystack) {
        bool found = true;
        for (size_t i=0; i<len; i++) {
            if (haystack[i] == '\0' || haystack[i] != needle[i]) {
                found = false;
                break;
            }
        }
        if (found) return (char *)haystack;
        haystack++;
    }
    return NULL;
}

char *strtok(char *str, const char *delim)
{
    static char *saveptr = NULL;
    char *tok = strtok_r(str, delim, &saveptr);

    if (!tok) saveptr = NULL;
    return tok;
}

char *strtok_r(char *str, const char *delim, char **saveptr)
{
    int i, j;

    if (str == NULL) str = *saveptr;
    if (str == NULL) return NULL;

    // skip delimiters
    while (*str != '\0') {
        bool isdelim = false;
        for (i=0; delim[i] != '\0'; i++) {
            if (*str == delim[i]) {
                isdelim = true;
                break;
            }
        }
        if (isdelim) {
            str++;
        } else {
            break;
        }
    }

    // find next delimiter
    for (i=0; str[i] != '\0'; i++) {
        bool isdelim = false;
        for (j=0; delim[j] != '\0'; j++) {
            if (str[i] == delim[j]) {
                isdelim = true;
                break;
            }
        }
        if (isdelim) {
            break;
        }
    }
    if (str[i] == '\0') {
        // last token
        *saveptr = NULL;
    } else {
        str[i] = '\0';
        *saveptr = str+i+1;
    }
    return str;
}
