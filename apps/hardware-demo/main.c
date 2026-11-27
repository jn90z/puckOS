#define _POSIX_C_SOURCE 200809L
#include "display.h"
#include "input.h"
#include "audio.h"
#include "power.h"
#include <math.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
static volatile sig_atomic_t stop;
static void quit(int sig){(void)sig;stop=1;}
static uint32_t pixels[480*640];
static uint32_t hue(double h){h=fmod(h+3600,360)/60;int i=(int)h;unsigned t=(unsigned)((h-i)*255);unsigned r=0,g=0,b=0;switch(i){case 0:r=255;g=t;break;case 1:r=255-t;g=255;break;case 2:g=255;b=t;break;case 3:g=255-t;b=255;break;case 4:r=t;b=255;break;default:r=255;b=255-t;}return 0xff000000u|(r<<16)|(g<<8)|b;}
static void glyph(int x,int y,const unsigned char *rows,uint32_t c){for(int j=0;j<7;j++)for(int i=0;i<5;i++)if(rows[j]&(1<<(4-i)))for(int a=0;a<5;a++)for(int b=0;b<5;b++)pixels[(y+j*5+a)*480+x+i*5+b]=c;}
static void render(double h,int selected){uint32_t c=hue(h);for(int y=0;y<640;y++)for(int x=0;x<480;x++){double dx=x-240,dy=y-320,r=sqrt(dx*dx+dy*dy);double a=atan2(dy,dx)*180/3.141592653589793+360;double delta=fabs(fmod(a-h+540,360)-180);uint32_t p=0xff080e16;if(r>188&&r<211)p=hue(a);if(r>181&&r<218&&delta<3)p=0xffffffff;if(dx*dx+(dy+65)*(dy+65)<45*45)p=c;if(selected&&r>155&&r<159)p=c;pixels[y*480+x]=p;}
static const unsigned char letters[6][7]={{30,17,17,30,16,16,16},{0,0,17,17,17,19,13},{0,0,14,16,16,16,14},{16,16,18,20,24,20,18},{14,17,17,17,17,17,14},{15,16,16,14,1,1,30}};
for(int i=0;i<6;i++)glyph(152+i*30,330,letters[i],i<4?0xffffffff:c);}
static long ms(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec*1000L+t.tv_nsec/1000000;}
int main(int argc,char **argv){int sound=0;for(int i=1;i<argc;i++){if(!strcmp(argv[i],"--self-test")){render(185,0);puts("PuckOS demo render smoke test OK");return 0;}if(!strcmp(argv[i],"--sound"))sound=1;else{fprintf(stderr,"Usage: puckos-hardware-demo [--sound]\n");return 2;}}
signal(SIGINT,quit);signal(SIGTERM,quit);signal(SIGHUP,quit);struct puck_display d;if(puck_display_open(&d)){fprintf(stderr,"Unsupported or inaccessible framebuffer\n");return 1;}int ring=puck_input_open("2280000.adc"),button=puck_input_open("gpio-keys");if(ring<0||button<0){fprintf(stderr,"Input devices unavailable\n");if(ring>=0)close(ring);if(button>=0)close(button);puck_display_close(&d);return 1;}
struct pollfd p[2]={{ring,POLLIN,0},{button,POLLIN,0}};double h=185;int selected=0;long start=ms(),pressed=-1,tone_until=0,last=0;unsigned frames=0;printf("Rotate for color, press to select, hold 2 seconds to exit. 30 second maximum. Battery %ld uV\n",puck_battery_uv());fflush(stdout);
while(!stop&&ms()-start<30000){poll(p,2,20);struct input_event e;while(puck_input_read(ring,&e))if(e.type==EV_REL&&e.code==REL_DIAL){h=fmod(h+e.value*0.15+3600,360);selected=0;}while(puck_input_read(button,&e))if(e.type==EV_KEY&&e.code==KEY_ENTER){if(e.value==1){pressed=ms();selected=!selected;if(sound&&puck_audio_tone(880)==0)tone_until=ms()+100;}else if(e.value==0)pressed=-1;}long now=ms();if(pressed>=0&&now-pressed>=2000)break;if(tone_until&&now>=tone_until){puck_audio_tone(0);tone_until=0;}if(now-last>=33){render(h,selected);if(puck_display_draw(&d,pixels))break;frames++;last=now;}}
if(sound)puck_audio_tone(0);close(ring);close(button);puck_display_close(&d);printf("Demo ended; framebuffer restore attempted. Average FPS %.1f\n",frames*1000.0/fmax(1,ms()-start));return 0;}
