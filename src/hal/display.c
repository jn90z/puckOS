#include "display.h"
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
void puck_display_close(struct puck_display *d){if(d->map&&d->map!=MAP_FAILED){if(d->saved)memcpy(d->map,d->saved,d->len);munmap(d->map,d->len);}free(d->saved);if(d->fd>=0)close(d->fd);}
int puck_display_open(struct puck_display *d){memset(d,0,sizeof *d);d->fd=open("/dev/fb0",O_RDWR);if(d->fd<0)return -1;if(ioctl(d->fd,FBIOGET_VSCREENINFO,&d->v)||ioctl(d->fd,FBIOGET_FSCREENINFO,&d->f))goto fail;
struct fb_var_screeninfo *v=&d->v;
if(v->xres!=480||v->yres!=640||v->bits_per_pixel!=32||v->red.offset!=16||v->green.offset!=8||v->blue.offset!=0||v->red.length!=8||v->green.length!=8||v->blue.length!=8||v->xoffset!=0||d->f.line_length<1920||v->yres_virtual<640||v->yoffset>v->yres_virtual-640)goto fail;
d->len=(size_t)d->f.line_length*v->yres_virtual;if(d->len>d->f.smem_len||d->len>16*1024*1024)goto fail;
d->map=mmap(NULL,d->len,PROT_READ|PROT_WRITE,MAP_SHARED,d->fd,0);if(d->map==MAP_FAILED)goto fail;d->saved=malloc(d->len);if(!d->saved)goto fail;memcpy(d->saved,d->map,d->len);return 0;
fail:puck_display_close(d);return -1;}
int puck_display_draw(struct puck_display *d,const uint32_t *p){struct fb_var_screeninfo v;if(ioctl(d->fd,FBIOGET_VSCREENINFO,&v)||v.xres!=480||v.yres!=640||v.xoffset||v.yoffset>d->v.yres_virtual-640)return -1;for(unsigned y=0;y<640;y++)memcpy(d->map+(size_t)(y+v.yoffset)*d->f.line_length,p+y*480,1920);return 0;}
