
void pspDisplayWaitVblankStart();
void pspDisplaySetMode(long,long,long);
void pspDisplaySetFrameBuf(char *topaddr,long linesize,long pixelsize,long);
/* pixelsize:
 * 0 = 565
 * 1 = 5551
 * 2 = 4444
 * 3 = 8888
 */

#define LINELENGTH 1024
#define PIXELSIZE 2
#define FRAMESIZE 0x44000

// x * 512 == x << 9
// #define PIXEL(screen, x, y, col) frame[screen][((y) << 9) + (x)] = col
#define PIXEL(screen, x, y, col) frame[screen][((y) << 9) + (x)] = col

void rast_init();

// flip buffer over
void rast_flip();

//fill buffer with one colour
void rast_fill(unsigned short col);
void rast_fill_both(unsigned short col);
void rast_blank_z();

//draw a pixel to the current buffer
inline void rast_pixel(int x, int y, unsigned short col);

//blit
void rast_blit(int dx, int dy, unsigned short *src, int sw, int sh);

//text
void rast_drawtext(int x, int y, unsigned short colour, char *text);

//line
void rast_line(int sx, int sy, int dx, int dy, unsigned short col);

//polygon
void rast_vertex(int x, int y);
void rast_coord(int x, int y, float tcx, float tcy, float zc);
void rast_poly_line(unsigned short col); //wireframe
void rast_poly_fill(unsigned short col); //filled
void rast_poly_textured();

