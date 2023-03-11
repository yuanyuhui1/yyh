#ifndef _MACROS_SIZE_D_H_
#define _MACROS_SIZE_D_H_

#define Implementation_InitController()

#define DISPLAY_X			    320
#define DISPLAY_Y			    240

#ifdef TYPE_4_IMPLEMENTATION
    #define XPOS_BYTE_OFFSET		8//16		// TFT display - row byte offset to common HMI area
#elif defined (TYPE_5_IMPLEMENTATION)
    #define XPOS_BYTE_OFFSET	    32	    // TFT display - col byte offset to common HMI area
#else
    #error implementation must be type4 or type5
#endif

#ifdef TYPE_4_IMPLEMENTATION
#define BYTES_PER_ROW			160		// TFT display - num bytes for one row
#else
#define BYTES_PER_ROW			((TUSIGN32)640)		// TFT display - num bytes for one row
#endif
#define COMMON_HMI_ADDR_OFFSET	0//5760   // TFT display - total byte offset to beginning of											
                                        // start row of common HMI area ((240-12-128)*320/2)		

#define YPOS_BYTE_OFFSET	        107	// TFT display - row byte offset to common HMI area
#define DISPLAY_SETTING               0x0020   // Landscape configuration (2.2")

#define ROWS_PER_PAGE           (PAGE_PIXEL_HEIGHT*3)
#define SLIDER_BAR_Y_OFFSET     (BYTES_PER_ROW*(ROWS_PER_PAGE+3))
#define SOFTKEY_Y_OFFSET        (BYTES_PER_ROW*PAGE_PIXEL_HEIGHT*4)
#define OTHER_Y_OFFSET          (BYTES_PER_ROW*PAGE_PIXEL_HEIGHT*2)

#define ROW_SEPARATION          2

#ifdef TYPE_4_IMPLEMENTATION
    #define RIGHT_SOFTKEY_X_OFFSET  15
    #define CONFIG_HEADER_OFFSET    15221   // 95 rows + 42 pixels
    #define TITLEBAR_ICON_OFFSET    142     // 36 pixels from end
    #define TITLEBAR_END_OFFSET     4640    // 29 rows
    #define SLIDER_BAR_OFFSET       6712    // 42 rows - 16 pixels
    #define SLIDER_BAR_SLOT_OFFSET  1920    // 12 rows before marker slide area
    #define ABB_SPLASH_START        9449    // 59 rows + 18 pixels
    #define SOFTKEY_START_PAGE      21
#else
    #define FRAME_TITLE_START_X     10      //
    #define LEFT_SOFTKEY_X_OFFSET   336     // 168 pixels
    #define RIGHT_SOFTKEY_X_OFFSET  336     // 168 pixels

    #define LEFT_KEY_Y_OFFSET       (diagnosticHistory ? 34 : 32)      // 32 rows down


    #define RIGHT_KEY_Y_OFFSET      (-158)  // 158 rows up
    #define BUTTON_OFFSET           16
    #define CONFIG_HEADER_OFFSET    60884   // 95 rows + 42 pixels
    #define TITLEBAR_ICON_OFFSET    0       // 36 pixels from end
    #define TITLEBAR_END_OFFSET     18560   // 29 rows
    #define SLIDER_BAR_OFFSET       26208   // 40 rows - 16 pixels
    #define SLIDER_BAR_SLOT_OFFSET  ((TUSIGN32)(7680))    // 12 rows before marker slide area
    #define ABB_SPLASH_START        37796   // 59 rows + 18 pixels
#endif

#define NON_TITLEBAR_ROWS               200
#define TOP_SOFTKEY_NON_TITLEBAR_ROWS   6

#endif  // _MACROS_SIZE_D_H_
