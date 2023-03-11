#ifndef __DIGTALOUTPUT_FREQUENCY_
#define __DIGTALOUTPUT_FREQUENCY_


#define ZERO_FREQUENCY   (0.0f)


//12bytes
typedef struct _TFrequencyOutputData
{   // just Timer A4 has two different modes:  (1)Event Counter Mode;(2)Programmable Output Mode
    TUSIGN8     needreinit;
    // Long Period
    TUSIGN16    baseFreqCounter;    // Counter value for Base frequency [in Timer Mode]
    TUSIGN16    NumCounter;         // Number of Counter [in Eventer Counter Mode]
    TUSIGN8     OldTA0Status;
    TUSIGN8     NeedChangeReg;
    TUSIGN8     SpaceNow;
    TUSIGN16    NumCounterMark;     //time div for big frequence,
    TUSIGN16    NumCounterSpace;    //time div for small frequence,
} TFrequencyOutputData;


extern TFrequencyOutputData gFrequencyOutput; // Frequency Output working data
extern TUSIGN8 getNewPeriodType(void);
extern TUSIGN16 UpdateFrequency_DIGITALOUTPUT(TFLOAT deviceValue);
extern void SetShortPeriod(void);
extern void SetLongPeriod(void);
extern void InitFrequencyOutput(void);
void UpdateFrequency(void);

#endif