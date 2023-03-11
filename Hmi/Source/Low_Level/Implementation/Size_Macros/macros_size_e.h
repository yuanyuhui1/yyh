#ifndef _MACROS_SIZE_E_H_
#define _MACROS_SIZE_E_H_

#define Implementation_InitController()

#define DISPLAY_X			    240
#define DISPLAY_Y			    320

#ifdef TYPE_4_IMPLEMENTATION
    #define XPOS_BYTE_OFFSET		    0	// TFT display - row byte offset to common HMI area
#elif defined (TYPE_5_IMPLEMENTATION)
	#define XPOS_BYTE_OFFSET	        0	// TFT display - col byte offset to common HMI area
#else
    #error implementation must be type4 or type5
#endif

#ifdef TYPE_4_IMPLEMENTATION
#define BYTES_PER_ROW			120		// TFT display - num bytes for one row
#else
#define BYTES_PER_ROW			((TUSIGN32)480)		// TFT display - num bytes for one row
#endif

#define COMMON_HMI_ADDR_OFFSET	0//23040 	// TFT display - total byte offset to beginning of
                                        // common HMI area (112*320/2)

#define YPOS_BYTE_OFFSET	        192	// TFT display - row byte offset to common HMI area
#define DISPLAY_SETTING               0x0080

#define ROWS_PER_PAGE           (PAGE_PIXEL_HEIGHT*3)
#define SLIDER_BAR_Y_OFFSET     (BYTES_PER_ROW*((PAGE_PIXEL_HEIGHT*3)*4))
#define SOFTKEY_Y_OFFSET        (BYTES_PER_ROW*((PAGE_PIXEL_HEIGHT*3)*4))
#define OTHER_Y_OFFSET          (BYTES_PER_ROW*(PAGE_PIXEL_HEIGHT*3))

#define ROW_SEPARATION          2

#ifdef TYPE_4_IMPLEMENTATION
    #define RIGHT_SOFTKEY_X_OFFSET  0
    #define CONFIG_HEADER_OFFSET    15970   // 133 rows + 20 pixels
    #define TITLEBAR_ICON_OFFSET    102     // 36 pixels from end
    #define TITLEBAR_END_OFFSET     3480    // 29 rows
    #define SLIDER_BAR_OFFSET       8156    // 68 rows - 8 pixels
    #define SLIDER_BAR_SLOT_OFFSET  1440    // 12 rows before marker slide area
    #define ABB_SPLASH_START        12601    // 105 rows + 2 pixels
    #define SOFTKEY_START_PAGE      21
#else
    #define FRAME_TITLE_START_X     0       //
    #define LEFT_SOFTKEY_X_OFFSET   0       // 0 pixels
    #define RIGHT_SOFTKEY_X_OFFSET  0       // 0 pixels
    #define LEFT_KEY_Y_OFFSET       120     // 60 rows down
    #define RIGHT_KEY_Y_OFFSET      120     // 60 rows down
    #define CONFIG_HEADER_OFFSET    63880   // 133 rows + 20 pixels
    #define TITLEBAR_ICON_OFFSET    408     // 36 pixels from end
    #define TITLEBAR_END_OFFSET     13920   // 29 rows
    #define SLIDER_BAR_OFFSET       32624   // 68 rows - 8 pixels
    #define SLIDER_BAR_SLOT_OFFSET  ((TUSIGN32)5760)    // 12 rows before marker slide area
    #define ABB_SPLASH_START        50404    // 105 rows + 2 pixels
#endif

#define NON_TITLEBAR_ROWS       291

#endif  // _MACROS_SIZE_E_H_
