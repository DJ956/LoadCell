#include <xc.h>
#include <stdint.h>

#define LCD_ADD 0x3F
    
#define En 0b00000100
#define Rs 0b00000001

void lcd_init();
void lcd_backlight();    
void lcd_cmd(uint8_t cmd);
void lcd_clear();
void lcd_set_cursor(uint8_t col, uint8_t row);
void lcd_print(char *str);
    
void command(uint8_t val, uint8_t mode);