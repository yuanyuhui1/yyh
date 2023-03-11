//--------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright      Copyright ABB, 2011.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Subsystem Hart
 Module
 Description    class definition T_UNIT::HART

                subsystem interface
                for Subsystems based on T_UNIT.

 Remarks
 </pre>
*/
//----------------------------------------------------------------------------------------------------------
//@SubGen Version@<0x020101>

#ifndef __SYSTEM_H__
  #error system.h must be included before hart.h
#endif

#ifndef __COMMON_TYPE_H__
  #error common_type.h must be include before hart.h
#endif

#ifndef __T_UNIT_H__
  #error t_unit.h must be include before hart.h
#endif

#if (T_UNIT_VERSION<0x020100)
  #error T_UNIT_VERSION must be greater or equal than 2.1.0
#endif

//............................................
//!  extend include list if neccessary

#ifdef __HART_H__
  #error hart.h included more than once
#endif

#define __HART_H__


#define HART_VERSION (0x010000) // 1.0.0
//----------------------------------------------------------------------------------------------------------
//!  define macros, enums

//@SubGen start@  <TYPES>
  // BurstModeEnumTable
  #define HART_BURSTMODE_OFF 0 //Burst Mode off
  #define HART_BURSTMODE_ENABLE_ON_TOKEN_PASSING_DLL 1 //Burst Mode enabled on Token-Passing Data Link Layer

  // BurstCmdNumberHARTEnumTable
  #define HART_BURST_COMMAND_1 1 //HART Command 1
  #define HART_BURST_COMMAND_2 2 //HART Command 2
  #define HART_BURST_COMMAND_3 3 //HART Command 3
  #define HART_BURST_COMMAND_9 9 //HART Command 9
  #define HART_BURST_COMMAND_33 33 //HART Command 33
  #define HART_BURST_COMMAND_48 48 //HART Command 48

  // BurstTriggerModeCodeTable
  #define HART_BURST_TRIGGER_MODE_CONTINUOUS 0 //
  #define HART_BURST_TRIGGER_MODE_WINDOW 1 //
  #define HART_BURST_TRIGGER_MODE_RISING 2 //
  #define HART_BURST_TRIGGER_MODE_FAILING 3 //
  #define HART_BURST_TRIGGER_MODE_ON_CHANGE 4 //

  // ConfigExitTimerEnumTable
  #define HART_CONFIG_TIMER_STOP 0 //Stop the Configtimer
  #define HART_CONFIG_TIMER_START 1 //Start the Configtimer

  // AutoAdjustConfigStateEnumTable
  #define HART_INIT_FALSE 0 //no initialisation realised
  #define HART_INIT_ONLY_CONTROLLER_FINISHED 3 //init only controllerdata
  #define HART_INIT_ONLY_VALVERANGE_FINISHED 5 //init only valve range
  #define HART_INIT_OK 7 //initialisation realised

  // CounterResetEnumTable
  #define HART_NO_RESET 0 //no reset will be done
  #define HART_RESET_WDOG_COUNTER 2 //Erase Watchdog Counter
  #define HART_RESET_POWERDN_COUNTER 4 //Erase Power Down Counter
  #define HART_RESET_EEPROM_ERR_COUNTER 8 //Erase EEPROM Error Counter
  #define HART_RESET_COMM_ERR_COUNTER 16 //Erase communication error Counter
  #define HART_RESET_COMM_TIMEOUT_COUNTER 32 //Erase communication time-out counter

  // eepromControlTable
  #define HART_EEP_CTRL_SAVE_DATA 0 //
  #define HART_EEP_CTRL_LOAD_DATA 1 //
  #define HART_EEP_CTRL_PASSIVE 255 //

  // PV_AlarmSelctionCodeTable
  #define HART_PV_ALARMSEL_HIGH 0 //
  #define HART_PV_ALARMSEL_LOW 1 //
  #define HART_PV_ALARMSEL_HOLDLASTOUTPVALUE 239 //
  #define HART_PV_ALARMSEL_NOT_USED 250 //
  #define HART_PV_ALARMSEL_NONE 251 //
  #define HART_PV_ALARMSEL_UNKNOWN 252 //
  #define HART_PV_ALARMSEL_SPECIAL 253 //

  // PV_Transfer_FunctionCodeTable
  #define HART_PV_TRANSF_FC_LINEAR 0 //
  #define HART_PV_TRANSF_FC_SQUAREROOT 1 //
  #define HART_PV_TRANSF_FC_SQUAREROOT_THIRDPOW 2 //
  #define HART_PV_TRANSF_FC_SQUAREROOT_FIFTHPOW 3 //
  #define HART_PV_TRANSF_FC_SPECIALCURVE 4 //
  #define HART_PV_TRANSF_FC_SQUARE 5 //
  #define HART_PV_TRANSF_FC_NOT_USED 250 //
  #define HART_PV_TRANSF_FC_NONE 251 //
  #define HART_PV_TRANSF_FC_UNKOWN 252 //
  #define HART_PV_TRANSF_FC_SPECIAL 253 //

  // LoopCurrentModeTab
  #define HART_LOOP_CURRENT_MODE_DISABLED 0 //
  #define HART_LOOP_CURRENT_MODE_ENABLED 1 //

  // HartRevisionTable
  #define HART_HART_REVISION_5 5 //
  #define HART_HART_REVISION_7 7 //

  // FindDeviceSwitchTable
  #define HART_FIND_DEVICE_DISABLED 0 //
  #define HART_FIND_DEVICE_ONE_TIME 1 //
  #define HART_FIND_DEVICE_CONTINUOUS 2 //

  // 
  #define HART_LAYER7_CMD_INTERPRETER_CYCL_TIME 100 //
  #define HART_DEVICE_REV_MAPPED_TO_DIFF_VAL 255 //
  #define HART_T_ELECTRIC_CURRENT 7 //

  // ExtendedDeviceStatusTable
  #define HART_DEFAULT_EXTENDED_DEVICE_STATUS 0 //
  #define HART_MAINTENANCE_REQUIRED 1 //
  #define HART_DEVICE_VARIABLE_ALERT 2 //
  #define HART_CRITICAL_POWER_FAILURE 4 //

  // DeviceOperatingModeTable
  #define HART_DFEAULT_DEVICE_OPERATING_MODE 0 //

  // StandardizedStatus0Table
  #define HART_DEFAULT_STANDARDIZED_STATUS_0 0 //
  #define HART_SIMULATION_ACTIVE 1 //
  #define HART_NON_VOLATILE_MEMORY_DEFECT 2 //
  #define HART_VOLATILE_MEMORY_DEFECT 4 //
  #define HART_WATCHDOG_RESET_EXECUTED 8 //
  #define HART_VOLTAGE_CONDITIONS_OUT_OF_RANGE 16 //
  #define HART_ENVIRONMENTAL_CONDITIONS_OUT_OF_RANGE 32 //
  #define HART_ELECTRONIC_DEFECT 64 //

  // AnalogChannelFixedTable
  #define HART_DEFAULT_ANALOG_CHANNEL_FIXED 0 //
  #define HART_ANALOG_CHANNEL_1_FIXED 1 //
  #define HART_ANALOG_CHANNEL_2_FIXED 2 //
  #define HART_ANALOG_CHANNEL_3_FIXED 4 //
  #define HART_ANALOG_CHANNEL_4_FIXED 8 //

  // performDeviceResetTable
  #define HART_PERFORM_RESET_DEFAULT 0 //
  #define HART_PERFORM_RESET_PREPARE 85 //
  #define HART_PERFORM_RESET_GO 170 //
  #define HART_PERFORM_RESET 255 //

  // lockDeviceSelectionTable
  #define HART_LOCK_DEVICE_UNLOCKED 0 //unlocked, funciton disabled
  #define HART_LOCK_DEVICE_TEMPORARY 1 //i.e. device reset or power loss releases the lock
  #define HART_LOCK_DEVICE_PERMANENT 2 //i.e. device reset or power loss does not affect the lock
  #define HART_LOCK_DEVICE_ALL 3 //no changes in the device configuration, by any master is allowed

  // lockDeviceStateTable
  #define HART_LOCK_STATE_UNLOCKED 0 //See GetDeviceLockState(). Local panels check for unlocked else locked,
  #define HART_LOCK_STATE_SEC_MASTER_LOCKED 65 //See GetDeviceLockState().  The secondary master channel is protected,
  #define HART_LOCK_STATE_SEC_MASTER_LOCKED_ALL 66 //See GetDeviceLockState().  The secondary master initiated lock all, so only the sec master can remove this protection
  #define HART_LOCK_STATE_PRIM_MASTER_LOCKED 129 //See GetDeviceLockState().  The primary master channel is protected,
  #define HART_LOCK_STATE_PRIM_MASTER_LOCKED_ALL 130 //See GetDeviceLockState().  The primary master initiated lock all, so only the prim master can remove this protection

  // lockDeviceStatusTable
  #define HART_LOCK_STATUS_NOT_LOCKED 0 //See GetDeviceLockStatus(). Local panels check for unlocked else locked
  #define HART_LOCK_STATUS_LOCKED 1 //See GetDeviceLockStatus(). Local panels check for unlocked else locked
  #define HART_LOCK_STATUS_PERMANENT_LOCK 2 //See GetDeviceLockStatus(). Local panels check for unlocked else locked
  #define HART_LOCK_STATUS_PRIM_MASTER_LOCK 4 //See GetDeviceLockStatus(). Local panels check for unlocked else locked
  #define HART_LOCK_STATUS_ALL_LOCK 8 //See GetDeviceLockStatus(). Local panels check for unlocked else locked
  #define HART_LOCK_STATUS_GATEWAY_LOCK 16 //See GetDeviceLockStatus(). Local panels check for unlocked else locked

  // pvtLabelDistCode_Command15_Table
  #define HART_MFR_ID_HARTMANN_BRAUN 22 //Value as used in HART 5.9 devices with command 15
  #define HART_MFR_ID_ABB 26 //Value as used in HART 5.9 devices with command 15
  #define HART_VALUE_USED_IN_HART7 250 //Value as used in HART 7.x devices with command 15

  // T_HART_UNIT_TYPES
  #define HART_UM_T_NOT_CLASSIFIED 0 //Device Variable Classification introduced HART6,
  #define HART_UM_T_LENGTH 69 //Device Variable Classification introduced HART6
  #define HART_UM_T_MASS 71 //Device Variable Classification introduced HART6
  #define HART_UM_T_TIME 70 //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_CURRENT 84 //Device Variable Classification introduced HART6
  #define HART_UM_T_TEMPERATURE 64 //Device Variable Classification introduced HART6
  #define HART_UM_T_AMOUNT_OF_SUBSTANCE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_LUMINOUS_INTENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_AREA 76 //Device Variable Classification introduced HART6
  #define HART_UM_T_VOLUME 68 //Device Variable Classification introduced HART6
  #define HART_UM_T_VELOCITY 67 //Device Variable Classification introduced HART6
  #define HART_UM_T_ACCELERATION HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_WAVENUMBER HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MASS_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_SPECIFIC_VOLUME HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_CURRENT_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MAGNETIC_FIELD_STRENGTH HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_CONCENTRATION 90 //Device Variable Classification introduced HART6
  #define HART_UM_T_LUMINANCE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_REFRACTIVE_INDEX HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_VOLUME_FLOW 66 //Device Variable Classification introduced HART6
  #define HART_UM_T_MASS_FLOW 72 //Device Variable Classification introduced HART6
  #define HART_UM_T_PLANE_ANGLE 86 //Device Variable Classification introduced HART6
  #define HART_UM_T_SOLID_ANGLE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_FREQUENCY 80 //Device Variable Classification introduced HART6
  #define HART_UM_T_FORCE 78 //Device Variable Classification introduced HART6
  #define HART_UM_T_PRESSURE 65 //Device Variable Classification introduced HART6
  #define HART_UM_T_ENERGY 77 //Device Variable Classification introduced HART6
  #define HART_UM_T_POWER 79 //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_CHARGE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_EMF 83 //Device Variable Classification introduced HART6
  #define HART_UM_T_CAPACITANCE 82 //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_RESISTANCE 85 //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_RESISTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_CONDUCTANCE 87 //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_CONDUCTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MAGNETIC_FLUX HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MAGNETIC_FLUX_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_INDUCTANCE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_LUMINOUS_FLUX HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ILLUMINANCE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_RADIONUCLIDE_ACTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ABSORBED_DOSE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_EQUIVALENT_DOSE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_CATALYTIC_ACTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_DYNAMIC_VISCOSITY 74 //Device Variable Classification introduced HART6
  #define HART_UM_T_MOMENT_OF_FORCE 77 //TODO Device Variable Classification introduced HART6
  #define HART_UM_T_SURFACE_TENSION HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ANGULAR_VELOCITY 75 //Device Variable Classification introduced HART6
  #define HART_UM_T_ANGULAR_ACCELERATION HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_HEAT_FLUX_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_HEAT_CAPACITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_SPECIFIC_HEAT_CAPACITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_SPECIFIC_ENERGY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_THERMAL_CONDUCTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ENERGY_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_FIELD_STRENGTH HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_CHARGE_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ELECTRIC_FLUX_DENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_PERMITTIVITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_PERMEABILITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MOLAR_ENERGY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_MOLAR_ENTROPY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_EXPOSURE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_ABSORBED_DOSE_RATE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_RADIANT_INTENSITY HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_RADIANCE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_CATALYTIC_CONCENTRATION HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_VISCOSITY 74 //Device Variable Classification introduced HART6
  #define HART_UM_T_VOLUME_PER_VOLUME 88 //Device Variable Classification introduced HART6
  #define HART_UM_T_ANALYTICAL 81 //Device Variable Classification introduced HART6
  #define HART_UM_T_USER_UNIT_GROUP HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_FLOWPIPE HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6
  #define HART_UM_T_NORM_VOLUME HART_UM_T_NOT_CLASSIFIED //added20110426
  #define HART_UM_T_NORMAL_VOLUME_FLOW HART_UM_T_NOT_CLASSIFIED //added20110426
  #define HART_UM_T_UNKNOWN_UNIT_GROUP HART_UM_T_NOT_CLASSIFIED //Device Variable Classification introduced HART6

  // T_HART_UNITS
  #define HART_UM_UKNOWN_UNIT 252 //Global
  #define HART_UM_METER_PER_SECOND 21 //Velocity
  #define HART_UM_MILLIMETER_PER_SECOND NOT_SUPPORTED //Velocity TODO custom-unit
  #define HART_UM_MILLIMETER_PER_MINUTE NOT_SUPPORTED //Velocity
  #define HART_UM_METER_PER_HOUR 120 //Velocity
  #define HART_UM_KILOMETER_PER_HOUR NOT_SUPPORTED //Velocity
  #define HART_UM_INCH_PER_SECOND 114 //Velocity
  #define HART_UM_INCH_PER_MINUTE 115 //Velocity
  #define HART_UM_INCH_PER_HOUR NOT_SUPPORTED //Velocity
  #define HART_UM_FEET_PER_SECOND 20 //Velocity
  #define HART_UM_FEET_PER_MINUTE 116 //Velocity
  #define HART_UM_FEET_PER_HOUR NOT_SUPPORTED //Velocity
  #define HART_UM_YARD_PER_SECOND NOT_SUPPORTED //Velocity
  #define HART_UM_YARD_PER_MINUTE NOT_SUPPORTED //Velocity
  #define HART_UM_YARD_PER_HOUR NOT_SUPPORTED //Velocity
  #define HART_UM_MILE_PER_HOUR NOT_SUPPORTED //Velocity
  #define HART_UM_KNOT NOT_SUPPORTED //Velocity
  #define HART_UM_LITER_PER_SECOND 24 //Volume Flow
  #define HART_UM_LITER_PER_MINUTE 17 //Volume Flow
  #define HART_UM_LITER_PER_HOUR 138 //Volume Flow
  #define HART_UM_CUBIC_FEET_PER_SECOND 26 //Volume Flow
  #define HART_UM_CUBIC_FEET_PER_MINUTE 15 //Volume Flow
  #define HART_UM_CUBIC_FEET_PER_HOUR 130 //Volume Flow
  #define HART_UM_CUBIC_FEET_PER_DAY 27 //Volume Flow
  #define HART_UM_CUBIC_METER_PER_SECOND 28 //Volume Flow
  #define HART_UM_CUBIC_METER_PER_MINUTE 131 //Volume Flow
  #define HART_UM_CUBIC_METER_PER_HOUR 19 //Volume Flow
  #define HART_UM_CUBIC_METER_PER_DAY 29 //Volume Flow
  #define HART_UM_US_GALLON_PER_SECOND 22 //Volume Flow
  #define HART_UM_US_GALLON_PER_MINUTE 16 //Volume Flow
  #define HART_UM_US_GALLON_PER_HOUR 136 //Volume Flow
  #define HART_UM_US_GALLON_PER_DAY 235 //Volume Flow
  #define HART_UM_IMPERIAL_GALLON_PER_SECOND 137 //Volume Flow
  #define HART_UM_IMPERIAL_GALLON_PER_MINUTE 18 //Volume Flow
  #define HART_UM_IMPERIAL_GALLON_PER_HOUR 30 //Volume Flow
  #define HART_UM_IMPERIAL_GALLON_PER_DAY 31 //Volume Flow
  #define HART_UM_BARREL_US_BEER_PER_HOUR NOT_SUPPORTED //Volume Flow TODO custom-unit
  #define HART_UM_HECTOLITER_PER_HOUR NOT_SUPPORTED //Volume Flow TODO custom-unit
  #define HART_UM_MEGALITER_PER_DAY 25 //Volume Flow
  #define HART_UM_MEGA_US_GALLON_PER_DAY 23 //Volume Flow
  #define HART_UM_CUBIC_MICROMETER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MICROMETER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MICROMETER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MICROMETER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIMETER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIMETER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIMETER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIMETER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_CENTIMETER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_CENTIMETER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_CENTIMETER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_CENTIMETER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOMETER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOMETER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOMETER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOMETER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MEGAMETER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MEGAMETER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MEGAMETER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MEGAMETER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIFEET_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIFEET_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIFEET_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MILLIFEET_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOFEET_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOFEET_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOFEET_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_KILOFEET_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_CUBIC_MEGAFEET_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_INCH_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_INCH_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_INCH_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_INCH_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_FEET_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_FEET_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_FEET_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_ACRE_FEET_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_LITER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOLITER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGALITER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGALITER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_HECTOLITER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_HECTOLITER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_HECTOLITER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOLITER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOLITER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOLITER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_US_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_US_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_US_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_US_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_US_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_US_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_US_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_US_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_US_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_US_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_US_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_US_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_US_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_US_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_US_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_IMPERIAL_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_IMPERIAL_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_IMPERIAL_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_IMPERIAL_GALLON_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_IMPERIAL_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_IMPERIAL_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_IMPERIAL_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_IMPERIAL_GALLON_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_IMPERIAL_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_IMPERIAL_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_IMPERIAL_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_IMPERIAL_GALLON_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICRO_IMPERIAL_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLI_IMPERIAL_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILO_IMPERIAL_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGA_IMPERIAL_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLION_GALLON_PER_DAY NOT_SUPPORTED //Volume Flow TODO duplicate?
  #define HART_UM_BARREL_PER_SECOND 132 //Volume Flow
  #define HART_UM_BARREL_PER_MINUTE 133 //Volume Flow
  #define HART_UM_BARREL_PER_HOUR 134 //Volume Flow
  #define HART_UM_BARREL_PER_DAY 135 //Volume Flow
  #define HART_UM_MICROBARREL_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGABARREL_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGABARREL_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICROBARREL_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICROBARREL_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGABARREL_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICROBARREL_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MEGABARREL_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_US_BEER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_US_BEER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_KILOBARREL_US_BEER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_US_BEER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_US_BEER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_US_BEER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_US_BEER_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_US_BEER_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_US_BEER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MILLIBARREL_US_BEER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICROBARREL_US_BEER_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_MICROBARREL_US_BEER_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_LIQUID_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_LIQUID_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_LIQUID_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_FEDERAL_PER_SECOND NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_FEDERAL_PER_MINUTE NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_FEDERAL_PER_HOUR NOT_SUPPORTED //Volume Flow
  #define HART_UM_BARREL_FEDERAL_PER_DAY NOT_SUPPORTED //Volume Flow
  #define HART_UM_NORMAL_CUBIC_METER_PER_SECOND_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_NORMAL_CUBIC_METER_PER_MINUTE_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_NORMAL_CUBIC_METER_PER_HOUR_0C_1ATM 121 //Norm Volume Flow
  #define HART_UM_NORMAL_CUBIC_METER_PER_DAY_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_NORMAL_LITER_PER_SECOND_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_NORMAL_LITER_PER_MINUTE_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_NORMAL_LITER_PER_HOUR_0C_1ATM 122 //Norm Volume Flow
  #define HART_UM_NORMAL_LITER_PER_DAY_0C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_FEET_PER_MINUTE_60F_1ATM 123 //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_FEET_PER_HOUR_60F_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_METER_PER_SECOND_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_METER_PER_MINUTE_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_METER_PER_HOUR_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_CUBIC_METER_PER_DAY_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_LITER_PER_SECOND_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_LITER_PER_MINUTE_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_LITER_PER_HOUR_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_STANDARD_LITER_PER_DAY_20C_1ATM NOT_SUPPORTED //Norm Volume Flow
  #define HART_UM_GRAM_PER_SECOND 70 //Mass Flow
  #define HART_UM_GRAM_PER_MINUTE 71 //Mass Flow
  #define HART_UM_GRAM_PER_HOUR 72 //Mass Flow
  #define HART_UM_GRAM_PER_DAY NOT_SUPPORTED //Mass Flow
  #define HART_UM_KILOGRAM_PER_SECOND 73 //Mass Flow
  #define HART_UM_KILOGRAM_PER_MINUTE 74 //Mass Flow
  #define HART_UM_KILOGRAM_PER_HOUR 75 //Mass Flow
  #define HART_UM_KILOGRAM_PER_DAY 76 //Mass Flow
  #define HART_UM_METRIC_TON_PER_SECOND NOT_SUPPORTED //Mass Flow
  #define HART_UM_METRIC_TON_PER_MINUTE 77 //Mass Flow
  #define HART_UM_METRIC_TON_PER_HOUR 78 //Mass Flow
  #define HART_UM_METRIC_TON_PER_DAY 79 //Mass Flow
  #define HART_UM_POUND_PER_SECOND 80 //Mass Flow
  #define HART_UM_POUND_PER_MINUTE 81 //Mass Flow
  #define HART_UM_POUND_PER_HOUR 82 //Mass Flow
  #define HART_UM_POUND_PER_DAY 83 //Mass Flow
  #define HART_UM_SHORT_TON_PER_SECOND NOT_SUPPORTED //Mass Flow
  #define HART_UM_SHORT_TON_PER_MINUTE 84 //Mass Flow
  #define HART_UM_SHORT_TON_PER_HOUR 85 //Mass Flow
  #define HART_UM_SHORT_TON_PER_DAY 86 //Mass Flow
  #define HART_UM_LONG_TON_PER_SECOND NOT_SUPPORTED //Mass Flow
  #define HART_UM_LONG_TON_PER_MINUTE NOT_SUPPORTED //Mass Flow
  #define HART_UM_LONG_TON_PER_HOUR 87 //Mass Flow
  #define HART_UM_LONG_TON_PER_DAY 88 //Mass Flow
  #define HART_UM_KILOPOUND_US_PER_DAY NOT_SUPPORTED //Mass Flow
  #define HART_UM_KILOPOUND_US_PER_HOUR NOT_SUPPORTED //Mass Flow
  #define HART_UM_KILOPOUND_US_PER_MINUTE NOT_SUPPORTED //Mass Flow
  #define HART_UM_KILOPOUND_US_PER_SECOND NOT_SUPPORTED //Mass Flow
  #define HART_UM_OUNCE_PER_SECOND NOT_SUPPORTED //Mass Flow
  #define HART_UM_OUNCE_PER_MINUTE NOT_SUPPORTED //Mass Flow
  #define HART_UM_OUNCE_PER_HOUR NOT_SUPPORTED //Mass Flow
  #define HART_UM_OUNCE_PER_DAY NOT_SUPPORTED //Mass Flow
  #define HART_UM_CUBIC_METER 43 //Volume
  #define HART_UM_LITER 41 //Volume
  #define HART_UM_CUBIC_INCH 113 //Volume
  #define HART_UM_CUBIC_FEET 112 //Volume
  #define HART_UM_CUBIC_YARD 111 //Volume
  #define HART_UM_HECTOLITER 236 //Volume
  #define HART_UM_IMPERIAL_GALLON 42 //Volume
  #define HART_UM_US_GALLON 40 //Volume
  #define HART_UM_BUSHEL 110 //Volume
  #define HART_UM_CUBIC_CENTIMETER NOT_SUPPORTED //Volume
  #define HART_UM_CUBIC_MILLIMETER NOT_SUPPORTED //Volume
  #define HART_UM_CUBIC_MEGAMETER NOT_SUPPORTED //Volume
  #define HART_UM_CUBIC_MILE NOT_SUPPORTED //Volume
  #define HART_UM_CUBIC_MEGAFEET NOT_SUPPORTED //Volume
  #define HART_UM_CENTILITER NOT_SUPPORTED //Volume
  #define HART_UM_MILLILITER NOT_SUPPORTED //Volume
  #define HART_UM_KILOLITER NOT_SUPPORTED //Volume
  #define HART_UM_MEGALITER NOT_SUPPORTED //Volume
  #define HART_UM_PINT NOT_SUPPORTED //Volume
  #define HART_UM_QUART NOT_SUPPORTED //Volume
  #define HART_UM_BARREL 46 //Volume
  #define HART_UM_BARREL_LIQUID 124 //Volume
  #define HART_UM_BARREL_FEDERAL NOT_SUPPORTED //Volume
  #define HART_UM_STANDARD_CUBIC_FEET NOT_SUPPORTED //Volume TODO double ?
  #define HART_UM_KILOGALLON NOT_SUPPORTED //Volume
  #define HART_UM_KILO_IMPERIAL_GALLON NOT_SUPPORTED //Volume
  #define HART_UM_MEGABARREL NOT_SUPPORTED //Volume
  #define HART_UM_ACRE_INCH NOT_SUPPORTED //Volume
  #define HART_UM_ACRE_FEET NOT_SUPPORTED //Volume
  #define HART_UM_MEGAGALLON NOT_SUPPORTED //Volume
  #define HART_UM_NORMAL_CUBIC_METER_0C_1ATM 166 //Norm Volume
  #define HART_UM_NORMAL_LITER_0C_1ATM 167 //Norm Volume
  #define HART_UM_STANDARD_CUBIC_FOOT_60F_1ATM 168 //Norm Volume
  #define HART_UM_STANDARD_CUBIC_METER_20C_1ATM NOT_SUPPORTED //Norm Volume
  #define HART_UM_STANDARD_LITER_20C_1ATM NOT_SUPPORTED //Norm Volume
  #define HART_UM_GRAM 60 //Mass
  #define HART_UM_KILOGRAM 61 //Mass
  #define HART_UM_METRIC_TON 62 //Mass
  #define HART_UM_POUND 63 //Mass
  #define HART_UM_OUNCE 125 //Mass
  #define HART_UM_MILLIGRAM NOT_SUPPORTED //Mass
  #define HART_UM_MEGAGRAM NOT_SUPPORTED //Mass
  #define HART_UM_SHORT_TON 64 //Mass
  #define HART_UM_LONG_TON 65 //Mass
  #define HART_UM_MEGAPASCAL 237 //Pressure
  #define HART_UM_MILLIPASCAL NOT_SUPPORTED //Pressure
  #define HART_UM_MICROPASCAL NOT_SUPPORTED //Pressure
  #define HART_UM_PASCAL_U 11 //Pressure
  #define HART_UM_KILOPASCAL 12 //Pressure
  #define HART_UM_BAR 7 //Pressure
  #define HART_UM_MILLIBAR 8 //Pressure
  #define HART_UM_ATMOSPHERE 14 //Pressure
  #define HART_UM_POUNDFORCE_PER_SQUARE_INCH 6 //Pressure
  #define HART_UM_POUNDFORCE_PER_SQUARE_INCH_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_POUNDFORCE_PER_SQUARE_INCH_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_GRAMFORCE_PER_SQUARE_CENTIMETER 9 //Pressure
  #define HART_UM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER 10 //Pressure
  #define HART_UM_GIGAPASCAL NOT_SUPPORTED //Pressure
  #define HART_UM_HECTOPASCAL NOT_SUPPORTED //Pressure
  #define HART_UM_PASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_PASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_GIGAPASCA_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_GIGAPASCA_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MEGAPASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MEGAPASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_KILOPASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_KILOPASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIPASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIPASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MICROPASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MICROPASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_HECTOPASCAL_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_HECTOPASCAL_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_60F_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_60F_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_4C_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_4C_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_68F_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_68F_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_4C_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_4C_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_68F_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_68F_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_ABSOLUTE_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_4C_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_4C_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_68F_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_68F_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_HG_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_HG_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_HG_0C_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_HG_0C_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_HG_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_HG_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_HG_0C_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_HG_0C_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_H2O_60F 145 //Pressure
  #define HART_UM_INCH_H2O_4C 238 //Pressure
  #define HART_UM_INCH_H2O_68F 1 //Pressure
  #define HART_UM_MILLIMETER_H2O NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_H2O_4C 239 //Pressure
  #define HART_UM_MILLIMETER_H2O_68F 4 //Pressure
  #define HART_UM_FEET_H2O NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_4C NOT_SUPPORTED //Pressure
  #define HART_UM_FEET_H2O_68F 3 //Pressure
  #define HART_UM_INCH_HG NOT_SUPPORTED //Pressure
  #define HART_UM_INCH_HG_0C 2 //Pressure
  #define HART_UM_MILLIMETER_HG NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIMETER_HG_0C 5 //Pressure
  #define HART_UM_TORR 13 //Pressure
  #define HART_UM_GRAMFORCE_PER_SQUARE_CENTIMETER_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_GRAMFORCE_PER_SQUARE_CENTIMETER_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_BAR_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_BAR_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIBAR_GAUGE NOT_SUPPORTED //Pressure
  #define HART_UM_MILLIBAR_ABSOLUTE NOT_SUPPORTED //Pressure
  #define HART_UM_KELVIN 35 //Temperature
  #define HART_UM_CELSIUS 32 //Temperature
  #define HART_UM_FAHRENHEIT 33 //Temperature
  #define HART_UM_RANKINE 34 //Temperature
  #define HART_UM_KILOGRAM_PER_CUBIC_METER 92 //MassDensity
  #define HART_UM_GRAM_PER_CUBIC_CENTIMETER 91 //MassDensity
  #define HART_UM_KILOGRAM_PER_LITER 96 //MassDensity
  #define HART_UM_POUND_PER_CUBIC_INCH 98 //MassDensity
  #define HART_UM_POUND_PER_CUBIC_FOOT 94 //MassDensity
  #define HART_UM_MEGAGRAM_PER_CUBIC_METER NOT_SUPPORTED //MassDensity
  #define HART_UM_GRAM_PER_CUBIC_METER NOT_SUPPORTED //MassDensity
  #define HART_UM_METRIC_TON_PER_CUBIC_METER NOT_SUPPORTED //MassDensity
  #define HART_UM_GRAM_PER_MILLILITER 95 //MassDensity
  #define HART_UM_GRAM_PER_LITER 97 //MassDensity
  #define HART_UM_POUND_PER_US_GALLON 93 //MassDensity
  #define HART_UM_SHORT_TON_PER_CUBIC_YARD 99 //MassDensity
  #define HART_UM_POUND_PER_GALLON NOT_SUPPORTED //MassDensity
  #define HART_UM_MICROGRAM_PER_LITER 146 //MassDensity
  #define HART_UM_MICROGRAM_PER_CUBIC_METER 147 //MassDensity
  #define HART_UM_MILLIGRAM_PER_LITER NOT_SUPPORTED //MassDensity
  #define HART_UM_VOLT 58 //Voltage alias EMF
  #define HART_UM_MILLIVOLT 36 //Voltage alias EMF
  #define HART_UM_MEGAVOLT NOT_SUPPORTED //Voltage alias EMF
  #define HART_UM_KILOVOLT NOT_SUPPORTED //Voltage alias EMF
  #define HART_UM_MICROVOLT NOT_SUPPORTED //Voltage alias EMF
  #define HART_UM_OHM 37 //Resistance
  #define HART_UM_KILOOHM 163 //Resistance
  #define HART_UM_GIGAOHM NOT_SUPPORTED //Resistance
  #define HART_UM_MEGAOHM NOT_SUPPORTED //Resistance
  #define HART_UM_MILLIOHM NOT_SUPPORTED //Resistance
  #define HART_UM_MICROOHM NOT_SUPPORTED //Resistance
  #define HART_UM_SECOND 51 //Time
  #define HART_UM_MINUTE 50 //Time
  #define HART_UM_HOUR 52 //Time
  #define HART_UM_DAY 53 //Time
  #define HART_UM_KILOSECOND NOT_SUPPORTED //Time
  #define HART_UM_MILLISECOND NOT_SUPPORTED //Time
  #define HART_UM_MICROSECOND NOT_SUPPORTED //Time
  #define HART_UM_HERTZ 38 //Frequency
  #define HART_UM_TERAHERTZ NOT_SUPPORTED //Frequency
  #define HART_UM_GIGAHERTZ NOT_SUPPORTED //Frequency
  #define HART_UM_MEGAHERTZ NOT_SUPPORTED //Frequency
  #define HART_UM_KILO_HERTZ NOT_SUPPORTED //Frequency
  #define HART_UM_COUNT_PER_SECOND NOT_SUPPORTED //Frequency
  #define HART_UM_PER_SECOND NOT_SUPPORTED //Frequency
  #define HART_UM_PER_MINUTE NOT_SUPPORTED //Frequency
  #define HART_UM_REVOLUTION_PER_SECOND 118 //Angular Velocity
  #define HART_UM_REVOLUTION_PER_MINUTE 119 //Angular Velocity
  #define HART_UM_ONE_PER_32_MILLISECOND NOT_SUPPORTED //Frequency
  #define HART_UM_MILLIAMPERE 39 //Electric Current
  #define HART_UM_AMPERE NOT_SUPPORTED //Electric Current
  #define HART_UM_KILOAMPERE NOT_SUPPORTED //Electric Current
  #define HART_UM_MICROAMPERE NOT_SUPPORTED //Electric Current
  #define HART_UM_NANOAMPERE NOT_SUPPORTED //Electric Current
  #define HART_UM_PICOAMPERE NOT_SUPPORTED //Electric Current
  #define HART_UM_PERCENT 57 //Ratio
  #define HART_UM_PARTS_PER_MILLION 139 //Ratio
  #define HART_UM_PARTS_PER_BILLION 169 //Ratio
  #define HART_UM_PARTS_PER_THOUSAND NOT_SUPPORTED //Ratio
  #define HART_UM_PARTS_PER_TRILLION NOT_SUPPORTED //Ratio
  #define HART_UM_PICOFARAD 153 //Capacitance
  #define HART_UM_FARAD NOT_SUPPORTED //Capacitance
  #define HART_UM_MILLIFARAD NOT_SUPPORTED //Capacitance
  #define HART_UM_MICROFARAD NOT_SUPPORTED //Capacitance
  #define HART_UM_NANOFARAD NOT_SUPPORTED //Capacitance
  #define HART_UM_MILLIMETER 49 //Length
  #define HART_UM_CENTIMETER 48 //Length
  #define HART_UM_METER 45 //Length
  #define HART_UM_FOOT 44 //Length
  #define HART_UM_INCH 47 //Length
  #define HART_UM_KILOMETER NOT_SUPPORTED //Length
  #define HART_UM_MICROMETER NOT_SUPPORTED //Length
  #define HART_UM_NANOMETER NOT_SUPPORTED //Length
  #define HART_UM_PICOMETER NOT_SUPPORTED //Length
  #define HART_UM_ANGSTROM NOT_SUPPORTED //Length
  #define HART_UM_YARD NOT_SUPPORTED //Length
  #define HART_UM_MILE NOT_SUPPORTED //Length
  #define HART_UM_NAUTICAL_MILE NOT_SUPPORTED //Length
  #define HART_UM_MICRON NOT_SUPPORTED //Length
  #define HART_UM_MIL NOT_SUPPORTED //Length
  #define HART_UM_CENTISTOKE 54 //Viscosity
  #define HART_UM_CENTIPOISE 55 //Dynamic Viscosity
  #define HART_UM_MICROSIEMENS 56 //Conductance
  #define HART_UM_PH 59 //Analytical
  #define HART_UM_MILLISIEMENS_PER_CENTIMETER 66 //Conductivity
  #define HART_UM_MICROSIEMENS_PER_CENTIMETER 67 //Conductivity
  #define HART_UM_NEWTON 68 //Force
  #define HART_UM_NEWTON_METER 69 //Moment of Force
  #define HART_UM_DEKA_THERM 89 //Energy
  #define HART_UM_SPECIFIC_GRAVITY_UNIT 90 //Mass per Volume
  #define HART_UM_DEGREE_TWADDELL 100 //Mass per Volume
  #define HART_UM_DEGREE_BRIX 101 //TODO
  #define HART_UM_DEGREE_BAUME_HEAVY 102 //Mass per Volume
  #define HART_UM_DEGREE_BAUME_LIGHT 103 //Mass per Volume
  #define HART_UM_DEGREE_API 104 //Mass per Volume
  #define HART_UM_PERCENT_SOLID_PER_WEIGHT 105 //TODO
  #define HART_UM_PERCENT_SOLID_PER_VOLUME 106 //TODO
  #define HART_UM_DEGREE_BALLING 107 //TODO
  #define HART_UM_PROOF_PER_VOLUME 108 //TODO
  #define HART_UM_PROOF_PER_MASS 109 //TODO
  #define HART_UM_DEGREE_PER_SECOND 117 //Angluar Velocity
  #define HART_UM_FOOTPOUND_FORCE 126 //Energy
  #define HART_UM_KILOWATT 127 //Power
  #define HART_UM_KILOWATT_HOUR 128 //Energy
  #define HART_UM_HORSEPOWER 129 //Power
  #define HART_UM_MEGACALORIE_PER_HOUR 140 //Power
  #define HART_UM_MEGAJOULE_PER_HOUR 141 //Power
  #define HART_UM_BRITISH_THERMAL_UNIT_PER_HOUR 142 //Power
  #define HART_UM_DEGREE 143 //Angle (plane)
  #define HART_UM_RADIAN 144 //Angle (plane)
  #define HART_UM_PERCENT_CONSISTENSY 148 //Ratio ?
  #define HART_UM_PERCENT_VOLUME 149 //Ratio ?
  #define HART_UM_PERCENT_STEAM_QUALITY 150 //Ratio ?
  #define HART_UM_FEET_IN_SIXTEENTH 151 //Length
  #define HART_UM_CUBIC_FEET_PER_POUND 152 //Volume Density
  #define HART_UM_MILLILITER_PER_LITER 154 //Ratio ? Volume per volume ?
  #define HART_UM_MICROLITER_PER_LITER 155 //Ratio ? Volume per volume ?
  #define HART_UM_PERCENT_PLATO 160 //TODO ?
  #define HART_UM_PERCENT_EXPLOSION_LEVEL 161 //TODO ?
  #define HART_UM_MEGACALORIE 162 //Energy
  #define HART_UM_MEGAJOULE 164 //Energy
  #define HART_UM_BRITISH_THERMAL_UNIT 165 //Energy
  #define HART_UM_THOUSAND_BRITISH_THERMAL_UNIT 172 //Energy
  #define HART_UM_HENRY NOT_SUPPORTED //Inductance
  #define HART_UM_MILLIHENRY NOT_SUPPORTED //Inductance
  #define HART_UM_MICROHENRY NOT_SUPPORTED //Inductance
  #define HART_UM_NANOHENRY NOT_SUPPORTED //Inductance
  #define HART_UM_PICOHENRY NOT_SUPPORTED //Inductance
  #define HART_UM_SIEMENS_PER_METER NOT_SUPPORTED //Conductivity
  #define HART_UM_MEGASIEMENS_PER_METER NOT_SUPPORTED //Conductivity
  #define HART_UM_KILOSIEMENS_PER_METER NOT_SUPPORTED //Conductivity
  #define HART_UM_MILLISIEMENS_PER_METER NOT_SUPPORTED //Conductivity
  #define HART_UM_MICROSIEMENS_PER_METER NOT_SUPPORTED //Conductivity
  #define HART_UM_MICROSIEMENS_PER_MILLIMETER NOT_SUPPORTED //Conductivity
  #define HART_UM_MEGAWATT 170 //Power
  #define HART_UM_MILLION_BRITISH_THERMAL_UNIT_PER_SECOND 172 //Power
  #define HART_UM_MILLION_BRITISH_THERMAL_UNIT_PER_HOUR 173 //Power
  #define HART_UM_MILLION_BRITISH_THERMAL_UNIT_PER_DAY 174 //Power
  #define HART_UM_BRITISH_THERMAL_UNIT_PER_DAY 178 //Power
  #define HART_UM_BRITISH_THERMAL_UNIT_PER_MINUTE 179 //Power
  #define HART_UM_BRITISH_THERMAL_UNIT_PER_SECOND 181 //Power
  #define HART_UM_THOUSAND_BRITISH_THERMAL_UNIT_PER_SECOND 182 //Power
  #define HART_UM_THOUSAND_BRITISH_THERMAL_UNIT_PER_MINUTE 183 //Power
  #define HART_UM_THOUSAND_BRITISH_THERMAL_UNIT_PER_HOUR 184 //Power
  #define HART_UM_THOUSAND_BRITISH_THERMAL_UNIT_PER_DAY 185 //Power
  #define HART_UNIT_FREQUENCY_HERTZ 38 //Frequency
  #define HART_UNIT_TOTALIZER_KILOGRAMS 61 //Mass
  #define HART_UNIT_TIME_MILLISECONDS NOT_SUPPORTED //Time

  // T_HART_SELECTION
  #define HART_SEC_EXT_TEMPERATURE 0 //Select the burst in Tx as Select the burst in Tx as Temperature
  #define HART_SEC_PRESSURE 1 //Select the burst in Tx as Pressure
  #define HART_SEC_PRESSURE_ABS 2 //Select the burst in Tx as ABS Pressure
  #define HART_SEC_GAS_CONTENT 3 //Select the burst in Tx Gas Content?
  #define HART_SEC_DENSITY 4 //Select the burst in Tx Density
  #define HART_SEC_WITHOUT 5 //
  #define HART_SEC_INTER_TEMPERATURE 6 //

  // T_CURRENT_UNITS
  #define HART_CURRENT_MILLIAMPERES 3 //CurrentUnit
  #define HART_CURRENT_UNIT_TYPE 7 //CurrentType

  // T_HART_FAMILY_CODES
  #define HART_TEMPERATURE 4 //temperature
  #define HART_PRESSRUE 5 //pressure
  #define HART_VALVE 6 //value/acturator
  #define HART_SIMPLE_PID_CONTROL 7 //simple_PID_control
  #define HART_PH 8 //pH
  #define HART_CONDUCTIVITY 9 //conductivity
  #define HART_TOTALIZER 10 //totalizer
  #define HART_LEVEL 11 //level
  #define HART_VORTEX_FLOW 12 //Vortex_flow
  #define HART_MAG_FLOW 13 //mag_flow
  #define HART_CORIOLIS_FLOW 14 //coriolis_flow

  // T_HART_CURRENT_CLASSIFICATION
  #define HART_CURRENT_CLASSIFICATION 84 //CURRENT_CLASSIFICATION

  // T_BURST_IN_UNIT_TYPE
  #define HART_BURST_IN_TEMPERATURE 9 //burst in temperature
  #define HART_BURST_IN_PRESSURE 51 //burst in pressure
  #define HART_BURST_IN_DENSITY 25 //burst in density
  #define HART_BURST_IN_CONTENT 139 //burst in content
  #define HART_BURST_IN_NONE 0 //

  // T_SERVICE_LEVEL
  #define HART_SERVICE_service 0 //
  #define HART_SERVICE_advanced 1 //
  #define HART_SERVICE_standard 2 //

  // BoardsTable
  #define HART_CB_BOARD 0 //

  // BURST_IN_STATUS
  #define HART_BURST_IN_OK 0 //
  #define HART_BURST_IN_FAILED 1 //

  // HART_RECEIVE_BUFFER_PROCESS_CMD
  #define HART_SP_SAVE_RX_DATA 0 //use for subsystem serviceport put HART receive data
//@SubGen end@ <TYPES>


//----------------------------------------------------------------------------------------------------------
//!  define the datatypes for EXE and SRV-Methods input and output


//----------------------------------------------------------------------------------------------------------
//!  dataclass-indexes
enum _E_HART_DC_INDEXES
{
    //@SubGen start@    <DATACLASSES>
    HART_DC_IDX_dyn,
    HART_DC_IDX_rare,
    HART_DC_IDX_freqCst,
    HART_DC_IDX_freqNo,
    HART_DC_IDX_cst,
    HART_DC_IDX_freqStatic,
    HART_DC_IDX_CONSTANT,
    HART_DC_IDX_DYNAMIC,
    HART_DC_IDX_STATIC_RARE,
    HART_DC_IDX_NUMBER_OF_DATACLASSES
    //@SubGen end@    <DATACLASSES>
};

//----------------------------------------------------------------------------------------------------------
//!  definition of class T_HART
typedef struct _T_HART
{
    // inherit T_UNIT
    T_UNIT unit;

    //@SubGen start@    <METHODS>
    void (* HARTCommandInterpreterEXE)(void);
    void (* IncrementConfiChangeCounter)(void);
    TUSIGN8 (* GetDeviceLockStatus)(void);
    TBOOL (* HART_PutReceiveBuffer_SRV)(TUSIGN16 systemIdx, TUSIGN8 processCmd, TUSIGN8* ptrArray, TUSIGN8 arrayLength);
    //@SubGen end@    <METHODS>
} T_HART;


//! define the T_HART-object
extern SLOW const T_HART hart;

//! the this-pointer for the T_UNIT-methods in T_HART
#define HART_ME (&hart.unit)

//-------------------------------------------------------------------------------------------------
//! define a macro to ease access to often used T_UNIT method
//! underscore mimics the -> operator in C++ classes
#define hart_Get(objectIndex,attributeIndex,ptrValue) \
   hart.unit.Get(HART_ME,objectIndex,attributeIndex,ptrValue)
#define hart_Put(objectIndex,attributeIndex,ptrValue) \
   hart.unit.Put(HART_ME,objectIndex,attributeIndex,ptrValue)
#define hart_Check(objectIndex,attributeIndex,ptrValue) \
   hart.unit.Check(HART_ME,objectIndex,attributeIndex,ptrValue)

#define hart_GetAttributeDescription(objectIndex,attributeIndex,ptrDescriptor) \
   hart.unit.GetAttributeDescription(HART_ME,objectIndex,attributeIndex,ptrDescriptor)
#define hart_GetObjectDescription(objectIndex,ptrDescriptor) \
   hart.unit.GetObjectDescription(HART_ME,objectIndex,ptrDescriptor)

