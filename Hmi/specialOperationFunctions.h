//+@ HMIGen device specific include files @+
// ADD DEVICE SPECIFIC INCLUDED FILES HERE

//-@ HMIGen device specific include files @-


void specialOp_NONE(void);
void specialOp_INH_ENTER_CONFIG(void);
void specialOp_INH_EXIT_CONFIG(void);
void specialOp_INH_RETURN_TO_OPERATOR_PAGE(void);


#ifdef OPTIMIZE_V3_4
    #pragma optimize=size none
#else
    #pragma optimize=z 0
#endif
void specialOp_NONE(void)
{
}

//+@ HMIGen specialOp_INH_ENTER_CONFIG @+
#ifdef OPTIMIZE_V3_4
    #pragma optimize=size none
#else
    #pragma optimize=z 0
#endif
void specialOp_INH_ENTER_CONFIG(void)
{
    // Any specific functions that must be performed when HMI enters configuration mode should be performed here
}
//-@ HMIGen specialOp_INH_ENTER_CONFIG @-

//+@ HMIGen specialOp_INH_EXIT_CONFIG @+
#ifdef OPTIMIZE_V3_4
    #pragma optimize=size none
#else
    #pragma optimize=z 0
#endif
void specialOp_INH_EXIT_CONFIG(void)
{
    // Any specific functions that must be performed when HMI exits configuration mode should be performed here
}
//-@ HMIGen specialOp_INH_EXIT_CONFIG @-

//+@ HMIGen specialOp_INH_RETURN_TO_OPERATOR_PAGE @+
#ifdef OPTIMIZE_V3_4
    #pragma optimize=size none
#else
    #pragma optimize=z 0
#endif
void specialOp_INH_RETURN_TO_OPERATOR_PAGE(void)
{
    // Any specific functions that must be performed when HMI returns to the operator page should be performed here
}
//-@ HMIGen specialOp_INH_RETURN_TO_OPERATOR_PAGE @-



void (*specialOperationTable[])(void) =
{
&specialOp_NONE,
&specialOp_INH_ENTER_CONFIG,
&specialOp_INH_EXIT_CONFIG,
&specialOp_INH_RETURN_TO_OPERATOR_PAGE,
};


#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
void SpecialOperationExecute(TUSIGN16 specialOpIdx)
{
    if (specialOpIdx <= LAST_IDF_SPECIAL)   {(*specialOperationTable[specialOpIdx])();}
}
