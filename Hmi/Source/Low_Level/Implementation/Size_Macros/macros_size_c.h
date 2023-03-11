#ifndef _MACROS_SIZE_B_H_
#define _MACROS_SIZE_B_H_

#define Implementation_InitController()             \
{                                                   \
    LcdWrite(SYSTEM_RESET);                         \
    LcdWrite(SET_COLUMN_MIRROR_ON);                 \
    LcdWrite(SET_LCD_BIAS_RATIO_HIGH);              \
    LcdWrite(COMMMON_OP_MODE_SEL_LOW);              \
    LcdWrite(REGULATOR_RESISTOR_SELECT + 0x03);     \
    LcdWrite(SET_POWER_CTRL + (VC_ON|VR_ON|VF_ON)); \
    LcdWrite(SET_VREF_PM);                          \
    LcdWrite(DEFAULT_CONTRAST);                     \
    LcdWrite(SET_DISPLAY_ON);                       \
    LcdWrite(SET_START_LINE);                       \
    LcdWrite(START_INC_COLUMN_ADDR);                \
}

#define Implementation_InitController_Flip_H()      \
{                                                   \
    LcdWrite(SYSTEM_RESET);                         \
    LcdWrite(SET_COLUMN_MIRROR_OFF);                \
    LcdWrite(SET_LCD_BIAS_RATIO_HIGH);              \
    LcdWrite(COMMMON_OP_MODE_SEL_LOW);              \
    LcdWrite(REGULATOR_RESISTOR_SELECT + 0x03);     \
    LcdWrite(SET_POWER_CTRL + (VC_ON|VR_ON|VF_ON)); \
    LcdWrite(SET_VREF_PM);                          \
    LcdWrite(DEFAULT_CONTRAST);                     \
    LcdWrite(SET_DISPLAY_ON);                       \
    LcdWrite(SET_START_LINE);                       \
    LcdWrite(START_INC_COLUMN_ADDR);                \
}

#define Implementation_InitController_Flip_V()      \
{                                                   \
    LcdWrite(SYSTEM_RESET);                         \
    LcdWrite(SET_COLUMN_MIRROR_ON);                 \
    LcdWrite(SET_LCD_BIAS_RATIO_HIGH);              \
    LcdWrite(COMMMON_OP_MODE_SEL_HIGH);             \
    LcdWrite(REGULATOR_RESISTOR_SELECT + 0x03);     \
    LcdWrite(SET_POWER_CTRL + (VC_ON|VR_ON|VF_ON)); \
    LcdWrite(SET_VREF_PM);                          \
    LcdWrite(DEFAULT_CONTRAST);                     \
    LcdWrite(SET_DISPLAY_ON);                       \
    LcdWrite(SET_START_LINE);                       \
    LcdWrite(START_INC_COLUMN_ADDR);                \
}

#define Implementation_InitController_Rotate()      \
{                                                   \
    LcdWrite(SYSTEM_RESET);                         \
    LcdWrite(SET_COLUMN_MIRROR_OFF);                \
    LcdWrite(SET_LCD_BIAS_RATIO_HIGH);              \
    LcdWrite(COMMMON_OP_MODE_SEL_HIGH);             \
    LcdWrite(REGULATOR_RESISTOR_SELECT + 0x03);     \
    LcdWrite(SET_POWER_CTRL + (VC_ON|VR_ON|VF_ON)); \
    LcdWrite(SET_VREF_PM);                          \
    LcdWrite(DEFAULT_CONTRAST);                     \
    LcdWrite(SET_DISPLAY_ON);                       \
    LcdWrite(SET_START_LINE);                       \
    LcdWrite(START_INC_COLUMN_ADDR);                \
}

#endif  // _MACROS_SIZE_B_H_
