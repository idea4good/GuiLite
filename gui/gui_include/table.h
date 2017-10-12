#ifndef _TABLE_H_
#define  _TABLE_H_

#define  MAX_COL_NUM  30
#define  MAX_ROW_NUM  30

class c_table: public c_wnd
{
public:
	virtual c_wnd* clone(){return new c_table();}
	void set_sheet_align(unsigned int align_type){ m_align_type = align_type;}
	void set_row_num(unsigned int row_num){ m_row_num = row_num;}
	void set_col_num(unsigned int col_num){ m_col_num = col_num;}
	void set_row_height(unsigned int height);
	void set_col_width(unsigned int width);
	int set_row_height(unsigned int index, unsigned int height);
	int set_col_width(unsigned int index, unsigned int width);

	int set_item(int row, int col, char* str, unsigned int with_bg_color = -1);
	int set_item(int row, int col, unsigned int str_id, unsigned int with_bg_color = -1);

	unsigned int get_row_num(){ return m_row_num;}
	unsigned int get_col_num(){ return m_col_num;}
	c_rect get_item_rect(int row, int col);
protected:
	virtual void pre_create_wnd();

	void draw_item(int col, int row, const char* str, unsigned int with_bg_color = -1);
	void draw_item(int col, int row, unsigned int str_id, unsigned int with_bg_color = -1);

	unsigned int m_align_type;	
	unsigned int m_row_num;
	unsigned int m_col_num;
	unsigned int m_row_height[MAX_ROW_NUM];
	unsigned int m_col_width[MAX_COL_NUM];
};
#endif
