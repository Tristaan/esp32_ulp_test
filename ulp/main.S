#include "soc/soc_ulp.h"     // for WRITE_RTC_REG
#include "soc/rtc_io_reg.h"  // for RTC_GPIO_*
#include "soc/rtc_cntl_reg.h"

.set gpio_2, 12              // gpio pin 2 is rtc pin 12

.text

.global entry                // ulp start address used by main core program
entry:

  // use digital function, not rtc function
  WRITE_RTC_REG(RTC_IO_TOUCH_PAD2_REG, RTC_IO_TOUCH_PAD2_MUX_SEL_S, 1, 1)

  // gpio_2 shall be output, not input
  WRITE_RTC_REG(RTC_GPIO_OUT_REG, RTC_GPIO_OUT_DATA_S + gpio_2, 1, 1)
  move  r3, 9999

on:
  WRITE_RTC_REG(RTC_GPIO_ENABLE_W1TS_REG, RTC_GPIO_ENABLE_W1TS_S + gpio_2, 1, 1)

  move  r1, 200           // wait in ms
  move  r2, off           // return address
  jump  delay             // call subroutine

off:
  WRITE_RTC_REG(RTC_GPIO_ENABLE_W1TC_REG, RTC_GPIO_ENABLE_W1TC_S + gpio_2, 1, 1)

  move  r1, 800           // wait in ms
  move  r2, on            // return address
  jump  delay             // call subroutine

delay:
  wait  8000              // wait 8000 clock ticks at 8MHz -> 1ms
  sub   r1, r1, 1         // decrement ms count
  jump  r2, eq            // if ms count is zero then return to caller
  sub   r3, r3, 1         // subtract loop count
  jump  exit_0, eq
  jump  delay             // else continue to wait

exit_0:
  // Disable timer wakeup
  WRITE_RTC_REG(RTC_CNTL_STATE0_REG, RTC_CNTL_ULP_CP_SLP_TIMER_EN_S, 1, 0);

exit_1:
  READ_RTC_FIELD(RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_RDY_FOR_WAKEUP)

	or r0, r0, r1
	jump exit_1, eq

  wake
  halt