#ifndef PANEL_H
#define PANEL_H

class panel{
public:
	bool systems[5];
	int top;
	panel(bool a);
	void setSys1(void);
	void setSys2(void);
	void setSys3(void);
	void setSys4(void);
	void setSys5(void);
	void disSys1(void);
	void disSys2(void);
	void disSys3(void);
	void disSys4(void);
	void disSys5(void);
	bool getSys1(void);
	bool getSys2(void);
	bool getSys3(void);
	bool getSys4(void);
	bool getSys5(void);
	bool getSys(char a);
	int getTop(void);
	void getNewTop(void);
};

#endif