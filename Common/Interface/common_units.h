//----------------------------------------------------------------------------------------------------------
/*
 Copyright              Copyright ABB, 2006.
                        All rights reserved. Reproduction, modification,
                        use or disclosure to third parties without express
                        authority is forbidden.


 System         Common Units Handling Mechanism
 Module         COMMON_UNITS
 Description    Contains a list of common units codes and units type codes plus other
                units related common definitions.

 <pre>
 </pre>
 Remarks

*/
//----------------------------------------------------------------------------------------------------------

#ifdef __COMMON_UNITS_H__
  #error common_units.h included more than once
#endif

#define __COMMON_UNITS_H__


#define SZ_CUSTOM_UNITS_NAME	5		// Number of characters (excluding NULL terminator) for
										// a custom units name string.


// Table of units type codes.
typedef enum
{
	UNITSTYPE_NOUNITS,			// for using units mechanism with value that has no associated units
	UNITSTYPE_VELOCITY,
	UNITSTYPE_FLOW,				// for mass flow and volume flow units together
	UNITSTYPE_MASS,
	UNITSTYPE_MASSFLOW,
	UNITSTYPE_VOLUME,
	UNITSTYPE_VOLUMEFLOW,
	UNITSTYPE_TOTALIZER,		// for mass and volume units together
	UNITSTYPE_PRESSURE,
	UNITSTYPE_TEMPERATURE,
	UNITSTYPE_DENSITY,
	UNITSTYPE_VOLTAGE,
	UNITSTYPE_RESISTANCE,
	UNITSTYPE_TIME,
	UNITSTYPE_FREQUENCY,
	UNITSTYPE_CURRENT,
	UNITSTYPE_RATIO,
	UNITSTYPE_CAPACITANCE,
	UNITSTYPE_INDUCTANCE,
	UNITSTYPE_CONDUCTIVITY,
	UNITSTYPE_LENGTH,
	
    //Eric Add Enegry Unit Type
    UNITSTYPE_POWER,
	UNITSTYPE_ENERGY,
      
} T_COMMON_UNITSTYPE;


// Definition for units code associated with UNITSTYPE_NOUNITS
#define NO_UNITS_CODE		(0)


// Definition for custom units code, can be used for each units type.
#define CUSTOM_UNITS_CODE	(0xFF)


// Definitions for units codes for units type UNITSTYPE_VELOCITY.
typedef enum
{
	VELOCITY_MILLIMETRES_PER_SECOND,
	VELOCITY_MILLIMETRES_PER_MINUTE,
	VELOCITY_MILLIMETRES_PER_HOUR,
    VELOCITY_CENTIMETRES_PER_SECOND,
	VELOCITY_CENTIMETRES_PER_MINUTE,
	VELOCITY_CENTIMETRES_PER_HOUR,
	VELOCITY_METRES_PER_SECOND,
	VELOCITY_METRES_PER_MINUTE,
	VELOCITY_METRES_PER_HOUR,
	VELOCITY_INCHES_PER_SECOND,
	VELOCITY_INCHES_PER_MINUTE,
	VELOCITY_INCHES_PER_HOUR,
	VELOCITY_FEET_PER_SECOND,
	VELOCITY_FEET_PER_MINUTE,
	VELOCITY_FEET_PER_HOUR,
	VELOCITY_YARDS_PER_SECOND,
	VELOCITY_YARDS_PER_MINUTE,
	VELOCITY_YARDS_PER_HOUR,
	VELOCITY_KILOMETRES_PER_SECOND,
	VELOCITY_KILOMETRES_PER_MINUTE,
	VELOCITY_KILOMETRES_PER_HOUR,
	VELOCITY_MILES_PER_SECOND,
	VELOCITY_MILES_PER_MINUTE,
	VELOCITY_MILES_PER_HOUR,	
	VELOCITY_PERCENT,
	
	VELOCITY_CUSTOM_UNITS = CUSTOM_UNITS_CODE
		
} T_COMMON_VELOCITYUNITS;


// Definitions for units codes for units type UNITSTYPE_FLOW.
// (This is a combination of mass-flow and volume-flow units.)
typedef enum
{
	FLOW_MILLILITRES_PER_SECOND,
	FLOW_MILLILITRES_PER_MINUTE,
	FLOW_MILLILITRES_PER_HOUR,
	FLOW_MILLILITRES_PER_DAY,
	FLOW_LITRES_PER_SECOND,
	FLOW_LITRES_PER_MINUTE,
	FLOW_LITRES_PER_HOUR,
	FLOW_LITRES_PER_DAY,
	FLOW_CUBIC_FEET_PER_SECOND,
	FLOW_CUBIC_FEET_PER_MINUTE,
	FLOW_CUBIC_FEET_PER_HOUR,
	FLOW_CUBIC_FEET_PER_DAY,
	FLOW_CUBIC_METRES_PER_SECOND,
	FLOW_CUBIC_METRES_PER_MINUTE,
	FLOW_CUBIC_METRES_PER_HOUR,
	FLOW_CUBIC_METRES_PER_DAY,
	FLOW_US_GALLONS_PER_SECOND,
	FLOW_US_GALLONS_PER_MINUTE,
	FLOW_US_GALLONS_PER_HOUR,
	FLOW_US_GALLONS_PER_DAY,
	FLOW_IMPERIAL_GALLONS_PER_SECOND,
	FLOW_IMPERIAL_GALLONS_PER_MINUTE,
	FLOW_IMPERIAL_GALLONS_PER_HOUR,
	FLOW_IMPERIAL_GALLONS_PER_DAY,
	FLOW_BREW_BARRELS_PER_SECOND,
	FLOW_BREW_BARRELS_PER_MINUTE,
	FLOW_BREW_BARRELS_PER_HOUR,
	FLOW_BREW_BARRELS_PER_DAY,
	FLOW_HECTOLITRES_PER_SECOND,
	FLOW_HECTOLITRES_PER_MINUTE,
	FLOW_HECTOLITRES_PER_HOUR,
	FLOW_HECTOLITRES_PER_DAY,	
	FLOW_MEGA_LITRES_PER_SECOND,
	FLOW_MEGA_LITRES_PER_MINUTE,
	FLOW_MEGA_LITRES_PER_HOUR,
	FLOW_MEGA_LITRES_PER_DAY,
	FLOW_MEGA_US_GALLONS_PER_SECOND,
	FLOW_MEGA_US_GALLONS_PER_MINUTE,
	FLOW_MEGA_US_GALLONS_PER_HOUR,
	FLOW_MEGA_US_GALLONS_PER_DAY,
	FLOW_MEGA_IMPERIAL_GALLONS_PER_SECOND,
	FLOW_MEGA_IMPERIAL_GALLONS_PER_MINUTE,
	FLOW_MEGA_IMPERIAL_GALLONS_PER_HOUR,
	FLOW_MEGA_IMPERIAL_GALLONS_PER_DAY,			
	FLOW_GRAMS_PER_SECOND,
	FLOW_GRAMS_PER_MINUTE,
	FLOW_GRAMS_PER_HOUR,
	FLOW_GRAMS_PER_DAY,
    FLOW_KILOGRAMS_PER_SECOND,
	FLOW_KILOGRAMS_PER_MINUTE,
	FLOW_KILOGRAMS_PER_HOUR,
	FLOW_KILOGRAMS_PER_DAY,
	FLOW_TONS_PER_SECOND,
	FLOW_TONS_PER_MIN,
	FLOW_TONS_PER_HOUR,
	FLOW_TONS_PER_DAY,
    FLOW_POUNDS_PER_SECOND,
	FLOW_POUNDS_PER_MINUTE,
	FLOW_POUNDS_PER_HOUR,
	FLOW_POUNDS_PER_DAY,	
	FLOW_PERCENT,

	FLOW_CUSTOM_UNITS = CUSTOM_UNITS_CODE            // Custom units (volume and mass)

} T_COMMON_FLOWUNITS;


// Definitions for units codes for units type UNITSTYPE_MASS.
typedef enum
{
	MASS_GRAMS,
	MASS_KILOGRAMS,
	MASS_TONNES,
	MASS_POUNDS,
	MASS_OUNCES,
	
	MASS_CUSTOM_UNITS = CUSTOM_UNITS_CODE
	
} T_COMMON_MASSUNITS;


// Definitions for units codes for units type UNITSTYPE_MASSFLOW.
typedef enum
{
	MASSFLOW_GRAMS_PER_SECOND,
	MASSFLOW_GRAMS_PER_MINUTE,
	MASSFLOW_GRAMS_PER_HOUR,
	MASSFLOW_GRAMS_PER_DAY,
    MASSFLOW_KILOGRAMS_PER_SECOND,
	MASSFLOW_KILOGRAMS_PER_MINUTE,
	MASSFLOW_KILOGRAMS_PER_HOUR,
	MASSFLOW_KILOGRAMS_PER_DAY,
	MASSFLOW_TONS_PER_SECOND,
	MASSFLOW_TONS_PER_MIN,
	MASSFLOW_TONS_PER_HOUR,
	MASSFLOW_TONS_PER_DAY,
    MASSFLOW_POUNDS_PER_SECOND,
	MASSFLOW_POUNDS_PER_MINUTE,
	MASSFLOW_POUNDS_PER_HOUR,
	MASSFLOW_POUNDS_PER_DAY,	
	MASSFLOW_PERCENT,	
	
	MASSFLOW_CUSTOM_UNITS = CUSTOM_UNITS_CODE
	
} T_COMMON_MASSFLOW;



// Definitions for units codes for units type UNITSTYPE_VOLUME.
typedef enum
{
	VOLUME_CUBIC_METRES,
	VOLUME_CUBIC_CENTIMETRES,
	VOLUME_CUBIC_MILLIMETRES,
	VOLUME_LITRES,
	VOLUME_CENTILITRES,
	VOLUME_MILLILITRES,
	VOLUME_CUBIC_INCHES,
	VOLUME_CUBIC_FEET,
	VOLUME_CUBIC_YARDS,
	VOLUME_KILOLITRES,
	VOLUME_HECTOLITRES,
	VOLUME_IMP_GALLONS,
	VOLUME_US_GALLONS_LIQUID,
	VOLUME_US_GALLONS_DRY,
	VOLUME_US_BARRELS_DRY,				// (1 = 30.5 US Gallons)
	VOLUME_US_BARRELS_BEER,				// (1 = 31 US Gallons)
	VOLUME_US_BARRELS_LIQUID,			// (1 = 31.5 US Gallons)
	VOLUME_US_BARRELS_OIL,				// (1 = 42 US Gallons, for oil/petroleum)
	VOLUME_IMP_BARRELS_BEER,			// (1 = 36 Imp Gallons)
	VOLUME_IMP_BARRELS_LIQUID,			// (1 = 31.5 Imp Gallons, for non-beer liquid)
	VOLUME_US_BUSHELS,
	VOLUME_IMP_BUSHELS,
	VOLUME_CUBIC_DECIMETRES,
	VOLUME_MEGALITRES,
	VOLUME_MEGA_IMP_GALLONS,
	VOLUME_MEGA_US_GALLONS_LIQUID,

	VOLUME_CUSTOM_UNITS = CUSTOM_UNITS_CODE
	
}T_COMMON_VOLUME;



// Definitions for units codes for units type UNITSTYPE_VOLUMEFLOW.
typedef enum
{
	VOLUMEFLOW_MILLILITRES_PER_SECOND,
	VOLUMEFLOW_MILLILITRES_PER_MINUTE,
	VOLUMEFLOW_MILLILITRES_PER_HOUR,
	VOLUMEFLOW_MILLILITRES_PER_DAY,
	VOLUMEFLOW_LITRES_PER_SECOND,
	VOLUMEFLOW_LITRES_PER_MINUTE,
	VOLUMEFLOW_LITRES_PER_HOUR,
	VOLUMEFLOW_LITRES_PER_DAY,
	VOLUMEFLOW_CUBIC_FEET_PER_SECOND,
	VOLUMEFLOW_CUBIC_FEET_PER_MINUTE,
	VOLUMEFLOW_CUBIC_FEET_PER_HOUR,
	VOLUMEFLOW_CUBIC_FEET_PER_DAY,
	VOLUMEFLOW_CUBIC_METRES_PER_SECOND,
	VOLUMEFLOW_CUBIC_METRES_PER_MINUTE,
	VOLUMEFLOW_CUBIC_METRES_PER_HOUR,
	VOLUMEFLOW_CUBIC_METRES_PER_DAY,
	VOLUMEFLOW_US_GALLONS_PER_SECOND,
	VOLUMEFLOW_US_GALLONS_PER_MINUTE,
	VOLUMEFLOW_US_GALLONS_PER_HOUR,
	VOLUMEFLOW_US_GALLONS_PER_DAY,
	VOLUMEFLOW_IMPERIAL_GALLONS_PER_SECOND,
	VOLUMEFLOW_IMPERIAL_GALLONS_PER_MINUTE,
	VOLUMEFLOW_IMPERIAL_GALLONS_PER_HOUR,
	VOLUMEFLOW_IMPERIAL_GALLONS_PER_DAY,
	VOLUMEFLOW_BREW_BARRELS_PER_SECOND,
	VOLUMEFLOW_BREW_BARRELS_PER_MINUTE,
	VOLUMEFLOW_BREW_BARRELS_PER_HOUR,
	VOLUMEFLOW_BREW_BARRELS_PER_DAY,
	VOLUMEFLOW_HECTOLITRES_PER_SECOND,
	VOLUMEFLOW_HECTOLITRES_PER_MINUTE,
	VOLUMEFLOW_HECTOLITRES_PER_HOUR,
	VOLUMEFLOW_HECTOLITRES_PER_DAY,
	VOLUMEFLOW_MEGA_LITRES_PER_SECOND,
	VOLUMEFLOW_MEGA_LITRES_PER_MINUTE,
	VOLUMEFLOW_MEGA_LITRES_PER_HOUR,
	VOLUMEFLOW_MEGA_LITRES_PER_DAY,
	VOLUMEFLOW_MEGA_US_GALLONS_PER_SECOND,
	VOLUMEFLOW_MEGA_US_GALLONS_PER_MINUTE,
	VOLUMEFLOW_MEGA_US_GALLONS_PER_HOUR,
	VOLUMEFLOW_MEGA_US_GALLONS_PER_DAY,
	VOLUMEFLOW_MEGA_IMPERIAL_GALLONS_PER_SECOND,
	VOLUMEFLOW_MEGA_IMPERIAL_GALLONS_PER_MINUTE,
	VOLUMEFLOW_MEGA_IMPERIAL_GALLONS_PER_HOUR,
	VOLUMEFLOW_MEGA_IMPERIAL_GALLONS_PER_DAY,		
	VOLUMEFLOW_PERCENT,

	VOLUMEFLOW_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

}T_COMMON_VOLUMEFLOW;

//Eric add Power Units
typedef enum
{
  
  WATT,
  KILOWATT,
  MILLIWATT,
  KILOJOULE_PER_SECOND,
  KILOJOULE_PER_MINUTE,
  KILOJOULE_PER_HOUR,
  KILOJOULE_PER_DAY
  
}T_COMMON_POWER;

//Eric add Energy Units
typedef enum
{
  JOULE,
  KILOJOULE,

}T_COMMON_ENERGY;

// Definitions for units codes for units type UNITSTYPE_TOTALIZER.
// (This is a combination of mass and volume units.)
typedef enum
{
	TOTALIZER_CUBIC_METRES,
	TOTALIZER_CUBIC_CENTIMETRES,
	TOTALIZER_CUBIC_MILLIMETRES,
	TOTALIZER_LITRES,
	TOTALIZER_CENTILITRES,
	TOTALIZER_MILLILITRES,
	TOTALIZER_CUBIC_INCHES,
	TOTALIZER_CUBIC_FEET,
	TOTALIZER_CUBIC_YARDS,
	TOTALIZER_KILOLITRES,
	TOTALIZER_HECTOLITRES,
    
	TOTALIZER_GRAMS,
	TOTALIZER_KILOGRAMS,
	TOTALIZER_TONNES,
	TOTALIZER_POUNDS,
	TOTALIZER_OUNCES,	
	TOTALIZER_IMP_GALLONS,
	TOTALIZER_US_GALLONS_LIQUID,
	TOTALIZER_US_GALLONS_DRY,
	TOTALIZER_US_BARRELS_DRY,			// (1 = 30.5 US Gallons)
	TOTALIZER_US_BARRELS_BEER,			// (1 = 31 US Gallons)
	TOTALIZER_US_BARRELS_LIQUID,		// (1 = 31.5 US Gallons, for non-beer liquid)
	TOTALIZER_US_BARRELS_OIL,			// (1 = 42 US Gallons, for oil/petroleum, aka bbl: "blue barrel")
	TOTALIZER_IMP_BARRELS_BEER,			// (1 = 36 Imp Gallons)
	TOTALIZER_IMP_BARRELS_LIQUID,		// (1 = 31.5 Imp Gallons, for non-beer liquid)
	TOTALIZER_US_BUSHELS,
	TOTALIZER_IMP_BUSHELS,
	TOTALIZER_CUBIC_DECIMETRES,
	TOTALIZER_MEGALITRES,
	TOTALIZER_MEGA_IMP_GALLONS,
	TOTALIZER_MEGA_US_GALLONS_LIQUID,
    
    //Eric Add Energy Units
    TOTALIZER_JOULE,
    TOTALIZER_KILOJOULE,
    
	
	TOTALIZER_CUSTOM_UNITS = CUSTOM_UNITS_CODE
	
}T_COMMON_TOTALIZER;


// Definitions for units codes for units type UNITSTYPE_PRESSURE.
typedef enum
{
	PRESSURE_MEGAPASCALS,
	PRESSURE_MILLIPASCALS,
	PRESSURE_MICROPASCALS,
	PRESSURE_PASCALS,
	PRESSURE_KILOPASCALS,
	PRESSURE_BAR,
	PRESSURE_MILLIBAR,
	PRESSURE_ATMOSPHERES,
	PRESSURE_POUNDS_PER_SQUARE_INCH,
	PRESSURE_POUNDS_PER_SQUARE_INCH_ABSOLUTE,
	PRESSURE_POUNDS_PER_SQUARE_INCH_GUAGE,
	PRESSURE_GRAMS_PER_SQUARE_CENTIMETRE,
	PRESSURE_KILOGRAMS_PER_SQUARE_CENTIMETRE,
	PRESSURE_INCHES_H2O,
	PRESSURE_MILLIMETRES_H2O,
	PRESSURE_PERCENT,
	
	PRESSURE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	
	
}T_COMMON_PRESSURE;


// Definitions for units codes for units type UNITSTYPE_TEMPERATURE.
typedef enum
{
	TEMPERATURE_KELVIN,
	TEMPERATURE_CELSIUS,
	TEMPERATURE_FAHRENHEIT,
	TEMPERATURE_PERCENT,
	
	TEMPERATURE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	
	
}T_COMMON_TEMPERATURE;


// Definitions for units codes for units type UNITSTYPE_DENSITY.
typedef enum
{
	DENSITY_KILOGRAMS_PER_CUBIC_METRE,
	DENSITY_GRAMS_PER_CUBIC_CENTIMETRE,
	DENSITY_KILOGRAMS_PER_LITRES,
	DENSITY_POUNDS_PER_CUBIC_INCH,
	DENSITY_POUNDS_PER_CUBIC_FOOT,
	
	DENSITY_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_DENSITY;



// Definitions for units codes for units type UNITSTYPE_VOLTAGE.
typedef enum
{
	VOLTAGE_VOLTS,
	VOLTAGE_MILLIVOLTS,
	VOLTAGE_MICROVOLTS,
	VOLTAGE_PERCENT,
		
	VOLTAGE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_VOLTAGE;


// Definitions for units codes for units type UNITSTYPE_RESISTANCE.
typedef enum
{
	RESISTANCE_OHMS,
	RESISTANCE_KOHMS,
	
	RESISTANCE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_RESISTANCE;


// Definitions for units codes for units type UNITSTYPE_TIME.
typedef enum
{
	TIME_MICROSECONDS,
	TIME_MILLISECONDS,
	TIME_SECONDS,
	TIME_MINUTES,
	TIME_HOURS,
	TIME_DAYS,
	TIME_WEEKS,
	
	TIME_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_TIME;



// Definitions for units codes for units type UNITSTYPE_FREQUENCY.
typedef enum
{
	FREQUENCY_HERTZ,
	FREQUENCY_KILOHERTZ,
	FREQUENCY_MEGAHERTZ,
	FREQUENCY_PERCENT,
	
	FREQUENCY_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_FREQUENCY;


// Definitions for units codes for units type UNITSTYPE_CURRENT.
typedef enum
{
	CURRENT_MICROAMPS,
	CURRENT_MILLIAMPS,
	CURRENT_AMPS,
	CURRENT_PERCENT,
	
	CURRENT_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_CURRENT;


// Definitions for units codes for units type UNITSTYPE_RATIO.
// (Use these units type codes where the value needs no particular units type context,
// else use a definition of the required ratiometric units code for the associated units type,
// eg FLOW_PERCENT for value of units type UNITSTYPE_FLOW).
typedef enum
{
	RATIO_PERCENT,
	RATIO_PERMILLE,
	RATIO_PARTS_PER_MILLON,	

	RATIO_CUSTOM_UNITS = CUSTOM_UNITS_CODE

} T_COMMON_RATIO;

// Definitions for units codes for units type UNITSTYPE_CAPACITANCE.
typedef enum
{
	CAPACITANCE_PICOFARADS,
	CAPACITANCE_NANOFARADS,
	CAPACITANCE_MICROFARADS,
	CAPACITANCE_FARADS,
	
	CAPACITANCE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_CAPACITANCE;

// Definitions for units codes for units type UNITSTYPE_INDUCTANCE.
typedef enum
{
	INDUCTANCE_MILLIHENRIES,
	INDUCTANCE_HENRIES,
	
	INDUCTANCE_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_INDUCTANCE;

// Definitions for units codes for units type UNITSTYPE_CONDUCTIVITY.
typedef enum
{
	CONDUCTIVITY_MICROSIEMENS,
	CONDUCTIVITY_MILLISIEMENS,
	CONDUCTIVITY_SIEMENS,
	
	CONDUCTIVITY_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_CONDUCTIVITY;

// Definitions for units codes for units type UNITSTYPE_LENGTH.
typedef enum
{
	LENGTH_MILLIMETRE,
	LENGTH_CENTIMETRE,
	LENGTH_METRE,
	LENGTH_FEET,
	LENGTH_INCH,

	LENGTH_CUSTOM_UNITS = CUSTOM_UNITS_CODE	

} T_COMMON_LENGTH;




