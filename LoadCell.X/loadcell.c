#include "loadcell.h"
#include "./mcc_generated_files/mcc.h"

#define CELL_CLK 0x00
#define CELL_DAT 0x00


/**
 * LoadCell clock pin set low
 */
void set_CELL_CLK_LOW(){ LATA &= ~ CELL_CLK;}

/**
 * LoadCell clock pin set high
 */
void set_CELL_CLK_HIGH(){ LATA |= CELL_CLK;}


uint8_t get_CELL_DAT_VAL(){
    if(CELL_DAT == 0x01){ return PORTAbits.RA0; }
    if(CELL_DAT == 0x02){ return PORTAbits.RA1; }
    if(CELL_DAT == 0x04){ return PORTAbits.RA2; }
    if(CELL_DAT == 0x08){ return PORTAbits.RA3; }
    if(CELL_DAT == 0x10){ return PORTAbits.RA4; }
    if(CELL_DAT == 0x20){ return PORTAbits.RA5; }
    if(CELL_DAT == 0x40){ return PORTAbits.RA6; }
    if(CELL_DAT == 0x80){ return PORTAbits.RA7; }
    
    return 0;
}

long get_scale_val(uint8_t n)
{
    set_CELL_CLK_LOW();
    
    long weight_count = 0;
    long weight_add = 0;
    for (uint8_t j = 0; j < n; j++)
    {
        while (get_CELL_DAT_VAL() == 0)
            ;
        while (get_CELL_DAT_VAL() == 1)
            ;
        __delay_us(10);
        
        uint8_t i;
        for (i = 0; i < 24; i++){ 
            weight_count <<= 1;
            set_CELL_CLK_HIGH();
            __delay_us(10);
            set_CELL_CLK_LOW();
            weight_count += get_CELL_DAT_VAL();
            __delay_us(10);
        }
        
        for (i = 0; i < 1; i++){ //i=1:ChAGain128　2:ChBGain32　3:ChAGain64
            set_CELL_CLK_HIGH();
            __delay_us(10);
            set_CELL_CLK_LOW();
            __delay_us(10);
        }
        
        weight_add += weight_count;
        weight_count = 0;
    }
    weight_count = weight_add / n;
    return weight_count;
}

float scale_convert_gram(signed long count)
{ //ADCのカウント値をグラムに変換
    float temp = count - weight_zero;
    temp = temp / 3035;
    return temp;
}