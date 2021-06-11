
/* Index for the two analog directions */
#define CTRL_ANALOG_X   0
#define CTRL_ANALOG_Y   1

/* Button bit masks */
#define CTRL_SQUARE      0x8000
#define CTRL_TRIANGLE   0x1000
#define CTRL_CIRCLE      0x2000
#define CTRL_CROSS      0x4000
#define CTRL_UP         0x0010
#define CTRL_DOWN      0x0040
#define CTRL_LEFT      0x0080
#define CTRL_RIGHT      0x0020
#define CTRL_START      0x0008
#define CTRL_SELECT      0x0001
#define CTRL_LTRIGGER   0x0100
#define CTRL_RTRIGGER   0x0200

typedef struct _ctrl_data
{
	unsigned long frame;                    // ~N~^~JԨtick)
	unsigned long buttons;
	unsigned char  analog[4];            // [0]:X [1]:Y
	unsigned long unused;
} ctrl_data_t;

void  sceCtrlInit(int unknown);
void  sceCtrlSetAnalogMode(int on);
void  sceCtrlRead(ctrl_data_t* paddata, int unknown);

