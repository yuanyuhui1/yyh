//-------------------------------------------------------------------------------------------------
/*
 <pre>
 Copyright              Copyright ABB, 2011.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.

 System         Subsystem Modbus
 Module
 Description    Implementation units index conversion

 Remarks
 </pre>
*/
//-------------------------------------------------------------------------------------------------


#include <intrinsics.h>
#include <limits.h>
#include "System/Interface/system.h"
#include "System/Interface/common_type.h"
#include "System/Interface/assert.h"
#include "RTOS/Interface/rtos.h"
#include "T_DATA_OBJ/Interface/simple_type.h"
#include "T_DATA_OBJ/Interface/t_data_obj.h"
#include "T_Unit/Interface/t_unit.h"
#include "UnitsOfMeasure/Interface/UnitsOfMeasure.h"

#include "Modbus_unit_indexconv.h"
//--------------------------------------------------------------------------------------------------
// Data In2Out

typedef struct tableElement
{
  TUSIGN8 one;
  TUSIGN8 two;
} T_MODBUS_TABLE_ELEMENT;

typedef const T_MODBUS_TABLE_ELEMENT T_MODBUS_CONV_TABLE_SUB_INDEX[];

  


typedef struct in2outConvGrpTblEntry {
  TUSIGN8 inTblNr;
  T_MODBUS_TABLE_ELEMENT *in2outConvTbl;
  TUSIGN16 in2outConvTblLength;
} T_MODBUS_IN2OUT_CONV_GRP_TBL_ENTRY;


//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts units codes from outside to internal used codes
 \author Martin Kuegler-Walter
 \date   2013-11-19
 \param  TUSIGN8 index (out)
 \param  TUSIGN8 *indexA  (first index in)
 \param  TUSIGN8 *indexB  (second index in)
 \return TBOOL (eTRUE: Success, eFALSE: Fail)
\test
test-date: 2016-08-25
\n by: ZuoChen Wang
\n environment:IAR Embedded Workbench Renesas RX V2.41.3
\n intention:
\n result module test:
\n result Lint Level 3:
\bug       
 \warning
*/
//-------------------------------------------------------------------------------------------------
void Out2inConv(TUSIGN8 index, TUSIGN8 *indexA, TUSIGN8 *indexB)
{

  //--------------------------------------------------------------------------------------------------
  // Data Out2In

  static const TUSIGN8 out2inUnitCodeConversionTable[256][2] = {
    {  0,   0},  //   0: 
    {UM_T_PRESSURE, UM_INCH_H2O_68F},                     // 001: Inches water at 68 °Fahrenheit 
    {UM_T_PRESSURE, UM_INCH_HG_0C},                       // 002: Inches mercury at 0 °Celsius 
    {UM_T_PRESSURE, UM_FEET_H2O_68F},                     // 003: Feet water at 68 °Fahrenheit 
    {UM_T_PRESSURE, UM_MILLIMETER_H2O_68F},               // 004: Millimeters water at 68 °Fahrenheit 
    {UM_T_PRESSURE, UM_MILLIMETER_HG_0C},                 // 005: Millimeters mercury at 0 °Celsius 
    {UM_T_PRESSURE, UM_POUNDFORCE_PER_SQUARE_INCH},       // 006: Pounds/square inch 
    {UM_T_PRESSURE, UM_BAR},                              // 007: Bar 
    {UM_T_PRESSURE, UM_MILLIBAR},                         // 008: Millibar 
    {UM_T_PRESSURE, UM_GRAMFORCE_PER_SQUARE_CENTIMETER},  // 009: Grams/square centimeter 

    {UM_T_PRESSURE, UM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER}, // 010: Kilograms/square centimeter 
    {UM_T_PRESSURE, UM_PASCAL_U},                            // 011: Pascals 
    {UM_T_PRESSURE, UM_KILOPASCAL},                          // 012: Kilopascals 
    {UM_T_PRESSURE, UM_TORR},                                // 013: Torr at 0 °Celsius 
    {UM_T_PRESSURE, UM_ATMOSPHERE},                          // 014: Atmospheres 

    
    {UM_T_VOLUME_FLOW, UM_CUBIC_FEET_PER_MINUTE},      // 015: Cubic feet/minute
    {UM_T_VOLUME_FLOW, UM_US_GALLON_PER_MINUTE},       // 016: Gallons/minute
    {UM_T_VOLUME_FLOW, UM_LITER_PER_MINUTE},           // 017: Liters/minute
    {UM_T_VOLUME_FLOW, UM_IMPERIAL_GALLON_PER_MINUTE}, // 018: Imperial gallons/minute
    {UM_T_VOLUME_FLOW, UM_CUBIC_METER_PER_HOUR},       // 019: Cubic meters/hour

    {  0,   0},  // 020: 
    {  0,   0},  // 021: 
    {UM_T_VOLUME_FLOW, UM_US_GALLON_PER_SECOND},       // 022: Gallons/second
    {UM_T_VOLUME_FLOW, UM_MEGA_US_GALLON_PER_DAY},     // 023: Million U.S: gallons/day
    {UM_T_VOLUME_FLOW, UM_LITER_PER_SECOND},           // 024: Liters/second
    {UM_T_VOLUME_FLOW, UM_MEGALITER_PER_DAY},          // 025: Million liters/day
    {UM_T_VOLUME_FLOW, UM_CUBIC_FEET_PER_SECOND},      // 026: Cubic feet/second
    {UM_T_VOLUME_FLOW, UM_CUBIC_FEET_PER_DAY},         // 027: Cubic feet/day
    {UM_T_VOLUME_FLOW, UM_CUBIC_METER_PER_SECOND},     // 028: Cubic meters/second
    {UM_T_VOLUME_FLOW, UM_CUBIC_METER_PER_DAY},        // 029: Cubic meters/day

    {UM_T_VOLUME_FLOW, UM_IMPERIAL_GALLON_PER_HOUR},   // 030: Imperial gallons/hour
    {UM_T_VOLUME_FLOW, UM_IMPERIAL_GALLON_PER_DAY},    // 031: Imperial gallons/day
    {UM_T_TEMPERATURE, UM_CELSIUS},     // 032: Degrees Celsius
    {UM_T_TEMPERATURE, UM_FAHRENHEIT},  // 033: Degrees Fahrenheit
    {UM_T_TEMPERATURE, UM_RANKINE},     // 034: Degrees Rankine
    {UM_T_TEMPERATURE, UM_KELVIN},      // 035: Kelvin
    {  0,   0},  // 036: 
    {  0,   0},  // 037: 
    {  0,   0},  // 038: 
    {  0,   0},  // 039: 

    {UM_T_VOLUME, UM_US_GALLON},       // 040: U.S. gallons
    {UM_T_VOLUME, UM_LITER},           // 041: Liters
    {UM_T_VOLUME, UM_IMPERIAL_GALLON}, // 042: Imperial gallons
    {UM_T_VOLUME, UM_CUBIC_METER},     // 043: Cubic meter
    {  0,   0},  // 044: 
    {  0,   0},  // 045: 
    {UM_T_VOLUME, UM_BARREL_LIQUID},   // 046: Barrels(42 U.S. gallons)
    {  0,   0},  // 047: 
    {  0,   0},  // 048: 
    {  0,   0},  // 049: 

    {  0,   0},  // 050: 
    {  0,   0},  // 051: 
    {  0,   0},  // 052: 
    {  0,   0},  // 053: 
    {  0,   0},  // 054: 
    {  0,   0},  // 055: 
    {  0,   0},  // 056: 
    {  0,   0},  // 057: 
    {  0,   0},  // 058: 
    {  0,   0},  // 059: 

    {UM_T_MASS, UM_GRAM},        // 060: Grams
    {UM_T_MASS, UM_KILOGRAM},    // 061: Kilograms
    {UM_T_MASS, UM_METRIC_TON},  // 062: Metric tons
    {UM_T_MASS, UM_POUND},       // 063: Pounds
    {UM_T_MASS, UM_SHORT_TON},   // 064: Short tons (2  0 pounds)
    {UM_T_MASS, UM_LONG_TON},    // 065: Long tons (2240 pounds)
    {  0,   0},  // 066: 
    {  0,   0},  // 067: 
    {  0,   0},  // 068: 
    {  0,   0},  // 069: 

    {UM_T_MASS_FLOW, UM_GRAM_PER_SECOND},       // 070: Grams/second
    {UM_T_MASS_FLOW, UM_GRAM_PER_MINUTE},       // 071: Grams/minute
    {UM_T_MASS_FLOW, UM_GRAM_PER_HOUR},         // 072: Grams/hour
    {UM_T_MASS_FLOW, UM_KILOGRAM_PER_SECOND},   // 073: Kilograms/second
    {UM_T_MASS_FLOW, UM_KILOGRAM_PER_MINUTE},   // 074: Kilograms/minute
    {UM_T_MASS_FLOW, UM_KILOGRAM_PER_HOUR},     // 075: Kilograms/hour
    {UM_T_MASS_FLOW, UM_KILOGRAM_PER_DAY},      // 076: Kilograms/day
    {UM_T_MASS_FLOW, UM_METRIC_TON_PER_MINUTE}, // 077: Metric tons/minute
    {UM_T_MASS_FLOW, UM_METRIC_TON_PER_HOUR},   // 078: Metric tons/hour
    {UM_T_MASS_FLOW, UM_METRIC_TON_PER_DAY},    // 079: Metric tins/day

    {UM_T_MASS_FLOW, UM_POUND_PER_SECOND},      // 080: Pounds/second
    {UM_T_MASS_FLOW, UM_POUND_PER_MINUTE},      // 081: Pounds/minute
    {UM_T_MASS_FLOW, UM_POUND_PER_HOUR},        // 082: Pounds/hour
    {UM_T_MASS_FLOW, UM_POUND_PER_DAY},         // 083: Pounds/day
    {UM_T_MASS_FLOW, UM_SHORT_TON_PER_MINUTE},  // 084: Short tons (2  0 pounds)/minute
    {UM_T_MASS_FLOW, UM_SHORT_TON_PER_HOUR},    // 085: Short tons (2  0 pounds)/hour
    {UM_T_MASS_FLOW, UM_SHORT_TON_PER_DAY},     // 086: Short tons (2  0 pounds)/day
    {UM_T_MASS_FLOW, UM_LONG_TON_PER_HOUR},     // 087: Long tons (2240 pounds)/hour
    {UM_T_MASS_FLOW, UM_LONG_TON_PER_DAY},      // 088: Long tons (2240 pounds)/day
    {  0,   0},  // 089: 

    {  0,   0},  // 090: Specific gravity units
    {UM_T_MASS_DENSITY, UM_GRAM_PER_CUBIC_CENTIMETER}, // 091: Grams/cubic centimeter
    {UM_T_MASS_DENSITY, UM_KILOGRAM_PER_CUBIC_METER},  // 092: Kilograms/cubic meter
    {UM_T_MASS_DENSITY, UM_POUND_PER_US_GALLON},       // 093: Pounds/galon
    {UM_T_MASS_DENSITY, UM_POUND_PER_CUBIC_FOOT},      // 094: Pounds/cubic foot
    {UM_T_MASS_DENSITY, UM_GRAM_PER_MILLILITER},       // 095: Grams/milliliter
    {UM_T_MASS_DENSITY, UM_KILOGRAM_PER_LITER},        // 096: Kilograms/liter
    {UM_T_MASS_DENSITY, UM_GRAM_PER_LITER},            // 097: Grams/liter
    {UM_T_MASS_DENSITY, UM_POUND_PER_CUBIC_INCH},      // 098: Pounds/cubic inch
    {UM_T_MASS_DENSITY, UM_SHORT_TON_PER_CUBIC_YARD},  // 099: Short tons (2  0 pounds)/cubic yard

    {  0,   0},  // 100: 
    {  0,   0},  // 101: 
    {  0,   0},  // 102: 
    {  0,   0},  // 103: 
    {  0,   0},  // 104: Degrees API
    {  0,   0},  // 105: 
    {  0,   0},  // 106: 
    {  0,   0},  // 107: 
    {  0,   0},  // 108: 
    {  0,   0},  // 109: 

    {  0,   0},  // 110: 
    {  0,   0},  // 111: 
    {UM_T_VOLUME, UM_CUBIC_FEET},  // 112: Cubic feet
    {  0,   0},  // 113: 
    {  0,   0},  // 114: 
    {  0,   0},  // 115: 
    {  0,   0},  // 116: 
    {  0,   0},  // 117: 
    {  0,   0},  // 118: 
    {  0,   0},  // 119: 

    {  0,   0},  // 120: 
    {  0,   0},  // 121: 
    {  0,   0},  // 122: 
    {  0,   0},  // 123: 
    {  0,   0},  // 124: 
    {  0,   0},  // 125: 
    {  0,   0},  // 126: 
    {  0,   0},  // 127: 
    {  0,   0},  // 128: 
    {  0,   0},  // 129: 

    {UM_T_VOLUME_FLOW, UM_CUBIC_FEET_PER_HOUR},        // 130: Cubic feet/hour
    {UM_T_VOLUME_FLOW, UM_CUBIC_METER_PER_MINUTE},     // 131: Cubic meters/minute
    {UM_T_VOLUME_FLOW, UM_BARREL_PER_SECOND},          // 132: Barrels/second
    {UM_T_VOLUME_FLOW, UM_BARREL_PER_MINUTE},          // 133: Barrels/minute
    {UM_T_VOLUME_FLOW, UM_BARREL_PER_HOUR},            // 134: Barrels/hour
    {UM_T_VOLUME_FLOW, UM_BARREL_PER_DAY},             // 135: Barrels/day
    {UM_T_VOLUME_FLOW, UM_US_GALLON_PER_HOUR},         // 136: U.S. gallons/hour
    {UM_T_VOLUME_FLOW, UM_IMPERIAL_GALLON_PER_SECOND}, // 137: Imerial gallons/second
    {UM_T_VOLUME_FLOW, UM_LITER_PER_HOUR},             // 138: Liters/hour
    {  0,   0},  // 139: 
    
    {  0,   0},  // 140: 
    {  0,   0},  // 141: 
    {  0,   0},  // 142: 
    {  0,   0},  // 143: 
    {  0,   0},  // 144: 
    {  0,   0},  // 145: 
    {  0,   0},  // 146: 
    {  0,   0},  // 147: 
    {  0,   0},  // 148: 
    {  0,   0},  // 149: 

    {  0,   0},  // 150: 
    {  0,   0},  // 151: 
    {  0,   0},  // 152: 
    {  0,   0},  // 153: 
    {  0,   0},  // 154: 
    {  0,   0},  // 155: 
    {  0,   0},  // 156: 
    {  0,   0},  // 157: 
    {  0,   0},  // 158: 
    {  0,   0},  // 159: 

    {  0,   0},  // 160: 
    {  0,   0},  // 161: 
    {  0,   0},  // 162: 
    {  0,   0},  // 163: 
    {  0,   0},  // 164: 
    {  0,   0},  // 165: 
    {  0,   0},  // 166: 
    {  0,   0},  // 167: 
    {  0,   0},  // 168: 
    {  0,   0},  // 169: 

    {  0,   0},  // 170: 
    {  0,   0},  // 171: 
    {  0,   0},  // 172: 
    {  0,   0},  // 173: 
    {  0,   0},  // 174: 
    {  0,   0},  // 175: 
    {  0,   0},  // 176: 
    {  0,   0},  // 177: 
    {  0,   0},  // 178: 
    {  0,   0},  // 179: 

    {  0,   0},  // 180: 
    {  0,   0},  // 181: 
    {  0,   0},  // 182: 
    {  0,   0},  // 183: 
    {  0,   0},  // 184: 
    {  0,   0},  // 185: 
    {  0,   0},  // 186: 
    {  0,   0},  // 187: 
    {  0,   0},  // 188: 
    {  0,   0},  // 189: 

    {  0,   0},  // 190: 
    {  0,   0},  // 191: 
    {  0,   0},  // 192: 
    {  0,   0},  // 193: 
    {  0,   0},  // 194: 
    {  0,   0},  // 195: 
    {  0,   0},  // 196: 
    {  0,   0},  // 197: 
    {  0,   0},  // 198: 
    {  0,   0},  // 199: 

    {  0,   0},  // 200: 
    {  0,   0},  // 201: 
    {  0,   0},  // 202: 
    {  0,   0},  // 203: 
    {  0,   0},  // 204: 
    {  0,   0},  // 205: 
    {  0,   0},  // 206: 
    {  0,   0},  // 207: 
    {  0,   0},  // 208: 
    {  0,   0},  // 209: 

    {  0,   0},  // 210: 
    {  0,   0},  // 211: 
    {  0,   0},  // 212: 
    {  0,   0},  // 213: 
    {  0,   0},  // 214: 
    {  0,   0},  // 215: 
    {  0,   0},  // 216: 
    {  0,   0},  // 217: 
    {  0,   0},  // 218: 
    {  0,   0},  // 219: 

    {  0,   0},  // 220: 
    {  0,   0},  // 221: 
    {  0,   0},  // 222: 
    {  0,   0},  // 223: 
    {  0,   0},  // 224: 
    {  0,   0},  // 225: 
    {  0,   0},  // 226: 
    {  0,   0},  // 227: 
    {  0,   0},  // 228: 
    {  0,   0},  // 229: 

    {  0,   0},  // 230: 
    {  0,   0},  // 231: 
    {  0,   0},  // 232: 
    {  0,   0},  // 233: 
    {  0,   0},  // 234: 
    {UM_T_VOLUME_FLOW, UM_US_GALLON_PER_DAY},  // 235: U.S. gallons/day
    {  0,   0},  // 236: 
    {  0,   0},  // 237: 
    {  0,   0},  // 238: 
    {  0,   0},  // 239: 

    {  0,   0},  // 240: 
    {  0,   0},  // 241: 
    {  0,   0},  // 242: 
    {  0,   0},  // 243: 
    {  0,   0},  // 244: 
    {  0,   0},  // 245: 
    {  0,   0},  // 246: 
    {  0,   0},  // 247: 
    {  0,   0},  // 248: 
    {  0,   0},  // 249: 

    {  0,   0},  // 250: 
    {  0,   0},  // 251: 
    {  0,   0},  // 252: 
    {UM_T_UNKNOWN_UNIT_GROUP,   UM_CUSTOMER_UNIT},  // 253: Special
    {  0,   0},  // 254: 
    {  0,   0}   // 255: 
  };


  *indexA = out2inUnitCodeConversionTable[index][0];
    //lint -e{961}
  *indexB = out2inUnitCodeConversionTable[index][1];
}


//--------------------------------------------------------------------------------------------------
/*!
 \brief  Converts internal used units codes to units codes used outside
 \author Martin Kuegler-Walter
 \date   2013-11-19
 \param  TUSIGN8 *indexA  (first index in)
 \param  TUSIGN8 *indexB  (second index in)
 \param  TUSIGN8 index (out)
 \return TBOOL (eTRUE: Success, eFALSE: Fail)
\test
test-date: 2016-08-25
\n by: ZuoChen Wang
\n environment:IAR Embedded Workbench Renesas RX V2.41.3
\n intention:
\n result module test:
\n result Lint Level 3:
\bug   
 \warning
*/
//-------------------------------------------------------------------------------------------------
TBOOL In2outConv(TUSIGN8 indexA, TUSIGN8 indexB, TUSIGN8 *index)
{
    TBOOL retval = eFALSE;
    TUSIGN8 i,j;

    // preset index to 0
    *index = 0;
    //_T_GLOBAL:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvGlobal = {
      { 253, UM_CUSTOMER_UNIT}, // 253: Customer Unit
    };

    //_T_VOLUME_FLOW:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvVolumeFlow = {
      { 15, UM_CUBIC_FEET_PER_MINUTE},      // 015: Cubic feet/minute
      { 16, UM_US_GALLON_PER_MINUTE},       // 016: Gallons/minute
      { 17, UM_LITER_PER_MINUTE},           // 017: Liters/minute
      { 18, UM_IMPERIAL_GALLON_PER_MINUTE}, // 018: Imperial gallons/minute
      { 19, UM_CUBIC_METER_PER_HOUR},       // 019: Cubic meters/hour
      { 22, UM_US_GALLON_PER_SECOND},       // 022: Gallons/second
      { 23, UM_MEGA_US_GALLON_PER_DAY},     // 023: Million U.S: gallons/day
      { 24, UM_LITER_PER_SECOND},           // 024: Liters/second
      { 25, UM_MEGALITER_PER_DAY},          // 025: Million liters/day
      { 26, UM_CUBIC_FEET_PER_SECOND},      // 026: Cubic feet/second
      { 27, UM_CUBIC_FEET_PER_DAY},         // 027: Cubic feet/day
      { 28, UM_CUBIC_METER_PER_SECOND},     // 028: Cubic meters/second
      { 29, UM_CUBIC_METER_PER_DAY},        // 029: Cubic meters/day
      { 30, UM_IMPERIAL_GALLON_PER_HOUR},   // 030: Imperial gallons/hour
      { 31, UM_IMPERIAL_GALLON_PER_DAY},    // 031: Imperial gallons/day
      
      {130, UM_CUBIC_FEET_PER_HOUR},        // 130: Cubic feet/hour
      {131, UM_CUBIC_METER_PER_MINUTE},     // 131: Cubic meters/minute
      {132, UM_BARREL_PER_SECOND},          // 132: Barrels/second
      {133, UM_BARREL_PER_MINUTE},          // 133: Barrels/minute
      {134, UM_BARREL_PER_HOUR},            // 134: Barrels/hour
      {135, UM_BARREL_PER_DAY},             // 135: Barrels/day
      {136, UM_US_GALLON_PER_HOUR},         // 136: U.S. gallons/hour
      {137, UM_IMPERIAL_GALLON_PER_SECOND}, // 137: Imerial gallons/second
      {138, UM_LITER_PER_HOUR},             // 138: Liters/hour
      {235, UM_US_GALLON_PER_DAY}           // 235: U.S. gallons/day
    }; 

      
    //_T_TEMPERATURE:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvTemperature = {
      { 32, UM_CELSIUS},     // 032: Degrees Celsius
      { 33, UM_FAHRENHEIT},  // 033: Degrees Fahrenheit
      { 34, UM_RANKINE},     // 034: Degrees Rankine
      { 35, UM_KELVIN}       // 035: Kelvin
    };
      
    //_T_VOLUME:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvVolume = {
      { 40, UM_US_GALLON},       // 040: U.S. gallons
      { 41, UM_LITER},           // 041: Liters
      { 42, UM_IMPERIAL_GALLON}, // 042: Imperial gallons
      { 43, UM_CUBIC_METER},     // 043: Cubic meter
      { 46, UM_BARREL_LIQUID},   // 046: Barrels(42 U.S. gallons)
      {112, UM_CUBIC_FEET}       // 112: Cubic feet
    };
      
    //_T_MASS:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvMass = {
      { 60, UM_GRAM},        // 060: Grams
      { 61, UM_KILOGRAM},    // 061: Kilograms
      { 62, UM_METRIC_TON},  // 062: Metric tons
      { 63, UM_POUND},       // 063: Pounds
      { 64, UM_SHORT_TON},   // 064: Short tons (2000 pounds)
      { 65, UM_LONG_TON}     // 065: Long tons (2240 pounds)
    };

      
    //_T_MASS_FLOW:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvFlow = {
      { 70, UM_GRAM_PER_SECOND},       // 070: Grams/second
      { 71, UM_GRAM_PER_MINUTE},       // 071: Grams/minute
      { 72, UM_GRAM_PER_HOUR},         // 072: Grams/hour
      { 73, UM_KILOGRAM_PER_SECOND},   // 073: Kilograms/second
      { 74, UM_KILOGRAM_PER_MINUTE},   // 074: Kilograms/minute
      { 75, UM_KILOGRAM_PER_HOUR},     // 075: Kilograms/hour
      { 76, UM_KILOGRAM_PER_DAY},      // 076: Kilograms/day
      { 77, UM_METRIC_TON_PER_MINUTE}, // 077: Metric tons/minute
      { 78, UM_METRIC_TON_PER_HOUR},   // 078: Metric tons/hour
      { 79, UM_METRIC_TON_PER_DAY},    // 079: Metric tins/day
      { 80, UM_POUND_PER_SECOND},      // 080: Pounds/second
      { 81, UM_POUND_PER_MINUTE},      // 081: Pounds/minute
      { 82, UM_POUND_PER_HOUR},        // 082: Pounds/hour
      { 83, UM_POUND_PER_DAY},         // 083: Pounds/day
      { 84, UM_SHORT_TON_PER_MINUTE},  // 084: Short tons (2000 pounds)/minute
      { 85, UM_SHORT_TON_PER_HOUR},    // 085: Short tons (2000 pounds)/hour
      { 86, UM_SHORT_TON_PER_DAY},     // 086: Short tons (2000 pounds)/day
      { 87, UM_LONG_TON_PER_HOUR},     // 087: Long tons (2240 pounds)/hour
      { 88, UM_LONG_TON_PER_DAY}       // 088: Long tons (2240 pounds)/day
    };

      
    //_T_MASS_DENSITY:
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvDensity = {
      { 91, UM_GRAM_PER_CUBIC_CENTIMETER}, // 091: Grams/cubic centimeter
      { 92, UM_KILOGRAM_PER_CUBIC_METER},  // 092: Kilograms/cubic meter
      { 93, UM_POUND_PER_US_GALLON},       // 093: Pounds/galon
      { 94, UM_POUND_PER_CUBIC_FOOT},      // 094: Pounds/cubic foot
      { 95, UM_GRAM_PER_MILLILITER},       // 095: Grams/milliliter
      { 96, UM_KILOGRAM_PER_LITER},        // 096: Kilograms/liter
      { 97, UM_GRAM_PER_LITER},            // 097: Grams/liter
      { 98, UM_POUND_PER_CUBIC_INCH},      // 098: Pounds/cubic inch
      { 99, UM_SHORT_TON_PER_CUBIC_YARD}   // 099: Short tons (2000 pounds)/cubic yard
    };

    // _T_PRESSURE
    static const T_MODBUS_CONV_TABLE_SUB_INDEX in2outConvPressure = {
      {  1, UM_INCH_H2O_68F},                     // 001: Inches water at 68 °Fahrenheit 
      {  2, UM_INCH_HG_0C},                       // 002: Inches mercury at 0 °Celsius 
      {  3, UM_FEET_H2O_68F},                     // 003: Feet water at 68 °Fahrenheit 
      {  4, UM_MILLIMETER_H2O_68F},               // 004: Millimeters water at 68 °Fahrenheit 
      {  5, UM_MILLIMETER_HG_0C},                 // 005: Millimeters mercury at 0 °Celsius 
      {  6, UM_POUNDFORCE_PER_SQUARE_INCH},       // 006: Pounds/square inch 
      {  7, UM_BAR},                              // 007: Bar 
      {  8, UM_MILLIBAR},                         // 008: Millibar 
      {  9, UM_GRAMFORCE_PER_SQUARE_CENTIMETER},  // 009: Grams/square centimeter 
      { 10, UM_KILOGRAMFORCE_PER_SQUARE_CENTIMETER},       // 010: Kilograms/square centimeter 
      { 11, UM_PASCAL_U},                                  // 011: Pascals 
      { 12, UM_KILOPASCAL},                                // 012: Kilopascals 
      { 13, UM_TORR},                                      // 013: Torr at 0 °Celsius 
      { 14, UM_ATMOSPHERE}                                 // 014: Atmospheres  
    };
    
    static const T_MODBUS_IN2OUT_CONV_GRP_TBL_ENTRY in2outConvGrpTbl[] = {
    {UM_T_VOLUME_FLOW,        (T_MODBUS_TABLE_ELEMENT *) in2outConvVolumeFlow,  sizeof(in2outConvVolumeFlow)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_TEMPERATURE,        (T_MODBUS_TABLE_ELEMENT *) in2outConvTemperature, sizeof(in2outConvTemperature)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_VOLUME,             (T_MODBUS_TABLE_ELEMENT *) in2outConvVolume,      sizeof(in2outConvVolume)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_MASS,               (T_MODBUS_TABLE_ELEMENT *) in2outConvMass,        sizeof(in2outConvMass)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_MASS_FLOW,          (T_MODBUS_TABLE_ELEMENT *) in2outConvFlow,        sizeof(in2outConvFlow)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_MASS_DENSITY,       (T_MODBUS_TABLE_ELEMENT *) in2outConvDensity,     sizeof(in2outConvDensity)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_PRESSURE,           (T_MODBUS_TABLE_ELEMENT *) in2outConvPressure,     sizeof(in2outConvPressure)/sizeof(T_MODBUS_TABLE_ELEMENT)},
    {UM_T_UNKNOWN_UNIT_GROUP, (T_MODBUS_TABLE_ELEMENT *) in2outConvGlobal,      sizeof(in2outConvGlobal)/sizeof(T_MODBUS_TABLE_ELEMENT)}
    };

    static const TUSIGN8 in2outConvGrpTblLength = sizeof(in2outConvGrpTbl)/sizeof(T_MODBUS_IN2OUT_CONV_GRP_TBL_ENTRY);
    // seach for matching table

    for(i=0; (i<in2outConvGrpTblLength) && (indexA != in2outConvGrpTbl[i].inTblNr); i++)
    {
    };

    // if matching table was found

    if(i<in2outConvGrpTblLength)
    {

      // search for matching table entry
      for(j=0; (j<in2outConvGrpTbl[i].in2outConvTblLength) && (indexB != in2outConvGrpTbl[i].in2outConvTbl[j].two); j++)
      {
      };
      
      // if matching entry was found
      if(j<in2outConvGrpTbl[i].in2outConvTblLength)
      {
        // set the matching index
        *index = in2outConvGrpTbl[i].in2outConvTbl[j].one;
        retval = eTRUE;
      }
    }

    return retval;
}