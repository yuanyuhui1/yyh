#ifndef __DIGITALOUTPUT_LOGIC_
#define __DIGITALOUTPUT_LOGIC_


typedef struct _TLogicOutputState
{
    TUSIGN8 onOffCommand;
    TUSIGN8 logicSignalValidation;
} TLogicOutputState;


//18bytes
typedef struct _TLogic
{
    TUSIGN8  FAST* const pSimuEnabled;
    const TUSIGN8  FAST* const pLgcSimCmd;
    const TUSIGN8  FAST* const pLgcSgnlSrc;
    const TUSIGN8  FAST* const pLgcActStte;
    TUSIGN8  FAST* const pOutputLogic;
    TBOOL  *pAvailable;
    TUSIGN8  (*Update)(const struct _TLogic *me,TUSIGN8 doalarm);
}TLogic;

#define CONSTRUCT_TLOGIC(_pSimuEnabled,_pLgcSimCmd,_pLgcSgnlSrc,_pLgcActStte,_pOutputLogic,_pAvailable) \
{   \
    _pSimuEnabled, \
    _pLgcSimCmd,   \
    _pLgcSgnlSrc,  \
    _pLgcActStte,  \
    _pOutputLogic, \
    _pAvailable,   \
    UpdateLogic_DIGITALOUTPUT   \
}

extern TUSIGN8 UpdateNone_DIGITALOUTPUT(const TLogic *me);
extern TUSIGN8 UpdateLogic_DIGITALOUTPUT(const TLogic *me, TUSIGN8 alarmState);

#endif