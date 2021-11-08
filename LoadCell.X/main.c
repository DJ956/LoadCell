#include "mcc_generated_files/mcc.h"
#include "loadcell.h"
#include "lcd_lib.h"
#include <stdio.h>

char gramstr[16];
char valstr[16];

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
    lcd_set_cursor(0, 1);
    lcd_print("Load cell Unit");
   
    __delay_ms(1000);
    
    weight_zero = get_scale_val(3);
    
    lcd_clear();
    lcd_set_cursor(0,0);
    sprintf(valstr, "%lu val ", weight_zero);
    lcd_print(valstr);
    __delay_ms(500);
    
    lcd_clear();
    lcd_set_cursor(0,0);
    sprintf(valstr, "Divide val: %d", DIVIDE_VAL);
    lcd_print(valstr);
    __delay_ms(1500);
    
    while (1)
    {
        weight_dat = get_scale_val(1);
        float weight_gram = scale_convert_gram(weight_dat);
        
        sprintf(valstr, "%lu val", weight_dat);
        
        sprintf(gramstr, "%3.2f gram", weight_gram);
        
        lcd_clear();
        lcd_set_cursor(0, 0);
        lcd_print(valstr);
        lcd_set_cursor(0, 1);
        lcd_print(gramstr);
        
        __delay_ms(1000);
    }
}