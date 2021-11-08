#include "./mcc_generated_files/mcc.h"

#include "i2c.h"
#include "lcd_lib.h"

#define ROW 2
#define COL 16

uint8_t _backlightval;

void lcd_cmd(uint8_t cmd){
    i2c_start();
    i2c_write(LCD_ADD << 1);  
    i2c_write(cmd);
    i2c_stop();    
    __delay_us(10);
}

void lcd_init(){
    __delay_ms(400);
    
    lcd_cmd(0x30);            
    lcd_cmd(0x34);        
    lcd_cmd(0x30);    
    __delay_ms(5);  //write4bits(0x03 << 4)
    
    lcd_cmd(0x30);        
    lcd_cmd(0x34);        
    lcd_cmd(0x30);
    __delay_ms(5);  //write4bits(0x03 << 4)
    
    lcd_cmd(0x30);     
    lcd_cmd(0x34);        
    lcd_cmd(0x30);
    __delay_us(300);  //write4bits(0x03 << 4)
    
    lcd_cmd(0x20);    
    lcd_cmd(0x24);    
    lcd_cmd(0x20);     
    __delay_us(10); //write4bits(0x02 << 4)
    
    //command(0x28)
    lcd_cmd(0x20);    
    lcd_cmd(0x24);    
    lcd_cmd(0x20);  //write4bits(0x20);
    
    __delay_us(100);
    
    lcd_cmd(0x80);    
    lcd_cmd(0x84);    
    lcd_cmd(0x80);  //write4bits(0x80)
    
    //display
    //command(0x0C)
    lcd_cmd(0x00);    
    lcd_cmd(0x04);    
    lcd_cmd(0x00);  //write4bits(0x00);
    
    __delay_us(100);
    
    lcd_cmd(0xC0);    
    lcd_cmd(0xC4);    
    lcd_cmd(0xC0);  //write4bits(0xC0);
    
    //clear
    //command(0x01);
    lcd_cmd(0x00);    
    lcd_cmd(0x04);    
    lcd_cmd(0x00);  //write4bits(0x00);
    
    __delay_us(100);
    
    lcd_cmd(0x10);    
    lcd_cmd(0x14);    
    lcd_cmd(0x10);  //write4bits(0x10);
    
    __delay_ms(5); //takes a long time
    
    //command(0x06);
    lcd_cmd(0x00);    
    lcd_cmd(0x04);    
    lcd_cmd(0x00);  //write4bits(0x00);
    
    __delay_us(100);
    
    lcd_cmd(0x60);    
    lcd_cmd(0x64);    
    lcd_cmd(0x60);  //write4bits(0x60);
    
    //home()
    //command(0x02)
    lcd_cmd(0x00);    
    lcd_cmd(0x04);    
    lcd_cmd(0x00);  //write4bits(0x00);
    
    __delay_us(100);
    
    lcd_cmd(0x20);    
    lcd_cmd(0x24);    
    lcd_cmd(0x20);  //write4bits(0x20);
    
    __delay_ms(3);
}

void lcd_backlight(){
    lcd_cmd(0x08);
    _backlightval = 0x08;
    __delay_us(10);
}

void lcd_clear(){
    for(uint8_t row = 0; row < ROW; row++){
        for(uint8_t col = 0; col < COL; col++){
            lcd_set_cursor(col, row);
            lcd_print(" ");
        }
    }
}

/**
 * 
 * @param col col
 * @param row row
 */
void lcd_set_cursor(uint8_t col, uint8_t row){
    uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    if(row > ROW){
        row = ROW - 1;
    }    
    uint8_t cmd = 0x80 | (col + row_offsets[row]);    
    command(cmd, 0);
}

void lcd_print(char *str) {
    while (*str) {
        uint8_t cmd = *str++;
        command(cmd, Rs);     
    }
}

void command(uint8_t val, uint8_t mode){    
    uint8_t highnib = (val & 0xf0) | _backlightval | mode;
	uint8_t lownib = ((val << 4) & 0xf0) | _backlightval | mode;
    
	lcd_cmd(highnib);
    lcd_cmd(highnib | En);
    lcd_cmd(highnib & ~En);
    
    __delay_us(100);
        
	lcd_cmd(lownib);
    lcd_cmd(lownib | En);
    lcd_cmd(lownib & ~En);  
}