
#define REND_TRI 0
#define REND_QUAD 1
#define REND_POLY 4

void rend_reset();

int rend_pushmatrix();
int rend_popmatrix();

void rend_translate(float x, float y, float z);

//x, y, z is the axis to rotate around
void rend_rotate(float x, float y, float z, float angle);

//set current colour
void rend_colour(unsigned short colour);
//for the yanks
#define rend_color rend_colour

void rend_texture(unsigned short *tex, int texturew, int textureh);
unsigned short rend_texture_point(float tcx, float tcy);

void rend_point(float x, float y, float z);

void rend_line(float x1, float y1, float z1, float x2, float y2, float z2);

//next vertex
void rend_vertex(float x, float y, float z);
void rend_vertex_tc(float x, float y, float z, float tx, float ty);
void rend_poly_line(); //wireframe
void rend_poly_fill(); //filled
void rend_poly_textured();

