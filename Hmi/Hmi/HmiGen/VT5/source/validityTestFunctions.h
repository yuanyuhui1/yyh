//+@ HMIGen device specific include files @+
// ADD DEVICE SPECIFIC INCLUDED FILES HERE
#include "hmi_framework_definition.h"
#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/assert.h"
#include "Mapper/Interface/Mapper.h"
#include "Mapper/Interface/Mapper_idx.h"
#include "Mapper/Source/Mapper_Measurement.h"
#include "MVMeasurement/Interface/MVMeasurement.h"
#include "MVMeasurement/Interface/MVMeasurement_idx.h"
#include "Coordinator/Interface/Coordinator.h"
#include "Coordinator/Interface/Coordinator_idx.h"

//-@ HMIGen device specific include files @-

#ifdef VALIDITY_TEST_CALLER
TUSIGN8 signalValidityCallerID;
//  0 = SIGNALS_TEST_NOT_APPLICABLE
//  1 = SIGNALS_TEST_FOR_SIGNALS_VIEW
//  2 = SIGNALS_TEST_FOR_OPERATOR_PAGE (Operator page line descriptor)
//  3 = SIGNALS_TEST_FOR_WAVEFORM      (Waveform PRV / CRV signal)
#endif

TUSIGN8 valid_always(void);
TUSIGN8 valid_never(void);
TUSIGN8 valid_if_DO_function_pulse(void);
TUSIGN8 valid_if_DO_function_freq(void);
TUSIGN8 valid_if_DO_function_logic(void);
TUSIGN8 valid_if_OpMode_include_mass(void);
TUSIGN8 valid_if_OpMode_include_standard(void);
TUSIGN8 valid_if_OpMode_include_power(void);
TUSIGN8 valid_if_use_temperature(void);
TUSIGN8 valid_if_OpMode_use_pressure(void);
TUSIGN8 valid_if_AI_active(void);
TUSIGN8 valid_if_AI_content_T(void);
TUSIGN8 valid_if_AI_content_P(void);
TUSIGN8 valid_if_AI_content_P_abs(void);
TUSIGN8 valid_if_AI_content_Den(void);
TUSIGN8 valid_if_AI_content_GasContent(void);
TUSIGN8 valid_if_AI_content_Cutoff(void);
TUSIGN8 valid_if_AI_content_intT(void);
TUSIGN8 valid_if_OpMode_include_liquid_mass(void);
TUSIGN8 valid_if_OpMode_useVEC(void);
TUSIGN8 valid_if_OpMode_useDEC(void);
TUSIGN8 valid_if_OpMode_only_liquid_power(void);
TUSIGN8 valid_if_OpMode_only_gas_mass(void);
TUSIGN8 valid_if_OpMode_use_gasRef(void);
TUSIGN8 valid_if_OpMode_include_gas_standard(void);
TUSIGN8 valid_if_OpMode_only_gas_power(void);
TUSIGN8 valid_if_OpMode_belongs_steam(void);
TUSIGN8 valid_if_OpMode_include_steam_mass(void);
TUSIGN8 valid_if_OpMode_only_WS_energy(void);
TUSIGN8 valid_if_OpMode_use_densityRef(void);
TUSIGN8 valid_if_need_presetDensity(void);
TUSIGN8 valid_if_OpMode_use_ref_T(void);
TUSIGN8 valid_if_use_presetT(void);
TUSIGN8 valid_if_OpMode_use_extT(void);
TUSIGN8 valid_if_OpMode_use_act_content(void);
TUSIGN8 valid_if_OpMode_only_volume(void);
TUSIGN8 valid_if_OpMode_only_standard(void);
TUSIGN8 valid_if_OpMode_only_bia_volume(void);
TUSIGN8 valid_if_OpMode_only_bia_standard(void);
TUSIGN8 valid_if_OpMode_only_mass(void);
TUSIGN8 valid_if_OpMode_only_power(void);
TUSIGN8 valid_if_CO_T(void);
TUSIGN8 valid_if_simulationMode_Qv(void);
TUSIGN8 valid_if_simulationMode_Qv_percentage(void);
TUSIGN8 valid_if_simulationMode_Qm(void);
TUSIGN8 valid_if_simulationMode_Qm_percentage(void);
TUSIGN8 valid_if_simulationMode_T(void);
TUSIGN8 valid_if_simulationMode_T_pipe(void);
TUSIGN8 valid_if_simulationMode_T_housing(void);
TUSIGN8 valid_if_simulationMode_DO_freq(void);
TUSIGN8 valid_if_simulationMode_DO_logic(void);
TUSIGN8 valid_if_simulationMode_DO_pulse(void);
TUSIGN8 valid_if_simulationMode_AI(void);
TUSIGN8 valid_if_simulationMode_sensor_freq(void);
TUSIGN8 valid_if_sensorType_vortex(void);
TUSIGN8 valid_if_sensorType_swirle(void);
TUSIGN8 valid_if_OpMode_include_partial_volume(void);
TUSIGN8 valid_if_OpMode_include_partial_standard(void);
TUSIGN8 valid_if_KSet_liquid(void);
TUSIGN8 valid_if_KSet_gas(void);
TUSIGN8 valid_if_use_presetDensity(void);
TUSIGN8 valid_if_OpMode_use_act_T(void);
TUSIGN8 valid_if_displayMode1_allows_1st_line(void);
TUSIGN8 valid_if_displayMode1_allows_2nd_line(void);
TUSIGN8 valid_if_displayMode1_allows_3rd_line(void);
TUSIGN8 valid_if_displayMode1_allows_bargraph(void);
TUSIGN8 valid_if_displayMode2_allows_1st_line(void);
TUSIGN8 valid_if_displayMode2_allows_2nd_line(void);
TUSIGN8 valid_if_displayMode2_allows_3rd_line(void);
TUSIGN8 valid_if_displayMode2_allows_bargraph(void);
TUSIGN8 valid_if_displayMode3_allows_1st_line(void);
TUSIGN8 valid_if_displayMode3_allows_2nd_line(void);
TUSIGN8 valid_if_displayMode3_allows_3rd_line(void);
TUSIGN8 valid_if_displayMode3_allows_bargraph(void);
TUSIGN8 valid_if_displayMode4_allows_1st_line(void);
TUSIGN8 valid_if_displayMode4_allows_2nd_line(void);
TUSIGN8 valid_if_displayMode4_allows_3rd_line(void);
TUSIGN8 valid_if_displayMode4_allows_bargraph(void);
TUSIGN8 valid_if_K_median(void);
TUSIGN8 valid_if_k_1point(void);
TUSIGN8 valid_if_k_2point(void);
TUSIGN8 valid_if_k_3point(void);
TUSIGN8 valid_if_k_4point(void);
TUSIGN8 valid_if_k_5point(void);
TUSIGN8 valid_if_k_6point(void);
TUSIGN8 valid_if_k_7point(void);
TUSIGN8 valid_if_k_8point(void);
TUSIGN8 valid_if_SignalsViewValidityTest_1(void);
TUSIGN8 valid_if_SignalsViewValidityTest_4(void);
TUSIGN8 valid_if_SignalsViewValidityTest_3(void);
TUSIGN8 valid_if_SignalsViewValidityTest_2(void);
TUSIGN8 valid_if_SignalsViewValidityTest_5(void);
TUSIGN8 valid_if_SignalsViewValidityTest_8(void);
TUSIGN8 valid_if_SignalsViewValidityTest_6(void);
TUSIGN8 valid_if_SignalsViewValidityTest_7(void);
TUSIGN8 valid_if_SignalsViewValidityTest_11(void);
TUSIGN8 valid_if_SignalsViewValidityTest_9(void);
TUSIGN8 valid_if_SignalsViewValidityTest_10(void);
TUSIGN8 valid_if_SignalsViewValidityTest_12(void);
TUSIGN8 valid_if_autoscroll_enabled(void);
TUSIGN8 valid_if_no_user_logged_in(void);
TUSIGN8 valid_based_upon_service_level_protection(void);
TUSIGN8 valid_if_resetPasswordsAvailable(void);
TBOOL PasswordResetDisabledByHardwareWriteProtection(void);