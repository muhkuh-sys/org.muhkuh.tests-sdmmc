#include "interface.h"

#ifndef __BOOT_SDIO_H__
#define __BOOT_SDIO_H__


typedef struct SDIO_BOOT_OPTIONS_STRUCT
{
	unsigned long   ulPowerUpTimeoutTicks;
	unsigned long   ulResetDelayTicks;
	unsigned long   ulInitialSpeedKHz;
	unsigned long   ulMaximumSpeedKHz;
	unsigned long   ulInitialClockGenerationUs;
	unsigned short ausPortControl[8];
//	unsigned char  atFileName[12+1];
	unsigned char   ucHwTimeoutExponentInitialization;
	unsigned char   ucHwTimeoutExponentTransfer;
} SDIO_BOOT_OPTIONS_T;


int boot_sdio(const SDIO_BOOT_OPTIONS_T *ptSdioOptions, const TEST_PARAMETER_T *ptTestParameter);


#endif  /* __BOOT_SDIO_H__ */

