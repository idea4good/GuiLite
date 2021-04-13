#include "../core_include/word.h"

#ifdef GUILITE_ON

c_lattice_font_op the_lattice_font_op = c_lattice_font_op();
c_font_operator* c_word::fontOperator = &the_lattice_font_op;

#endif
