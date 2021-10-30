#include <xc.h>
#include <stdint.h>

//RA1
#define CELL_CLK 0x02
//RA3
#define CELL_DAT 0x08

unsigned short long weight_dat;
unsigned short long weight_zero;

/**
 * get scale value
 * @param scale count
 * @return 
 */
unsigned short long get_scale_val(uint8_t n);

/**
 * scale value to gram value
 * @param count
 * @return gram value
 */
float scale_convert_gram(unsigned short long count);