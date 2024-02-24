#include "soc/rtc_cntl_reg.h"
#include "soc/soc_ulp.h"

.text

.global entry
entry:
_config:
    WRITE_RTC_FIELD(RTC_CNTL_EXT_WAKEUP1_REG, RTC_CNTL_EXT_WAKEUP1_SEL, 0b10000000000000000)
_wakeup:
    READ_RTC_FIELD(RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_RDY_FOR_WAKEUP)
    and r0, r0, 1
    jump _wakeup, eq
    READ_RTC_FIELD(RTC_CNTL_EXT_WAKEUP1_STATUS_REG, RTC_CNTL_EXT_WAKEUP1_STATUS)
    and r0, r0, 0b10000000000000000
    jump _wakeup, eq

    wake
    halt