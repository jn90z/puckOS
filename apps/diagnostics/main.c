#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <poll.h>
#include <sys/utsname.h>
static void show(const char *p){char b[512]; FILE *f=fopen(p,"r"); printf("\n--- %s ---\n",p); if(!f){perror(p);return;} while(fgets(b,sizeof b,f)) fputs(b,stdout); fclose(f);}
int main(int argc,char **argv){
 if(argc==2 && !strcmp(argv[1],"--self-test")){puts("PuckOS diagnostics 0.1.0");return 0;}
 struct utsname u; if(!uname(&u)) printf("PuckOS diagnostics 0.1.0: %s %s\n",u.release,u.machine);
 const char *paths[]={"/proc/cpuinfo","/proc/meminfo","/proc/mtd","/proc/bus/input/devices","/sys/class/power_supply/adc-div-battery/uevent","/sys/class/power_supply/imx_usb_charger/uevent","/sys/bus/iio/devices/iio:device0/name"};
 for(unsigned i=0;i<sizeof paths/sizeof *paths;i++)show(paths[i]);
 int fd=open("/dev/fb0",O_RDONLY); struct fb_var_screeninfo v; struct fb_fix_screeninfo f;
 if(fd>=0){if(!ioctl(fd,FBIOGET_VSCREENINFO,&v)&&!ioctl(fd,FBIOGET_FSCREENINFO,&f))printf("\nLCD %ux%u virtual %ux%u bpp %u stride %u RGB offsets %u/%u/%u\n",v.xres,v.yres,v.xres_virtual,v.yres_virtual,v.bits_per_pixel,f.line_length,v.red.offset,v.green.offset,v.blue.offset);else perror("framebuffer ioctl");close(fd);}else perror("framebuffer");
 if(argc==2&&!strcmp(argv[1],"--input")){struct pollfd p[2]={{open("/dev/input/event2",O_RDONLY),POLLIN,0},{open("/dev/input/event3",O_RDONLY),POLLIN,0}}; puts("Rotate and press; Ctrl-C to stop (30s idle timeout)."); while(poll(p,2,30000)>0){for(int i=0;i<2;i++)if(p[i].revents&POLLIN){struct input_event e;if(read(p[i].fd,&e,sizeof e)==sizeof e)printf("event%d type=%u code=%u value=%d\n",i+2,e.type,e.code,e.value);} fflush(stdout);} for(int i=0;i<2;i++)if(p[i].fd>=0)close(p[i].fd);}
 return 0;
}
