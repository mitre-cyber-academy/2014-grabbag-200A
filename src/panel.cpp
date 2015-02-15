#include "panel.h"

panel::panel(bool a)
{
	if(a)
	{
		top = 0;
		systems[0] = 1;
		systems[1] = 1;
		systems[2] = 1;
		systems[3] = 1;
		systems[4] = 1;
	}
	else
	{
		top = -1;
		systems[0] = 0;
		systems[1] = 0;
		systems[2] = 0;
		systems[3] = 0;
		systems[4] = 0;
	}
}

void panel::setSys1(void)
{
	systems[0] = 1;
}

void panel::setSys2(void)
{
	systems[1] = 1;
}

void panel::setSys3(void)
{
	systems[2] = 1;
}

void panel::setSys4(void)
{
	systems[3] = 1;
}

void panel::setSys5(void)
{
	systems[4] = 1;
}

void panel::disSys1(void)
{
	systems[0] = 0;
}

void panel::disSys2(void)
{
	systems[1] = 0;
}

void panel::disSys3(void)
{
	systems[2] = 0;
}

void panel::disSys4(void)
{
	systems[3] = 0;
}

void panel::disSys5(void)
{
	systems[4] = 0;
}

bool panel::getSys1(void)
{
	return systems[0];
}

bool panel::getSys2(void)
{
	return systems[1];
}

bool panel::getSys3(void)
{
	return systems[2];
}

bool panel::getSys4(void)
{
	return systems[3];
}

bool panel::getSys5(void)
{
	return systems[4];
}

int panel::getTop(void)
{
	return top;
}

void panel::getNewTop(void)
{
	if(systems[0] == 1)
	{
		top = 0;
	}
	else if(systems[1] == 1)
	{
		top = 1;
	}
	else if(systems[2] == 1)
	{
		top = 2;
	}
	else if(systems[3] == 1)
	{
		top = 3;
	}
	else if(systems[4] == 1)
	{
		top = 4;
	}
	else
	{
		top = -1;
	}
}