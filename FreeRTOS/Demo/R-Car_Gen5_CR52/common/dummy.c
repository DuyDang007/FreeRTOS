/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>

//Dummy for these function when using newlib
__attribute__((weak)) int _close(int fd)
{
//  (void)fd;
  return (0);
}
__attribute__((weak)) int _write (int fd, char* buf, int nbytes)
{
    return (0);
}
__attribute__((weak)) int _fstat(int fd, void *buf)
{
  (void)fd;

  return (0);
}

__attribute__((weak)) int _isatty(int fd)
{
  (void)fd;
  return (1);
}
__attribute__((weak)) off_t _lseek(int fd, off_t offset, int whence)
{
  (void)fd;
  (void)offset;
  (void)whence;
  //errno = ESPIPE;
  return ((off_t)-1);
}
__attribute__((weak)) int
_read (int fd, char* buf, int nbytes)
{
}

__attribute__((weak))  pid_t _getpid (void)
{
  return  1;

}

__attribute__((weak)) int _kill (pid_t pid, int sig)
{
  return 1;
}

__attribute__((weak)) void vApplicationTickHook( void )
{
    /* Weak function  */
    /* if want to use this function redefine this function */
}
