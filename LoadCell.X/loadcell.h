#include <xc.h>
#include <stdint.h>

//RA0
#define CELL_CLK 0x01
//RA1
#define CELL_DAT 0x02

//DIVIDE value
#define DIVIDE_VAL 493

unsigned long weight_dat;
unsigned long weight_zero;

/**
 * get scale value
 * @param scale count
 * @return 
 */
unsigned long get_scale_val(int n);

/**
 * scale value to gram value
 * @param count
 * @return gram value
 */
float scale_convert_gram(unsigned long count);