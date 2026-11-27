#pragma once
#include <stdint.h>
#include <stddef.h>
#include <linux/fb.h>
struct puck_display {int fd;struct fb_var_screeninfo v;struct fb_fix_screeninfo f;unsigned char *map,*saved;size_t len;};
int puck_display_open(struct puck_display *d);
int puck_display_draw(struct puck_display *d,const uint32_t *pixels);
void puck_display_close(struct puck_display *d);
