#include "common.h"

void putchar(char ch);

void *memcpy(void *dst, const void *src, size_t n){ //return void pointer
  uint8_t *d = (uint8_t *) dst;
  const uint8_t *s = (const uint8_t *) src;
  while (n--) *d++ = *s++;
  return dst;
}

void *memset(void *buf, char c, size_t n){
  uint8_t *p = (uint8_t *) buf;
  while (n--) *p++ = c;
  return buf;
}

char *strcpy(char *dst, const char *src){
  char *d = dst;
  while (*src) *d++ = *src++;
  *d = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2){
  while (*s1 && *s2){
    if (*s1 != *s2){
      break;
    }
    *s1++;
    *s2++;
  }
  return (unsigned int *)s1 - (unsigned int *)s2;
}

void printf(const char *fmt, ...){  //fmt is the actual string, while ... for other arguments
  va_list vargs;
  va_start(vargs, fmt); // vargs = first arg after the string
  
  while(*fmt){
    if (*fmt == '%'){
      fmt++; //skip '%'
      switch (*fmt) {
        case '\0': //'%' at the end of the string (fmt) => print it
          putchar('%');
          goto end;
        case '%':
          putchar('%');
          break;
        case 's': {
                const char *s =va_arg(vargs, const char *);
                while (*s) {
                  putchar(*s);
                  s++;
                }
                break;
              }
        case 'd':{
                   int value = va_arg(vargs, int);
                   unsigned magnitude = value; 
                   if (value < 0){
                     putchar('-');
                     magnitude = -magnitude;
                   }

                   unsigned divisor = 1;
                   while (magnitude / divisor > 9) divisor *= 10;

                   while(divisor > 0) {
                     putchar('0' + magnitude / divisor);
                     magnitude %= divisor;
                     divisor /= 10;
                   }
                   break;
                 }
          case 'x':{
                     unsigned value = va_arg(vargs, unsigned); 
                     for (int i = 7; i>= 0; i--){ // 64 bit system
                        unsigned nibble = (value >> (i * 4)) & 0xf;
                        putchar("0123456789abcdef"[nibble]);
                     }
                   }
      }
    } else {
      putchar(*fmt);
    }

    fmt++;
  }

end:
  va_end(vargs); //free vargs
}
