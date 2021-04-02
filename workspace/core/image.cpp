#include "../core_include/image.h"

#ifdef GUILITE_ON

c_bitmap_operator the_bitmap_op = c_bitmap_operator();
c_image_operator* c_image::image_operator = &the_bitmap_op;

#endif
