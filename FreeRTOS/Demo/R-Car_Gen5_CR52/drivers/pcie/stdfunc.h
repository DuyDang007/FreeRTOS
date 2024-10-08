/**********************************************************/
/* Sample program : Standard Functions Header             */
/* File Name      : stdfunc.h                             */
/* Copyright (C) Renesas Electronics Corp. 2024.          */
/**********************************************************/

#pragma once

#include <stddef.h>

char *memcpy(void *s1,void *s2,unsigned long n);
size_t strlen(const char *str);
void *memset(void *s, int c, size_t n);

char *strcat_arm(char *s1, const char *s2);
char *strcpy_arm(char *s1, const char *s2);
char *memcpy_arm(char *s1,const char *s2,unsigned long n);
int32_t memcmp_arm(const int8_t *s1,const int8_t *s2,uint32_t n);
uint32_t strlen_arm(const int8_t *s);
