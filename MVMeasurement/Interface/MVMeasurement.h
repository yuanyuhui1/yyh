//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2007.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem MVMeasurement
 Module
 Description    Class definition T_UNIT::MVMEASUREMENT

                Subsystem interface 
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before MVMeasurement.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be included before MVMeasurement.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be included before MVMeasurement.h
#endif

#if (T_UNIT_VERSION<0x020300)
  #error T_UNIT_VERSION must be greater than or equal to 2.3.0
#endif

//-------------------------------------------------------------------------------------------------
//!  extend include list if neccessary

#ifdef __MVMEASUREMENT_H__
  #error MVMeasurement.h included more than once
#endif

#define __MVMEASUREMENT_H__

#define MVMEASUREMENT_VERSION 0x010000   // Version 1.0.0

//-------------------------------------------------------------------------------------------------
//!  define macros, enums

#define NOT_SUPPORT_OBJIDX 0XFF
//@SubGen start@  <TYPES>
  // _UNIT_TYPES
  #define MVM_T_TEMPERATURE 9 //SI-Base-Quantities
  #define MVM_T_TIME 5 //
  #define MVM_T_VOLUME 17 //
  #define MVM_T_MASS_DENSITY 25 //Derivatives A
  #define MVM_T_SPECIFIC_VOLUME 27 //Derivatives A
  #define MVM_T_VOLUME_FLOW 39 //Derivatives A
  #define MVM_T_MASS_FLOW 41 //Derivatives A
  #define MVM_T_NORMAL_VOLUME_FLOW 42 //
  #define MVM_T_FREQUENCY 47 //
  #define MVM_T_PRESSURE 51 //Containing special Names
  #define MVM_T_POWER 55 //Containing special Names

  // _T_VOLUME_FLOW
  #define MVM_CUBIC_METER_PER_SECOND 13 //
  #define MVM_CUBIC_METER_PER_MINUTE 14 //
  #define MVM_CUBIC_METER_PER_HOUR 15 //
  #define MVM_CUBIC_METER_PER_DAY 16 //
  #define MVM_CUBIC_FEET_PER_SECOND 29 //
  #define MVM_CUBIC_FEET_PER_MINUTE 30 //
  #define MVM_CUBIC_FEET_PER_HOUR 31 //
  #define MVM_CUBIC_FEET_PER_DAY 32 //
  #define MVM_CUBIC_KILOFEET_PER_SECOND 35 //
  #define MVM_CUBIC_KILOFEET_PER_MINUTE 36 //
  #define MVM_CUBIC_KILOFEET_PER_HOUR 37 //
  #define MVM_CUBIC_KILOFEET_PER_DAY 38 //
  #define MVM_LITER_PER_SECOND 48 //
  #define MVM_LITER_PER_MINUTE 49 //
  #define MVM_LITER_PER_HOUR 50 //
  #define MVM_LITER_PER_DAY 51 //
  #define MVM_KILOLITER_PER_SECOND 52 //
  #define MVM_HECTOLITER_PER_SECOND 56 //
  #define MVM_HECTOLITER_PER_MINUTE 57 //
  #define MVM_HECTOLITER_PER_HOUR 58 //
  #define MVM_HECTOLITER_PER_DAY 59 //
  #define MVM_KILOLITER_PER_MINUTE 60 //
  #define MVM_KILOLITER_PER_HOUR 61 //
  #define MVM_KILOLITER_PER_DAY 62 //
  #define MVM_US_GALLON_PER_SECOND 63 //
  #define MVM_US_GALLON_PER_MINUTE 64 //
  #define MVM_US_GALLON_PER_HOUR 65 //
  #define MVM_US_GALLON_PER_DAY 66 //
  #define MVM_IMPERIAL_GALLON_PER_SECOND 83 //
  #define MVM_IMPERIAL_GALLON_PER_MINUTE 84 //
  #define MVM_IMPERIAL_GALLON_PER_HOUR 85 //
  #define MVM_IMPERIAL_GALLON_PER_DAY 86 //
  #define MVM_BARREL_PER_SECOND 104 //
  #define MVM_BARREL_PER_MINUTE 105 //
  #define MVM_BARREL_PER_HOUR 106 //
  #define MVM_BARREL_PER_DAY 107 //
  #define MVM_BARREL_US_BEER_PER_DAY 127 //
  #define MVM_BARREL_US_BEER_PER_HOUR 128 //
  #define MVM_BARREL_US_BEER_PER_MINUTE 129 //
  #define MVM_BARREL_US_BEER_PER_SECOND 130 //

  // _T_MASS_FLOW
  #define MVM_GRAM_PER_SECOND 1 //g/s
  #define MVM_GRAM_PER_MINUTE 2 //g/min
  #define MVM_GRAM_PER_HOUR 3 //g/h
  #define MVM_KILOGRAM_PER_SECOND 5 //kg/s
  #define MVM_KILOGRAM_PER_MINUTE 6 //kg/min
  #define MVM_KILOGRAM_PER_HOUR 7 //kg/h
  #define MVM_KILOGRAM_PER_DAY 8 //kg/d
  #define MVM_METRIC_TON_PER_SECOND 9 //
  #define MVM_METRIC_TON_PER_MINUTE 10 //
  #define MVM_METRIC_TON_PER_HOUR 11 //
  #define MVM_METRIC_TON_PER_DAY 12 //
  #define MVM_POUND_PER_SECOND 13 //
  #define MVM_POUND_PER_MINUTE 14 //
  #define MVM_POUND_PER_HOUR 15 //
  #define MVM_POUND_PER_DAY 16 //
  #define MVM_SHORT_TON_PER_SECOND 17 //
  #define MVM_SHORT_TON_PER_MINUTE 18 //
  #define MVM_SHORT_TON_PER_HOUR 19 //
  #define MVM_SHORT_TON_PER_DAY 20 //

  // _T_POWER
  #define MVM_WATT 1 //W
  #define MVM_MEGAWATT 4 //MW
  #define MVM_KILOWATT 5 //KW
  #define MVM_KILOJOULE_PER_SECOND 19 //KJ/s
  #define MVM_KILOJOULE_PER_MINUTE 20 //KJ/min
  #define MVM_KILOJOULE_PER_HOUR 21 //KJ/h
  #define MVM_KILOJOULE_PER_DAY 22 //KJ/d
  #define MVM_MEGAJOULE_PER_HOUR 25 //MJ/h
  #define MVM_BRITISH_THERMAL_UNIT_PER_SECOND 27 //BTU/s
  #define MVM_BRITISH_THERMAL_UNIT_PER_MINUTE 28 //BTU/min
  #define MVM_BRITISH_THERMAL_UNIT_PER_HOUR 29 //BTU/hour
  #define MVM_BRITISH_THERMAL_UNIT_PER_DAY 30 //BTU/day
  #define MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_MINUTE 32 //MBTU/min
  #define MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_HOUR 33 //MBTU/hour
  #define MVM_THOUSAND_BRITISH_THERMAL_UNIT_PER_DAY 34 //MBTU/day
  #define MVM_MILLION_BRITISH_THERMAL_UNIT_PER_HOUR 37 //MMBTU/hour
  #define MVM_MILLION_BRITISH_THERMAL_UNIT_PER_DAY 38 //MMBTU/day

  // _T_PRESSURE
  #define MVM_PASCAL_U 1 //
  #define MVM_GIGAPASCAL 2 //
  #define MVM_MEGAPASCAL 3 //
  #define MVM_KILOPASCAL 4 //
  #define MVM_MILLIPASCAL 5 //
  #define MVM_MICROPASCAL 6 //
  #define MVM_HECTOPASCAL 7 //
  #define MVM_BAR 8 //
  #define MVM_MILLIBAR 9 //
  #define MVM_PASCAL_ABSOLUTE 10 //
  #define MVM_PASCAL_GAUGE 11 //
  #define MVM_GIGAPASCA_ABSOLUTE 12 //
  #define MVM_GIGAPASCA_GAUGE 13 //
  #define MVM_MEGAPASCAL_ABSOLUTE 14 //
  #define MVM_MEGAPASCAL_GAUGE 15 //
  #define MVM_KILOPASCAL_ABSOLUTE 16 //
  #define MVM_KILOPASCAL_GAUGE 17 //
  #define MVM_MILLIPASCAL_ABSOLUTE 18 //
  #define MVM_MILLIPASCAL_GAUGE 19 //
  #define MVM_MICROPASCAL_ABSOLUTE 20 //
  #define MVM_MICROPASCAL_GAUGE 21 //
  #define MVM_HECTOPASCAL_ABSOLUTE 22 //
  #define MVM_HECTOPASCAL_GAUGE 23 //
  #define MVM_INCH_H2O_60F_ABSOLUTE 24 //
  #define MVM_INCH_H2O_60F_GAUGE 25 //
  #define MVM_INCH_H2O_4C_ABSOLUTE 26 //
  #define MVM_INCH_H2O_4C_GAUGE 27 //
  #define MVM_INCH_H2O_68F_ABSOLUTE 28 //
  #define MVM_INCH_H2O_68F_GAUGE 29 //
  #define MVM_MILLIMETER_H2O_ABSOLUTE 30 //
  #define MVM_MILLIMETER_H2O_GAUGE 31 //
  #define MVM_MILLIMETER_H2O_4C_ABSOLUTE 32 //
  #define MVM_MILLIMETER_H2O_4C_GAUGE 33 //
  #define MVM_MILLIMETER_H2O_68F_ABSOLUTE 34 //
  #define MVM_MILLIMETER_H2O_68F_GAUGE 35 //
  #define MVM_FEET_H2O_ABSOLUTE_GAUGE 36 //
  #define MVM_FEET_H2O_GAUGE 37 //
  #define MVM_FEET_H2O_4C_ABSOLUTE 38 //
  #define MVM_FEET_H2O_4C_GAUGE 39 //
  #define MVM_FEET_H2O_68F_ABSOLUTE 40 //
  #define MVM_FEET_H2O_68F_GAUGE 41 //
  #define MVM_INCH_HG_ABSOLUTE 42 //
  #define MVM_INCH_HG_GAUGE 43 //
  #define MVM_INCH_HG_0C_ABSOLUTE 44 //
  #define MVM_INCH_HG_0C_GAUGE 45 //
  #define MVM_MILLIMETER_HG_ABSOLUTE 46 //
  #define MVM_MILLIMETER_HG_GAUGE 47 //
  #define MVM_MILLIMETER_HG_0C_ABSOLUTE 48 //
  #define MVM_MILLIMETER_HG_0C_GAUGE 49 //
  #define MVM_INCH_H2O_60F 50 //
  #define MVM_INCH_H2O_4C 51 //
  #define MVM_INCH_H2O_68F 52 //
  #define MVM_MILLIMETER_H2O 53 //
  #define MVM_MILLIMETER_H2O_4C 54 //
  #define MVM_MILLIMETER_H2O_68F 55 //
  #define MVM_FEET_H2O 56 //
  #define MVM_FEET_H2O_4C 57 //
  #define MVM_FEET_H2O_68F 58 //
  #define MVM_INCH_HG 59 //
  #define MVM_INCH_HG_0C 60 //
  #define MVM_MILLIMETER_HG 61 //
  #define MVM_MILLIMETER_HG_0C 62 //
  #define MVM_TORR 63 //
  #define MVM_ATMOSPHERE 64 //
  #define MVM_POUNDFORCE_PER_SQUARE_INCH 65 //
  #define MVM_POUNDFORCE_PER_SQUARE_INCH_ABSOLUTE 66 //
  #define MVM_POUNDFORCE_PER_SQUARE_INCH_GAUGE 67 //
  #define MVM_GRAMFORCE_PER_SQUARE_CENTIMETER 68 //
  #define MVM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER 69 //
  #define MVM_GRAMFORCE_PER_SQUARE_CENTIMETER_ABSOLUTE 70 //
  #define MVM_GRAMFORCE_PER_SQUARE_CENTIMETER_GAUGE 71 //
  #define MVM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER_ABSOLUTE 72 //
  #define MVM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER_GAUGE 73 //
  #define MVM_BAR_ABSOLUTE 74 //
  #define MVM_BAR_GAUGE 75 //
  #define MVM_MILLIBAR_GAUGE 76 //

  // _T_TEMPERATURE
  #define MVM_KELVIN 1 //
  #define MVM_CELSIUS 2 //
  #define MVM_FAHRENHEIT 3 //
  #define MVM_RANKINE 4 //

  // _STEAMSTATUS
  #define MVM_WATER 0 //
  #define MVM_SATURATED_STEAM 1 //
  #define MVM_OVERHEAT_STEAM 2 //

  // _STEAMTYPE
  #define MVM_STEAMTYPE_OVERHEAT 0 //
  #define MVM_STEAMTYPE_SATURATED 1 //
  #define MVM_STEAMTYPE_WATER 2 //

  // _LIQUIDCORRECTION
  #define MVM_DENSITY_CORRECTION 0 //
  #define MVM_VOLUME_CORRECTION 1 //
  #define MVM_WITHOUT_CORRECTION 2 //

  // _TSELECTION
  #define MVM_TPRESET 0 //
  #define MVM_TSENSOR 1 //

  // _GAS_REF_CONDITIONS
  #define MVM_UK 0 //14.7 psia 60?F (15.4?C) (UK)
  #define MVM_US 1 //14.7 psia70?F (21.6?C) (US)
  #define MVM_DEG0 2 //1013.25 mbar and 0?C
  #define MVM_DEG20 3 //(CGPM),1013.25 mbar and 20?C

  // _GAS_COMPUTATION_TYPE
  #define MVM_IDEAL_GAS 0 //Real gas equation
  #define MVM_ISO12213_MOLAR 1 //ISO12213_2/AGA8
  #define MVM_ISO12213_PHISICAL_PROPERTY 2 //ISO12213_3/GERG88

  // _GAS_MASS_COMPUTATION
  #define MVM_REFERENCY_DENSITY 0 //Select the Standard Density
  #define MVM_ACTUAL_DENSITY 1 //Seletct  Preset Density or AI. or Hart.

  // _MVM_DIAGNOSIS_ALARM_TABLE
  #define MVM_ALM_FLOWRATE_REACH_QMAX 0 //flowrate reach Q max alarm limits
  #define MVM_ALM_FLOWRATE_REACH_QMIN 1 //flowrate reach Q min alarm limits
  #define MVM_ALM_FLOWRATE_GT_103 2 //flow rate is greater than 103 per QMax
  #define MVM_ALM_STEAM_STATUS_MISMATCH 3 //steam status mismatch
  #define MVM_ALM_LOW_Q_VALUE_CUTOFF 4 //

  // 
  #define MVM_DIAG_SIZE 5 //

  // _RELATIVE_DENSITY_REF
  #define MVM_RELATIVE_DENSITY_REF_DEG0_101325 0 //T2 = 0?C , 1.01325 bar
  #define MVM_RELATIVE_DENSITY_REF_DEG15_101325 1 //T2 = 20?C , 1.01325 bar
  #define MVM_RELATIVE_DENSITY_REF__60F_101592 2 //T2 = 60?F ,1.01592bar
  #define MVM_RELATIVE_DENSITY_REF__60F_101560 3 //T2 = 60?F ,14.73Psi

  // _CALORIFIC_REF
  #define MVM_CALORIFIC_REF_DEG25_DEG0_101325 0 //T1=25 Deg,t2=0  P2=1.01325 Bar
  #define MVM_CALORIFIC_REF_DEG0_DEG0_101325 1 //T1=0 Deg,t2=0  P2=1.01325 Bar
  #define MVM_CALORIFIC_REF_DEG15_DEG15_101325 2 //T1=15 Deg,t2=15 P2=1.01325 Bar
  #define MVM_CALORIFIC_REF_60F_6OF_101592 3 //T1=60F Deg,t2=60F  P2=1.01592Bar
  #define MVM_CALORIFIC_REF_60F_6OF_101560 4 //T1=60F Deg,t2=60F  P2=1.01560 Bar

  // _MVM_STATUS_TABLE
  #define MVM_UNPROTECTED 0 //
  #define MVM_CALCULATE 1 //
  #define MVM_PROTECTED 2 //

  // _MVM_SW
  #define MVM_ENABLE 0 //
  #define MVM_DISABLE 1 //

  // _GAS_CFG
  #define MVM_NONGONFIGED 0 //
  #define MVM_GONFIGING 1 //
  #define MVM_GONFIGED 2 //

  // _MATRIX_TABLE
  #define MVM_SIZE3X3 0 //
  #define MVM_SIZE4X3 1 //four pressure and three temperature
  #define MVM_SIZE9X7 2 //nine pressure and seven temperature

  // _PWR_FWD_REV_SW
  #define MVM_FWD_ONLY 0 //Just measure forward flow
  #define MVM_FWD_REV 1 //measure forward and reverse flow
//@SubGen end@ <TYPES>


//how many bits per word
#define MVM_DIAG_WORD_SIZE   8
  
#if( MVM_DIAG_WORD_SIZE == 8 )
    #define MVM_DIAG_BIT_(index)   ((index) &  7)
    #define MVM_DIAG_WRD_(index)   ((index) >> 3)
	  // words = (MVM_DIAG_SIZE) >> 3
#elif( MVM_DIAG_WORD_SIZE == 16 )
    #define MVM_DIAG_BIT_(index)   ((index) & 15)
    #define MVM_DIAG_WRD_(index)   ((index) >> 4)
	  // words = (MVM_DIAG_SIZE) >> 4
#elif( MVM_DIAG_WORD_SIZE == 32 )
    #define MVM_DIAG_BIT_(index)   ((index) & 31)
    #define MVM_DIAG_WRD_(index)   ((index) >> 5)
	  // words = (MVM_DIAG_SIZE) >> 5
#else
    #error invalid definition for MVM_DIAG_WORD_SIZE
#endif


  
  //#if( CURR_DIAG_WRD_(CURR_DIAG_SIZE) > 1 )
  
#if(MVM_DIAG_SIZE > MVM_DIAG_WORD_SIZE)
  #define MVM_SETDIAG_(index,var)           (var[MVM_DIAG_WRD_(index)] |=  (1U << MVM_DIAG_BIT_(index)));(##var##DUPLICATED[MVM_DIAG_WRD_(index)] |=  (1U << MVM_DIAG_BIT_(index)))
  #define MVM_CLRDIAG_(index,var)           (var[MVM_DIAG_WRD_(index)] &= ~(1U << MVM_DIAG_BIT_(index)));(##var##DUPLICATED[MVM_DIAG_WRD_(index)] &= ~(1U << MVM_DIAG_BIT_(index)))
  #define MVM_GETDIAG_(index,var)    (TBOOL)(var[MVM_DIAG_WRD_(index)] &   (1U << MVM_DIAG_BIT_(index)))
#else
  #define MVM_SETDIAG_(index,var)           (var |=  (1U << MVM_DIAG_BIT_(index)));(var##Duplicated |=  (1U << MVM_DIAG_BIT_(index)))
  #define MVM_CLRDIAG_(index,var)           (var &= ~(1U << MVM_DIAG_BIT_(index)));(var##Duplicated &= ~(1U << MVM_DIAG_BIT_(index)))
  #define MVM_GETDIAG_(index,var)    (TBOOL)(var &   (1U << MVM_DIAG_BIT_(index)))
#endif


//-------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//-------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_MVMEASUREMENT_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    MVMEASUREMENT_DC_IDX_Protected,
    MVMEASUREMENT_DC_IDX_Unprotected,
    MVMEASUREMENT_DC_IDX_DynamicDuplicated,
    MVMEASUREMENT_DC_IDX_REPLACE_STATIC_FREQUENT_STATIC_DEFAULT,
    MVMEASUREMENT_DC_IDX_CONSTANT,
    MVMEASUREMENT_DC_IDX_STATIC_RARE,
    MVMEASUREMENT_DC_IDX_STATIC_FREQUENT_STATIC_DEFAULT,
    MVMEASUREMENT_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//-------------------------------------------------------------------------------------------------
//!  definition of class T_MVMEASUREMENT
typedef struct _T_MVMEASUREMENT
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    TFLOAT (* CalculateGasQnEXE)(TFLOAT volumeFlow, TFLOAT tReal, TFLOAT pReal);
    TFLOAT (* CalculateGasQnPartialEXE)(TFLOAT inNVF, TFLOAT biogasPerc);
    TFLOAT (* CalculateGasQvPartialEXE)(TFLOAT inVF, TFLOAT biogasPerc);
    TFLOAT (* CalculateGasQmEXE)(TFLOAT inVF, TFLOAT inNVF, TFLOAT densityReal);
    TFLOAT (* CalculateGasQpEXE)(TFLOAT inVF);
    TFLOAT (* CalculateLiquidQnEXE)(TFLOAT inVF, TFLOAT tReal);
    TFLOAT (* CalculateLiquidQmEXE)(TFLOAT inVF, TFLOAT DensityReal, TFLOAT tReal);
    TFLOAT (* CalculateLiquidQpEXE)(TFLOAT inVF, TFLOAT T1Real, TFLOAT T2Real);
    TFLOAT (* CalculateSteamQmEXE)(TFLOAT inVF, TFLOAT DensityReal);
    TFLOAT (* CalculateSteamQpEXE)(TFLOAT inMF);
    void (* UpdateDiagnosisEXE)(void);
    TFLOAT (* CalcSteamDensityEXE)(TFLOAT inTReal, TFLOAT inPReal);
    void (* CalcSteamHEXE)(TFLOAT inPReal, TFLOAT inT1Real, TFLOAT inT2Real);
    void (* CalcCompressFactor)(void);
    void (* CheckDataCrcSRV)(void);
    TFLOAT (* GetQnPercentageSRV)(void);
    TFLOAT (* GetQmPercentageSRV)(void);
    TFLOAT (* GetQpPercentageSRV)(void);
    TFLOAT (* GetQnPartialPercentageSRV)(void);
    TFLOAT (* GetQvPartialPercentageSRV)(void);
    void (* UpdateLiquidQnMaxDN)(void);
    void (* UpdateGasQnMaxDN)(void);
    void (* UpdateLiquidPowerMaxDN)(void);
    void (* UpdateGasPowerMaxDN)(void);
    void (* UpdateSteamPowerMaxDN)(void);
    void (* UpdateQmMaxDN)(void);
    void (* UpdateBiagasQvMaxDN)(void);
    void (* UpdateQAlarmEXE)(TFLOAT perc);
    //@SubGen end@    <METHODS>
} T_MVMEASUREMENT;


//-------------------------------------------------------------------------------------------------
//! define the T_MVMEASUREMENT-object
extern SLOW const T_MVMEASUREMENT mVMeasurement;

//-------------------------------------------------------------------------------------------------
//! the this-pointer for the T_UNIT-methods in T_MVMEASUREMENT
#define MVMEASUREMENT_ME (&mVMeasurement.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define mVMeasurement_Get(objectIndex,attributeIndex,ptrValue) \
   mVMeasurement.unit.Get(MVMEASUREMENT_ME,objectIndex,attributeIndex,ptrValue)
#define mVMeasurement_Put(objectIndex,attributeIndex,ptrValue) \
   mVMeasurement.unit.Put(MVMEASUREMENT_ME,objectIndex,attributeIndex,ptrValue)
#define mVMeasurement_Check(objectIndex,attributeIndex,ptrValue) \
   mVMeasurement.unit.Check(MVMEASUREMENT_ME,objectIndex,attributeIndex,ptrValue)

#define mVMeasurement_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   mVMeasurement.unit.GetAttributeDescription(MVMEASUREMENT_ME,objectIndex,attributeIndex,ptrDescriptor)
#define mVMeasurement_GetObjectDescription(objectIndex,ptrDescriptor) \
   mVMeasurement.unit.GetObjectDescription(MVMEASUREMENT_ME,objectIndex,ptrDescriptor)