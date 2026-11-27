#include "input.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
int puck_input_open(const char *name){for(int i=0;i<32;i++){char p[64],n[256]={0};snprintf(p,sizeof p,"/dev/input/event%d",i);int fd=open(p,O_RDONLY|O_NONBLOCK);if(fd<0)continue;if(ioctl(fd,EVIOCGNAME(sizeof n),n)>=0&&!strcmp(n,name))return fd;close(fd);}return -1;}
int puck_input_read(int fd,struct input_event *e){return read(fd,e,sizeof *e)==sizeof *e;}
