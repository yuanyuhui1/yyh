//--------------------------------------------------------------------------------------------------
/*
 Copyright      Copyright ABB, 2022.
                All rights reserved. Reproduction, modification,
                use or disclosure to third parties without express
                authority is forbidden.

 System         Common Framework
 Module         Electronic Services
 Description    prototypes of functions for TMP112
 Remarks

*/
//--------------------------------------------------------------------------------------------------


#ifndef __TMP112_H__
#define __TMP112_H__

/* The TMP112 family features a one-shot temperature-measurement mode. When the device
** is in shutdown mode, writing a 1 to the OS bit begins a single temperature conversion.
** During the conversion, the OS bit reads 0. The device returns to the SHUTDOWN state 
** at the completion of the single conversion. After the conversion, the OS bit reads 1.
** This feature is useful for reducing power consumption in the TMP112 family when 
** continuous temperature monitoring is not required.
*/
#define TMP112_SHUTDOWN_MODE        0

/* ------ TMP112 Register address definition ------------------*/
#define TMP112_TEMP_REGISTER        0x00    // Temperature Register (Read Only)
#define TMP112_CONFIG_REGISTER      0x01    // Configuration Register (Read/Write)
#define TMP112_TLOW_REGISTER        0x02    // TLOW Register (Read/Write)
#define TMP112_THIGH_REGISTER       0x03    // THIGH Register (Read/Write)


/* ------ Bit definition for shutdown mode in Configuration Register Byte 1 ----*/
#define SHUTDOWN_MODE_Pos           0U
#define SHUTDOWN_MODE_Msk           (0x1U << SHUTDOWN_MODE_Pos)
#define SHUTDOWN_MODE               SHUTDOWN_MODE_Msk
#define SHUTDOWN_MODE_0             (0x0U << SHUTDOWN_MODE_Pos)   // normal mode
#define SHUTDOWN_MODE_1             (0x1U << SHUTDOWN_MODE_Pos)   // shutdown mode

/* ------ Bit definition for thermostat mode in Configuration Register Byte 1 ----*/
#define THERMOSTAT_MODE_Pos         1U
#define THERMOSTAT_MODE_Msk         (0x1U << THERMOSTAT_MODE_Pos)
#define THERMOSTAT_MODE             THERMOSTAT_MODE_Msk
#define THERMOSTAT_MODE_0           (0x0U << THERMOSTAT_MODE_Pos)   // Comparator mode
#define THERMOSTAT_MODE_1           (0x1U << THERMOSTAT_MODE_Pos)   // Interrupt mode

/* Bit definition for polarity of the ALERT pin output in Configuration Register Byte 1 */
#define POLARITY_MODE_Pos           2U
#define POLARITY_MODE_Msk           (0x1U << POLARITY_MODE_Pos)
#define POLARITY_MODE               POLARITY_MODE_Msk
#define POLARITY_MODE_0             (0x0U << POLARITY_MODE_Pos)   // ALERT pin active low
#define POLARITY_MODE_1             (0x1U << POLARITY_MODE_Pos)   // ALERT pin active high

/* ------ Bits definition for fault queue in Configuration Register Byte 1 ----*/
#define FAULT_QUEUE_Pos             3U
#define FAULT_QUEUE_Msk             (0x3U << FAULT_QUEUE_Pos)
#define FAULT_QUEUE                 FAULT_QUEUE_Msk
#define FAULT_QUEUE_0               (0x0U << FAULT_QUEUE_Pos)   // consecutive faults 1
#define FAULT_QUEUE_1               (0x1U << FAULT_QUEUE_Pos)   // consecutive faults 2
#define FAULT_QUEUE_2               (0x2U << FAULT_QUEUE_Pos)   // consecutive faults 4
#define FAULT_QUEUE_3               (0x3U << FAULT_QUEUE_Pos)   // consecutive faults 6

/* ------ Bit definition for converter resolution in Configuration Register Byte 1 ----*/
#define RESOLUTION_STATE_Pos        5U
#define RESOLUTION_STATE_Msk        (0x3U << RESOLUTION_STATE_Pos)
#define RESOLUTION_STATE            RESOLUTION_STATE_Msk
#define RESOLUTION_STATE_0           (0x0U << CONVERSION_MODE_Pos)
#define RESOLUTION_STATE_1           (0x1U << CONVERSION_MODE_Pos)
#define RESOLUTION_STATE_2           (0x2U << CONVERSION_MODE_Pos)
#define RESOLUTION_STATE_3           (0x3U << CONVERSION_MODE_Pos)   // 12 bit resolution

/*  Bit definition for one-short temperature-measurement mode in Configuration Register Byte 1 */
#define ONESHORT_MODE_Pos           7U
#define ONESHORT_MODE_Msk           (0x1U << ONESHORT_MODE_Pos)
#define ONESHORT_MODE               ONESHORT_MODE_Msk
#define ONESHORT_MODE_0             (0x0U << ONESHORT_MODE_Pos)   // normal mode
#define ONESHORT_MODE_1             (0x1U << ONESHORT_MODE_Pos)   // one-short mode

/* ------ Bit definition for extended mode in Configuration Register Byte 2 ----*/
#define EXTENDED_MODE_Pos           4U
#define EXTENDED_MODE_Msk           (0x1U << EXTENDED_MODE_Pos)
#define EXTENDED_MODE               EXTENDED_MODE_Msk
#define EXTENDED_MODE_0             (0x0U << EXTENDED_MODE_Pos)   // normal mode: 12-bit data format temperature register
#define EXTENDED_MODE_1             (0x1U << EXTENDED_MODE_Pos)   // extended mode: 13-bit data format temperature register

/* ------ Bit definition for alert state in Configuration Register Byte 2 ----*/
#define ALERT_STATE_Pos             5U
#define ALERT_STATE_Msk             (0x1U << ALERT_STATE_Pos)
#define ALERT_STATE                 ALERT_STATE_Msk

/* ---- Bits definition for continuous conversion mode in Configuration Register Byte 2 ----*/
#define CONVERSION_MODE_Pos         6U
#define CONVERSION_MODE_Msk         (0x3U << CONVERSION_MODE_Pos)
#define CONVERSION_MODE             CONVERSION_MODE_Msk
#define CONVERSION_MODE_0           (0x0U << CONVERSION_MODE_Pos)   // 0.25 Hz
#define CONVERSION_MODE_1           (0x1U << CONVERSION_MODE_Pos)   // 1 Hz
#define CONVERSION_MODE_2           (0x2U << CONVERSION_MODE_Pos)   // 4 Hz (defualt)
#define CONVERSION_MODE_3           (0x3U << CONVERSION_MODE_Pos)   // 8 Hz

#define TMP112_CHIP_ADDRESS         0x48
#define TMP112_I2C_ADDRESS          (TMP112_CHIP_ADDRESS << 1)
#define TMP112_REGISTER_SIZE        2
#define TMP112_CONVERSION_TIME      35 // The TMP112 has a typical conversion time of 26 ms, max 35 ms
#define NORMAL_MODE_RESOLUTION      0.0625f // 0.0625°C/count
#define NORMAL_MODE_TMP_BITS_SHIFT  4 // The first 12 bits are used to indicate temperature
#define THIGH_LIMIT_VALUE           85.0f
#define TLOW_LIMIT_VALUE            80.0f
#define THIGH_LIMIT_TMP_REG         ((uint16_t)(THIGH_LIMIT_VALUE / NORMAL_MODE_RESOLUTION) << NORMAL_MODE_TMP_BITS_SHIFT)
#define TLOW_LIMIT_TMP_REG          ((uint16_t)(TLOW_LIMIT_VALUE / NORMAL_MODE_RESOLUTION) << NORMAL_MODE_TMP_BITS_SHIFT)

/* Shutdown Mode */
#if (TMP112_SHUTDOWN_MODE == 1)
#define CONFIG_REGISTER_BYTE_1      (SHUTDOWN_MODE_1 | THERMOSTAT_MODE_0 | POLARITY_MODE_0 | FAULT_QUEUE_0 | \
                                     RESOLUTION_STATE_3 | ONESHORT_MODE_0)
#else
#define CONFIG_REGISTER_BYTE_1      (SHUTDOWN_MODE_0 | THERMOSTAT_MODE_0 | POLARITY_MODE_0 | FAULT_QUEUE_0 | \
                                     RESOLUTION_STATE_3 | ONESHORT_MODE_0)
#endif
#define CONFIG_REGISTER_BYTE_2      (EXTENDED_MODE_0 | ALERT_STATE | CONVERSION_MODE_2)

void TMP112_Read_Reg(uint16_t regAddress, uint8_t *pRegValue);
void TMP112_Write_Reg(uint16_t regAddress, uint8_t *pRegValue);



#endif /* __TMP112_H__ */