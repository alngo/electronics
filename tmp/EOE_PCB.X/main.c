
/*
 * HARDWARE:
 *              RA0:	AN0         Lumistance
 *              RA1:	GPIO		SER         (yellow)
 *              RA2:	GPIO		RCLK		(orange)
 *              RA3:	GPIO		SRCLK		(brown)
 *              RA4:	SOSCO		RTCC		(quartz 32.768kHz)
 *
 *              RB0:                ICD3
 *              RB1:	GPIO(CN)	IRQ         (red)
 *              RB2:	GPIO(ODC)	LED_R		(red)
 *              RB3:	GPIO(ODC)	LED_G		(green)
 *              RB4:	SOSCI		RTCC		(quartz 32.768kHz)
 *              RB5:	GPIO(ODC)	LED_B		(blue)
 *              RB6:	GPIO		CE      	(brown)
 *              RB7:    INT0		INTA		(purple)
 *              RB8:    SCL1		I2C         (green)
 *              RB9:    SDA1		I2C     	(blue)
 *              RB10:	GPIO		One-Wire	(white)
 *              RB11:	SDI1		SPI     	(blue)
 *              RB12:	AN12		CTMU		(red)
 *              RB13:	SDO1		SPI     	(green)
 *              RB14:	SCK1		SPI     	(orange)
 *              RB15:	SS1     	SPI         (yellow)
 *
 * SOFTWARE:
 *              TIMER1:		CTMU / 1_wire
 *              TIMER2:         interupt
 *              TIMER3:		Alert led blink
 *              TIMER4:		delay_micro_sec
 *              TIMER5:		General purpose timer
 */

/*TODO
 * redefinir tous les delais en fonction des valeurs recommandees
 * get date and time from rpi in init_rtcc
 *
 *
 */

// PIC32MX170F256B Configuration Bit Settings

// 'C' source line config statements

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF            // Peripheral Module Disable Configuration (Allow only one reconfiguration)
#pragma config IOL1WAY = OFF             // Peripheral Pin Select Configuration (Allow only one reconfiguration)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc w/Div-by-N (FRCDIV))
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Enabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Enabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_4           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/8)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include "header.h"
#include "stock_value.h"

/*Global variables*/
uint8_t     I_can_check_sensors;    //lomg terme checking sensors
uint8_t     I_can_display;          //pour afficher 10 sec
uint8_t     g_mon_sleep = 0;            //global monitor sleeping
uint16_t    lum_manual;             //luminosity value
float       Temperature;
uint8_t     level = 0;
int16_t     lum_sleep;               //luminosity value while sleeping (thresold wake up)
uint16_t    humidity;               //current hum val
t_save      tab_data[336];
uint16_t    nb_save = 0;
uint8_t     SLEEPON = 0;
void    save_data();

void    init(void)
{
    disable_interrupt();       //disable interrupts while initialization
    init_gpio();        //0k

    init_rtcc();
    init_led();         //0k
    init_delay();
    init_interrupt();
 
    init_bargraph();    //0k

//    init_I2C_soft();
    init_gpio_exp();
//    init_level();
    init_lcd();
//    init_display();

    init_manual_adc();    //0k
    init_light();         //0k
    init_moisture();      //0k
    init_temp();          //?? (Morgane doit verifier si la valeur de Temperature est valide)

//    init_auto_adc();
//    init_sleep();

//    init_pump();

//    init_spi();
//    init_radio();

    init_watchdog();
}

void        get_sensors()
{
                        //check_level();  //ca put du cul il ecrit nimp sur lecran grr   (commentaire epic!)
                        //check_moisture();
			//check_temp();
                        get_light_manual();
                        //save_data();
}

void        display_sensors()
{
                        //check_level();  //ca put du cul il ecrit nimp sur lecran grr   (commentaire epic!)
                        //check_moisture();
			//check_temp();
                        get_light_manual();

//                      radio_test();
//                      pump_on_off();
                        display_update();
}

uint8_t ret = 0;
void    main(void)
{
    init();

    led_alert(BLU_BIT | GRE_BIT | RED_BIT);
    bargraph_write(0b01100000000000000011);

//    I_can_check_sensors = 1;
//    I_can_display = 1;
//    lcd_backlight_inv();
    while(1)
    {
//          get_light_manual();
//          check_moisture();
//          check_temp();

//gpio_exp_write_byte_to_reg_soft(REG_OLATA, 0b10101010);
//ret = gpio_exp_read_byte_from_reg_soft(REG_OLATA);
//ret = gpio_exp_read_byte_from_reg(REG_IOCONA);
//ret = gpio_exp_read_byte_from_reg(REG_GPIOA);
//ret = gpio_exp_read_byte_from_reg(REG_OLATA);
//ret = gpio_exp_read_byte_from_reg(REG_IODIRA);
//        display_sensors();
//        while (I_can_display)
//        {
//            display_sensors();
//        }
//        if (I_can_check_sensors)
//        {
//            get_sensors();
//            I_can_check_sensors = 0;
//        }
//        if(g_mon_sleep)
//        {
//            if (!SLEEPON)
//                sleep();
//           __asm("wait");
//        }
     //   display_update();
        WDTCONSET = 0x0001;	//reset watchdog
    }
}