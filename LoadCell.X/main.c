#include "mcc_generated_files/mcc.h"
#include "loadcell.h"
#include "lcd_lib.h"
#include <stdio.h>

void main(void)
{    
    SYSTEM_Initialize();

    //init pull up
    WPUB = 0x24;
    OPTION_REGbits.nWPUEN = 0;
    
    //set digital mode
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    //RA0 cell clock is output
    //RA3 cell data is input
    TRISA = 0x04;
    //init LATA
    LATA = 0x00;
    LATB = 0x00;
    
    
    //init i2c
    SSP2ADD = 0x13;    
    SSP2CON1 = 0x28;
    SSP2CON2 = 0x0;
    SSP2STAT = 0;
    
    lcd_init();
    lcd_backlight();
    lcd_set_cursor(0, 0);
    
    lcd_print("HIZ");   
    
    __delay_ms(1000);
    
    lcd_set_cursor(3, 0);
    lcd_print("12:00");
    
    weight_zero = get_scale_val(50);

    char str[16];
    while (1)
    {
        weight_dat = get_scale_val(8);
        float weight_gram = scale_convert_gram(weight_dat);
        
        sprintf(str, "%d g", weight_gram);
        lcd_print(str);
    }
}