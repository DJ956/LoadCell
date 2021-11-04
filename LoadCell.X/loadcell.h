#include <xc.h>
#include <stdint.h>

//RA0
#define CELL_CLK 0x01
//RA1
#define CELL_DAT 0x02

//DIVIDE value
#define DIVIDE_VAL 486

signed long weight_dat;
signed long weight_zero;

/**
 * get scale value
 * @param scale count
 * @return 
 */
long get_scale_val(uint8_t n);

/**
 * scale value to gram value
 * @param count
 * @return gram value
 */
float scale_convert_gram(signed long count);