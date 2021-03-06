
#ifndef HEADER_H
#define	HEADER_H


//#define level_on_interrupt      //Enable gpio_ext interrupt to get level, else get level with timer

/*GENERAL*/
//#define FALSE 0
//#define TRUE  !FALSE

#include <xc.h>

enum {OFF, ON};

typedef enum{false, true} boolean;

typedef char                int8_t;
typedef short               int16_t;
typedef int                 int32_t;
typedef long long           int64_t;

typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;

void init(void);

#include    "bargraph.h"
#include    "gpio_exp.h"
#include    "I2C.h"
#include    "lcd.h"
#include    "led.h"
#include    "level.h"
#include    "display.h"
#include    "light.h"
#include    "radio.h"
#include    "adc.h"
#include    "rtcc.h"


/*Moisture sensor*/
#define max_ctmu 950
#define min_ctmu 0

extern int	I_can_check_sensors;

#endif