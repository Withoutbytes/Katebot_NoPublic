#pragma once

#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)

#define BeginTick size_t start, end; start = GetTickCount();
#define EndTick(name) end = GetTickCount();printf(xorstr("[%s] Code block was executed in %.f ticks\n").crypt_get(),name, ((double) end - start));

enum concol
{
	black = 0,
	dark_blue = 1,
	dark_green = 2,
	dark_aqua, dark_cyan = 3,
	dark_red = 4,
	dark_purple = 5, dark_pink = 5, dark_magenta = 5,
	dark_yellow = 6,
	dark_white = 7,
	gray = 8,
	blue = 9,
	green = 10,
	aqua = 11, cyan = 11,
	red = 12,
	purple = 13, pink = 13, magenta = 13,
	yellow = 14,
	white = 15
};

class ConsoleFuncs
{
public:
	void SetConsoleCursor( bool cursor );
	void GotoXY( int x, int y );
	void SetConsoleColor( concol textColor, concol bgColor );
	void ClearScreen();
	int  Getch();
	void SetConsoleSize( int w, int h );
	
	// Print Info
	void PrintVector(char* name, Vector a);
};

extern ConsoleFuncs* g_pCon;
