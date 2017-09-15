#ifndef DB_TREND_H_
#define DB_TREND_H_

typedef struct
{//member's order must be equal with database table colmn's
	long  time;
	short hr;
	short spo2;
	short rr;
	short pr;
	short temp;
	short nibp_sys;
	short nibp_dia;
	short nibp_mean;
}VALUE_SET;

typedef struct
{//member's order must be equal with database table colmn's
	char              id;
	short             low_limit[3];
	short             high_limit[3];
	unsigned short    low_alarm_id;
	unsigned short    high_alarm_id;
	char              digit;
}LIMIT_CONFIG_ITEM;

typedef struct
{//member's order must be equal with database table colmn's
	int             id;
	int             data;
}CONFIG_ITEM;

struct sqlite3;
class c_database
{
public:
	int init();
	int init_limit_cfg(LIMIT_CONFIG_ITEM* cfg, int item_cnt);
	int init_sys_settings(CONFIG_ITEM* sys_settings, int item_cnt);
	int read(long start_time, long end_time, VALUE_SET* data_set, int len);
	int write(VALUE_SET &data_set);

	bool read(int limit_cfg_id, LIMIT_CONFIG_ITEM* data_set);
	int modify(int limit_cfg_id, LIMIT_CONFIG_ITEM* data_set);

	bool read(int sys_settings_id, CONFIG_ITEM* data_set);
	int modify(int sys_settings_id, CONFIG_ITEM* data_set);

	int display_all();
	static c_database* get_instance()
	{
		static c_database s_the_db;
		return &s_the_db;
	}
private:
	sqlite3 *m_db_monitor;
	static int	m_real_data_rd_cnt;
	static int	m_real_data_rd_len;
	static bool m_rd_cfg_ok;
	static int 	sql_callback_read_real_data(void *arg, int nr, char **values, char **names);
	static int 	sql_callback_read_limit_cfg(void *arg, int nr, char **values, char **names);
	static int 	sql_callback_read_sys_settings(void *arg, int nr, char **values, char **names);
	c_database();
	c_database(const c_database&);
	c_database& operator=(const c_database&);
	~c_database();
};

#endif
