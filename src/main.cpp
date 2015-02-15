#include <iostream>
#include <cctype>
#include <climits>
#include <ctime>
#include <cstdlib>
#include "panel.h"

using namespace std;

void generateInterface();
void showDirections();
void move(panel *a, panel *b, panel *c);
void viewStatus(panel a, panel b, panel c);
void unlock(panel c);
void reset(panel *a, panel *b, panel *c);
void crash();

char resistor[] = {'/','*', '(', '}'};
char transistor[] = {'-', '{' , '/', '}'};
char capacitor[] = {'}', '|', '|', '*'};
char voltmeter[] = {'z', ',', ')', 'z'};
char pmos[] = {'z', ')', ')', '+'};
char nmos[] = {'(',',', '}', '+'};
char cmos[] = {',', ')', '!', '!'};
char metal1[] = {'~', 'y', 'z', 'y'};
char metal2[] = {'-', 'y', '.', '-'};
char metal3[] = {',', '*', '+', '.'};

// 720e5c7eedd2b41bb11304e34199faba5a654236
// /*(}-{/}}||*z,)zz))+(,}+,)!!~yzy-y.-,*+. Encrypted XOR 18


int main()
{
	panel top = panel(1);
	panel mid = panel(0);
	panel bottom = panel(0);


	char choice = '\0';

	while(1)
	{
		generateInterface();

		cout << "Enter input: " << endl;
		cin >> choice;
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		choice = toupper(choice);

		switch(choice)
		{
			case 'D':
				showDirections();
				break;
			case 'R':
				move(&top, &mid, &bottom);
				break;
			case 'S':
				viewStatus(top, mid, bottom);
				break;
			case 'U':
				unlock(bottom);
				break;
			case 'X':
				reset(&top, &mid, &bottom);
				break;
			case 'Q':
				return(0);
				break;
			default:
				cout << "Invalid input!" << endl;
				break;
		}
	}

	return(0); //Not needed, but used to make compiler be quiet
}

void generateInterface()
{
	cout << "***** COMMANDS *****" << endl;
	cout << "View directions - D" << endl;
	cout << "Redirect - R" << endl;
	cout << "View status - S" << endl;
	cout << "Unlock - U" << endl;
	cout << "Reset - X" << endl;
	cout << "Quit - Q" << endl;
	cout << "********************" << endl;
}

void showDirections()
{
	cout << "\n\n\n\n\n";
	cout << "*************************************************************** DIRECTIONS ******************************************************************************\n" << endl;
	cout << "The are three panels that contain five systems each. The systems are numbered based on their level of power consumption." << endl;
	cout << "The systems of power range from 1 (being the lowest) to 5 (being the highest)." << endl;
	cout << endl;
	cout << "Goal: Activate the power of all 5 systems of the bottom pane by redirecting power among the three panels." << endl;
	cout << endl;
	cout << "Restrictions:" << endl;
	cout << endl;
	cout << "\t- You can only redirect one system at a time." << endl;
	cout << "\t- You can only redirect the lowest power system on a panel to another panel." << endl;
	cout << "\t(Example: If the lowest system activated on a panel is 2, you must move the power of system 2 before the higher systems like 3, 4, or 5)" << endl;
	cout << endl;
	cout << "\t- You can only redirect power to a panel if it is of a lesser power than the systems already stored in the panel." << endl;
	cout << "\t(Example: You can redirect power of magnitude 1 or 2 to a panel that has power 3 activated, but you cannot redirect the power of system 4 or 5)" << endl;
	cout << "\nGood luck!\n" << endl;
	cout << "*********************************************************************************************************************************************************" << endl;
	cout << "\n\n\n";
}

void move(panel *a, panel *b, panel *c)
{
	char panFrom = '\0';
	char panTo = '\0';
	bool isGood = 0;

	cout << "\n\n\n\n\n";

	cout << "Select a panel to move power from [(T)op/(M)iddle/(B)ottom/(C)ancel]: ";

	while(!isGood)
	{
		cin >> panFrom;
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		panFrom = toupper(panFrom);

		if((panFrom == 'T' && a->getTop() >= 0) || (panFrom == 'M' && b->getTop() >= 0) || (panFrom == 'B' && c->getTop() >= 0))
		{
			isGood = 1;
		}
		else if(panFrom == 'C')
		{
			cout << "\n\n\n\n" << endl;
			return;
		}
		else
		{
			cout << "Invalid input! Panel has no power or improper choice!" << endl;
			cout << "Select a panel to move power from [(T)op/(M)iddle/(B)ottom/(C)ancel]: ";
		}
	}

	isGood = 0;

	cout << "Select a panel to move the system ";

	switch(panFrom)
	{
		case 'T':
			cout << a->getTop() + 1 << " to [(M)iddle/(B)ottom/(C)ancel]: ";
			break;
		case 'M':
			cout << b->getTop() + 1 << " to [(T)op/(B)ottom/(C)ancel]: ";
			break;
		case 'B':
			cout << c->getTop() + 1 << " to [(T)op/(M)iddle/(C)ancel]: ";
			break;
	}

	while(!isGood)
	{
		cin >> panTo;
		cin.clear();
		cin.ignore(INT_MAX, '\n');

		panTo = toupper(panTo);

		if((panTo == 'T' || panTo == 'M' || panTo == 'B') && (panTo != panFrom))
		{
			isGood = 1;
		}
		else if(panTo == 'C')
		{
			cout << "\n\n\n\n" << endl;
			return;
		}
		else
		{
			cout << "Invalid input!" << endl;

			cout << "Select a panel to move system ";

			switch(panFrom)
			{
				case 'T':
					cout << a->systems[a->getTop()] << " to [(M)iddle/(B)ottom/(C)ancel]: ";
					break;
				case 'M':
					cout << b->systems[b->getTop()] << " to [(T)op/(B)ottom/(C)ancel]: ";
					break;
				case 'B':
					cout << c->systems[c->getTop()] << " to [(T)op/(M)iddle/(C)ancel]: ";
					break;
			}
		}
	}

	if(panFrom == 'T') //Transferring from top panel
	{
		if(a->getTop() == 0) //System 1
		{
			if(panTo == 'M')
			{
				b->setSys1();
				a->disSys1();
				a->getNewTop();
				b->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}

			if(panTo == 'B')
			{
				c->setSys1();
				a->disSys1();
				a->getNewTop();
				c->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}
		}

		if(a->getTop() == 1) //System 2
		{
			if(panTo == 'M')
			{
				if(!b->getSys1())
				{
					b->setSys2();
					a->disSys2();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1())
				{
					c->setSys2();
					a->disSys2();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(a->getTop() == 2) //System 3
		{
			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2())
				{
					b->setSys3();
					a->disSys3();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2())
				{
					c->setSys3();
					a->disSys3();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}
		}

		if(a->getTop() == 3) //System 4
		{
			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2() && !b->getSys3())
				{
					b->setSys4();
					a->disSys4();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2() && !c->getSys3())
				{
					c->setSys4();
					a->disSys4();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(a->getTop() == 4) //System 5
		{
			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2() && !b->getSys3() && !b->getSys4())
				{
					b->setSys5();
					a->disSys5();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2() && !c->getSys3() && !c->getSys4())
				{
					c->setSys5();
					a->disSys5();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}
	}

	if(panFrom == 'M') //Transferring from middle panel
	{
		if(b->getTop() == 0) //System 1
		{
			if(panTo == 'T')
			{
				a->setSys1();
				b->disSys1();
				a->getNewTop();
				b->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}

			if(panTo == 'B')
			{
				c->setSys1();
				b->disSys1();
				b->getNewTop();
				c->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}
		}

		if(b->getTop() == 1) //System 2
		{
			if(panTo == 'T')
			{
				if(!a->getSys1())
				{
					a->setSys2();
					b->disSys2();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1())
				{
					c->setSys2();
					b->disSys2();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(b->getTop() == 2) //System 3
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2())
				{
					a->setSys3();
					b->disSys3();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2())
				{
					c->setSys3();
					b->disSys3();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}
		}

		if(b->getTop() == 3) //System 4
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2() && !a->getSys3())
				{
					a->setSys4();
					b->disSys4();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2() && !c->getSys3())
				{
					c->setSys4();
					b->disSys4();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(b->getTop() == 4) //System 5
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2() && !a->getSys3() && !a->getSys4())
				{
					a->setSys5();
					b->disSys5();
					a->getNewTop();
					b->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'B')
			{
				if(!c->getSys1() && !c->getSys2() && !c->getSys3() && !c->getSys4())
				{
					c->setSys5();
					b->disSys5();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}
	}

	if(panFrom == 'B') //Transferring from bottom panel
	{
		if(c->getTop() == 0) //System 1
		{
			if(panTo == 'T')
			{
				a->setSys1();
				c->disSys1();
				a->getNewTop();
				c->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}

			if(panTo == 'M')
			{
				b->setSys1();
				c->disSys1();
				b->getNewTop();
				c->getNewTop();

				viewStatus(*a, *b, *c);
				cout << "Transfer successful. All systems normal.";
				cout << "\n\n\n\n\n";
				return;
			}
		}

		if(c->getTop() == 1) //System 2
		{
			if(panTo == 'T')
			{
				if(!a->getSys1())
				{
					a->setSys2();
					c->disSys2();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'M')
			{
				if(!b->getSys1())
				{
					b->setSys2();
					c->disSys2();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(c->getTop() == 2) //System 3
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2())
				{
					a->setSys3();
					c->disSys3();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}

			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2())
				{
					b->setSys3();
					c->disSys3();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				} 
			}
		}

		if(c->getTop() == 3) //System 4
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2() && !a->getSys3())
				{
					a->setSys4();
					c->disSys4();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2() && !b->getSys3())
				{
					b->setSys4();
					c->disSys4();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}

		if(c->getTop() == 4) //System 5
		{
			if(panTo == 'T')
			{
				if(!a->getSys1() && !a->getSys2() && !a->getSys3() && !a->getSys4())
				{
					a->setSys5();
					c->disSys5();
					a->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}

			if(panTo == 'M')
			{
				if(!b->getSys1() && !b->getSys2() && !b->getSys3() && !b->getSys4())
				{
					b->setSys5();
					c->disSys5();
					b->getNewTop();
					c->getNewTop();

					viewStatus(*a, *b, *c);
					cout << "Transfer successful. All systems normal.";
					cout << "\n\n\n\n\n";
					return;
				}
				else
				{
					crash();
				}
			}
		}
	}
}

void viewStatus(panel a, panel b, panel c)
{
	cout << "\n\n\n\n\n\n";

	cout << "Top Panel" << endl;
	cout << "-------------" << endl;
	cout << "System 1:" <<  (a.getSys1() ? " Active" : " Inactive") << endl;
	cout << "System 2:" <<  (a.getSys2() ? " Active" : " Inactive") << endl;
	cout << "System 3:" <<  (a.getSys3() ? " Active" : " Inactive") << endl;
	cout << "System 4:" <<  (a.getSys4() ? " Active" : " Inactive") << endl;
	cout << "System 5:" <<	(a.getSys5() ? " Active" : " Inactive") << endl;
	cout << "\n\n";

	cout << "Middle Panel" << endl;
	cout << "-------------" << endl;
	cout << "System 1:" <<  (b.getSys1() ? " Active" : " Inactive") << endl;
	cout << "System 2:" <<  (b.getSys2() ? " Active" : " Inactive") << endl;
	cout << "System 3:" <<  (b.getSys3() ? " Active" : " Inactive") << endl;
	cout << "System 4:" <<  (b.getSys4() ? " Active" : " Inactive") << endl;
	cout << "System 5:" <<  (b.getSys5() ? " Active" : " Inactive") << endl;
	cout << "\n\n";

	cout << "Bottom Panel" << endl;
	cout << "-------------" << endl;
	cout << "System 1:" <<  (c.getSys1() ? " Active" : " Inactive") << endl;
	cout << "System 2:" <<  (c.getSys2() ? " Active" : " Inactive") << endl;
	cout << "System 3:" <<  (c.getSys3() ? " Active" : " Inactive") << endl;
	cout << "System 4:" <<  (c.getSys4() ? " Active" : " Inactive") << endl;
	cout << "System 5:" <<  (c.getSys5() ? " Active" : " Inactive") << endl;

	cout << "\n\n\n\n\n\n";
}

void unlock(panel c)
{
	char temp = '\0';

	if(c.getSys1() && c.getSys2() && c.getSys3() && c.getSys4() && c.getSys5())
	{
		cout << "\n\n\n\n" << endl;

		cout << "Key: ";

		for(size_t i = 0; i < 4; i++)
		{
			temp = resistor[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = transistor[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = capacitor[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = voltmeter[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = pmos[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = nmos[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = cmos[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = metal1[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = metal2[i] ^ 0x18;
			cout << temp;
		}

		for(size_t i = 0; i < 4; i++)
		{
			temp = metal3[i] ^ 0x18;
			cout << temp;
		}

		cout << "\n\n\n\n\n" << endl;
	}
	else
	{
		cout << "\n\n\n\n" << endl;
		cout << "Insufficient power." << endl;
		cout << "\n\n\n\n" << endl;
	}
}

void crash()
{
	srand((unsigned)time(0));

	int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	while(1)
	{
		cout << arr[rand()%10];
	}
}

void reset(panel *a, panel *b, panel *c)
{
	a->setSys1();
	a->setSys2();
	a->setSys3();
	a->setSys4();
	a->setSys5();

	b->disSys1();
	b->disSys2();
	b->disSys3();
	b->disSys4();
	b->disSys5();

	c->disSys1();
	c->disSys2();
	c->disSys3();
	c->disSys4();
	c->disSys5();

	a->getNewTop();
	b->getNewTop();
	c->getNewTop();
}