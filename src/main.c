
#include "main.h"

#include "portcontrol.h"
#include "rdy_run.h"
#include <string.h>
#include "systime.h"
#include "uprintf.h"
#include "version.h"

#include "boot_sdio.h"


/*-------------------------------------------------------------------------*/


const SDIO_BOOT_OPTIONS_T tSdioOptions =
{
	.ulPowerUpTimeoutTicks = 200000U,    /* The timeout to get the SDIO core out of the power down mode. This is 1ms. */
	.ulResetDelayTicks     = 1000000U,   /* The delay between power enable and reset release. This is 5ms. */
	.ulInitialSpeedKHz = 400,            /* Start with 400kHz. */
	.ulMaximumSpeedKHz = 25000,          /* Allow a maximum of 25MHz. */
	.ulInitialClockGenerationUs = 3032,  /* Delay for 3032us, which is enough for the required 74 clock cycles at 400kHz. */
	.ausPortControl =
	{
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLDOWN50K),    /* SDIO_CLK  */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_CMD  */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_DAT0 */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_DAT1 */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_DAT2 */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_DAT3 */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K),      /* SDIO_CD   */
		PORTCONTROL_CONFIGURATION(REEMUX_0, 0, REEMUX_DRV_08MA, REEMUX_UDC_PULLUP50K)       /* SDIO_WP   */
	},
	.ucHwTimeoutExponentInitialization = 20,
	.ucHwTimeoutExponentTransfer       = 27
};



TEST_RESULT_T test_main(const TEST_PARAMETER_T *ptParameter)
{
	TEST_RESULT_T tResult;
	int iResult;


	/* Be pessimistic. */
	tResult = TEST_RESULT_ERROR;

	systime_init();

	uprintf("\f. *** SD/MMC test by cthelen@hilscher.com ***\n");
	uprintf("V" VERSION_ALL "\n\n");

	/* Switch all LEDs off. */
	rdy_run_setLEDs(RDYRUN_OFF);

	iResult = boot_sdio(&tSdioOptions, ptParameter);

	/* Switch the SYS LED to green if the test was successful. Switch it
	 * to red if an error occurred.
	 */
	if( iResult==0 )
	{
		rdy_run_setLEDs(RDYRUN_GREEN);
		tResult = TEST_RESULT_OK;
	}
	else
	{
		rdy_run_setLEDs(RDYRUN_YELLOW);
		tResult = TEST_RESULT_ERROR;
	}

	/* Return the result. */
	return tResult;
}
