#ifndef _TAP_H
#define _TAP_H


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ERROR_PRINT_THEN_EXIT(msg...) \
  fprintf(stderr, ##msg);       \
  exit(1);


#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

int tap_alloc(char *dev);


  #endif