#include "DIO.h"
#include "DIO_Cfg.h"

const DioConfigParamType DIO_ConfigParam[DIO_NUM_OF_GROUPS] =
{
    {///SSD1 SELECTOR GRPOPID 0
                PIN4,
                PORT_A,
                OUTPUT,
                PULL_UP
    },
    {///SSD2 SELECTOR GRPOPID 1
                PIN5,
                PORT_A,
                OUTPUT,
                PULL_UP
    },
    { ///SEVEN SEG CONFIGRATION PORT GRPOPID 2
                ALL_PORT_MASK,
                PORT_D,
                OUTPUT,
                PULL_UP
    },
    {///ON/OFF PUSH BTN GRPOPID 3
                PIN0,
                PORT_B,
                INPUT,
                PULL_DOWN
    },
    {///UP PUSH BTN GRPOPID 4
                PIN1,
                PORT_B,
                INPUT,
                PULL_DOWN
        
    },
    {///DOWN PUSH BTN GRPOPID 5
                PIN2,
                PORT_B,
                INPUT,
                PULL_DOWN
        
    },
    {///HEATER ELEMENT GROPID 6
                PIN5,
                PORT_C,
                OUTPUT,
                PULL_DOWN
    },
    {///COOLER ELEMENT GROPID 7
                PIN2,
                PORT_C,
                OUTPUT,
                PULL_DOWN
    },
    {///AN2 GROPID 8
                PIN2,
                PORT_A,
                INPUT,
                PULL_DOWN
    },
    {///AN2 GROPID 9
                PIN3,
                PORT_B,
                OUTPUT,
                PULL_UP
    }
};
