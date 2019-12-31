#ifndef GUILITE_WIDGETS_INCLUDE_LABEL_H
#define GUILITE_WIDGETS_INCLUDE_LABEL_H

class c_label : public c_wnd
{
public:
	virtual void on_paint();
protected:
	virtual void pre_create_wnd();
};

#endif
