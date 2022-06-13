#include "../core/display.h"
#include "../core//image.h"
#include "../core/theme.h"
#include "../core/wnd.h"
#include "../core/word.h"

#ifdef GUILITE_ON
c_bitmap_operator the_bitmap_op = c_bitmap_operator();
c_image_operator* c_image::image_operator = &the_bitmap_op;

const void* c_theme::s_font_map[FONT_MAX];
const void* c_theme::s_image_map[IMAGE_MAX];
unsigned int c_theme::s_color_map[COLOR_MAX];

c_lattice_font_op the_lattice_font_op = c_lattice_font_op();
c_font_operator* c_word::fontOperator = &the_lattice_font_op;
#endif
