#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <math.h>
int cx1,cy1,R;
struct Vector{

double v[2];

Vector(double x = 0, double y = 0)

{ v[0] = x; v[1] = y; }

double& operator[]
(int i)
{ return v[i];

}
};
int Round(double x)
{

return (int)
(x+0.5);
}
int radius(int x1, int y1, int x2, int y2)
{

return sqrt(
(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)
);
}
void draw8points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{

SetPixel(hdc, xc+a, yc+b, color);

SetPixel(hdc, xc-a, yc+b, color);

SetPixel(hdc, xc-a, yc-b, color);

SetPixel(hdc, xc+a, yc-b, color);

SetPixel(hdc, xc+b, yc+a, color);

SetPixel(hdc, xc-b, yc+a, color);

SetPixel(hdc, xc-b, yc-a, color);

SetPixel(hdc, xc+b, yc-a, color);
}
void DrawCirclePolar(HDC hdc, int xc, int yc, int R, COLORREF c) {

int x = R, y = 0;

double theta = 0;

double dtheta = 1.0 / R;

draw8points(hdc, xc, yc, x, y, c);

while(x>y) {

theta += dtheta;

x = Round(R * cos(theta)
);

y = Round(R * sin(theta)
);

draw8points(hdc, xc, yc, x, y, c);

}
}
COLORREF clipping(int x1, int y1, int x2, int y2, int R)
{

int Nrad = radius(x1, y1, x2, y2);

if(Nrad>R)

return RGB(0,0,255);

else

return RGB(255,0,0);
}
void DrawHermiteCurve(HDC hdc,Vector& p1, Vector& T1, Vector& p2, Vector& T2,COLORREF c)
{

double a0 = p1[0], a1 = T1[0],

a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],

a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];

double b0 = p1[1], b1 = T1[1],

b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],

b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];

for (double t = 0; t <= 1; t += 0.001)

{

double t2 = t*t, t3 = t2*t;

double x = a0 + a1*t + a2*t2 + a3*t3;

double y = b0 + b1*t + b2*t2 + b3*t3;

c = clipping(x,y,cx1,cy1,R);

SetPixel(hdc, Round(x), Round(y), c);

}
}
int EX1=0, EY1=0, EX2=0, EY2=0,cx2=0, cy2=0;
static Vector p[4];
int i = 0;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

HDC hdc = GetDC(hwnd);

switch (message)

{

case WM_DESTROY:

PostQuitMessage (0);

break;

case WM_LBUTTONDOWN:

if(i == 0 )

{

cx1 = LOWORD(lParam);

cy1 = HIWORD(lParam);

i++;

}

else if(i == 1)

{

cx2 = LOWORD(lParam);

cy2 = HIWORD(lParam);

R = radius(cx1,cy1,cx2,cy2);

int r=R/4;

int EX1=cx1-(cx1/4);

int EY1=cy1-(cy1/4);

int EX2=cx1+(cx1/4);

int EY2=cy1-(cy1/4);

DrawCirclePolar(hdc,cx1,cy1,R,RGB(0,128,0)
);

DrawCirclePolar(hdc,EX1,EY1,r,RGB(0,128,0)
);

DrawCirclePolar(hdc,EX2,EY2,r,RGB(0,128,0)
);

i++;

}

else if (i==2)

{

p[0] = Vector(LOWORD(lParam), HIWORD(lParam)
);

i++;

}

else if (i==3)

{

p[1] = Vector(LOWORD(lParam), HIWORD(lParam)
);

i++;

}

else if (i==4)

{

p[2] = Vector(LOWORD(lParam), HIWORD(lParam)
);

i++;

}

else if (i==5)

{

p[3] = Vector(LOWORD(lParam), HIWORD(lParam)
);

Vector T1(3 * (p[1]
[0] - p[0]
[0]
), 3 * (p[1]
[1] - p[0]
[1]
)
);

Vector T2(3 * (p[3]
[0] - p[2]
[0]
), 3 * (p[3]
[1] - p[2]
[1]
)
);

DrawHermiteCurve(hdc, p[0], T1, p[3], T2,RGB(255, 0, 0)
);

i++;

}

else{

default:

return DefWindowProc (hwnd, message, wParam, lParam);

}

return 0;
}
}
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
/* Make the class name into a global variable */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
int WINAPI WinMain (HINSTANCE hThisInstance,

HINSTANCE hPrevInstance,
LPSTR lpszArgument,
int nCmdShow)
{

HWND hwnd;
/* This is the handle for our window */

MSG messages;
/* Here messages to the application are saved */

WNDCLASSEX wincl;
/* Data structure for the windowclass */

/* The Window structure */

wincl.hInstance = hThisInstance;

wincl.lpszClassName = szClassName;

wincl.lpfnWndProc = WindowProcedure;
/* This function is called by windows */

wincl.style = CS_DBLCLKS;
/* Catch double-clicks */

wincl.cbSize = sizeof (WNDCLASSEX);

/* Use default icon and mouse-pointer */

wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);

wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);

wincl.hCursor = LoadCursor (NULL, IDC_ARROW);

wincl.lpszMenuName = NULL;
/* No menu */

wincl.cbClsExtra = 0;
/* No extra bytes after the window class */

wincl.cbWndExtra = 0;
/* structure or the window instance */

/* Use Windows's default colour as the background of the window */

wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

/* Register the window class, and if it fails quit the program */

if (
!RegisterClassEx (&wincl)
)

return 0;

/* The class is registered, let's create the program*/

hwnd = CreateWindowEx (

0,
/* Extended possibilites for variation */

szClassName,
/* Classname */

_T("Code:
:Blocks Template Windows App"),
/* Title Text */

WS_OVERLAPPEDWINDOW, /* default window */

CW_USEDEFAULT,
/* Windows decides the position */

CW_USEDEFAULT,
/* where the window ends up on the screen */

544,
/* The programs width */

375,
/* and height in pixels */

HWND_DESKTOP,
/* The window is a child-window to desktop */

NULL,
/* No menu */

hThisInstance,
/* Program Instance handler */

NULL
/* No Window Creation data */

);

ShowWindow (hwnd, nCmdShow);

while (GetMessage (&messages, NULL, 0, 0)
)

{

TranslateMessage(&messages);

DispatchMessage(&messages);

}

return messages.wParam;
