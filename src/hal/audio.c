#include "audio.h"
#include "input.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
int puck_audio_tone(int hz){if(hz!=0&&(hz<200||hz>2000))return -1;int probe=puck_input_open("pwm-beeper");if(probe<0)return -1;char path[256];if(ioctl(probe,EVIOCGPHYS(sizeof path),path)<0){close(probe);return -1;}close(probe);/* Known diamond3 event1; recheck identity before writing. */int fd=open("/dev/input/event1",O_WRONLY);if(fd<0)return -1;char name[128]={0};if(ioctl(fd,EVIOCGNAME(sizeof name),name)<0||strcmp(name,"pwm-beeper")){close(fd);return -1;}struct input_event e={0};e.type=EV_SND;e.code=SND_TONE;e.value=hz;int ok=write(fd,&e,sizeof e)==sizeof e;close(fd);return ok?0:-1;}
