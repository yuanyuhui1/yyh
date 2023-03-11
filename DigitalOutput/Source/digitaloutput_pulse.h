#ifndef __DIGITALOUTPUT_PULSE_
#define __DIGITALOUTPUT_PULSE_

// Numerical definitions
#define SHORTPULSE                         1
#define LONGPULSE                          2

// Numerical definitions
#define ZERO_PULSE_OUTPUT                  0
#define PULSE_OUTPUT_COUNT_LIMIT           4294967295    // 2^32-1
#define UPDATE_INTERVAL                    100.0f
#define SECONDS_PER_MILLISECOND            0.001f   // so we can express pulse-width in seconds
#define PROPORTION_OF_MAX_FREQUENCY        (((TDOUBLE)PULSE_OUTPUT_COUNT_LIMIT + 1.0) * UPDATE_INTERVAL * SECONDS_PER_MILLISECOND)
#define MIN_GAP_TIME                       0.1f
#define MAX_PULSE_TIME                     (60.0f * 1000)  //60s
#define PULSE_RANGE_PERCENT                1.03f 

//1byte
// Enumeration to describe the different states of operation of the digital output
typedef enum
{
    NO_PULSES         = 0,
    NORMAL_PULSES     = 1,
    LONG_PULSE        = 2,
    LONG_GAP          = 3
} TPulseOutputAction;
//26bytes
typedef struct _TPulseOutputData
{
    TUSIGN8     pulseType;              // Short or Long pulse
    TUSIGN8     pulseWidthChange;
    
    TUSIGN32    pulseFwdCountLow;       // (1) forward roll-over count (lower 32 bit)
    TUSIGN32    pulseAddCountLow;       // (2) value added to (1) or (2) each Update ISR (lower 32 bit)
    TUSIGN16    pulseWidthCountTarget;  // (4) value to be loaded to (3)

    TUSIGN32    pulseFwdCountHigh;      // (7) forward roll-over count (upper 32 bit)
    TUSIGN32    pulseAddCountHigh;      // (8) value added to (7) each Update ISR (upper 32 bit)
    TFLOAT      pulseWidth;             // (9) actual pulse width in milli seconds
    TUSIGN16    pulseWidthFreqCount;    // (10)counter to be loaded for long pulse-width timer

} TPulseOutputData;

extern TPulseOutputData gPulseOutput;
extern volatile TPulseOutputAction pulseOutputAction;
extern TBOOL GetQmaxData(TUSIGN8 ConnectedIdx);
extern void UpdatePulseOutput(void);
extern TUSIGN16 UpdatePulse_DIGITALOUTPUT(TFLOAT deviceValue);

#endif