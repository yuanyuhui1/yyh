#ifndef _FUNCTIONS_TYPE_5_H_
#define _FUNCTIONS_TYPE_5_H_

#include "System/Interface/system.h"
#if(HOST_FRAMEWORK_VERSION < 0x020300)
#include "../../../../embos/interface/rtos.h"
#else
#include "RTOS/Interface/rtos.h"
#endif

#include "Hmi/Source/Low_Level/Implementation/Icons/display.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/io.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/diagnostics.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/alarm.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/control.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/comms.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/basic_setup.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/calibrate.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/device.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/service.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/profile.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/totalizer.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/function.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/device_info.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/titlebar_icons.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/slider_bar.h"
#include "Hmi/Source/Low_Level/Implementation/Icons/softkey_background.h"
#include "../implementation/icons/sensor_setup.h"

#ifndef GHW_PCSIM
    #pragma diag_suppress=Pm011,Pm084,Pm025,Pm077,Pm017
#endif
#include <gdisp.h>
#include <gi_disp.h>
#ifndef GHW_PCSIM
    #pragma diag_default=Pm011,Pm084,Pm025,Pm077,Pm017
#endif

#define MAX_ANIMATION_ICONS \
(                           \
DISPLAY_MAX_STAGES +        \
DEVICE_MAX_STAGES  +        \
IO_MAX_STAGES +             \
CONTROL_MAX_STAGES +        \
ALARMS_MAX_STAGES  +        \
PROFILE_MAX_STAGES +        \
TOTALIZER_MAX_STAGES +      \
FUNCTIONS_MAX_STAGES +      \
COMMS_MAX_STAGES +          \
DIAGNOSTICS_MAX_STAGES +    \
SERVICE_MAX_STAGES +        \
DEVICE_INFO_MAX_STAGES +    \
BASIC_SETUP_MAX_STAGES +    \
CALIBRATE_MAX_STAGES +      \
SENSOR_SETUP_MAX_STAGES     \
)

extern TBOOL diagnosticHistory;
extern TBOOL manualAdjust;
extern TBOOL stringParam;

extern GCODE PGSYMBOL FCODE pScrollArrowsIcon;
extern GCODE PGSYMBOL FCODE pNemurFailIcon;
extern GCODE PGSYMBOL FCODE pNemurCheckFuncIcon;
extern GCODE PGSYMBOL FCODE pNemurOutofSpecIcon;
extern GCODE PGSYMBOL FCODE pNemurMaintenanceIcon;
extern GCODE PGSYMBOL FCODE pNemurInformationIcon;

extern GCODE PGSYMBOL FCODE pArmLogoutIcon;
extern GCODE PGSYMBOL FCODE pArmReadOnlyLevelIcon;
extern GCODE PGSYMBOL FCODE pArmStandardLevelIcon;
extern GCODE PGSYMBOL FCODE pArmAdvancedLevelIcon;
extern GCODE PGSYMBOL FCODE pArmServiceLevelIcon;

/*------------------------------------------------------------------------------
Function:			TransmitImage
Parameters:		
	imageByte		Pointer to the data to be written to the display
Returns:			None
Description:		Writes data to the display.
//------------------------------------------------------------------------------*/
#ifdef DISPLAY_SIZE_E       // if Portrait

static void Implementation_TransmitImage_Type5(TUSIGN8 *imageByte)
{	
	TINT16 startYpos;
	TUSIGN16 startXpos;
    TUSIGN16 fore = DEFAULT_COL_FORE;
    TUSIGN16 back = DEFAULT_COL_BACK;
    TUSIGN16 xOFFSET = XPOS_BYTE_OFFSET;
    const TUSIGN8 yPosOnEntry = yPos;

    TUSIGN16 DrawRepeatLineShift = 0;

    if ((drawRepeat) && (diagnosticHistory))
    {
        if      (yPos == 5)     { DrawRepeatLineShift = 6; }
        else if (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    if ((drawRepeat) && (manualAdjust))
    {
        if      (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    if ((drawRepeat) && (stringParam))
    {
        if      (yPos == 4)     { DrawRepeatLineShift = 6; }
        else if (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    // Set HMI colours
    if(yPos >= SOFTKEY_START_PAGE)
    {
        // Soft Keys
        fore = SOFTKEY_COL_FORE;
        if(!xPos)
        {
            // Move one pixel from left edge
            xPos = 2;
        }
        else
        {
            // Move one pixel from right edge
            xPos -= 2;
        }
    }


    if(drawTitleBar)
    {
        // Title bar
        fore = TITLEBAR_COL_FORE;
        back = TITLEBAR_COL_BACK;
    }
    else
    {
        yPos += 3;
    }

    if(drawRepeat)
    {
        if(!diagnosticHistory)
        {
            // Ticker tape &  adjust bargraph
            fore = DRAWREPEAT_COL_FORE;
        }
        yPos -= 2;
    }

	if(writeText)
	{
		startYpos = (yPos*PAGE_PIXEL_HEIGHT) + textLineShift;
		startXpos = (xPos*2) + xOFFSET;
	}
	else
	{
		startYpos = (yPos*PAGE_PIXEL_HEIGHT*3) + DrawRepeatLineShift;
		startXpos = (xPos*4) + xOFFSET;
	}
	TUSIGN32 addrOffset;
	TUSIGN16 maxCols = numBytesToWrite;
	TINT16 col = 0;
	TINT16 row;

	// Error checking
	if(numBytesToWrite > DISPLAY_X)
	{
		numBytesToWrite = DISPLAY_X-xPos;
		replyPacket.bit.procError = 1;
	}
	if(yPos >= ((MAX_PAGES*3)+3))
    {
		yPos = 0;
		replyPacket.bit.procError = 1;
	}	
	if(xPos >= (MAX_COMMON_HMI_WIDTH*2))
	{
		xPos = 0;
		replyPacket.bit.procError = 1;
	}	
	
    if((diagnosticHistory) && (yPosOnEntry < SOFTKEY_START_PAGE))
    {
        startYpos += 18;
        startXpos += 5;
    }

    // Write common HMI patterns to display
    const TUSIGN16 foreToWrite = fore;
    const TUSIGN16 backToWrite = back;
    for(row=0; row<PAGE_PIXEL_HEIGHT; row++)
    {
        col = 0;

        if(writeText)
        {
            addrOffset = BYTES_PER_ROW*(startYpos + row) + startXpos;
        }
        else
        {
            addrOffset = BYTES_PER_ROW*(startYpos + row) + startXpos + (row*2*BYTES_PER_ROW);
        }

        if((yPosOnEntry >= SOFTKEY_START_PAGE) && (xPos < COMMON_HMI_WIDTH_MID))
        {
            addrOffset += SOFTKEY_Y_OFFSET;
        }
        else if((yPosOnEntry >= SOFTKEY_START_PAGE) && (xPos > COMMON_HMI_WIDTH_MID))
        {
            addrOffset += SOFTKEY_Y_OFFSET;
            addrOffset += RIGHT_SOFTKEY_X_OFFSET;
        }
        else if(!drawTitleBar)
        {
            addrOffset += OTHER_Y_OFFSET;
        }

        while(col < maxCols)		
        {
            if(writeText)
            {		
                if((col >= characterInvertStartIndex) && (col <= characterInvertEndIndex))
                {
                    // This is an inverted pattern
                    fore = INVERT_COL_FORE;
                    back = INVERT_COL_BACK;
                }
                else
                {
                    // Normal pattern
                    fore = foreToWrite;
                    back = backToWrite;
                }
                // Note: The msb bit corresponds to the bottom row pixel
                if(imageByte[col] & (0x01<<row))
                {
                    // foreground
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);
                }
                else
                {
                    // background
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);						
                }
            }
            else
            {	
                // The msb bit corresponds to the bottom row pixel
                if(imageByte[col] & (0x01<<row))
                {
                    // foreground
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,fore);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),fore);
                }
                else
                {
                    // background
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,back);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),back);
                }
            }

            if(writeText)
            {	
                addrOffset += 2;
            }
            else
            {
                addrOffset += 4;
            }
            col++;
        }
    }
}

#else   // if Landscape

static void Implementation_TransmitImage_Type5(TUSIGN8 *imageByte)
{	
	TINT16 startYpos;
	TUSIGN16 startXpos;
    TUSIGN16 fore = DEFAULT_COL_FORE;
    TUSIGN16 back = DEFAULT_COL_BACK;
    TUSIGN16 xOFFSET = XPOS_BYTE_OFFSET;
    TBOOL isLeftSoftKey = eFALSE;
    TBOOL isRightSoftKey = eFALSE;
    TUSIGN8 index = 0;

    TUSIGN16 DrawRepeatLineShift = 0;

    if ((drawRepeat) && (diagnosticHistory))
    {
        if      (yPos == 5)     { DrawRepeatLineShift = 6; }
        else if (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    if ((drawRepeat) && (manualAdjust))
    {
        if      (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    if ((drawRepeat) && (stringParam))
    {
        if      (yPos == 4)     { DrawRepeatLineShift = 6; }
        else if (yPos == 6)     { DrawRepeatLineShift = 8; }
        else                    { DrawRepeatLineShift = 0; }
    }

    if (diagnosticHistory)
    {
        if (yPos >= SOFTKEY_START_PAGE)
        {
            textLineShift = 2;
        }

        if (textLineShift > 2)
        {
            textLineShift = 2;
        }

        if (DrawRepeatLineShift > 2)
        {
            DrawRepeatLineShift = 2;
        }
    }

    // Set HMI colours
    if(yPos >= SOFTKEY_START_PAGE)
    {
        // Soft Keys
        fore = SOFTKEY_COL_FORE;

        if(xPos > COMMON_HMI_WIDTH_MID)
        {
            // Right hand key
            isRightSoftKey = eTRUE;
            TUSIGN16 firstCharMarker = 0;

            if (AsianString)
            {
                //Asian
                // Remove padding characters
                while((string[firstCharMarker] == 0x01) && (string[firstCharMarker + 1] == 0x01))
                {
                    firstCharMarker += 2;                       // Skip over leading space
                    numBytesToWrite -= FONT_ASIAN_WIDTH_TFT;    // Adjust numBytesToWrite
                }

                // Find number of characters in string
                index = 0;
                while(string[firstCharMarker+index] )
                {
                    index += 2;
                }

                // Move bit patterns to the required position
                memmove(imageByte, &imageByte[(firstCharMarker*FONT_ASIAN_WIDTH_TFT)/2], numBytesToWrite);


            }
            else
            {
                // European
                while(string[firstCharMarker] == ' ')
                {
                    firstCharMarker++;                  // Skip over leading space
                    numBytesToWrite -= FONT1_WIDTH;     // Adjust numBytesToWrite
                }

                // Find number of characters in string
                index = 0;
                while(string[firstCharMarker+index])
                {
                    index++;
                }

                // Move bit patterns to the required position
                memmove(imageByte, &imageByte[firstCharMarker*FONT1_WIDTH*2], numBytesToWrite);
            }
        }
        else
        {
            // Left hand key
            isLeftSoftKey = eTRUE;

            if (AsianString)
            {
                //Asian
                // Find number of characters in string
                index = 0;
                while(string[index])
                {
                    index += 2;
                }

                // Remove padding characters
                index -= 2;
                while ((string[index] == 0x01) && (string[index+1] == 0x01) && (index > 0))
                {
                    index -= 2;                                 // Skip over leading space
                    numBytesToWrite -= FONT_ASIAN_WIDTH_TFT;    // Adjust numBytesToWrite
                }
            }
            else
            {
                // European
                // Find number of characters in string
                index = 0;
                while(string[index])
                {
                    index++;
                }

                // Remove padding characters
                index --;
                while ((string[index] == ' ') && (index > 0))
                {
                    index --;                           // Skip over leading space
                    numBytesToWrite -= FONT1_WIDTH;     // Adjust numBytesToWrite
                }
            }
        }
        // right justify
        xPos = MAX_COMMON_HMI_WIDTH - numBytesToWrite;

    }
    if(drawTitleBar)
    {
        // Title bar
        fore = TITLEBAR_COL_FORE;
        back = TITLEBAR_COL_BACK;
    }

    if((drawRepeat) && (!diagnosticHistory))
    {
        // Ticker tape &  adjust bargraph
        fore = DRAWREPEAT_COL_FORE;
    }

	if(writeText)
	{
		startYpos = yPos*PAGE_PIXEL_HEIGHT;
		startXpos = (xPos*2) + xOFFSET;

        if(isLeftSoftKey)
        {
            // Indicator only
            startXpos += LEFT_SOFTKEY_X_OFFSET;
            startYpos += LEFT_KEY_Y_OFFSET;
         }
        else if(isRightSoftKey)
        {
            // Indicator only
            startXpos += RIGHT_SOFTKEY_X_OFFSET;
            startYpos += RIGHT_KEY_Y_OFFSET;
        }
        else
        {
            startYpos += textLineShift;
        }
	}
	else
	{
		startYpos = (yPos*PAGE_PIXEL_HEIGHT*3) + DrawRepeatLineShift;
		startXpos = (xPos*4) + xOFFSET;
	}
	TUSIGN32 addrOffset;
	TUSIGN16 maxCols = numBytesToWrite;
	TINT16 col = 0;
	TINT16 row;

	// Error checking
	if(numBytesToWrite > DISPLAY_X)
	{
		numBytesToWrite = DISPLAY_X-xPos;
		replyPacket.bit.procError = 1;
	}
	if(yPos >= (MAX_PAGES*3))
	{
		yPos = 0;
		replyPacket.bit.procError = 1;
	}	
	if(xPos >= (MAX_COMMON_HMI_WIDTH*2))
	{
		xPos = 0;
		replyPacket.bit.procError = 1;
	}	
	
    if((diagnosticHistory) && (yPos < SOFTKEY_START_PAGE))
    {
        if(startYpos >= 96)
        {
            startYpos += 14;
        }
        else
        {
            startYpos += 18;
        }
        startXpos += 3;
    }

    // Soft key background
    if(yPos == SOFTKEY_START_PAGE)
    {
        // Left hand side
        const TUSIGN16 BUTTON_WIDTH = numBytesToWrite+(BUTTON_OFFSET/2);
        assert(BUTTON_WIDTH <= SOFTKEY_BCKGND_LHS_WIDTH);
        TUSIGN32 rowStartAddress = BYTES_PER_ROW*startYpos + startXpos;
        rowStartAddress -= BUTTON_OFFSET;    // 10 pixels to left
        rowStartAddress -= (5*BYTES_PER_ROW);    // Up 5 rows
        const TUSIGN32 BUTTON_START_POS = rowStartAddress;

        // Blank area to the left of the softkey icon
        if(isLeftSoftKey)
        {
            // Blank to the left of this softkey icon
            TUSIGN32 blankStartAddress = BYTES_PER_ROW*(startYpos - 5);
            for(TUSIGN16 row=0; row < SOFTKEY_BCKGND_HEIGHT; row++)
            {
                for(TUSIGN16 col=0; col<(startXpos/2)-8; col++)
                {
                    // Only write desired button blankStartAddress
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+blankStartAddress,DEFAULT_COL_BACK);
                }

                blankStartAddress += BYTES_PER_ROW;
            }
        }

        // Draw softkey icon
        TUSIGN16 counter = 0;
        for(TUSIGN16 row=0; row < SOFTKEY_BCKGND_HEIGHT; row++)
        {
            for(TUSIGN16 col=0; col<SOFTKEY_BCKGND_LHS_WIDTH; col+=2)
            {
                if(col < BUTTON_WIDTH)
                {
                    TUSIGN8 byte = softkey_bckgnd_lhs[counter];
                    for(TUSIGN8 nib = 0; nib < 2; nib++)
                    {
                        TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);

                        // Only write desired button width
                        DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+rowStartAddress,colourTable[tableIndex]);
                    }
                }
                counter++;
            }

            rowStartAddress += BYTES_PER_ROW;
        }

        // Right hand side
        const TUSIGN32 RHS_START_POS = BUTTON_START_POS + (BUTTON_WIDTH*2);
        rowStartAddress = RHS_START_POS;
        counter = 0;
        const TUSIGN8 *ptr_icon;
        if(isLeftSoftKey)
        {
            ptr_icon = softkey_bckgnd_rhs_bot;
        }
        else
        {
            ptr_icon = softkey_bckgnd_rhs_top;
        }
        for(TUSIGN16 row=0; row < SOFTKEY_BCKGND_HEIGHT; row++)
        {
            for(TUSIGN16 col=0; col<SOFTKEY_BCKGND_RHS_WIDTH; col+=2)
            {
                TUSIGN8 byte = ptr_icon[counter];
                for(TUSIGN8 nib = 0; nib < 2; nib++)
                {
                    TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+rowStartAddress,colourTable[tableIndex]);
                }
                counter++;
            }

            rowStartAddress += BYTES_PER_ROW;
        }
    }

    // Write common HMI patterns to display
    const TUSIGN16 foreToWrite = fore;
    const TUSIGN16 backToWrite = back;
    for(row=0; row<PAGE_PIXEL_HEIGHT; row++)
    {
        col = 0;

        if(writeText)
        {
            addrOffset = BYTES_PER_ROW*(startYpos + row) + startXpos;
        }
        else
        {
            addrOffset = BYTES_PER_ROW*(startYpos + row) + startXpos + (row*2*BYTES_PER_ROW);
        }

        if(!drawTitleBar && (yPos < SOFTKEY_START_PAGE))
        {
            addrOffset += OTHER_Y_OFFSET;
        }
        while(col < maxCols)		
        {
            if(writeText)
            {		
                if((col >= characterInvertStartIndex) && (col <= characterInvertEndIndex))
                {
                    // This is an inverted pattern
                    fore = INVERT_COL_FORE;
                    back = INVERT_COL_BACK;
                }
                else
                {
                    // Normal pattern
                    fore = foreToWrite;
                    back = backToWrite;
                }
                // Note: The msb bit corresponds to the bottom row pixel
                if(imageByte[col] & (0x01<<row))
                {
                    // foreground
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);
                }
                else
                {
                    // background
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);						
                }
            }
            else
            {	
                // The msb bit corresponds to the bottom row pixel
                if(imageByte[col] & (0x01<<row))
                {
                    // foreground
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,fore);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,fore);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),fore);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),fore);
                }
                else
                {
                    // background
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,back);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+BYTES_PER_ROW,back);

                    // replicate pixel underneath this pixel
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),back);
                    DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset+(2*BYTES_PER_ROW),back);
                }
            }

            if(writeText)
            {	
                addrOffset += 2;
            }
            else
            {
                addrOffset += 4;
            }
            col++;
        }
    }
}
#endif      // #else (if not CM10 build)

/*------------------------------------------------------------------------------
Function:				DrawIcon
Parameters:				None	
Returns:				None
Description:			Draws an icon that is held in memory
						
Packet Format:
FUNCTION CODE, LENGTH, XPOS, YPOS, ICON NUMBER, CHECKSUM
//------------------------------------------------------------------------------*/
static void Implementation_DrawIcon_Type5(void)
{
	if((*dataPtr >= FIRST_COLOUR_ANIMATION) && (*dataPtr <= LAST_COLOUR_ANIMATION))
    {
        // Large colour animation icons
        const ICON_INFO * const animatedIconTable[MAX_ANIMATION_ICONS] =
        {
            // Device Setup
            &device1_info,&device2_info,&device3_info,&device4_info,&device5_info,&device6_info,

            // Display
            &display1_info,&display2_info,

            // I/O
            &io1_info,&io2_info,&io3_info,&io4_info,&io5_info,&io6_info,

            // Control
            &control1_info,&control2_info,&control3_info,&control4_info,&control5_info,&control6_info,

            // Alarms
            &alarms1_info,&alarms2_info,&alarms3_info,&alarms4_info,&alarms5_info,&alarms6_info,

            // Profile
            &profile1_info,&profile2_info,&profile3_info,&profile4_info,&profile5_info,&profile6_info,

            // Totalizer
            &totalizer1_info,&totalizer2_info,&totalizer3_info,&totalizer4_info,&totalizer5_info,&totalizer6_info,

            // Functions
            &functions1_info,&functions2_info,

            // Communications
            &comms1_info,&comms2_info,&comms3_info,&comms4_info,

            // Diagnostics
            &diagnostics1_info,&diagnostics2_info,

            // Service
            &service1_info,&service2_info,&service3_info,&service4_info,

            // Device Info.
            &deviceInfo1_info,&deviceInfo2_info,

            // Basic Setup
            &basic_setup1_info,&basic_setup2_info,&basic_setup3_info,

            // Calibrate
            &calibrate1_info, &calibrate2_info,

            // Sensor Setup - TODO - correct animations
            &sensorSetup1_info,&sensorSetup2_info,&sensorSetup3_info,&sensorSetup4_info,&sensorSetup5_info,&sensorSetup6_info,
        };

        typedef const struct _DEMO_ANIMATION
        {
            TUSIGN8 firstAnimationStage;
            const ICON_INFO *const*const ptrIconInfo;
            TUSIGN8 maxStages;
        }DEMO_ANIMATION;
        DEMO_ANIMATION demoAnimation[MAX_CONFIG_HEADERS] =
        {
            {ICON_PART_DEVICE_COLOUR_1,&animatedIconTable[ICON_PART_DEVICE_COLOUR_1-FIRST_COLOUR_ANIMATION],DEVICE_MAX_STAGES},
            {ICON_PART_DISPLAY_COLOUR_1,&animatedIconTable[ICON_PART_DISPLAY_COLOUR_1-FIRST_COLOUR_ANIMATION],DISPLAY_MAX_STAGES},
            {ICON_PART_IO_COLOUR_1,&animatedIconTable[ICON_PART_IO_COLOUR_1-FIRST_COLOUR_ANIMATION],IO_MAX_STAGES},
            {ICON_PART_CONTROL_COLOUR_1,&animatedIconTable[ICON_PART_CONTROL_COLOUR_1-FIRST_COLOUR_ANIMATION],CONTROL_MAX_STAGES},
            {ICON_PART_ALARM_COLOUR_1,&animatedIconTable[ICON_PART_ALARM_COLOUR_1-FIRST_COLOUR_ANIMATION],ALARMS_MAX_STAGES},
            {ICON_PART_PROFILE_COLOUR_1,&animatedIconTable[ICON_PART_PROFILE_COLOUR_1-FIRST_COLOUR_ANIMATION],PROFILE_MAX_STAGES},
            {ICON_PART_TOTALIZER_COLOUR_1,&animatedIconTable[ICON_PART_TOTALIZER_COLOUR_1-FIRST_COLOUR_ANIMATION],TOTALIZER_MAX_STAGES},
            {ICON_PART_FUNCTIONS_COLOUR_1,&animatedIconTable[ICON_PART_FUNCTIONS_COLOUR_1-FIRST_COLOUR_ANIMATION],FUNCTIONS_MAX_STAGES},
            {ICON_PART_COMMS_COLOUR_1,&animatedIconTable[ICON_PART_COMMS_COLOUR_1-FIRST_COLOUR_ANIMATION],COMMS_MAX_STAGES},
            {ICON_PART_DIAGNOSTICS_COLOUR_1,&animatedIconTable[ICON_PART_DIAGNOSTICS_COLOUR_1-FIRST_COLOUR_ANIMATION],DIAGNOSTICS_MAX_STAGES},
            {ICON_PART_SERVICE_COLOUR_1,&animatedIconTable[ICON_PART_SERVICE_COLOUR_1-FIRST_COLOUR_ANIMATION],SERVICE_MAX_STAGES},
            {ICON_PART_DEVICE_INFO_COLOUR_1,&animatedIconTable[ICON_PART_DEVICE_INFO_COLOUR_1-FIRST_COLOUR_ANIMATION],DEVICE_INFO_MAX_STAGES},
            {ICON_PART_BASIC_SETUP_COLOUR_1,&animatedIconTable[ICON_PART_BASIC_SETUP_COLOUR_1-FIRST_COLOUR_ANIMATION],BASIC_SETUP_MAX_STAGES},
            {ICON_PART_CALIBRATE_COLOUR_1,&animatedIconTable[ICON_PART_CALIBRATE_COLOUR_1-FIRST_COLOUR_ANIMATION],CALIBRATE_MAX_STAGES},
            {ICON_PART_SENSOR_SETUP_COLOUR_1,&animatedIconTable[ICON_PART_SENSOR_SETUP_COLOUR_1-FIRST_COLOUR_ANIMATION],SENSOR_SETUP_MAX_STAGES},
        };

        static TUSIGN32 lastTime = 0;
        static TINT8 iconStage = 0;
        static TINT8 prevIconStage = -1;
        static TINT8 prevConfigHeaderType = -1;

        // Find correct config header in demo animation table
        for(configHeaderType = 0; configHeaderType < MAX_CONFIG_HEADERS; configHeaderType++)
        {
            if(demoAnimation[configHeaderType].firstAnimationStage == *dataPtr)
                break;
        }
        assert(configHeaderType < MAX_CONFIG_HEADERS);

        TUSIGN32 curTime = OS_GetTime();
        if((curTime - lastTime) > 500)
        {
            iconStage++;
            if(iconStage >= demoAnimation[configHeaderType].maxStages)
            {
                // Stages have wrapped
                iconStage = 0;
            }
        }
        if((configHeaderType != prevConfigHeaderType) || (configHeaderInit))
        {
            // Config header changed - reset stage
            configHeaderInit = eFALSE;
            iconStage=0;
            if(prevConfigHeaderType == -1)
            {
                // First time a config header has been displayed
                prevConfigHeaderType = 0;
            }
        }

        // Exit if icon to be drawn has not altered
        else if(iconStage == prevIconStage)
        {
            return;
        }

        const ICON_INFO *ptrIconInfo = demoAnimation[configHeaderType].ptrIconInfo[iconStage];
        const TUSIGN8 *ptr_icon = ptrIconInfo->patternMap;

        TUSIGN16 icon_y_offset = ptrIconInfo->ys*(BYTES_PER_ROW*2);
        TUSIGN16 icon_x_offset = ptrIconInfo->xs*4;
        TUSIGN32 addrOffset = CONFIG_HEADER_OFFSET + icon_y_offset + icon_x_offset;
        const TUSIGN8 MAX_ROWS = ptrIconInfo->height;
        const TUSIGN8 MAX_COLS = ptrIconInfo->width;

        // Draw icon (one row at a time using auto-increment)
        TUSIGN16 counter = 0,iconCountOffset = 0;
        for(TUSIGN16 row=0; row < MAX_ROWS; row++)
        {
            for(TUSIGN8 incRow = 0; incRow < 2; incRow++)
            {
                counter = 0;
                for(TUSIGN16 col=0; col<MAX_COLS; col+=2)
                {
                    TUSIGN8 byte = ptr_icon[iconCountOffset+counter];
                    for(TUSIGN8 nib = 0; nib < 2; nib++)
                    {
                        TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);

                        // Origin
                        DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);
                        DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);
                    }
                    counter++;
                }

                // Move marker down 1 row
                addrOffset += BYTES_PER_ROW;
            }
            // Store current icon pattern counter
            iconCountOffset += counter;
        }

        lastTime = OS_GetTime();
        prevConfigHeaderType = configHeaderType;
        prevIconStage = iconStage;
    }
    else
    {
        TBOOL iconDrawnByLib = eFALSE;
        PGSYMBOL ptrGraphicsLibIcon = 0;

        // Check icon is within correct range
        assert(*dataPtr >= FIRST_SMALL_ICON);

        // Small colour icons (used in titlebar)
        TUSIGN32 addrOffset = TITLEBAR_ICON_OFFSET;
        const TUSIGN8 MAX_COLS = TITLEBAR_ICON_WIDTH;
        const TUSIGN8 MAX_ROWS = TITLEBAR_ICON_HEIGHT;
        const TUSIGN8 *ptr_icon;
        switch(*dataPtr)
        {
            case ICON_NUMBER_EDIT:
                ptr_icon = numeric_icon;
            break;

            case ICON_STRING_EDIT:
                ptr_icon = alpha_icon;
            break;

            case ICON_ENUM_EDIT:
                ptr_icon = enum_icon;
            break;

            case ICON_ADJUST_EDIT1:
            case ICON_ADJUST_EDIT2:
                ptr_icon = tool_icon;
            break;

            case ICON_CLOCK:
                ptr_icon = clock_icon;
            break;

            case ICON_KEY:
                ptr_icon = key_icon;
            break;

            case ICON_HOURGLASS:
                ptr_icon = hourglass_icon;
            break;

            case ICON_NEMUR_CHECKFUNCTION:
                ptrGraphicsLibIcon = pNemurCheckFuncIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_NEMUR_OFFSPEC:
                ptrGraphicsLibIcon = pNemurOutofSpecIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_NEMUR_MAINTENANCEREQD:
                ptrGraphicsLibIcon = pNemurMaintenanceIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_NEMUR_FAILURE:
                ptrGraphicsLibIcon = pNemurFailIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_NEMUR_INFORMATIONONLY:
                ptrGraphicsLibIcon = pNemurInformationIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_SCROLL_ARROWS:
                ptrGraphicsLibIcon = pScrollArrowsIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_ARM_LOGOUT:
                ptrGraphicsLibIcon = pArmLogoutIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_ARM_READONLY_LEVEL:
                ptrGraphicsLibIcon = pArmReadOnlyLevelIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_ARM_STANDARD_LEVEL:
                ptrGraphicsLibIcon = pArmStandardLevelIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_ARM_ADVANCED_LEVEL:
                ptrGraphicsLibIcon = pArmAdvancedLevelIcon;
                iconDrawnByLib = eTRUE;
            break;

            case ICON_ARM_SERVICE_LEVEL:
                ptrGraphicsLibIcon = pArmServiceLevelIcon;
                iconDrawnByLib = eTRUE;
            break;

            default:
                assert(0);
            break;
        }
        // Draw icon
        if(!iconDrawnByLib)
        {
            TUSIGN16 counter = 0;
            for(TUSIGN16 row=0; row < MAX_ROWS; row++)
            {
                for(TUSIGN16 col=0; col<MAX_COLS; col+=2)
                {
                    TUSIGN8 byte = ptr_icon[counter];
                    for(TUSIGN8 nib = 0; nib < 2; nib++)
                    {
                        TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);
                        DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);
                    }
                    counter++;
                }

                addrOffset += BYTES_PER_ROW;
            }
        }
        else
        {
            GCOLOR oldcolorb = ggetcolorb();
            GCOLOR oldcolorf = ggetcolorf();
            gresetvp();
            gsetcolorb(G_LIGHTGREY);
            if(*dataPtr == ICON_SCROLL_ARROWS)
            {
                gsetcolorf(G_DARKGREY);
                #ifdef DISPLAY_SIZE_D
                gputsym(290,112,ptrGraphicsLibIcon);
                #else
                gputsym((GXT)((GDISPW/2)-6),(GYT)(GDISPH-21),ptrGraphicsLibIcon);
                #endif
            }
            else
            {
                #ifdef DISPLAY_SIZE_D
                gputsym(16,32,ptrGraphicsLibIcon);
                //gputsym(30,36,ptrGraphicsLibIcon);
                #else
                gputsym(2,64,ptrGraphicsLibIcon);
                //gputsym(2,44,ptrGraphicsLibIcon);
                #endif
            }
            gsetcolorb(oldcolorb);
            gsetcolorf(oldcolorf);
        }
    }
}

/*------------------------------------------------------------------------------
Function:				BlankArea
Parameters:				None	
Returns:				None
Description:			Redraws the entire background for the display.
						
Packet Format:
FUNCTION CODE, LENGTH, PAGE INFO, CHECKSUM
//------------------------------------------------------------------------------*/
#ifdef OPTIMIZE_V3_4
#pragma optimize=size medium
#else
#pragma optimize=z 6
#endif
static void Implementation_BlankArea_Type5(void)
{
    if((blankAreaInfo.field.endPage - blankAreaInfo.field.startPage) == 7)
    {
        // Blank title bar area
        TUSIGN16 MAX_ROWS = TITLEBAR_ICON_HEIGHT+1;
        for(TUSIGN16 row=0; row<MAX_ROWS; row++)
        {
            for(TUSIGN16 col=0; col<DISPLAY_X; col++)
            {
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET,TITLEBAR_COL_BACK);
            }
        }

        // Blank area beneath title bar
        MAX_ROWS = NON_TITLEBAR_ROWS;
        #ifdef DISPLAY_SIZE_D
        if(blankForTopSoftkeyRedraw_LLHMI)
        {
            MAX_ROWS = TOP_SOFTKEY_NON_TITLEBAR_ROWS;
        }
        #endif
        for(TUSIGN32 row=0; row<MAX_ROWS; row++)
        {
            for(TUSIGN16 col=0; col<DISPLAY_X; col++)
            {
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+TITLEBAR_END_OFFSET,DEFAULT_COL_BACK);
            }
        }
    }

    // Blank of part of display (not titlebar)
    else if(blankAreaInfo.field.startPage)
    {
        TUSIGN32 FirstPos,LastPos;
        TINT16   FirstLine,LastLine;


        FirstLine  = (blankAreaInfo.field.startPage * PAGE_PIXEL_HEIGHT * 3);
        switch (blankAreaInfo.field.startPage)
        {
        case 1:     FirstLine += ROW_SEPARATION * -1;   break;
        case 2:     FirstLine += ROW_SEPARATION *  0;   break;
        case 3:     FirstLine += ROW_SEPARATION *  1;   break;
        case 4:     FirstLine += ROW_SEPARATION *  2;   break;
        case 5:     FirstLine += ROW_SEPARATION *  3;   break;
        case 6:     FirstLine += ROW_SEPARATION *  4;   break;
        default:                                        break;
        }

        #ifdef DISPLAY_SIZE_E   // if portrait
        FirstLine += PAGE_PIXEL_HEIGHT * 3;
        #endif

        FirstPos = (TUSIGN32)FirstLine * BYTES_PER_ROW;

        LastLine = ((blankAreaInfo.field.endPage * PAGE_PIXEL_HEIGHT * 3)) + ((PAGE_PIXEL_HEIGHT * 3) - 1);
        if (blankAreaInfo.field.endPage < 7)
        {
            switch (blankAreaInfo.field.endPage)
            {
            case 1:     LastLine += ROW_SEPARATION * -1;    break;
            case 2:     LastLine += ROW_SEPARATION *  0;    break;
            case 3:     LastLine += ROW_SEPARATION *  1;    break;
            case 4:     LastLine += ROW_SEPARATION *  2;    break;
            case 5:     LastLine += ROW_SEPARATION *  3;    break;
            case 6:     LastLine += ROW_SEPARATION *  4;    break;
            default:                                        break;
            }

            #ifdef DISPLAY_SIZE_E   // if portrait
            LastLine += PAGE_PIXEL_HEIGHT * 3;

            #endif

            LastPos  = ((TUSIGN32)LastLine + 1) * BYTES_PER_ROW;
        }
        else
        {
            LastPos = (TUSIGN32)DISPLAY_Y * BYTES_PER_ROW;
        }

        FirstPos += OTHER_Y_OFFSET;
        LastPos += OTHER_Y_OFFSET;

        #ifdef DISPLAY_SIZE_D   // if landscape
        if (FirstPos < TITLEBAR_END_OFFSET)
        {
            FirstPos = TITLEBAR_END_OFFSET;

        }
        #endif

        for (  ; FirstPos < LastPos; LastPos -=2)
        {
            DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+FirstPos,DEFAULT_COL_BACK);
        }
    }
    else
    {
    }

}

/*------------------------------------------------------------------------------
Function:				DrawSliderBar
Parameters:				None	
Returns:				None
Description:			Draw a slider bar, at a given position, on the right hand
						edge of the screen. A user defined number is drawn above
						the slider bar (top right of screen).
						
Packet Format:
FUNCTION CODE, LENGTH, NUMBER, YPOS, PIXEL PATTERNS...(3 or 6), CHECKSUM
//------------------------------------------------------------------------------*/
static void Implementation_DrawSliderBar_Type5(void)
{
    // Draw slider bar without marker
    TUSIGN32 addrOffset = SLIDER_BAR_OFFSET;
    TUSIGN16 counter = 0;
    for(TUSIGN8 row=0; row < SLIDER_BAR_HEIGHT; row++)
    {
        for(TUSIGN8 col=0; col<SLIDER_BAR_WIDTH; col+=2)
        {
            TUSIGN8 byte = slider_bar[counter];
            for(TUSIGN8 nib = 0; nib < 2; nib++)
            {
                TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);
            }
            counter++;
        }

        addrOffset += BYTES_PER_ROW;
    }

    // Draw slider bar marker...
    // Find first set pixel
    TUSIGN8 firstSliderCol = dataPtr[2];
    TUSIGN8 shift=0;
    for(; shift < 8; shift++)
    {
        if(firstSliderCol&(0x01<<shift))
            break;
    }
    assert(shift < 8);

    TUSIGN8 bar_start_row = (dataPtr[1]-1)*ROWS_PER_PAGE+(shift*3)-3;
    addrOffset = SLIDER_BAR_OFFSET + SLIDER_BAR_SLOT_OFFSET + (bar_start_row*BYTES_PER_ROW);
    counter = 0;
    for(TUSIGN8 row=0; row < SLIDER_BAR_MARKER_HEIGHT; row++)
    {
        for(TUSIGN8 col=0; col<SLIDER_BAR_MARKER_WIDTH; col+=2)
        {
            TUSIGN8 byte = slider_marker[counter];
            for(TUSIGN8 nib = 0; nib < 2; nib++)
            {
                TUSIGN8 tableIndex = nib ? (byte & 0x0F) : ((byte & 0xF0) >> 4);
                DISPLAY_WRITE(DISPLAY_START_ADDR+COMMON_HMI_ADDR_OFFSET+addrOffset,colourTable[tableIndex]);
            }
            counter++;
        }

        addrOffset += BYTES_PER_ROW;
    }
}

#endif  // #ifndef _FUNCTIONS_TYPE_5_H_
