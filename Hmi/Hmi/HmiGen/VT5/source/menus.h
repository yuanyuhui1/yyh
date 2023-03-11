#ifndef _MENUS_H
#define _MENUS_H

#ifndef __SUBSYSTEM_IDX_H__
  #error subsystem_idx.h must be included
#endif

#ifndef _STRINGDEFINITIONS
//  #error StringDefinitions.h must be included
#endif

#ifndef _FRAMETABLEDEFINITIONS_H
  #error FrameTableDefinitions.h must be included
#endif

#ifndef _VALIDITYTEST_H
  #error validityTest.h must be included
#endif

#pragma diag_suppress=Pm022

const MENU_ITEM_TYPE menuTable[] =
{
//0 "Sensor"
 173, IDF_VALID_always,

//1 "Transmitter"
 174, IDF_VALID_always,

//2 "Access Control"
 175, IDF_VALID_always,

//3 "Sensor"
 173, IDF_VALID_always,

//4 "Transmitter"
 176, IDF_VALID_always,

//5 "Plant/Customized"
 177, IDF_VALID_always,

//6 "Language"
 29, IDF_VALID_always,

//7 "Operator Pages"
 178, IDF_VALID_always,

//8 "Autoscroll"
 179, IDF_VALID_always,

//9 "Flowrate Format"
 180, IDF_VALID_always,

//10 "Totalizer Format"
 181, IDF_VALID_always,

//11 "Date/Time Format"
 182, IDF_VALID_always,

//12 "Display Test"
 183, IDF_VALID_always,

//13 "Back Mode"
 184, IDF_VALID_always,

//14 "Bluetooth"
 185, IDF_VALID_always,

//15 "Digital Output "
 186, IDF_VALID_always,

//16 "Field Input"
 187, IDF_VALID_always,

//17 FRAMETYPEALARMHISTORY
 188, IDF_VALID_always,

//18 "Clear History"
 189, IDF_VALID_always,

//19 "Group Masking"
 190, IDF_VALID_always,

//20 "Individual Masking"
 191, IDF_VALID_always,

//21 "Alarm Limits"
 192, IDF_VALID_always,

//22 "Service Port"
 193, IDF_VALID_always,

//23 "Ethernet"
 194, IDF_VALID_always,

//24 "Diagnosis Control"
 195, IDF_VALID_always,

//25 "Diagnosis Values"
 196, IDF_VALID_always,

//26 "Simulation Mode"
 197, IDF_VALID_always,

//27 "Volume Flow Unit"
 198, IDF_VALID_if_simulationMode_Qv,

//28 "Volume Flow"
 199, IDF_VALID_if_simulationMode_Qv,

//29 "Volume Flow[%]"
 200, IDF_VALID_if_simulationMode_Qv_percentage,

//30 "Mass Flow Unit"
 201, IDF_VALID_if_simulationMode_Qm,

//31 "Mass Flow"
 202, IDF_VALID_if_simulationMode_Qm,

//32 "Mass Flow[%]"
 203, IDF_VALID_if_simulationMode_Qm_percentage,

//33 "Temperature Unit"
 204, IDF_VALID_if_simulationMode_T,

//34 "Medium Temperature"
 205, IDF_VALID_if_simulationMode_T_pipe,

//35 "Housing Temperature"
 206, IDF_VALID_if_simulationMode_T_housing,

//36 "Freq on DO "
 207, IDF_VALID_if_simulationMode_DO_freq,

//37 "Logic on DO"
 208, IDF_VALID_if_simulationMode_DO_logic,

//38 "Pulse on DO"
 209, IDF_VALID_if_simulationMode_DO_pulse,

//39 "AI Value"
 210, IDF_VALID_if_simulationMode_AI,

//40 "Sensor Freq"
 211, IDF_VALID_if_simulationMode_sensor_freq,

//41 "Output Readings"
 212, IDF_VALID_always,

//42 "Alarm Simulation"
 213, IDF_VALID_always,

//43 "Start"
 214, IDF_VALID_always,

//44 "Stop"
 215, IDF_VALID_always,

//45 "Reset"
 216, IDF_VALID_always,

//46 "Preset"
 217, IDF_VALID_always,

//47 "Overflow"
 218, IDF_VALID_always,

//48 "Transmitter"
 176, IDF_VALID_always,

//49 "Sensor"
 173, IDF_VALID_always,

//50 "Function Activation"
 219, IDF_VALID_always,

//51 "Linearisation"
 220, IDF_VALID_always,

//52 "FE parameters"
 221, IDF_VALID_always,

//53 "Factory Defaults"
 222, IDF_VALID_always,

//54 "Force Replace"
 223, IDF_VALID_always,

//55 "Sensor Type"
 224, IDF_VALID_always,

//56 "Meter(V) Size"
 225, IDF_VALID_if_sensorType_vortex,

//57 "Meter(S) Size"
 226, IDF_VALID_if_sensorType_swirle,

//58 "QvMaxDN"
 227, IDF_VALID_always,

//59 "QvpMaxDN"
 228, IDF_VALID_if_OpMode_include_partial_volume,

//60 "QmMaxDN"
 229, IDF_VALID_if_OpMode_include_mass,

//61 "QnMaxDN"
 230, IDF_VALID_if_OpMode_include_standard,

//62 "QnpMaxDN"
 231, IDF_VALID_if_OpMode_include_partial_standard,

//63 "QpowerMaxDN"
 232, IDF_VALID_if_OpMode_include_power,

//64 "Sensor ID"
 233, IDF_VALID_always,

//65 "SAP/ERP No."
 234, IDF_VALID_always,

//66 "Sensor Run Hours"
 235, IDF_VALID_always,

//67 "Calibration"
 236, IDF_VALID_always,

//68 "Transmitter Type"
 237, IDF_VALID_always,

//69 "Transmitter ID"
 238, IDF_VALID_always,

//70 "SAP/ERP No."
 234, IDF_VALID_always,

//71 "Transmitter Version"
 239, IDF_VALID_always,

//72 "Transmitter Run Hours"
 240, IDF_VALID_always,

//73 "Calibration"
 236, IDF_VALID_always,

//74 "Manufacturer"
 241, IDF_VALID_always,

//75 "Street"
 242, IDF_VALID_always,

//76 "City"
 243, IDF_VALID_always,

//77 "Phone"
 244, IDF_VALID_always,

//78 "Standard Password"
 245, IDF_VALID_always,

//79 "Read Only Switch"
 246, IDF_VALID_always,

//80 "Service Account"
 247, IDF_VALID_always,

//81 "QvMaxDN"
 227, IDF_VALID_always,

//82 "QvpMaxDN"
 228, IDF_VALID_if_OpMode_include_partial_volume,

//83 "QmMaxDN"
 229, IDF_VALID_if_OpMode_include_mass,

//84 "QnMaxDN"
 230, IDF_VALID_if_OpMode_include_standard,

//85 "QnpMaxDN"
 231, IDF_VALID_if_OpMode_include_partial_standard,

//86 "QpowerMaxDN"
 232, IDF_VALID_if_OpMode_include_power,

//87 "QvMax"
 248, IDF_VALID_if_OpMode_only_volume,

//88 "QvpMax"
 156, IDF_VALID_if_OpMode_only_bia_volume,

//89 "QmMax"
 158, IDF_VALID_if_OpMode_only_mass,

//90 "QnMax"
 155, IDF_VALID_if_OpMode_only_standard,

//91 "Qnpmax"
 249, IDF_VALID_if_OpMode_only_bia_standard,

//92 "QpowerMax"
 159, IDF_VALID_if_OpMode_only_power,

//93 "Sensor Location Tag"
 250, IDF_VALID_always,

//94 "Sensor TAG"
 251, IDF_VALID_always,

//95 "Units"
 252, IDF_VALID_always,

//96 "Damping Qv"
 160, IDF_VALID_if_OpMode_only_volume,

//97 "Damping Qvp"
 163, IDF_VALID_if_OpMode_only_bia_volume,

//98 "Damping Qm"
 165, IDF_VALID_if_OpMode_only_mass,

//99 "Damping Qn"
 162, IDF_VALID_if_OpMode_only_standard,

//100 "Damping Qnp"
 253, IDF_VALID_if_OpMode_only_bia_standard,

//101 "Damping Qpower"
 166, IDF_VALID_if_OpMode_only_power,

//102 "Damping Temperature"
 169, IDF_VALID_if_CO_T,

//103 "Low Flow Cutoff"
 172, IDF_VALID_always,

//104 "Operating Mode"
 46, IDF_VALID_always,

//105 "Compensation Setting"
 254, IDF_VALID_always,

//106 "Field optimization"
 255, IDF_VALID_always,

//107 "Operator Page 1"
 256, IDF_VALID_always,

//108 "Operator Page 2"
 257, IDF_VALID_always,

//109 "Operator Page 3"
 258, IDF_VALID_always,

//110 "Operator Page 4"
 259, IDF_VALID_always,

//111 "Function"
 281, IDF_VALID_always,

//112 "Setup Pulse Out"
 282, IDF_VALID_if_DO_function_pulse,

//113 "Setup Freq Out"
 283, IDF_VALID_if_DO_function_freq,

//114 "Alarm Config"
 284, IDF_VALID_if_DO_function_logic,

//115 "Active Mode"
 72, IDF_VALID_if_DO_function_logic,

//116 "Analog In Value"
 86, IDF_VALID_always,

//117 "T Ext. Upper Range"
 95, IDF_VALID_if_AI_content_T,

//118 "T Ext. Lower Range"
 96, IDF_VALID_if_AI_content_T,

//119 "Pressure Upper Range"
 97, IDF_VALID_if_AI_content_P,

//120 "Pressure Lower Range"
 98, IDF_VALID_if_AI_content_P,

//121 "P(abs) Upper Range"
 99, IDF_VALID_if_AI_content_P_abs,

//122 "P(abs) Lower Range"
 100, IDF_VALID_if_AI_content_P_abs,

//123 "Density Upper Range"
 101, IDF_VALID_if_AI_content_Den,

//124 "Density Lower Range"
 102, IDF_VALID_if_AI_content_Den,

//125 "Gas% Upper Range"
 103, IDF_VALID_if_AI_content_GasContent,

//126 "Gas% Lower Range"
 105, IDF_VALID_if_AI_content_GasContent,

//127 "Ext.Cutoff Trigger"
 106, IDF_VALID_if_AI_content_Cutoff,

//128 "T Int. Upper Range"
 110, IDF_VALID_if_AI_content_intT,

//129 "T Int. Lower Range"
 111, IDF_VALID_if_AI_content_intT,

//130 "React. On Input Fail"
 285, IDF_VALID_always,

//131 "Maintenance Required"
 286, IDF_VALID_always,

//132 "Function Check"
 287, IDF_VALID_always,

//133 "Out Of Specification"
 288, IDF_VALID_always,

//134 "Min Flowrate Alarm"
 289, IDF_VALID_always,

//135 "Max Flowrate Alarm"
 290, IDF_VALID_always,

//136 "Flow > 103%"
 291, IDF_VALID_always,

//137 "Flow CutOff Alarm"
 292, IDF_VALID_always,

//138 "Int. T Sensor Fault"
 293, IDF_VALID_always,

//139 "Medium Temp OffSpec."
 294, IDF_VALID_always,

//140 "Housing Temp OffSpec."
 295, IDF_VALID_always,

//141 "Min Flow Alarm"
 296, IDF_VALID_always,

//142 "Max Flow Alarm"
 297, IDF_VALID_always,

//143 "Min Temp.Alarm"
 298, IDF_VALID_always,

//144 "Max Temp.Alarm"
 299, IDF_VALID_always,

//145 "Min P(abs) Alarm"
 300, IDF_VALID_if_OpMode_use_pressure,

//146 "Max P(abs) Alarm"
 301, IDF_VALID_if_OpMode_use_pressure,

//147 "Min Re Alarm"
 302, IDF_VALID_always,

//148 "Baud Rate"
 303, IDF_VALID_always,

//149 "General"
 304, IDF_VALID_always,

//150 "Internet Layer"
 305, IDF_VALID_always,

//151 "PROFINET"
 306, IDF_VALID_always,

//152 "ModBus TCP"
 307, IDF_VALID_always,

//153 "WebServer"
 308, IDF_VALID_always,

//154 "Diagnostics"
 309, IDF_VALID_always,

//155 "Sensor Self Check"
 310, IDF_VALID_always,

//156 "Maintenance Cycle"
 311, IDF_VALID_always,

//157 "Reynolds Number"
 312, IDF_VALID_always,

//158 "Ext. Temperature"
 313, IDF_VALID_if_OpMode_use_extT,

//159 "Housing Temperature"
 206, IDF_VALID_always,

//160 "AI Value"
 210, IDF_VALID_if_AI_active,

//161 "DO Pulse"
 327, IDF_VALID_if_DO_function_pulse,

//162 "DO Frequency"
 328, IDF_VALID_if_DO_function_freq,

//163 "DO State"
 329, IDF_VALID_if_DO_function_logic,

//164 "All Totalizers"
 365, IDF_VALID_always,

//165 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//166 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//167 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//168 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//169 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//170 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//171 "All Totalizers"
 365, IDF_VALID_always,

//172 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//173 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//174 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//175 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//176 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//177 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//178 "All Totalizers"
 365, IDF_VALID_always,

//179 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//180 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//181 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//182 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//183 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//184 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//185 "Enter Preset Value"
 372, IDF_VALID_always,

//186 "Set To Preset Value"
 373, IDF_VALID_always,

//187 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//188 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//189 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//190 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//191 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//192 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//193 "Reset Run Hour"
 374, IDF_VALID_always,

//194 "Run Hours"
 375, IDF_VALID_always,

//195 "Maintenance Cycle"
 311, IDF_VALID_always,

//196 "Max Housing Temp"
 376, IDF_VALID_always,

//197 "Sensor Type"
 224, IDF_VALID_always,

//198 "Meter(V) Size"
 225, IDF_VALID_if_sensorType_vortex,

//199 "Meter(S) Size"
 226, IDF_VALID_if_sensorType_swirle,

//200 "QvMaxDN"
 377, IDF_VALID_always,

//201 "RTD Connection"
 378, IDF_VALID_always,

//202 "Max Medium Temp"
 379, IDF_VALID_always,

//203 "Reset Run Hour"
 374, IDF_VALID_always,

//204 "Run Hours"
 375, IDF_VALID_always,

//205 "Maintenance Cycle"
 311, IDF_VALID_always,

//206 "SNR"
 380, IDF_VALID_always,

//207 "SNR Threshold"
 381, IDF_VALID_always,

//208 "Auto Check En."
 382, IDF_VALID_always,

//209 "Auto Check Range"
 383, IDF_VALID_always,

//210 "Sig. Amplitude"
 384, IDF_VALID_always,

//211 "Vbr. Amplitude"
 385, IDF_VALID_always,

//212 "FFT Magnitude"
 386, IDF_VALID_always,

//213 "Calibration Status"
 387, IDF_VALID_always,

//214 "Int. RTD Password"
 388, IDF_VALID_always,

//215 "AI Password"
 389, IDF_VALID_always,

//216 "Energy Mode Password"
 390, IDF_VALID_always,

//217 "k-Linearisation"
 391, IDF_VALID_always,

//218 "k Set"
 392, IDF_VALID_always,

//219 "Liquid Linear."
 393, IDF_VALID_if_KSet_liquid,

//220 "Gas linear."
 394, IDF_VALID_if_KSet_gas,

//221 "Read Parameters"
 395, IDF_VALID_always,

//222 "Write Parameters"
 396, IDF_VALID_always,

//223 "Save As Cust. Def"
 397, IDF_VALID_always,

//224 "Save As Fact. Def"
 398, IDF_VALID_always,

//225 "Reset To Cust. Def"
 399, IDF_VALID_always,

//226 "Reset To Fact. Def"
 400, IDF_VALID_always,

//227 "Vibration Compensa."
 401, IDF_VALID_always,

//228 "Lock-in Algorithm"
 402, IDF_VALID_always,

//229 "Gain Control"
 403, IDF_VALID_always,

//230 "Auto Zero Function"
 404, IDF_VALID_always,

//231 "Freq Range"
 405, IDF_VALID_always,

//232 "Save As Fact. Def"
 398, IDF_VALID_always,

//233 "Reset To Fact. Def"
 400, IDF_VALID_always,

//234 "Cal. Date"
 445, IDF_VALID_always,

//235 "Cal. Cert. No."
 446, IDF_VALID_always,

//236 "Cal. Location"
 447, IDF_VALID_always,

//237 "Transmitter FW Ver."
 448, IDF_VALID_always,

//238 "Transmitter HW Ver."
 449, IDF_VALID_always,

//239 "Frontend FW Version"
 450, IDF_VALID_always,

//240 "Frontend HW Version"
 451, IDF_VALID_always,

//241 "Bootloader Version"
 452, IDF_VALID_always,

//242 "Cal. Date"
 445, IDF_VALID_always,

//243 "Cal. Cert. No.."
 453, IDF_VALID_always,

//244 "Cal. Location"
 447, IDF_VALID_always,

//245 "Unit Qv"
 75, IDF_VALID_always,

//246 "Unit Qm"
 76, IDF_VALID_if_OpMode_include_mass,

//247 "Unit Qnv"
 456, IDF_VALID_if_OpMode_include_standard,

//248 "Unit Qpower"
 78, IDF_VALID_if_OpMode_include_power,

//249 "Unit Density "
 79, IDF_VALID_always,

//250 "Unit Temperature "
 80, IDF_VALID_if_use_temperature,

//251 "Unit Pressure"
 81, IDF_VALID_if_OpMode_use_pressure,

//252 "Unit Volume"
 457, IDF_VALID_always,

//253 "Unit Mass "
 458, IDF_VALID_if_OpMode_include_mass,

//254 "Unit Std/Norm Vol."
 459, IDF_VALID_if_OpMode_include_standard,

//255 "Unit Energy "
 85, IDF_VALID_if_OpMode_include_power,

//256 "Liquid Mass Corr."
 112, IDF_VALID_if_OpMode_include_liquid_mass,

//257 "Volume Exp.Coef."
 116, IDF_VALID_if_OpMode_useVEC,

//258 "Density Exp.Coef."
 118, IDF_VALID_if_OpMode_useDEC,

//259 "Spec. Heat Capacity"
 460, IDF_VALID_if_OpMode_only_liquid_power,

//260 "Gas Density Selec."
 121, IDF_VALID_if_OpMode_only_gas_mass,

//261 "Gas Ref. Conditions"
 124, IDF_VALID_if_OpMode_use_gasRef,

//262 "Gas Std. Mode"
 129, IDF_VALID_if_OpMode_include_gas_standard,

//263 "Gas Energy Density"
 133, IDF_VALID_if_OpMode_only_gas_power,

//264 "Water/Steam Type"
 135, IDF_VALID_if_OpMode_belongs_steam,

//265 "Density Selection"
 139, IDF_VALID_if_OpMode_include_steam_mass,

//266 "Energy calc. method"
 144, IDF_VALID_if_OpMode_only_WS_energy,

//267 "Ref. Density "
 147, IDF_VALID_if_OpMode_use_densityRef,

//268 "Preset Density "
 148, IDF_VALID_if_use_presetDensity,

//269 "Ref. Temperature "
 149, IDF_VALID_if_OpMode_use_ref_T,

//270 "Preset Int.Temp"
 150, IDF_VALID_if_OpMode_use_act_T,

//271 "Preset Ext.Temp"
 151, IDF_VALID_if_OpMode_use_extT,

//272 "Preset Pressure(abs)"
 152, IDF_VALID_if_OpMode_use_pressure,

//273 "Preset Gas Content"
 153, IDF_VALID_if_OpMode_use_act_content,

//274 "Dynamic Viscosity"
 461, IDF_VALID_always,

//275 "Temperature Offset"
 462, IDF_VALID_if_use_temperature,

//276 "Auto Zero"
 170, IDF_VALID_always,

//277 "Auto Zero Status"
 463, IDF_VALID_always,

//278 "Low Flow Thld."
 464, IDF_VALID_always,

//279 "Advanced Filter"
 465, IDF_VALID_always,

//280 "Vibration Compensa."
 401, IDF_VALID_always,

//281 "Display Mode"
 466, IDF_VALID_always,

//282 "1st Line"
 467, IDF_VALID_if_displayMode1_allows_1st_line,

//283 "2nd Line"
 468, IDF_VALID_if_displayMode1_allows_2nd_line,

//284 "3rd Line"
 469, IDF_VALID_if_displayMode1_allows_3rd_line,

//285 "Bargraph"
 470, IDF_VALID_if_displayMode1_allows_bargraph,

//286 "Display Mode"
 466, IDF_VALID_always,

//287 "1st Line"
 467, IDF_VALID_if_displayMode2_allows_1st_line,

//288 "2nd Line"
 468, IDF_VALID_if_displayMode2_allows_2nd_line,

//289 "3rd Line"
 469, IDF_VALID_if_displayMode2_allows_3rd_line,

//290 "Bargraph"
 470, IDF_VALID_if_displayMode2_allows_bargraph,

//291 "Display Mode"
 466, IDF_VALID_always,

//292 "1st Line"
 467, IDF_VALID_if_displayMode3_allows_1st_line,

//293 "2nd Line"
 468, IDF_VALID_if_displayMode3_allows_2nd_line,

//294 "3rd Line"
 469, IDF_VALID_if_displayMode3_allows_3rd_line,

//295 "Bargraph"
 470, IDF_VALID_if_displayMode3_allows_bargraph,

//296 "Display Mode"
 466, IDF_VALID_always,

//297 "1st Line"
 467, IDF_VALID_if_displayMode4_allows_1st_line,

//298 "2nd Line"
 468, IDF_VALID_if_displayMode4_allows_2nd_line,

//299 "3rd Line"
 469, IDF_VALID_if_displayMode4_allows_3rd_line,

//300 "Bargraph"
 470, IDF_VALID_if_displayMode4_allows_bargraph,

//301 "Pulses Per Unit"
 65, IDF_VALID_if_DO_function_pulse,

//302 "Pulse Width"
 67, IDF_VALID_if_DO_function_pulse,

//303 "Lower Freqency"
 69, IDF_VALID_if_DO_function_freq,

//304 "Upper Freqency"
 71, IDF_VALID_if_DO_function_freq,

//305 "General Alarm"
 471, IDF_VALID_always,

//306 "Min Flowrate Alarm"
 289, IDF_VALID_always,

//307 "Max Flowrate Alarm"
 290, IDF_VALID_always,

//308 "Max Sensor T Alarm"
 472, IDF_VALID_always,

//309 "Min Sensor T Alarm"
 473, IDF_VALID_always,

//310 "Flow Cutoff Alarm"
 474, IDF_VALID_always,

//311 "Protocol Stack"
 478, IDF_VALID_always,

//312 "DHCP Client"
 479, IDF_VALID_always,

//313 "Host IP Address"
 480, IDF_VALID_always,

//314 "Subnet Mask"
 481, IDF_VALID_always,

//315 "Gateway IP Address"
 482, IDF_VALID_always,

//316 "NTP Server IP Address"
 483, IDF_VALID_always,

//317 "Host Name"
 484, IDF_VALID_always,

//318 "Domain Name"
 485, IDF_VALID_always,

//319 "DNS IP Address"
 486, IDF_VALID_always,

//320 "Access"
 487, IDF_VALID_always,

//321 "Vendor ID"
 488, IDF_VALID_always,

//322 "Device Name"
 489, IDF_VALID_always,

//323 "Device Type"
 490, IDF_VALID_always,

//324 "Access"
 487, IDF_VALID_always,

//325 "IEEE Format"
 491, IDF_VALID_always,

//326 "Access"
 487, IDF_VALID_always,

//327 "Reset Credentials"
 492, IDF_VALID_always,

//328 "Session Timeout"
 493, IDF_VALID_always,

//329 "TCP Connections"
 494, IDF_VALID_always,

//330 "Data Received"
 495, IDF_VALID_always,

//331 "Data Transmitted"
 496, IDF_VALID_always,

//332 "Receive Errors"
 497, IDF_VALID_always,

//333 "Transmit Collisions"
 498, IDF_VALID_always,

//334 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//335 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//336 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//337 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//338 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//339 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//340 "All Totalizers"
 365, IDF_VALID_always,

//341 "Act.Volume Totalizer"
 366, IDF_VALID_always,

//342 "Std.Volume Totalizer"
 367, IDF_VALID_if_OpMode_include_standard,

//343 "Mass Totalizer"
 368, IDF_VALID_if_OpMode_include_mass,

//344 "Energy Totalizer"
 369, IDF_VALID_if_OpMode_include_power,

//345 "Net Act.Vol.Totalizer"
 370, IDF_VALID_if_OpMode_include_partial_volume,

//346 "Net Std.Vol.Totalizer"
 371, IDF_VALID_if_OpMode_include_partial_standard,

//347 "Overflow Count"
 499, IDF_VALID_always,

//348 "Overflow Thres."
 500, IDF_VALID_always,

//349 "Overflow Count"
 499, IDF_VALID_always,

//350 "Overflow Thres."
 500, IDF_VALID_always,

//351 "Overflow Count"
 499, IDF_VALID_always,

//352 "Overflow Thres."
 500, IDF_VALID_always,

//353 "Overflow Count"
 499, IDF_VALID_always,

//354 "Overflow Thres."
 500, IDF_VALID_always,

//355 "Overflow Count"
 499, IDF_VALID_always,

//356 "Overflow Thres."
 500, IDF_VALID_always,

//357 "Overflow Count"
 499, IDF_VALID_always,

//358 "Overflow Thres."
 500, IDF_VALID_always,

//359 "Average  K"
 517, IDF_VALID_if_K_median,

//360 "K1"
 518, IDF_VALID_if_k_1point,

//361 "K2"
 519, IDF_VALID_if_k_2point,

//362 "K3"
 520, IDF_VALID_if_k_3point,

//363 "K4"
 521, IDF_VALID_if_k_4point,

//364 "K5"
 522, IDF_VALID_if_k_5point,

//365 "K6"
 523, IDF_VALID_if_k_6point,

//366 "K7"
 524, IDF_VALID_if_k_7point,

//367 "K8"
 525, IDF_VALID_if_k_8point,

//368 "Freq1"
 526, IDF_VALID_if_k_1point,

//369 "Freq2"
 527, IDF_VALID_if_k_2point,

//370 "Freq3"
 528, IDF_VALID_if_k_3point,

//371 "Freq4"
 529, IDF_VALID_if_k_4point,

//372 "Freq5"
 530, IDF_VALID_if_k_5point,

//373 "Freq6"
 531, IDF_VALID_if_k_6point,

//374 "Freq7"
 532, IDF_VALID_if_k_7point,

//375 "Freq8"
 533, IDF_VALID_if_k_8point,

//376 "Average K"
 534, IDF_VALID_if_K_median,

//377 "K1"
 518, IDF_VALID_if_k_1point,

//378 "K2"
 519, IDF_VALID_if_k_2point,

//379 "K3"
 520, IDF_VALID_if_k_3point,

//380 "K4"
 521, IDF_VALID_if_k_4point,

//381 "K5"
 522, IDF_VALID_if_k_5point,

//382 "K6"
 523, IDF_VALID_if_k_6point,

//383 "K7"
 524, IDF_VALID_if_k_7point,

//384 "K8"
 525, IDF_VALID_if_k_8point,

//385 "Freq1"
 526, IDF_VALID_if_k_1point,

//386 "Freq2"
 527, IDF_VALID_if_k_2point,

//387 "Freq3"
 528, IDF_VALID_if_k_3point,

//388 "Freq4"
 529, IDF_VALID_if_k_4point,

//389 "Freq5"
 530, IDF_VALID_if_k_5point,

//390 "Freq6"
 531, IDF_VALID_if_k_6point,

//391 "Freq7"
 532, IDF_VALID_if_k_7point,

//392 "Freq8"
 533, IDF_VALID_if_k_8point,

//393 "Correction On"
 535, IDF_VALID_always,

//394 "Filter Step"
 536, IDF_VALID_always,

//395 "Num Of Coef."
 537, IDF_VALID_always,

//396 "Min Integration Period"
 538, IDF_VALID_always,

//397 "Max Integration Period"
 539, IDF_VALID_always,

//398 "Estimation Error Margin"
 540, IDF_VALID_always,

//399 "Accuracy In High Samp."
 541, IDF_VALID_always,

//400 "Accuracy In Mid Samp."
 542, IDF_VALID_always,

//401 "Accuracy In Low Samp."
 543, IDF_VALID_always,

//402 "Analogue Gain"
 544, IDF_VALID_always,

//403 "Gain Ctrl On"
 545, IDF_VALID_always,

//404 "Gain Low Thld."
 546, IDF_VALID_always,

//405 "Gain High Thld."
 547, IDF_VALID_always,

//406 "Margin In Freq"
 548, IDF_VALID_always,

//407 "Gain Max Shift"
 549, IDF_VALID_always,

//408 "Low Flow Thld. On"
 550, IDF_VALID_always,

//409 "Low Flow Thld."
 464, IDF_VALID_always,

//410 "Coefficient"
 551, IDF_VALID_always,

//411 "Noise Samp. Size"
 552, IDF_VALID_always,

//412 "Static Decimation Bits"
 553, IDF_VALID_always,

//413 "Middle Decimation Bits"
 554, IDF_VALID_always,

//414 "Max Freq"
 555, IDF_VALID_always,

//415 "Min Freq"
 556, IDF_VALID_always,

//416 "Min Margin"
 557, IDF_VALID_always,

//417 "Max Margin"
 558, IDF_VALID_always,

//418 "Diagnostics"
 26, IDF_VALID_always,

//419 "Operator Page 1"
 733, IDF_VALID_always,

//420 "Operator Page 2"
 734, IDF_VALID_if_displayMode2_allows_1st_line,

//421 "Operator Page 3"
 735, IDF_VALID_if_displayMode3_allows_1st_line,

//422 "Operator Page 4"
 736, IDF_VALID_if_displayMode4_allows_1st_line,

//423 "Autoscroll"
 737, IDF_VALID_if_autoscroll_enabled,

//424 "Signals View"
 738, IDF_VALID_always,

//425 "Logout"
 740, IDF_VALID_if_no_user_logged_in,

//426 "Read Only"
 741, IDF_VALID_always,

//427 "Standard"
 742, IDF_VALID_always,

//428 "Service"
 28, IDF_VALID_based_upon_service_level_protection,

//429 "Reset Passwords"
 743, IDF_VALID_if_resetPasswordsAvailable,

};

const TUSIGN8 EasySetupValidityTestTable_1[] =
{
IDF_VALID_always,
IDF_VALID_always,
IDF_VALID_always,
IDF_VALID_if_DO_function_pulse,
IDF_VALID_if_DO_function_pulse,
IDF_VALID_if_DO_function_freq,
IDF_VALID_if_DO_function_freq,
IDF_VALID_if_DO_function_logic,
IDF_VALID_always,
IDF_VALID_if_OpMode_include_mass,
IDF_VALID_if_OpMode_include_standard,
IDF_VALID_if_OpMode_include_power,
IDF_VALID_always,
IDF_VALID_if_use_temperature,
IDF_VALID_if_OpMode_use_pressure,
IDF_VALID_always,
IDF_VALID_if_OpMode_include_mass,
IDF_VALID_if_OpMode_include_standard,
IDF_VALID_if_OpMode_include_power,
IDF_VALID_if_AI_active,
IDF_VALID_if_AI_content_T,
IDF_VALID_if_AI_content_T,
IDF_VALID_if_AI_content_P,
IDF_VALID_if_AI_content_P,
IDF_VALID_if_AI_content_P_abs,
IDF_VALID_if_AI_content_P_abs,
IDF_VALID_if_AI_content_Den,
IDF_VALID_if_AI_content_Den,
IDF_VALID_if_AI_content_GasContent,
IDF_VALID_if_AI_content_GasContent,
IDF_VALID_if_AI_content_Cutoff,
IDF_VALID_if_AI_content_intT,
IDF_VALID_if_AI_content_intT,
IDF_VALID_if_OpMode_include_liquid_mass,
IDF_VALID_if_OpMode_useVEC,
IDF_VALID_if_OpMode_useDEC,
IDF_VALID_if_OpMode_only_liquid_power,
IDF_VALID_if_OpMode_only_gas_mass,
IDF_VALID_if_OpMode_use_gasRef,
IDF_VALID_if_OpMode_include_gas_standard,
IDF_VALID_if_OpMode_only_gas_power,
IDF_VALID_if_OpMode_belongs_steam,
IDF_VALID_if_OpMode_include_steam_mass,
IDF_VALID_if_OpMode_only_WS_energy,
IDF_VALID_if_OpMode_use_densityRef,
IDF_VALID_if_need_presetDensity,
IDF_VALID_if_OpMode_use_ref_T,
IDF_VALID_if_use_presetT,
IDF_VALID_if_OpMode_use_extT,
IDF_VALID_if_OpMode_use_pressure,
IDF_VALID_if_OpMode_use_act_content,
IDF_VALID_if_OpMode_only_volume,
IDF_VALID_if_OpMode_only_standard,
IDF_VALID_if_OpMode_only_bia_volume,
IDF_VALID_if_OpMode_only_bia_standard,
IDF_VALID_if_OpMode_only_mass,
IDF_VALID_if_OpMode_only_power,
IDF_VALID_if_OpMode_only_volume,
IDF_VALID_if_OpMode_only_standard,
IDF_VALID_if_OpMode_only_bia_volume,
IDF_VALID_if_OpMode_only_bia_standard,
IDF_VALID_if_OpMode_only_mass,
IDF_VALID_if_OpMode_only_power,
IDF_VALID_if_CO_T,
IDF_VALID_if_CO_T,
IDF_VALID_if_CO_T,
IDF_VALID_always,
IDF_VALID_always,
};

const ESVT_LIST_OF_TABLES_ENTRY EasySetupValidityTestListOfTables[] = 
{
EasySetupValidityTestTable_1, 68,
};

#pragma diag_default=Pm022


#endif
