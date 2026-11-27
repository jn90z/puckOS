#include "power.h"
#include <stdio.h>
long puck_battery_uv(void){long v=-1;FILE *f=fopen("/sys/class/power_supply/adc-div-battery/voltage_now","r");if(f){if(fscanf(f,"%ld",&v)!=1)v=-1;fclose(f);}return v;}
