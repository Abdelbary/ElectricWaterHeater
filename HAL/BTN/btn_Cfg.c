#include "btn.h"
#include "btn_Cfg.h"

const BtnConfigType gcae_BUT_ConfigParam[BTN_NUM_OF_BUTTONS] =
{
	{///ON/OFF PUSH BTN GRPOPID 3
		0x03,
		ActiveLow
	},
	{///UP PUSH BTN GRPOPID 4
		0x04,
		ActiveLow
	},
	{///DOWN PUSH BTN GRPOPID 5
		0x05,
		ActiveLow
	}
};