#include "samd09c13a.h"
#include "xmastree.h"
#include "timer.h"

int main()
{

    //InitTC1();
    
    // Set port direction	
    PORT->Group[0].DIR.reg = MASK_GRN1_SET |
                                MASK_GRN2_SET |
                                MASK_RED1_SET |
                                MASK_RED2_SET |
                                MASK_YEL1_SET |
                                MASK_YEL2_SET;
    
    // Set initial LED state
    PORT->Group[0].OUT.reg = MASK_YEL2_SET;
    
    for (;;)
    {
        
        constexpr uint32_t delay_len = 5000u;
        volatile uint32_t delay;
        
        PORT->Group[0].OUT.reg = MASK_YEL2_SET;
        
        for (delay = 0u; delay < delay_len; ++delay) {}

        PORT->Group[0].OUT.reg = MASK_RED1_SET;
            
        for (delay = 0u; delay < delay_len; ++delay) {}
        
        PORT->Group[0].OUT.reg = MASK_GRN2_SET;
        
        for (delay = 0u; delay < delay_len; ++delay) {}
            
        PORT->Group[0].OUT.reg = MASK_YEL1_SET;
        
        for (delay = 0u; delay < delay_len; ++delay) {}
            
        PORT->Group[0].OUT.reg = MASK_RED2_SET;
        
        for (delay = 0u; delay < delay_len; ++delay) {}
            
        PORT->Group[0].OUT.reg = MASK_GRN1_SET;
        
        for (delay = 0u; delay < delay_len; ++delay) {}
        
    }

}
