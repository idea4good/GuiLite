#ifndef LABEL_H
#define LABEL_H

class c_label : public c_wnd
{
public:
	virtual c_wnd* clone(){return new c_label();}
	virtual void on_paint();
protected:
	virtual void pre_create_wnd();
private:
};

#endif
