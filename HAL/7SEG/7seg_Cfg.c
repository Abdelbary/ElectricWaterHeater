#include "7seg.h"
#include "7seg_Cfg.h"


#define SEVEN_SEG_ONE_SELECTOR_PIN   SEVEN_SEG_ONE_SELECTOR
#define SEVEN_SEG_TWO_SELECTOR_PIN   SEVEN_SEG_TWO_SELECTOR
#define SEVEN_SEG_ONE_CONFGI_INDEX   SEVEN_SEG_CONGIF
#define SEVEN_SEG_TWO_CONFGI_INDEX   SEVEN_SEG_CONGIF


const sevenSegConfigStruct sevenSegConfigParam[SEVEN_SEG_NUM] =
{
    {
        SEVEN_SEG_ONE_SELECTOR,
        SEVEN_SEG_CONGIF
    },
    {
        SEVEN_SEG_TWO_SELECTOR,
        SEVEN_SEG_CONGIF
    }
};
