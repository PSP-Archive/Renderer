#include "raster.h"
#include "render.h"
#include "control.h"

void sceKernelExitGame();

//each pixel == 2 bytes
//each line = 1024 bytes
//each frame = 0x44000 bytes

int exit_callback(void)
{
	// Exit game
	sceKernelExitGame();
	return 0;
}

void power_callback(int unknown, int pwrflags)
{
	   // Combine pwrflags and the above defined masks
} 

int CallBackThread(void *arg) {
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback", exit_callback);
	sceKernelRegisterExitCallback(cbid);
	KernelPollCallbacks();

	return 0;
}

int tri[3][3] = {{-1, 0, 0}, {1, 0, 0}, {0,1,0}};
int angle = 0;

int xmain() {
	int i, j;
	char *blah;

	int x = 0, y = 0, z = 0;
	ctrl_data_t ctl;

	//install callback thread
	i = sceKernelCreateThread("update_thread", CallBackThread,
			0x11, 0xFa0, 0, 0);
	sceKernelStartThread(i, 0, 0);
	
	rast_init();
	
	while(1) {

		sceCtrlRead(&ctl, 1);
		if (ctl.buttons & 0x10)
			y++;
		if (ctl.buttons & 0x40)
			y--;
		if (ctl.buttons & 0x80)
			x--;
		if (ctl.buttons & 0x20)
			x++;
		if (ctl.buttons & 0x1000) //triangle
			z++;
		if (ctl.buttons & 0x4000) // X
			z--;
		if (ctl.buttons & 0x01) //select
			sceKernelExitGame();
		if (ctl.buttons & 0x200) //rtrigger
			angle++;
		if (ctl.buttons & 0x100) //ltrigger
			angle--;

		rast_fill(0);
		rast_line(0,0,480,0,0xf000);
		rend_reset();
		rend_rotate(1, angle);

		rend_translate(0, 0, -5);

		rend_translate(x / 3.0f, y / 3.0f, z / 3.0f);
		
		rend_colour(0xffff);
		for (i = 0;i < 3;i++)
			rend_vertex(tri[i][0], tri[i][1], tri[i][2]);
		rend_poly_fill();

		rast_flip();
	}
	return 0;

}


