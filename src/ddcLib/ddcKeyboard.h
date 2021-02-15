#ifndef __ddcKeyboard__
#define __ddcKeyboard__

#include <termios.h>
#include <unistd.h>

typedef struct termios termios;
typedef enum ddKeyInfo ddKeyInfo;

enum ddKeyInfo { DDK_a=97,DDK_b,DDK_c,DDK_d,DDK_e,DDK_f,DDK_g,DDK_h,DDK_i,DDK_j,DDK_k,DDK_l,DDK_m,DDK_n,DDK_o,DDK_p,DDK_q,DDK_r,DDK_s,DDK_t,DDK_u,DDK_v,DDK_w,DDK_x,DDK_y,DDK_z,
		 DDK_A=65,DDK_B,DDK_C,DDK_D,DDK_E,DDK_F,DDK_G,DDK_H,DDK_I,DDK_J,DDK_K,DDK_L,DDK_M,DDK_N,DDK_O,DDK_P,DDK_Q,DDK_R,DDK_S,DDK_T,DDK_U,DDK_V,DDK_W,DDK_X,DDK_Y,DDK_Z,
		 DDK_D0=48,DDK_D1,DDK_D2,DDK_D3,DDK_D4,DDK_D5,DDK_D6,DDK_D7,DDK_D8,DDK_D9,
		 DDK_SPACE=32, DDK_EXCLAMATION,DDK_QUOTES,DDK_POUND,DDK_DOLLOR,
		 DDK_PERCENT,DDK_ANDSIGN,DDK_QUOTE,DDK_OPENBRACKET,DDK_CLOSEBRACKET,DDK_ASTERISK,DDK_PLUS,DDK_COMMA,DDK_MINUS,DDK_PERIOD,DDK_SLASH,
		 DDK_COLON=58,DDK_SEMICOLON,DDK_LESSTHAN,DDK_EQUALS,DDK_GREATERTHAN,DDK_QUESTIONMARK,DDK_AT,
		 DDK_OPENSQUAREBRACKET=91,DDK_BACKSLASH,DDK_CLOSESQUAREBRACKET,DDK_CARET,DDK_UNDERSCORE,DDK_GRAVE,
		 DDK_OPENCURLYBRACKET=123,DDK_BAR,DDK_CLOSECURLYBRACKET,DDK_TILDE,DDK_DELETE,
		 DDK_UP=500,DDK_DOWN,DDK_LEFT,DDK_RIGHT,
		 DDK_ESCAPE=27,
		 DDK_BACKSPACE=127,DDK_TAB=9,DDK_RETURN=10,DDK_ENTER=10 };

static bool __has_escape;
static bool __is_escaped;

ddKeyInfo ddKey_getch(void)
{
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 1;
	newt.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	unsigned char c;
	ssize_t nread = read(STDIN_FILENO, &c, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	if (c == '\x1b')
	{
		if (__has_escape)
			return '\x1b';
		__has_escape = true;
		ddKeyInfo dki = ddKey_getch();
		__has_escape = false;
		__is_escaped = false;
		return dki;
	}
	if (__has_escape) 
	{
		if (c == '[')
		{
			__is_escaped = true;
			return ddKey_getch();
		}
		else if (c == '\x1b')
		{
			__has_escape = false;
			__is_escaped = false;
			return DDK_ESCAPE;
		}
			
	}
	if (__has_escape && __is_escaped)
	{
		if (c == 'A')
			return DDK_UP;
		if (c == 'B')
			return DDK_DOWN;
		if (c == 'C')
			return DDK_RIGHT;
		if (c == 'D')
			return DDK_LEFT;
		else return c;
	}
	return c;
}

ddKeyInfo ddKey_getch_noesc(void)
{
	termios oldt;
	tcgetattr(STDIN_FILENO, &oldt);
	termios newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 1;
	newt.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	unsigned char c;
	ssize_t nread = read(STDIN_FILENO, &c, 1);
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return c;
}





#endif
