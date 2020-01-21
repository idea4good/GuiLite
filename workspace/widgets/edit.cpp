#include "../widgets_include/edit.h"

#ifdef GUILITE_ON

c_keyboard  c_edit::s_keyboard;

GL_BEGIN_MESSAGE_MAP(c_edit)
ON_KEYBORAD_UPDATE(c_edit::on_key_board_click)
GL_END_MESSAGE_MAP()

#endif
