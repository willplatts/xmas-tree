#include "samd09c13a.h"
#include "xmastree.h"

uint32_t NextState();
void SetGPIOS(uint32_t GPIOS);
void NextPattern();

// Define structure of LED state
typedef struct {
    uint32_t GPIOMASK;  // GPIOS to set
    uint32_t wait;      // How long to wait on this state
} state_t;

// Define array of state_t to hold patterns
constexpr uint8_t NUM_PATTERNS = 6u;
constexpr uint8_t NUM_STATES = 6u;

const state_t PATTERNS[NUM_PATTERNS][NUM_STATES] = 
{
    // Pattern 1
    {
        {MASK_YEL2_SET, 50u},
        {MASK_RED1_SET, 50u},
        {MASK_GRN2_SET, 50u},
        {MASK_YEL1_SET, 50u},
        {MASK_RED2_SET, 50u},
        {MASK_GRN1_SET, 50u}
    },
    // Pattern 2
    {
        {MASK_GRN1_SET | MASK_GRN2_SET, 100u},
        {0u, 250u},
        {MASK_RED1_SET | MASK_RED2_SET, 100u},
        {0u, 250u},
        {MASK_YEL1_SET | MASK_YEL2_SET, 100u},
        {0u, 250u}
    },
    // Pattern 3 - double flash
    {
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 20u},
        {0u, 20u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 20u},
        {0u, 200u},
        {0u, 0u},
        {0u, 0u}
    },
    // Pattern 4 - ultimate rave
    {
        {MASK_YEL2_SET, 10u},
        {MASK_RED1_SET, 10u},
        {MASK_GRN2_SET, 10u},
        {MASK_YEL1_SET, 10u},
        {MASK_RED2_SET, 10u},
        {MASK_GRN1_SET, 10u}
    },
    // Pattern 5 - all on
    {
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u},
        {MASK_GRN1_SET | MASK_GRN2_SET | MASK_RED1_SET | MASK_RED2_SET | MASK_YEL1_SET | MASK_YEL2_SET, 10u}
    },
    // OFF
    {
        {0,0},
        {0,0},
        {0,0},
        {0,0},
        {0,0},
        {0,0}
    }
};

// Global holding which pattern we are in
uint8_t CUR_PATTERN = 0u;

int main()
{    
    // Set port direction	
    PORT->Group[0].DIR.reg = MASK_GRN1_SET |
                                MASK_GRN2_SET |
                                MASK_RED1_SET |
                                MASK_RED2_SET |
                                MASK_YEL1_SET |
                                MASK_YEL2_SET;
    
    // Enable pin2 input
    PORT->Group[0u].PINCFG[2u].bit.INEN = 1;

    // Led state counter
    uint32_t led_wait = 0u;
    uint32_t led_loops = 0u;
    
    // button debounce
    uint32_t debounce_count = 0u;
    constexpr uint32_t DEBOUNCE_LIMIT = 50u;
    
    // Loop delay
    volatile uint32_t loop_delay;
    constexpr uint32_t LOOP_DELAY = 100u;
    
    for (;;)
    {
        
        
        if (++led_loops >= led_wait)
        {
            led_wait = NextState();
            led_loops = 0u;
        }

        // Check button
        if (PORT->Group[0u].IN.reg & 0x04u)
        {
            // button pressed - add to debounce
            ++debounce_count;
        }
        else
        {
            // button not pressed - clear debounce
            debounce_count = 0u;
        }
        
        if (debounce_count > DEBOUNCE_LIMIT)
        {
            // Wait until button released
            while (PORT->Group[0u].IN.reg & 0x4u) {}
                
            NextPattern();
            
        }
        
        // LOOP DELAY
        for (loop_delay = 0u; loop_delay < LOOP_DELAY; ++loop_delay) {}
        
    }

}

// Move to next state, set LEDS and return how long to wait until calling again
uint32_t NextState()
{
    static uint8_t state = 0u;
    
    // Go to next state
    if (++state >= NUM_STATES) state = 0u;
    
    // Set LEDs appropriately
    SetGPIOS(PATTERNS[CUR_PATTERN][state].GPIOMASK);
    
    return PATTERNS[CUR_PATTERN][state].wait;
}

// Set GPIOS for LEDS
void SetGPIOS(uint32_t GPIOS)
{
    PORT->Group[0].OUT.reg = GPIOS;
}

// Go to next pattern or sleep if we've done them all
void NextPattern()
{
    if (++CUR_PATTERN >= NUM_PATTERNS)
    {        
        CUR_PATTERN = 0u;
    }
}
