#define DISPLAY_STAGE2_WIDTH     90
#define DISPLAY_STAGE2_HEIGHT    34
#define DISPLAY_STAGE2_START_X   6
#define DISPLAY_STAGE2_START_Y   7

#define DISPLAY_MAX_STAGES       2

const unsigned char display1[ANIMATION_ICON_WIDTH*ANIMATION_ICON_HEIGHT/2] = 
{
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x09,0x00,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x11,0x11,0x00,0x00,0x01,0x11,0x10,
	0x00,0x00,0x11,0x11,0x00,0x00,0x01,0x11,
	0x10,0x00,0x00,0x11,0x11,0x00,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x01,0x11,0x11,0x10,0x00,0x11,
	0x11,0x11,0x00,0x01,0x11,0x11,0x10,0x00,
	0x11,0x11,0x11,0x00,0x01,0x11,0x11,0x10,
	0x00,0x09,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x11,0x10,0x01,0x11,
	0x01,0x11,0x00,0x11,0x10,0x11,0x10,0x01,
	0x11,0x01,0x11,0x00,0x11,0x10,0x11,0x10,
	0x01,0x11,0x00,0x09,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x11,0x00,
	0x00,0x11,0x01,0x10,0x00,0x01,0x10,0x11,
	0x00,0x00,0x11,0x01,0x10,0x00,0x01,0x10,
	0x11,0x00,0x00,0x11,0x00,0x09,0x00,0x70,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x11,0x00,0x00,0x11,0x01,0x10,0x00,0x01,
	0x10,0x11,0x00,0x00,0x11,0x01,0x10,0x00,
	0x01,0x10,0x11,0x00,0x00,0x11,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x11,0x00,0x00,0x11,0x01,0x10,
	0x00,0x01,0x10,0x11,0x00,0x00,0x11,0x01,
	0x10,0x00,0x01,0x10,0x11,0x00,0x00,0x11,
	0x00,0x09,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x01,0x10,0x01,0x10,
	0x00,0x11,0x00,0x11,0x00,0x01,0x10,0x01,
	0x10,0x00,0x11,0x00,0x11,0x00,0x01,0x10,
	0x01,0x10,0x00,0x09,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x11,
	0x11,0x00,0x00,0x01,0x11,0x10,0x00,0x00,
	0x11,0x11,0x00,0x00,0x01,0x11,0x10,0x00,
	0x00,0x11,0x11,0x00,0x00,0x09,0x00,0x70,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x11,0x11,0x00,0x00,0x01,0x11,0x10,
	0x00,0x00,0x11,0x11,0x00,0x00,0x01,0x11,
	0x10,0x00,0x00,0x11,0x11,0x00,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x01,0x10,0x01,0x10,0x00,0x11,
	0x00,0x11,0x00,0x01,0x10,0x01,0x10,0x00,
	0x11,0x00,0x11,0x00,0x01,0x10,0x01,0x10,
	0x00,0x09,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x11,0x00,0x00,0x11,
	0x01,0x10,0x00,0x01,0x10,0x11,0x00,0x00,
	0x11,0x01,0x10,0x00,0x01,0x10,0x11,0x00,
	0x00,0x11,0x00,0x09,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x11,0x00,
	0x00,0x11,0x01,0x10,0x00,0x01,0x10,0x11,
	0x00,0x00,0x11,0x01,0x10,0x00,0x01,0x10,
	0x11,0x00,0x00,0x11,0x00,0x09,0x00,0x70,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x11,0x00,0x00,0x11,0x01,0x10,0x00,0x01,
	0x10,0x11,0x00,0x00,0x11,0x01,0x10,0x00,
	0x01,0x10,0x11,0x00,0x00,0x11,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x11,0x10,0x01,0x11,0x01,0x11,
	0x00,0x11,0x10,0x11,0x10,0x01,0x11,0x01,
	0x11,0x00,0x11,0x10,0x11,0x10,0x01,0x11,
	0x00,0x09,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x01,0x11,0x11,0x10,
	0x00,0x11,0x11,0x11,0x00,0x01,0x11,0x11,
	0x10,0x00,0x11,0x11,0x11,0x00,0x01,0x11,
	0x11,0x10,0x00,0x09,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x11,
	0x11,0x00,0x00,0x01,0x11,0x10,0x00,0x00,
	0x11,0x11,0x00,0x00,0x01,0x11,0x10,0x00,
	0x00,0x11,0x11,0x00,0x00,0x09,0x00,0x70,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x09,0x00,0x70,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x33,0x33,0x00,
	0x00,0x03,0x33,0x30,0x00,0x00,0x33,0x33,
	0x00,0x00,0x03,0x33,0x30,0x00,0x00,0x33,
	0x33,0x00,0x00,0x09,0x00,0x70,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x03,0x33,
	0x33,0x30,0x00,0x33,0x33,0x33,0x00,0x03,
	0x33,0x33,0x30,0x00,0x33,0x33,0x33,0x00,
	0x03,0x33,0x33,0x30,0x00,0x09,0x00,0x70,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x33,0x30,0x03,0x33,0x03,0x33,0x00,0x33,
	0x30,0x33,0x30,0x03,0x33,0x03,0x33,0x00,
	0x33,0x30,0x33,0x30,0x03,0x33,0x00,0x09,
	0x00,0x70,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x33,0x00,0x00,0x33,0x03,0x30,
	0x00,0x03,0x30,0x33,0x00,0x00,0x33,0x03,
	0x30,0x00,0x03,0x30,0x33,0x00,0x00,0x33,
	0x00,0x09,0x00,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x33,0x00,0x00,0x33,
	0x03,0x30,0x00,0x03,0x30,0x33,0x00,0x00,
	0x33,0x03,0x30,0x00,0x03,0x30,0x33,0x00,
	0x00,0x33,0x00,0x09,0x00,0x77,0x77,0x44,
	0x44,0x77,0x44,0x44,0x77,0x77,0x77,0x77,
	0x44,0x44,0x77,0x44,0x44,0x77,0x77,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x33,0x00,
	0x00,0x33,0x03,0x30,0x00,0x03,0x30,0x33,
	0x00,0x00,0x33,0x03,0x30,0x00,0x03,0x30,
	0x33,0x00,0x00,0x33,0x00,0x09,0x00,0x77,
	0x77,0x74,0x04,0x74,0x44,0x47,0x77,0x44,
	0x44,0x77,0x74,0x44,0x47,0x40,0x47,0x77,
	0x77,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x03,0x30,0x03,0x30,0x00,0x33,0x00,0x33,
	0x00,0x03,0x30,0x03,0x30,0x00,0x33,0x00,
	0x33,0x00,0x03,0x30,0x03,0x30,0x00,0x09,
	0x00,0x77,0x77,0x77,0x44,0x74,0x04,0x47,
	0x74,0x40,0x04,0x47,0x74,0x40,0x47,0x44,
	0x77,0x77,0x77,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x33,0x33,0x00,0x00,0x03,
	0x33,0x30,0x00,0x00,0x33,0x33,0x00,0x00,
	0x03,0x33,0x30,0x00,0x00,0x33,0x33,0x00,
	0x00,0x09,0x00,0x77,0x77,0x77,0x74,0x74,
	0x04,0x77,0x44,0x44,0x44,0x44,0x77,0x40,
	0x47,0x47,0x77,0x77,0x77,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x33,0x33,0x00,
	0x00,0x03,0x33,0x30,0x00,0x00,0x33,0x33,
	0x00,0x00,0x03,0x33,0x30,0x00,0x00,0x33,
	0x33,0x00,0x00,0x09,0x00,0x77,0x77,0x77,
	0x77,0x74,0x44,0x77,0x77,0x77,0x77,0x77,
	0x77,0x44,0x47,0x77,0x77,0x77,0x77,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x03,0x30,
	0x03,0x30,0x00,0x33,0x00,0x33,0x00,0x03,
	0x30,0x03,0x30,0x00,0x33,0x00,0x33,0x00,
	0x03,0x30,0x03,0x30,0x00,0x09,0x00,0x77,
	0x77,0x77,0x77,0x77,0x44,0x77,0x44,0x44,
	0x44,0x44,0x77,0x44,0x77,0x77,0x77,0x77,
	0x77,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x33,0x00,0x00,0x33,0x03,0x30,0x00,0x03,
	0x30,0x33,0x00,0x00,0x33,0x03,0x30,0x00,
	0x03,0x30,0x33,0x00,0x00,0x33,0x00,0x09,
	0x00,0x77,0x77,0x77,0x77,0x77,0x74,0x47,
	0x74,0x40,0x04,0x47,0x74,0x47,0x77,0x77,
	0x77,0x77,0x77,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x33,0x00,0x00,0x33,0x03,0x30,
	0x00,0x03,0x30,0x33,0x00,0x00,0x33,0x03,
	0x30,0x00,0x03,0x30,0x33,0x00,0x00,0x33,
	0x00,0x09,0x00,0x00,0x00,0x00,0x00,0x00,
	0x77,0x77,0x77,0x44,0x44,0x77,0x77,0x77,
	0x00,0x00,0x00,0x00,0x00,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x33,0x00,0x00,0x33,
	0x03,0x30,0x00,0x03,0x30,0x33,0x00,0x00,
	0x33,0x03,0x30,0x00,0x03,0x30,0x33,0x00,
	0x00,0x33,0x00,0x09,0x00,0x77,0x77,0x77,
	0x77,0x77,0x00,0x77,0x77,0x77,0x77,0x77,
	0x77,0x00,0x77,0x77,0x77,0x77,0x77,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x33,0x30,
	0x03,0x33,0x03,0x33,0x00,0x33,0x30,0x33,
	0x30,0x03,0x33,0x03,0x33,0x00,0x33,0x30,
	0x33,0x30,0x03,0x33,0x00,0x09,0x00,0x77,
	0x77,0x77,0x77,0x77,0x77,0x00,0x07,0x77,
	0x77,0x70,0x00,0x77,0x77,0x77,0x77,0x77,
	0x77,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x03,0x33,0x33,0x30,0x00,0x33,0x33,0x33,
	0x00,0x03,0x33,0x33,0x30,0x00,0x33,0x33,
	0x33,0x00,0x03,0x33,0x33,0x30,0x00,0x09,
	0x00,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x70,0x00,0x00,0x07,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x33,0x33,0x00,0x00,0x03,
	0x33,0x30,0x00,0x00,0x33,0x33,0x00,0x00,
	0x03,0x33,0x30,0x00,0x00,0x33,0x33,0x00,
	0x00,0x09,0x00,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x09,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
	0x99,0x99,0x99,0x99,0x99,0x99,0x99,0x99,
};

const unsigned char display2[DISPLAY_STAGE2_WIDTH*DISPLAY_STAGE2_HEIGHT/2] = 
{
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x00,0x99,0x99,0x99,0x99,0x90,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x11,0x00,0x01,0x11,0x00,
	0x01,0x11,0x00,0x01,0x11,0x00,0x01,0x11,
	0x00,0x07,0x00,0x99,0x99,0x99,0x99,0x90,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
	0x00,0x10,0x10,0x00,0x10,0x10,0x00,0x10,
	0x10,0x00,0x10,0x10,0x00,0x10,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x10,0x00,0x10,0x10,
	0x00,0x10,0x10,0x00,0x10,0x10,0x00,0x10,
	0x10,0x00,0x10,0x07,0x00,0x99,0x99,0x99,
	0x99,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x10,0x00,0x10,0x10,0x00,0x10,0x10,
	0x00,0x10,0x10,0x00,0x10,0x10,0x00,0x10,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x11,
	0x00,0x01,0x11,0x00,0x01,0x11,0x00,0x01,
	0x11,0x00,0x01,0x11,0x00,0x07,0x00,0x99,
	0x99,0x99,0x99,0x90,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x10,0x00,0x10,0x10,0x00,
	0x10,0x10,0x00,0x10,0x10,0x00,0x10,0x10,
	0x00,0x10,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x10,0x00,0x10,0x10,0x00,0x10,0x10,0x00,
	0x10,0x10,0x00,0x10,0x10,0x00,0x10,0x07,
	0x00,0x99,0x99,0x99,0x99,0x90,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x10,
	0x10,0x00,0x10,0x10,0x00,0x10,0x10,0x00,
	0x10,0x10,0x00,0x10,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x11,0x00,0x01,0x11,0x00,
	0x01,0x11,0x00,0x01,0x11,0x00,0x01,0x11,
	0x00,0x07,0x00,0x99,0x99,0x99,0x99,0x90,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0x33,0x00,0x03,
	0x33,0x00,0x03,0x33,0x00,0x03,0x33,0x00,
	0x03,0x33,0x00,0x07,0x00,0x99,0x99,0x99,
	0x99,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x30,0x00,0x30,0x30,0x00,0x30,0x30,
	0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x30,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,
	0x30,0x30,0x00,0x30,0x30,0x00,0x30,0x30,
	0x00,0x30,0x30,0x00,0x30,0x07,0x00,0x99,
	0x99,0x99,0x99,0x90,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x30,0x00,0x30,0x30,0x00,
	0x30,0x30,0x00,0x30,0x30,0x00,0x30,0x30,
	0x00,0x30,0x07,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x03,0x33,0x00,0x03,0x33,0x00,0x03,0x33,
	0x00,0x03,0x33,0x00,0x03,0x33,0x00,0x07,
	0x00,0x99,0x99,0x99,0x99,0x90,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x30,
	0x30,0x00,0x30,0x30,0x00,0x30,0x30,0x00,
	0x30,0x30,0x00,0x30,0x07,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x30,0x00,0x30,0x30,0x00,0x30,
	0x30,0x00,0x30,0x30,0x00,0x30,0x30,0x00,
	0x30,0x07,0x00,0x99,0x99,0x99,0x99,0x90,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,
	0x00,0x30,0x30,0x00,0x30,0x30,0x00,0x30,
	0x30,0x00,0x30,0x30,0x00,0x30,0x07,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x03,0x33,0x00,0x03,
	0x33,0x00,0x03,0x33,0x00,0x03,0x33,0x00,
	0x03,0x33,0x00,0x07,0x00,0x99,0x99,0x99,
	0x99,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x07,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x77,0x00,0x99,
	0x99,0x99,0x99,0x90,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x44,0x44,0x77,0x44,0x44,
	0x77,0x77,0x77,0x77,0x44,0x44,0x77,0x44,
	0x44,0x77,0x77,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x74,0x04,0x74,0x44,0x47,0x77,0x44,0x44,
	0x77,0x74,0x44,0x47,0x40,0x47,0x77,0x77,
	0x00,0x99,0x99,0x99,0x99,0x90,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x77,0x44,0x74,
	0x04,0x47,0x74,0x40,0x04,0x47,0x74,0x40,
	0x47,0x44,0x77,0x77,0x77,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x77,0x74,0x74,0x04,0x77,0x44,
	0x44,0x44,0x44,0x77,0x40,0x47,0x47,0x77,
	0x77,0x77,0x00,0x99,0x99,0x99,0x99,0x90,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,
	0x77,0x74,0x44,0x77,0x77,0x77,0x77,0x77,
	0x77,0x44,0x47,0x77,0x77,0x77,0x77,0x00,
	0x99,0x99,0x99,0x99,0x90,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x77,0x77,0x77,0x44,
	0x77,0x44,0x44,0x44,0x44,0x77,0x44,0x77,
	0x77,0x77,0x77,0x77,0x00,0x99,0x99,0x99,
	0x99,0x90,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x77,0x77,0x77,0x74,0x47,0x74,0x40,
	0x04,0x47,0x74,0x47,0x77,0x77,0x77,0x77,
	0x77,0x00,0x99,0x99,0x99,0x99,0x90,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x77,0x77,0x77,0x44,0x44,0x77,0x77,
	0x77,0x00,0x00,0x00,0x00,0x00,0x00,0x99,
	0x99,0x99,0x99,0x90,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x77,0x77,0x77,0x00,0x77,
	0x77,0x77,0x77,0x77,0x77,0x00,0x77,0x77,
	0x77,0x77,0x77,0x00,0x99,0x99,0x99,0x99,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x77,0x77,0x77,0x77,0x00,0x07,0x77,0x77,
	0x70,0x00,0x77,0x77,0x77,0x77,0x77,0x77,
	0x00,0x99,0x99,0x99,0x99,0x90,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x77,0x77,0x77,
	0x77,0x77,0x70,0x00,0x00,0x07,0x77,0x77,
	0x77,0x77,0x77,0x77,0x77,0x00,0x99,0x99,
	0x99,0x99,0x90,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,
};

ICON_INFO display1_info = 
{
    display1,
    0,
    0,
    ANIMATION_ICON_WIDTH,
    ANIMATION_ICON_HEIGHT
};
ICON_INFO display2_info = 
{
    display2,
    DISPLAY_STAGE2_START_X,
    DISPLAY_STAGE2_START_Y,
    DISPLAY_STAGE2_WIDTH,
    DISPLAY_STAGE2_HEIGHT
};
