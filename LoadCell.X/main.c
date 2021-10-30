#include "mcc_generated_files/mcc.h"
#include "loadcell.h"

void main(void)
{    
    SYSTEM_Initialize();

    weight_zero = get_scale_val(50);

    
    while (1)
    {
        weight_dat = get_scale_val(8);
        float weight_gram = scale_convert_gram(weight_dat);
    }
}