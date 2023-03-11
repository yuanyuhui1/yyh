//+@ HMIGen device specific include files @+
// ADD DEVICE SPECIFIC INCLUDED FILES HERE

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

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_always(void)
{
 return 1;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_never(void)
{
 return 0;
}

//+@ HMIGen valid_if_DO_function_pulse @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_DO_function_pulse(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_DO_function_pulse @-

//+@ HMIGen valid_if_DO_function_freq @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_DO_function_freq(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_DO_function_freq @-

//+@ HMIGen valid_if_DO_function_logic @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_DO_function_logic(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_DO_function_logic @-

//+@ HMIGen valid_if_OpMode_include_mass @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_mass(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_mass @-

//+@ HMIGen valid_if_OpMode_include_standard @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_standard(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_standard @-

//+@ HMIGen valid_if_OpMode_include_power @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_power(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_power @-

//+@ HMIGen valid_if_use_temperature @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_use_temperature(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_use_temperature @-

//+@ HMIGen valid_if_OpMode_use_pressure @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_pressure(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_pressure @-

//+@ HMIGen valid_if_AI_active @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_active(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_active @-

//+@ HMIGen valid_if_AI_content_T @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_T(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_T @-

//+@ HMIGen valid_if_AI_content_P @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_P(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_P @-

//+@ HMIGen valid_if_AI_content_P_abs @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_P_abs(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_P_abs @-

//+@ HMIGen valid_if_AI_content_Den @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_Den(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_Den @-

//+@ HMIGen valid_if_AI_content_GasContent @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_GasContent(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_GasContent @-

//+@ HMIGen valid_if_AI_content_Cutoff @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_Cutoff(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_Cutoff @-

//+@ HMIGen valid_if_AI_content_intT @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_AI_content_intT(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_AI_content_intT @-

//+@ HMIGen valid_if_OpMode_include_liquid_mass @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_liquid_mass(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_liquid_mass @-

//+@ HMIGen valid_if_OpMode_useVEC @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_useVEC(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_useVEC @-

//+@ HMIGen valid_if_OpMode_useDEC @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_useDEC(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_useDEC @-

//+@ HMIGen valid_if_OpMode_only_liquid_power @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_liquid_power(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_liquid_power @-

//+@ HMIGen valid_if_OpMode_only_gas_mass @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_gas_mass(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_gas_mass @-

//+@ HMIGen valid_if_OpMode_use_gasRef @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_gasRef(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_gasRef @-

//+@ HMIGen valid_if_OpMode_include_gas_standard @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_gas_standard(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_gas_standard @-

//+@ HMIGen valid_if_OpMode_only_gas_power @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_gas_power(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_gas_power @-

//+@ HMIGen valid_if_OpMode_belongs_steam @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_belongs_steam(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_belongs_steam @-

//+@ HMIGen valid_if_OpMode_include_steam_mass @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_steam_mass(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_steam_mass @-

//+@ HMIGen valid_if_OpMode_only_WS_energy @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_WS_energy(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_WS_energy @-

//+@ HMIGen valid_if_OpMode_use_densityRef @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_densityRef(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_densityRef @-

//+@ HMIGen valid_if_need_presetDensity @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_need_presetDensity(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_need_presetDensity @-

//+@ HMIGen valid_if_OpMode_use_ref_T @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_ref_T(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_ref_T @-

//+@ HMIGen valid_if_use_presetT @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_use_presetT(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_use_presetT @-

//+@ HMIGen valid_if_OpMode_use_extT @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_extT(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_extT @-

//+@ HMIGen valid_if_OpMode_use_act_content @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_act_content(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_act_content @-

//+@ HMIGen valid_if_OpMode_only_volume @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_volume(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_volume @-

//+@ HMIGen valid_if_OpMode_only_standard @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_standard(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_standard @-

//+@ HMIGen valid_if_OpMode_only_bia_volume @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_bia_volume(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_bia_volume @-

//+@ HMIGen valid_if_OpMode_only_bia_standard @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_bia_standard(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_bia_standard @-

//+@ HMIGen valid_if_OpMode_only_mass @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_mass(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_mass @-

//+@ HMIGen valid_if_OpMode_only_power @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_only_power(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_only_power @-

//+@ HMIGen valid_if_CO_T @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_CO_T(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_CO_T @-

//+@ HMIGen valid_if_simulationMode_Qv @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_Qv(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_Qv @-

//+@ HMIGen valid_if_simulationMode_Qv_percentage @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_Qv_percentage(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_Qv_percentage @-

//+@ HMIGen valid_if_simulationMode_Qm @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_Qm(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_Qm @-

//+@ HMIGen valid_if_simulationMode_Qm_percentage @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_Qm_percentage(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_Qm_percentage @-

//+@ HMIGen valid_if_simulationMode_T @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_T(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_T @-

//+@ HMIGen valid_if_simulationMode_T_pipe @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_T_pipe(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_T_pipe @-

//+@ HMIGen valid_if_simulationMode_T_housing @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_T_housing(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_T_housing @-

//+@ HMIGen valid_if_simulationMode_DO_freq @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_DO_freq(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_DO_freq @-

//+@ HMIGen valid_if_simulationMode_DO_logic @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_DO_logic(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_DO_logic @-

//+@ HMIGen valid_if_simulationMode_DO_pulse @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_DO_pulse(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_DO_pulse @-

//+@ HMIGen valid_if_simulationMode_AI @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_AI(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_AI @-

//+@ HMIGen valid_if_simulationMode_sensor_freq @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_simulationMode_sensor_freq(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_simulationMode_sensor_freq @-

//+@ HMIGen valid_if_sensorType_vortex @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_sensorType_vortex(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_sensorType_vortex @-

//+@ HMIGen valid_if_sensorType_swirle @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_sensorType_swirle(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_sensorType_swirle @-

//+@ HMIGen valid_if_OpMode_include_partial_volume @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_partial_volume(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_partial_volume @-

//+@ HMIGen valid_if_OpMode_include_partial_standard @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_include_partial_standard(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_include_partial_standard @-

//+@ HMIGen valid_if_KSet_liquid @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_KSet_liquid(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_KSet_liquid @-

//+@ HMIGen valid_if_KSet_gas @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_KSet_gas(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_KSet_gas @-

//+@ HMIGen valid_if_use_presetDensity @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_use_presetDensity(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_use_presetDensity @-

//+@ HMIGen valid_if_OpMode_use_act_T @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_OpMode_use_act_T(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_OpMode_use_act_T @-

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode1_allows_1st_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_SIMPLE.displayMode_1 >= HMI_DISPLAY_MODE_LPV)
    #else
    if ((SFD_SIMPLE.displayMode_1 >= HMI_DISPLAY_MODE_1x4)
     && (SFD_SIMPLE.displayMode_1 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode1_allows_2nd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_LPV_SSV)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_MPV_MSV)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_MPV_MSV_BAR)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_SPV_SSV_STV))
    #else
    if ((SFD_SIMPLE.displayMode_1 >= HMI_DISPLAY_MODE_2x9)
     && (SFD_SIMPLE.displayMode_1 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode1_allows_3rd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_SPV_SSV_STV)
    #else
    if ((SFD_SIMPLE.displayMode_1 >= HMI_DISPLAY_MODE_3x9)
     && (SFD_SIMPLE.displayMode_1 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode1_allows_bargraph(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_LPV_BAR)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_MPV_MSV_BAR))
    #else
    if ((SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_1x6_BAR_A)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_1x6_BAR_B)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_1x9_BAR)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_2x9_BAR)
     || (SFD_SIMPLE.displayMode_1 == HMI_DISPLAY_MODE_GRAPHIC))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode2_allows_1st_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_2 >= HMI_DISPLAY_MODE_LPV)
    #else
    if ((SFD_MULTI_OP.displayMode_2 >= HMI_DISPLAY_MODE_1x4)
     && (SFD_MULTI_OP.displayMode_2 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode2_allows_2nd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_LPV_SSV)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_MPV_MSV)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_MPV_MSV_BAR)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_SPV_SSV_STV))
    #else
    if ((SFD_MULTI_OP.displayMode_2 >= HMI_DISPLAY_MODE_2x9)
     && (SFD_MULTI_OP.displayMode_2 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode2_allows_3rd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_SPV_SSV_STV)
    #else
    if ((SFD_MULTI_OP.displayMode_2 >= HMI_DISPLAY_MODE_3x9)
     && (SFD_MULTI_OP.displayMode_2 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode2_allows_bargraph(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_LPV_BAR)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_MPV_MSV_BAR))
    #else
    if ((SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_1x6_BAR_A)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_1x6_BAR_B)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_1x9_BAR)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_2x9_BAR)
     || (SFD_MULTI_OP.displayMode_2 == HMI_DISPLAY_MODE_GRAPHIC))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode3_allows_1st_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_3 >= HMI_DISPLAY_MODE_LPV)
    #else
    if ((SFD_MULTI_OP.displayMode_3 >= HMI_DISPLAY_MODE_1x4)
     && (SFD_MULTI_OP.displayMode_3 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode3_allows_2nd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_LPV_SSV)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_MPV_MSV)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_MPV_MSV_BAR)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_SPV_SSV_STV))
    #else
    if ((SFD_MULTI_OP.displayMode_3 >= HMI_DISPLAY_MODE_2x9)
     && (SFD_MULTI_OP.displayMode_3 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode3_allows_3rd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_SPV_SSV_STV)
    #else
    if ((SFD_MULTI_OP.displayMode_3 >= HMI_DISPLAY_MODE_3x9)
     && (SFD_MULTI_OP.displayMode_3 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode3_allows_bargraph(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_LPV_BAR)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_MPV_MSV_BAR))
    #else
    if ((SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_1x6_BAR_A)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_1x6_BAR_B)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_1x9_BAR)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_2x9_BAR)
     || (SFD_MULTI_OP.displayMode_3 == HMI_DISPLAY_MODE_GRAPHIC))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode4_allows_1st_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_4 >= HMI_DISPLAY_MODE_LPV)
    #else
    if ((SFD_MULTI_OP.displayMode_4 >= HMI_DISPLAY_MODE_1x4)
     && (SFD_MULTI_OP.displayMode_4 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode4_allows_2nd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_LPV_SSV)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_MPV_MSV)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_MPV_MSV_BAR)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_SPV_SSV_STV))
    #else
    if ((SFD_MULTI_OP.displayMode_4 >= HMI_DISPLAY_MODE_2x9)
     && (SFD_MULTI_OP.displayMode_4 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode4_allows_3rd_line(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_SPV_SSV_STV)
    #else
    if ((SFD_MULTI_OP.displayMode_4 >= HMI_DISPLAY_MODE_3x9)
     && (SFD_MULTI_OP.displayMode_4 <= HMI_DISPLAY_MODE_4x9))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_displayMode4_allows_bargraph(void)
{
    TUSIGN8 returnVal = 0;
    #ifdef DISPLAY_SIZE_F
    if ((SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_LPV_BAR)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_MPV_MSV_BAR))
    #else
    if ((SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_1x6_BAR_A)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_1x6_BAR_B)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_1x9_BAR)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_2x9_BAR)
     || (SFD_MULTI_OP.displayMode_4 == HMI_DISPLAY_MODE_GRAPHIC))
    #endif
    {
        returnVal = 1;
    }
    return returnVal;
}

//+@ HMIGen valid_if_K_median @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_K_median(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_K_median @-

//+@ HMIGen valid_if_k_1point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_1point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_1point @-

//+@ HMIGen valid_if_k_2point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_2point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_2point @-

//+@ HMIGen valid_if_k_3point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_3point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_3point @-

//+@ HMIGen valid_if_k_4point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_4point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_4point @-

//+@ HMIGen valid_if_k_5point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_5point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_5point @-

//+@ HMIGen valid_if_k_6point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_6point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_6point @-

//+@ HMIGen valid_if_k_7point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_7point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_7point @-

//+@ HMIGen valid_if_k_8point @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_k_8point(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_k_8point @-

//+@ HMIGen valid_if_SignalsViewValidityTest_1 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_1(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_1 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_4 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_4(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_4 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_3 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_3(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_3 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_2 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_2(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_2 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_5 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_5(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_5 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_8 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_8(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_8 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_6 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_6(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_6 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_7 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_7(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_7 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_11 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_11(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_11 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_9 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_9(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_9 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_10 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_10(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_10 @-

//+@ HMIGen valid_if_SignalsViewValidityTest_12 @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_SignalsViewValidityTest_12(void)
{
    // Add test function here
    #error VALIDITY TEST FUNCTION NOT COMPLETED
    // return 0 if item unavailable
    // return 1 if item available
    return 1;
}
//-@ HMIGen valid_if_SignalsViewValidityTest_12 @-

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_autoscroll_enabled(void)
{
#if defined(SECOND_OPERATOR_PAGE) || defined(THIRD_OPERATOR_PAGE) || defined(FOURTH_OPERATOR_PAGE) || defined(FIFTH_OPERATOR_PAGE) || defined(SIXTH_OPERATOR_PAGE)
    TUSIGN8 returnVal = 0;
    if (SFD_MULTI_OP.autoscrollEnable == eTRUE)
    {
        returnVal = 1;
    }
    return returnVal;
#else
    return 0;
#endif
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_no_user_logged_in(void)
{
    TUSIGN8 returnVal = 1;
    TUSIGN8 GetArmLevel = AdjusterGetARMLevel();
    if ((GetArmLevel == ARM_ACCESS_LOGOUT) || (GetArmLevel == ARM_ACCESS_READ_ONLY))
    {
        returnVal = 0;
    }
    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_based_upon_service_level_protection(void)
{
    TUSIGN8 returnVal = 0;
    #if (HIGH_LEVEL_VERSION_INFO >= 21324)
        TUSIGN8 LoginLevel = AdjusterGetARMLevel();
        switch (FrameTableGetServiceLevelProtection())
        {
        case HMI_SERVICE_LEVEL_PROTECTED_BY_ADVANCED_LEVEL:
            if ((LoginLevel == ARM_ACCESS_SERVICE)
             || (LoginLevel == ARM_ACCESS_ADVANCED))
            {
                returnVal = 1;
            }
            break;

        case HMI_SERVICE_LEVEL_PROTECTED_BY_STANDARD_LEVEL:
            if ((LoginLevel == ARM_ACCESS_SERVICE)
             || (LoginLevel == ARM_ACCESS_ADVANCED)
             || (LoginLevel == ARM_ACCESS_STANDARD))
            {
                returnVal = 1;
            }
            break;

        case HMI_SERVICE_LEVEL_PROTECTED_BY_ZERO_VALUE_OBJECT:
        case HMI_SERVICE_LEVEL_PROTECTED_BY_NZERO_VALUE_OBJECT:
            if (FrameTableServiceLevelIsEnabledByObject())
            {
                returnVal = 1;
            }
            break;

        case HMI_SERVICE_LEVEL_NOT_PROTECTED:
        default:
            returnVal = 1;
            break;
        }

    #else
        returnVal = 1;
    #endif

    return returnVal;
}

#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 valid_if_resetPasswordsAvailable(void)
{
    TUSIGN8 returnVal = 0;
    #if(HIGH_LEVEL_VERSION_INFO >= 21324)
    TUSIGN8 FailedEntryLimit = FrameTableGetFailedPasswordEntryLimit();
    TUSIGN8 FailedEntryCount = FrameTableGetFailedPasswordEntryCount();

    if ((PasswordResetDisabledByHardwareWriteProtection() == eFALSE) && (FailedEntryLimit > 0) && (FailedEntryCount >= FailedEntryLimit))
    {
        returnVal = 1;
    }
    #endif
    return returnVal;
}

//+@ HMIGen PasswordResetDisabledByHardwareWriteProtection @+
#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TBOOL PasswordResetDisabledByHardwareWriteProtection(void)
{
    // Add test function here
    // return eTRUE if Password Reset feature is disabled by hardware write protection switch
    // return eFALSE if Password Reset feature is not disabled by hardware write protection switch
    //               or if hardware write protection is not implemented
    #error FUNCTION NOT COMPLETED

    return eFALSE;
}
//-@ HMIGen PasswordResetDisabledByHardwareWriteProtection @-



TUSIGN8 (*validityTestTable[])(void) =
{
&valid_always,
&valid_never,
&valid_if_DO_function_pulse,
&valid_if_DO_function_freq,
&valid_if_DO_function_logic,
&valid_if_OpMode_include_mass,
&valid_if_OpMode_include_standard,
&valid_if_OpMode_include_power,
&valid_if_use_temperature,
&valid_if_OpMode_use_pressure,
&valid_if_AI_active,
&valid_if_AI_content_T,
&valid_if_AI_content_P,
&valid_if_AI_content_P_abs,
&valid_if_AI_content_Den,
&valid_if_AI_content_GasContent,
&valid_if_AI_content_Cutoff,
&valid_if_AI_content_intT,
&valid_if_OpMode_include_liquid_mass,
&valid_if_OpMode_useVEC,
&valid_if_OpMode_useDEC,
&valid_if_OpMode_only_liquid_power,
&valid_if_OpMode_only_gas_mass,
&valid_if_OpMode_use_gasRef,
&valid_if_OpMode_include_gas_standard,
&valid_if_OpMode_only_gas_power,
&valid_if_OpMode_belongs_steam,
&valid_if_OpMode_include_steam_mass,
&valid_if_OpMode_only_WS_energy,
&valid_if_OpMode_use_densityRef,
&valid_if_need_presetDensity,
&valid_if_OpMode_use_ref_T,
&valid_if_use_presetT,
&valid_if_OpMode_use_extT,
&valid_if_OpMode_use_act_content,
&valid_if_OpMode_only_volume,
&valid_if_OpMode_only_standard,
&valid_if_OpMode_only_bia_volume,
&valid_if_OpMode_only_bia_standard,
&valid_if_OpMode_only_mass,
&valid_if_OpMode_only_power,
&valid_if_CO_T,
&valid_if_simulationMode_Qv,
&valid_if_simulationMode_Qv_percentage,
&valid_if_simulationMode_Qm,
&valid_if_simulationMode_Qm_percentage,
&valid_if_simulationMode_T,
&valid_if_simulationMode_T_pipe,
&valid_if_simulationMode_T_housing,
&valid_if_simulationMode_DO_freq,
&valid_if_simulationMode_DO_logic,
&valid_if_simulationMode_DO_pulse,
&valid_if_simulationMode_AI,
&valid_if_simulationMode_sensor_freq,
&valid_if_sensorType_vortex,
&valid_if_sensorType_swirle,
&valid_if_OpMode_include_partial_volume,
&valid_if_OpMode_include_partial_standard,
&valid_if_KSet_liquid,
&valid_if_KSet_gas,
&valid_if_use_presetDensity,
&valid_if_OpMode_use_act_T,
&valid_if_displayMode1_allows_1st_line,
&valid_if_displayMode1_allows_2nd_line,
&valid_if_displayMode1_allows_3rd_line,
&valid_if_displayMode1_allows_bargraph,
&valid_if_displayMode2_allows_1st_line,
&valid_if_displayMode2_allows_2nd_line,
&valid_if_displayMode2_allows_3rd_line,
&valid_if_displayMode2_allows_bargraph,
&valid_if_displayMode3_allows_1st_line,
&valid_if_displayMode3_allows_2nd_line,
&valid_if_displayMode3_allows_3rd_line,
&valid_if_displayMode3_allows_bargraph,
&valid_if_displayMode4_allows_1st_line,
&valid_if_displayMode4_allows_2nd_line,
&valid_if_displayMode4_allows_3rd_line,
&valid_if_displayMode4_allows_bargraph,
&valid_if_K_median,
&valid_if_k_1point,
&valid_if_k_2point,
&valid_if_k_3point,
&valid_if_k_4point,
&valid_if_k_5point,
&valid_if_k_6point,
&valid_if_k_7point,
&valid_if_k_8point,
&valid_if_SignalsViewValidityTest_1,
&valid_if_SignalsViewValidityTest_4,
&valid_if_SignalsViewValidityTest_3,
&valid_if_SignalsViewValidityTest_2,
&valid_if_SignalsViewValidityTest_5,
&valid_if_SignalsViewValidityTest_8,
&valid_if_SignalsViewValidityTest_6,
&valid_if_SignalsViewValidityTest_7,
&valid_if_SignalsViewValidityTest_11,
&valid_if_SignalsViewValidityTest_9,
&valid_if_SignalsViewValidityTest_10,
&valid_if_SignalsViewValidityTest_12,
&valid_if_autoscroll_enabled,
&valid_if_no_user_logged_in,
&valid_based_upon_service_level_protection,
&valid_if_resetPasswordsAvailable,
};


#ifdef OPTIMIZE_V3_4
#pragma optimize=size none
#else
#pragma optimize=z 0
#endif
TUSIGN8 ValidityTestCheck(
                            #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
                            FRAME_FORMAT_TYPE const *FrameAddr,
                            #endif
                            TUSIGN16 testIdx
                            #ifdef VALIDITY_TEST_CALLER
                            , TUSIGN8 caller
                            #endif
                            )
{
    TUSIGN8 retVal = 0;

    #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES

    TBOOL testRequired = eFALSE;

    TUSIGN8 DirAccReq = FrameTable_GetDirectAccessRequest();

    if (FrameAddr)
    {
        // Attempting to select a Config Header
        if (DirAccReq)
        {
            // Attempting to select a Config Header with Direct Access
            if (FrameAddr == FrameTable_GetDirectAccessEntry(DirAccReq))
            {
                // Correct frame for direct access - access allowed
                retVal = 1;
            }
            else
            {
                // incorrect frame for direct access - access not allowed
                retVal = 0;
            }
        }
        else
        {
            // Attempting to select a config header without direct access
            if ((FrameAddr == FrameTable_GetDirectAccessEntry(1)) || (FrameAddr == FrameTable_GetDirectAccessEntry(2)) || (FrameAddr == FrameTable_GetDirectAccessEntry(3)) || (FrameAddr == FrameTable_GetDirectAccessEntry(4)) || (FrameAddr == FrameTable_GetDirectAccessEntry(5)) || (FrameAddr == FrameTable_GetDirectAccessEntry(6)) || (FrameAddr == FrameTable_GetDirectAccessEntry(7)) || (FrameAddr == FrameTable_GetDirectAccessEntry(8)) || (FrameAddr == FrameTable_GetDirectAccessEntry(9)) || (FrameAddr == FrameTable_GetDirectAccessEntry(10)))
            {
                // requested frame is a direct access frame - access not allowed
                retVal = 0;
            }
            else
            {
                // requested frame is not a direct access frame - use validity test
                testRequired = eTRUE;
            }
        }
    }
    else
    {
        // Not attempting to access a Config Header
        testRequired = eTRUE;
    }

    if (testRequired)
    #endif // end of #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
    {
        #ifdef VALIDITY_TEST_CALLER
        signalValidityCallerID = caller;
        #endif

        if (testIdx <= LAST_IDF_VALID)
        {
            retVal = (*validityTestTable[testIdx])();
        }
    }
    return retVal;
}

