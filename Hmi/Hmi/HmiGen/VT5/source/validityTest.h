#ifndef _VALIDITYTEST_H
#define _VALIDITYTEST_H

#ifndef __COMMON_TYPE_H__
#error common_type.h must be included
#endif

#ifndef _COMPILETIMEDEFINITIONS
#error compileTimeDefinitions.h must be included
#endif

#define IDF_VALID_always 0
#define IDF_VALID_never  1
#define IDF_VALID_if_DO_function_pulse 2
#define IDF_VALID_if_DO_function_freq 3
#define IDF_VALID_if_DO_function_logic 4
#define IDF_VALID_if_OpMode_include_mass 5
#define IDF_VALID_if_OpMode_include_standard 6
#define IDF_VALID_if_OpMode_include_power 7
#define IDF_VALID_if_use_temperature 8
#define IDF_VALID_if_OpMode_use_pressure 9
#define IDF_VALID_if_AI_active 10
#define IDF_VALID_if_AI_content_T 11
#define IDF_VALID_if_AI_content_P 12
#define IDF_VALID_if_AI_content_P_abs 13
#define IDF_VALID_if_AI_content_Den 14
#define IDF_VALID_if_AI_content_GasContent 15
#define IDF_VALID_if_AI_content_Cutoff 16
#define IDF_VALID_if_AI_content_intT 17
#define IDF_VALID_if_OpMode_include_liquid_mass 18
#define IDF_VALID_if_OpMode_useVEC 19
#define IDF_VALID_if_OpMode_useDEC 20
#define IDF_VALID_if_OpMode_only_liquid_power 21
#define IDF_VALID_if_OpMode_only_gas_mass 22
#define IDF_VALID_if_OpMode_use_gasRef 23
#define IDF_VALID_if_OpMode_include_gas_standard 24
#define IDF_VALID_if_OpMode_only_gas_power 25
#define IDF_VALID_if_OpMode_belongs_steam 26
#define IDF_VALID_if_OpMode_include_steam_mass 27
#define IDF_VALID_if_OpMode_only_WS_energy 28
#define IDF_VALID_if_OpMode_use_densityRef 29
#define IDF_VALID_if_need_presetDensity 30
#define IDF_VALID_if_OpMode_use_ref_T 31
#define IDF_VALID_if_use_presetT 32
#define IDF_VALID_if_OpMode_use_extT 33
#define IDF_VALID_if_OpMode_use_act_content 34
#define IDF_VALID_if_OpMode_only_volume 35
#define IDF_VALID_if_OpMode_only_standard 36
#define IDF_VALID_if_OpMode_only_bia_volume 37
#define IDF_VALID_if_OpMode_only_bia_standard 38
#define IDF_VALID_if_OpMode_only_mass 39
#define IDF_VALID_if_OpMode_only_power 40
#define IDF_VALID_if_CO_T 41
#define IDF_VALID_if_simulationMode_Qv 42
#define IDF_VALID_if_simulationMode_Qv_percentage 43
#define IDF_VALID_if_simulationMode_Qm 44
#define IDF_VALID_if_simulationMode_Qm_percentage 45
#define IDF_VALID_if_simulationMode_T 46
#define IDF_VALID_if_simulationMode_T_pipe 47
#define IDF_VALID_if_simulationMode_T_housing 48
#define IDF_VALID_if_simulationMode_DO_freq 49
#define IDF_VALID_if_simulationMode_DO_logic 50
#define IDF_VALID_if_simulationMode_DO_pulse 51
#define IDF_VALID_if_simulationMode_AI 52
#define IDF_VALID_if_simulationMode_sensor_freq 53
#define IDF_VALID_if_sensorType_vortex 54
#define IDF_VALID_if_sensorType_swirle 55
#define IDF_VALID_if_OpMode_include_partial_volume 56
#define IDF_VALID_if_OpMode_include_partial_standard 57
#define IDF_VALID_if_KSet_liquid 58
#define IDF_VALID_if_KSet_gas 59
#define IDF_VALID_if_use_presetDensity 60
#define IDF_VALID_if_OpMode_use_act_T 61
#define IDF_VALID_if_displayMode1_allows_1st_line 62
#define IDF_VALID_if_displayMode1_allows_2nd_line 63
#define IDF_VALID_if_displayMode1_allows_3rd_line 64
#define IDF_VALID_if_displayMode1_allows_bargraph 65
#define IDF_VALID_if_displayMode2_allows_1st_line 66
#define IDF_VALID_if_displayMode2_allows_2nd_line 67
#define IDF_VALID_if_displayMode2_allows_3rd_line 68
#define IDF_VALID_if_displayMode2_allows_bargraph 69
#define IDF_VALID_if_displayMode3_allows_1st_line 70
#define IDF_VALID_if_displayMode3_allows_2nd_line 71
#define IDF_VALID_if_displayMode3_allows_3rd_line 72
#define IDF_VALID_if_displayMode3_allows_bargraph 73
#define IDF_VALID_if_displayMode4_allows_1st_line 74
#define IDF_VALID_if_displayMode4_allows_2nd_line 75
#define IDF_VALID_if_displayMode4_allows_3rd_line 76
#define IDF_VALID_if_displayMode4_allows_bargraph 77
#define IDF_VALID_if_K_median 78
#define IDF_VALID_if_k_1point 79
#define IDF_VALID_if_k_2point 80
#define IDF_VALID_if_k_3point 81
#define IDF_VALID_if_k_4point 82
#define IDF_VALID_if_k_5point 83
#define IDF_VALID_if_k_6point 84
#define IDF_VALID_if_k_7point 85
#define IDF_VALID_if_k_8point 86
#define IDF_VALID_if_SignalsViewValidityTest_1 87
#define IDF_VALID_if_SignalsViewValidityTest_4 88
#define IDF_VALID_if_SignalsViewValidityTest_3 89
#define IDF_VALID_if_SignalsViewValidityTest_2 90
#define IDF_VALID_if_SignalsViewValidityTest_5 91
#define IDF_VALID_if_SignalsViewValidityTest_8 92
#define IDF_VALID_if_SignalsViewValidityTest_6 93
#define IDF_VALID_if_SignalsViewValidityTest_7 94
#define IDF_VALID_if_SignalsViewValidityTest_11 95
#define IDF_VALID_if_SignalsViewValidityTest_9 96
#define IDF_VALID_if_SignalsViewValidityTest_10 97
#define IDF_VALID_if_SignalsViewValidityTest_12 98
#define IDF_VALID_if_autoscroll_enabled 99
#define IDF_VALID_if_no_user_logged_in 100
#define IDF_VALID_based_upon_service_level_protection 101
#define IDF_VALID_if_resetPasswordsAvailable 102

#define LAST_IDF_VALID 102

TUSIGN8 ValidityTestCheck(
                            #ifdef HMI_SUPPORTS_DIRECT_ACCESS_FRAMES
                            FRAME_FORMAT_TYPE const *FrameAddr,
                            #endif
                            TUSIGN16 testIdx
                            #ifdef VALIDITY_TEST_CALLER
                            , TUSIGN8 caller
                            #endif
                            );

#endif
