#include <cstdlib>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <sys/ioctl.h>

#include <input.hpp>

void rotate_up  (int& curr, int& right, int& up) 
{
	int temp;
	temp = up;
	up = 7 - curr;
	curr = temp;	
}

void rotate_down  (int& curr, int& right, int& up) 
{
	int temp;
	temp = 7 - up;
	up = curr;
	curr = temp;

}

void rotate_right  (int& curr, int& right, int& up) 
{
	int temp;
	temp = right;
	right = 7 - curr;
	curr =temp;

}

void rotate_left  (int& curr, int& right, int& up) 
{
	int temp;
	temp = right;
	right = curr;
	curr = 7 - temp;
}

void one_side (const int key, std::vector<std::vector<char>>& side)
{		
	switch(key) 
	{
		case 1 :
			side = {
				{' ', ' ', ' '},
				{' ', '*', ' '},
				{' ', ' ', ' '},
			      };
			break;
		case 2 :
			side = {
				{'*', ' ', ' '},
				{' ', ' ', ' '},
				{' ', ' ', '*'},
			      };
			break;
		case 3 :
			side = {
				{'*', ' ', ' '},
				{' ', '*', ' '},
				{' ', ' ', '*'},
			      };
			break;
		case 4 :
			side = {
				{'*', ' ', '*'},
				{' ', ' ', ' '},
				{'*', ' ', '*'},
			      };
			break;
		case 5 :
			side = {
				{'*', ' ', '*'},
				{' ', '*', ' '},
				{'*', ' ', '*'},
			      };
			break;
		case 6 :
			side = {
				{'*', ' ', '*'},
				{'*', ' ', '*'},
				{'*', ' ', '*'},
			      };
			break;
		default :
			side = {
				{' ', ' ', ' '},
				{' ', ' ', ' '},
				{' ', ' ', ' '},
			      };
			break;      			     	 
	}
	
}


void move_cursor(int x, int y)
{
    	printf("%c[%d;%df", 0x1B, x, y);
}

void move_down(int& x, int y)
{
	printf("%c[%d;%df", 0x1B, ++x, y);
}

void center_x(int& row, int n = 0)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	row = size.ws_row / 2 - n / 2;
}

void center_y(int& col, int n = 0)
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	col = size.ws_col / 2 - n;
}

void print (const std::vector<std::vector<char>>& arr, const int clrx, const int clry)
{
	int y, x, clr_x, clr_y, size = arr.size();
	center_x(x, size);

	for (int i = 0; i < size; i++)
	{
		center_y(y, size);
   		for (int j = 0; j < size; j++)
    		{
			move_cursor(x, y);
			if(i >= (clry % size)   && i < (clry % size) + 3 && j > clrx % (size - 1) && j <= clrx % (size - 1) + 3 )
			{
				
				std::cout << "\033[36m"<<arr[i][j]<<"\033[0m\n";

			}
			else 
			{
				std::cout << arr[i][j];

			}
			y = y + 2;
	
    		}
		move_cursor(++x, y);
		std::cout<<"\n";

	}	

}

void key_rotate (const int key, int& curr, int& right, int& up)
{
	switch(key) 
	{
		case 'w': case 'W':
			rotate_up(curr, right,up);
     			 break; 
		case 'a': case 'A':
			 rotate_left(curr, right,up);
			 break; 
		case 's': case 'S':
			 rotate_down(curr, right,up);
			 break; 
		case 'd': case 'D':
			 rotate_right(curr, right,up);
			 break;   	 
		default : 
		 	 break;
	}

}
void define_matrix (std::vector<std::vector<char>>& arr)
{
	for ( int i = 0 ; i < arr.size(); i++ )
	{
		arr[i].resize(arr.size());
	}
	
	for(int i = 0; i < arr.size(); ++i)
	{
		for(int j = 0; j < arr.size(); ++j)
		{

			if(j % 4 == 0)
			{
				arr[i][j] = '|';

			}
			else 
			{
				arr[i][j] = '-';
			}
		}
	}

}

void change_matrix(std::vector<std::vector<char>>& dice, std::vector<std::vector<char>>& side, int& x, int& y, int& count, const int n)
{
	if(count >= n * n)
	{
		count = 0;
		y = 1;

	}
	for (std::vector<char> line : side)
    	{	
		x = (count % n) * 4 + 1;
		for (char el : line)
		{
			dice[y][x] = el;
			x++;			
		}
		y++;
	}
	count++;
	if(count % n == 0 )
	{
		y += 1;
	}
	else 
	{
		y -= 3;
	}

}
void print_dice_info(const int curr, const int right, const int up, const int n)
{
	int x, y = 10;
	center_x(x, n);
	move_cursor(x, y);
	std::cout << "\033[1;4;32mDice position is:\033[0m\n";
	move_down(x, y);
	std::cout << "current is: " << curr;
	move_down(x, y);
	std::cout << "right is: " << right;
	move_down(x, y);
	std::cout << "top is: " << up;
	
}
void menu(int& n)
{
	int key;
	system("clear");

	std::cout << "Buttons to rotate dice: \n	up -> w\n	down -> s\n	left -> s\n	right -> d \n\n\n";
	std::cout << "If you want to close the game press 'enter'\n\n\n";
	
	std::cout << "Input N from 1 to 10. \nSize of matrix will be N x N: ";
	std::cin >> n;
	if(n > 10)
	{
		std::cout<<"Input is wrong\n";
		exit(0);
	}
	system("clear");

}

int main ()
{	
	int 	n = 0,
		last = 0,curr= 1,
		right = 2,up = 3,
		key_press = 0,
		count = 0,
		x = 0, y = 1;
	
    	menu(n);

	std::vector<std::vector<char>>  side, dice(4 * n + 1);
	define_matrix(dice);
	

	system("clear");
	cbreak();

	while (true)
    	{

		key_press=keypress();
		one_side(curr, side);

		if(last != curr)
		{
			print_dice_info(curr, right, up, 4 * n + 1);
			int clrx = x, clry = y;
			change_matrix(dice, side,  x, y, count, n);
			clry = (clry == 4 * n + 1) ? 1 : clry;	
    			print(dice, clrx, clry); 
			std::cout<<clrx<<" "<<clry;
			last = curr;
			
		}

		if (key_press != 10) 
		{	
			key_rotate(key_press, curr, right, up);		
		}
		else 
		{
			normal();
			exit(0); 			
		}
        }
			
	return 0;
}

