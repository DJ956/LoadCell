#include "mcc_generated_files/mcc.h"
#include "loadcell.h"
#include "lcd_lib.h"
#include <stdio.h>

char str[16];

void main(void)
{    
    SYSTEM_Initialize();
    
    //set digital mode
    ANSELA = 0x00;
    ANSELB = 0x00;
    
    //RA0 cell clock is output
    //RA1 cell data is input
    TRISA = 0x02;
    //init LATA
    LATA = 0x00;
    LATB = 0x00;
    
    
    //init i2c
    SSPADD = 0x13;    
    SSPCON1 = 0x28;
    SSPCON2 = 0x0;
    SSPSTAT = 0;
    
    lcd_init();
    lcd_backlight();
    lcd_set_cursor(0, 0);
    
    
    lcd_print("Initialize");   
   
    __delay_ms(1000);
    
    weight_zero = get_scale_val(50);
    
    __delay_ms(3000);
    
    while (1)
    {
        weight_dat = get_scale_val(8);
        float weight_gram = scale_convert_gram(weight_dat);
        
        sprintf(str, "%3.2f g", weight_gram);
        
        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print(str);
        
        __delay_ms(100);
    }
}