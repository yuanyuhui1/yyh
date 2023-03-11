//TODO file Header



//#define UM_T_UNKNOWN_UNIT_GROUP       255
//#define HART_UM_UKNOWN_UNIT           252

//--------------------------------------------------------------------------------------------------

TBOOL HartConvUnitCodeFromHartUnit_HART(TUSIGN8 * pUnitCode, TUSIGN8 * pUnitTypeCode );
TBOOL HartConvUnitCodeFromSystemUnit_HART(TUSIGN8 * pUnitCode, TUSIGN8 * pUnitTypeCode);

//added 201105 for Burst Input Process

TBOOL HartConvUnitTypeFromSystemUnit_HART(TUSIGN8 * pUnitTypeCode, TUSIGN8 * pHartUnitTypeCode );
TBOOL HartConvUnitTypeFromHartUnit_HART(TUSIGN8 * pHartUnitTypeCode, TUSIGN8 * pUnitTypeCode );

//--------------------------------------------------------------------------------------------------


