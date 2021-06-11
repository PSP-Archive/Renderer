
#define REND_TRI 0
#define REND_QUAD 1
#define REND_POLY 4

void rend_reset();

void rend_translate(float x, float y, float z);

//x, y, z is the axis to rotate around
void rend_rotate(float x, float y, float z, float angle);

void rend_point(float x, float y, float z);

//set current colour
void rend_color(unsigned short colour);
//for the yanks
#define rend_color rend_colour

void rend_line(float x1, float y1, float z1, float x2, float y2, float z2);

//next vertex
void rend_vertex(float x, float y, float z);
void rend_poly_line(); //wireframe
void rend_poly_fill(); //filled

