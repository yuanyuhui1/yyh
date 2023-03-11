#ifndef _ANALOGINPUT_HW_H_
#define _ANALOGINPUT_HW_H_

// ADS1146 reset implementation
#define SFE_ADC_RESET_FEATURE_ENABLE

#define CYCLIC_RESET_TIMER              (60000U)
#define DATALENGTH                      15 
#define MAX_ADC_RESET_ERR               2
#define MAX_SPI_COMM_ERR                (3 * MAX_ADC_RESET_ERR)
#define SPI_ERR_RECOVER_CHECK_TIMES     3
#define MAX_OUT_OF_RANGE_ERR            3
#define OUT_ERR_RECOVER_TIMES           2
// bit mask for adConverter satte
#define ERR_SPICOM_FAIL                 0x01
#define ERR_OUT_OF_RANGE                0x02
#define ERR_CUT_OFF                     0x04


typedef struct {
  TUSIGN16    rawValue;
  TUSIGN8     state;
}_AD_CONVERTER;

extern _AD_CONVERTER adConverter;

//ADS1146 Register Map
//!Burnout Current Source Register
#define BCS                 0x01
#define BCS_REVERSE         0xFE
//!Bias Voltage Register
#define VBIAS               0x03
//!Multiplexer Control Register
#define MUX                 0x01
//!System Control Register 0
#define SYS0                0x00
//!Offset Calibration Coefficient Register 0
#define OFC0                0x04
//!Offset Calibration Coefficient Register 1
#define OFC1                0x05
//!Offset Calibration Coefficient Register 2
#define OFC2                0x06
//!Full-Scale Calibration Coefficient Register 0
#define FCS0                0x07
//!Full-Scale Calibration Coefficient Register 1
#define FCS1                0x08
//!Full-Scale Calibration Coefficient Register 2
#define FCS2                0x09
//!ID Register
#define ID                  0x08
#define IDRESET             0x00

#define WRREGLEN            0x0A
#define RDREGLEN            0x0A

//ADS1146 SPI Commands Definition
//System Control
//!Wake Up From Sleep Mode
#define WAKEUP              0x00
#define SLEEP               0x02
#define SYNC1               0x04
#define SYNC2               0x04
//for low power waste
#define NOP                 0xff
//To Force the SDO Pin stay low power
#define LP 	                0x00

//Data Read
#define RDATA               0x12
#define RDATAC              0x14
#define SDATAC              0x16

#define RREG00              0x20
#define RREG10              0x21
#define RREG20              0x22
#define RREG30              0x23
#define RREG40              0x24
#define RREG50              0x25
#define RREG60              0x26
#define RREG70              0x27
#define RREG80              0x28
#define RREG90              0x29
#define RREGA0              0x2A

#define WREG00              0x40
#define WREG10              0x41
#define WREG20              0x42
#define WREG30              0x43
#define WREG40              0x44
#define WREG50              0x45
#define WREG60              0x46
#define WREG70              0x47
#define WREG80              0x48
#define WREG90              0x49
#define WREGA0              0x4A

#define NBYTE01             0x00
#define NBYTE02             0x01
#define NBYTE03             0x02
#define NBYTE04             0x03
#define NBYTE05             0x04
#define NBYTE06             0x05
#define NBYTE07             0x06
#define NBYTE08             0x07
#define NBYTE09             0x08
#define NBYTE0A             0x09


//Calibration
#define SYSOCAL             0x60
#define SYSGCAL             0x61
#define SELFOCAL            0x62

// public interface for SPI ADC
void GetRawValue(void);
TUSIGN16 HW_initial_AI(void);

#endif // end of _ANALOGINPUT_HW_H_