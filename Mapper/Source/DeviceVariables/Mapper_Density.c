//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Mapper Subsystem
 Module			Density.c

 Description    Manage Density device variable 
 <brief>
 	This file is used to manage the device variable density
 </brief>
 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "Mapper_Density.h"
#include "AnalogInput/Interface/AnalogInput.h"
#include "AnalogInput/Interface/AnalogInput_idx.h"
#include "HART/Interface/Hart.h"
#include "HART/Interface/Hart_idx.h"
#include "Mapper/Source/Mapper_types.h"
#include "Mapper/Source/Mapper_attributes.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Interface/Mapper.h"
#include "Coordinator/Interface/Coordinator_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"

extern const TFLOAT pressureRefTable[];
extern const TFLOAT temperatureRefTable[];


//--------------------------------------------------------------------------------------------------
/*!
 \brief       Get the value of Density using fast path.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TFLOAT the value of density
 \warning     none
 \test
        test-date    :  May 14 2013 16:41:04
        by           :  Jax Yang
        environment  :  CSPY
        result       :  OK

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------
TFLOAT GetDensity(void)
{
	return mapperDynamicDuplicated.dvDensity;
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief       Density has three input channels:AnalogInput,Hart,preset,if the bit within 
              versionDependence of CDT is enabled,otherwise it only has preset and hart input channel.
 \author      Jax Yang
 \date        2012-09-17
 \param       none
 \return      TUSIGN16 frame work return codes
 \warning     none
 \test
        test-date    :  May 14 2013 16:41:04
        by           :  Jax Yang
        environment  :  CSPY
        result       :  OK

 \test  Lint level 3 OK

 \bug   none
*/
//--------------------------------------------------------------------------------------------------
TUSIGN16 UpdateDensity(void)
{
    TUSIGN16 errorCode = (TUSIGN16)OK;
    TUSIGN8 HartSelection = HART_SEC_WITHOUT;       // HART input Content Selection.
    TUSIGN8 Act_InputFail = CDT_PRESET;
	TUSIGN8 AISelection = AI_WITHOUT ;              // Analog input Content Selection.
	TFLOAT  DensityInput = 0.0f;                    // To store the Input Gas Content value.
    TUSIGN8 densitySimEnable = MAPPER_SIMU_DISABLE;
    TUSIGN8 source = 0;
    TUSIGN8 densitySelection = CDT_EXT_DENSITY;
    TUSIGN8  burstInStatus = 0xff;
    TFLOAT aiCurrent = 0.0f;
    static TFLOAT lastgoodDensity = 0.0f;
    TFLOAT realTemperature = 0.0f;
    TFLOAT realPressure = 0.0f;
    TUSIGN8 liquidCorrection = MVM_WITHOUT_CORRECTION;
    TFLOAT densityRef = 0.0f;
    TFLOAT densityExpandBeta2 = 0.0f;
    TFLOAT volumeExpandBeta1 = 0.0f;
    TFLOAT refTemperature = 0.0f;
    TFLOAT refPressure = 0.0f;
    TUSIGN8 gasDensitySelection = MVM_REFERENCY_DENSITY;
    TUSIGN8 gasRef = MVM_UK, gasComputeType = MVM_IDEAL_GAS;
    TFLOAT gasCompressFactorS = 0.0f, gasCompressFactorR = 0.0f;

    TUSIGN16 opMode = MAPPER_SELECTION_LIQUIDQV;

    errorCode = coordinator_Get((TUSIGN16)CDT_IDX_actualDensitySelection,(TINT16)WHOLE_OBJECT, &densitySelection);
    errorCode |=  coordinator_Get((TUSIGN16)CDT_IDX_temperature,(TINT16)WHOLE_OBJECT, &realTemperature);
    errorCode |=  mapper_Get((TUSIGN16)MAPPER_IDX_dvPressure,(TINT16)WHOLE_OBJECT, &realPressure);
    errorCode |=  mapper_Get((TUSIGN16)MAPPER_IDX_localOperatingModeSelection,(TINT16)WHOLE_OBJECT, &opMode);

    //Get the densitySimEnable
	errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensitySimEnable, (TINT16)WHOLE_OBJECT, &densitySimEnable);

    if(densitySimEnable == MAPPER_SIMU_ENABLE)
    {
        errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensitySim,(TINT16)WHOLE_OBJECT,&DensityInput);
    }
    else if((opMode & (MAPPER_SELECTION_STEAMQM|MAPPER_SELECTION_STEAMPOWER)) && (densitySelection != CDT_EXT_DENSITY))
    {
        DensityInput = mVMeasurement.CalcSteamDensityEXE(realTemperature, realPressure);
    }
    else
    {
        //Get Information from Hart Subsystem
        errorCode  = hart_Get((TUSIGN16)HART_IDX_burstUpTxSelection, (TINT16)WHOLE_OBJECT, &HartSelection);

        //Get Information from AI Subsystem
        errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiSelection, (TINT16)WHOLE_OBJECT, &AISelection);

        errorCode |= hart_Get((TUSIGN16)HART_IDX_burstInStatus, (TINT16)WHOLE_OBJECT, &burstInStatus);
        errorCode |= analogInput_Get((TUSIGN16)AI_IDX_aiTempCurr, (TINT16)WHOLE_OBJECT, &aiCurrent);

        if( AISelection == AI_DENSITY )
        {
            if(aiCurrent>=3.8 && aiCurrent<=20.5)
            {
                errorCode  |= analogInput_Get((TUSIGN16)AI_IDX_aiInValue, (TINT16)WHOLE_OBJECT, &DensityInput);
                source = MAPPER_DENSITY_FROM_AI;
                errorCode  |= mapper_Put((TUSIGN16)MAPPER_IDX_dvDensitySrc, (TINT16)WHOLE_OBJECT, &source);
            }
            else
            {
                errorCode |= coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                {
                    DensityInput = lastgoodDensity;
                }
                else
                {
                    errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT, &DensityInput);
                }
            }
        }
        else if( HartSelection == HART_SEC_DENSITY )
        {
            if(burstInStatus == HART_BURST_IN_OK)
            {
                errorCode |= hart_Get((TUSIGN16)HART_IDX_burstUpBaseValue, (TINT16)WHOLE_OBJECT, &DensityInput);
                source = MAPPER_DENSITY_FROM_HART;
                errorCode  |= mapper_Put((TUSIGN16)MAPPER_IDX_dvDensitySrc, (TINT16)WHOLE_OBJECT, &source);
            }
            else
            {
                errorCode |= coordinator_Get((TUSIGN16)CDT_IDX_reaction_InputFail,(TINT16)WHOLE_OBJECT,&Act_InputFail);
                if(Act_InputFail == CDT_LAST_GOOD_VALUE)
                {
                    DensityInput = lastgoodDensity;
                }
                else
                {
                    errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT,&DensityInput);
                }
            }
        }
        else
        {
            if (opMode & MAPPER_SELECTION_GASQM)
            {
                errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_gasDensitySelection, (TINT16)WHOLE_OBJECT, &gasDensitySelection);
                if (gasDensitySelection == MVM_REFERENCY_DENSITY)
                {
                    errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_densityRef, (TINT16)WHOLE_OBJECT, &densityRef);
                    errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_gasRef, (TINT16)WHOLE_OBJECT, &gasRef);
                    errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_gasComputeType, (TINT16)WHOLE_OBJECT, &gasComputeType);

                    refPressure = pressureRefTable[gasRef];
                    refTemperature = temperatureRefTable[gasRef];

                    // Denact = [DenRef*(Pact/Pref)*(Tref+273.15)/(273.15+T)] * Zref/Zt
                    if(MVM_IDEAL_GAS == gasComputeType)
                    {
                        DensityInput = (densityRef * realPressure * (refTemperature + 273.15f)) \
                                        / ((realTemperature + 273.15f) * refPressure );   
                    }
                    else
                    {
                        errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_compressFactorS, (TINT16)WHOLE_OBJECT, &gasCompressFactorS);
                        errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_compressFactorR, (TINT16)WHOLE_OBJECT, &gasCompressFactorR);
                    
                        DensityInput = (densityRef * ( realPressure * (refTemperature + 273.15f) * gasCompressFactorS)) \
                                        / ((realTemperature + 273.15f) * refPressure * gasCompressFactorR);   
                    }
                }
                else
                {
                    errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT,&DensityInput);
                }
            }
            else if ((opMode & (MAPPER_SELECTION_LIQUIDQM | MAPPER_SELECTION_LIQUIDPOWER)))
            {
                errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_liquidCorrection, (TINT16)WHOLE_OBJECT, &liquidCorrection);
                errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_densityRef, (TINT16)WHOLE_OBJECT, &densityRef);
                errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_refTemperature, (TINT16)WHOLE_OBJECT, &refTemperature);

                if (liquidCorrection == MVM_DENSITY_CORRECTION)
                {
                    errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_densityExpandBeta2, (TINT16)WHOLE_OBJECT, &densityExpandBeta2);
                    DensityInput = densityRef * (1.0f + ((realTemperature - refTemperature) * densityExpandBeta2));
                }
                else if (liquidCorrection == MVM_VOLUME_CORRECTION)
                {
                    errorCode |= mVMeasurement_Get((TUSIGN16)MVM_IDX_volumeExpandBeta1, (TINT16)WHOLE_OBJECT, &volumeExpandBeta1);
                    DensityInput = densityRef / (1.0f + ((realTemperature - refTemperature) * volumeExpandBeta1));
                }
                else
                {
                    errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT,&DensityInput);
                }
            }
            else
            {
                errorCode |= mapper_Get((TUSIGN16)MAPPER_IDX_dvDensityPreset,(TINT16)WHOLE_OBJECT,&DensityInput);
            }

            source = MAPPER_DENSITY_FROM_PRESET;
            errorCode  |= mapper_Put((TUSIGN16)MAPPER_IDX_dvDensitySrc, (TINT16)WHOLE_OBJECT, &source);
        }
    }

    lastgoodDensity = DensityInput;

    errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvDensity,(TINT16)WHOLE_OBJECT,&DensityInput);
    errorCode |= mapper_Put((TUSIGN16)MAPPER_IDX_dvDensityDuplicated,(TINT16)WHOLE_OBJECT,&DensityInput);

	return errorCode;
}