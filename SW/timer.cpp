#include "samd09c13a.h"
#include "xmastree.h"

void InitTC1()
{
    constexpr uint32_t TC1_period = 1000u;
    
    // Enable clock
    //PM->APBCMASK.reg = (0x1u << 6u);
    
    // Reset TC1
    TC1->COUNT32.CTRLA.reg = 0x1u;

    // Enable Match Compare 0 (MC0) interrupt
    TC1->COUNT32.INTENSET.reg = 0x10u;
    
    // Set CC0 (compare value)
    TC1->COUNT32.CC[0].reg = TC1_period;
    
    // Enable Timer
    TC1->COUNT32.CTRLA.reg =    (0x1u << 1u) |  // Enable
                                (0x2u << 2u) |  // 32 bit mode
                                (0x1u << 6u);   // MFQR mode, count up to CC0 
}

void TC1_Handler()
{
    PORT->Group[0].OUTTGL.reg = MASK_YEL2_SET;
}

