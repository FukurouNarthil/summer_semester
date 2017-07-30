// 引用的头文件
#include <windows.h> // Windows窗口程序编程，需要引用头文件 Windows.h
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "ball_and_board.h"

//主菜单背景图
#define BITMAP_FILE_BK "C:\\Users\\V554L\\Documents\\Visual Studio 2017\\Projects\\test3\\test3\\bone.bmp"

//花花
#define BITMAP_FLOWER "C:\\Users\\V554L\\Documents\\Visual Studio 2017\\Projects\\test3\\test3\\flower3.bmp"

// 画图时使用的像素点个数。
#define CELL_PIXEL			20

#define BOARD_MOVE_STEP	5		// 板在方向键的作用下，每次移动的像素；

// 用来绘图的颜色
#define COLOR_BALL			RGB(176,196,222)
//#define COLOR_BRICK_NORMAL			RGB(255,200,245)
//#define COLOR_BRICK_HARD          RGB(255,153, 204)
//#define COLOR_BRICK_DIAMOND          RGB(255, 102, 178)
#define COLOR_BRICK_NORMAL			RGB(229,204,255)
#define COLOR_BRICK_HARD          RGB(204,153, 255)
#define COLOR_BRICK_DIAMOND          RGB(190, 102, 255)
#define COLOR_BOARD          RGB(150, 205, 205)
#define COLOR_BOUNDARY		RGB(	190, 190, 190)
#define COLOR_TEXT			RGB(141, 238, 238)
#define COLOR_TEXT_START         RGB(255, 102, 178)
#define COLOR_CHOICE        RGB(255, 204, 229)

// 游戏的参数的设置 
#define INIT_TIMER_ELAPSE	300	// 初始的时钟周期，确定游戏初始速度
#define	ONE_LEVELS_SCORES	10	// 每升级一次需要的计分
#define SPEEDUP_RATIO		0.8 // 升级以后时间周期（确定游戏速度）提高的比例。
#define MAX_X		22	// 游戏界面大小
#define MAX_Y		23	// 游戏界面大小
#define INIT_X		11	// 球的初始位置
#define INIT_Y		20	// 球的初始位置
#define INIT_BRICK_X 1 //第一块砖相对左边界的位置
#define INIT_BRICK_Y 1 //第一块转相对上边界的位置
#define INIT_BOARD_LEFT 9  //板的左端点初始位置
#define INIT_BOARD_Y 2  //板距离下边框的高度
#define INIT_BOARD_LEN  4  //板的长度
#define INIT_DIR	UP_LEFT	// 球的初始方向
#define INIT_LEVEL_3_X  8
#define INIT_LEVEL_3_Y 3
#define INIT_LEVEL_2_X  8
#define INIT_LEVEL_2_Y 6
#define INIT_LEVEL_1_X  8
#define INIT_LEVEL_1_Y 9


// 全局变量
HINSTANCE hinst; /// HINSTANCE是用来表示程序运行实例的句柄，某些API函数会使用到这个变量。
RECT rectBoundary;
int flag = 0;

// 函数声明

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//BOOL SpeedUp(HWND hwnd);

int WINAPI WinMain(
	HINSTANCE hinstance, // 程序实例句柄，在程序运行，进程创建后，由操作系统向应用程序传入
	HINSTANCE hPrevInstance, // 父进程的程序实例句柄
	LPSTR lpCmdLine,  // 命令行参数，地位和作用类似C语言main函数参数argc和argv,但是没有按空格进行切分
	int nCmdShow)   // 用于指明窗口是否需要显示的参数。
{
	WNDCLASS wc;
	// 窗口句柄，hwnd变量是主窗口的句柄，这个程序中只用到了一个窗口。
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// 窗口类的样式，这里设置的样式表示窗口在大小变化是需要重绘
	wc.style = CS_HREDRAW | CS_VREDRAW;

	// 一个函数指针，这个函数用来处理窗口消息。 详见 MainWndProc函数的注释。
	wc.lpfnWndProc = MainWndProc;


	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon成员用来指定窗口的图标
	// 这里直接使用LoadIcon函数加载了一个系统预定义的图标，开发人员可也可以自己创建图标。
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor是鼠标光标，这里是设定了鼠标光标的样式。
	// 直接使用LoadCursor API函数载入了一个系统预定义的光标样式，还有IDC_CROSS,IDC_HAND等样式 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject的功能是加载一个系统预定义（在栈中）的GDI对象，
	// 这里加载的是一个白色的画刷，有关画刷和GDI对象，详见GDI说明。
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// 窗口的菜单的资源名。
	wc.lpszMenuName = "MainMenu";
	// 给窗口类起一个名字，在创建窗口时需要这个名字。
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// 窗口注册失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口class失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}
	// 窗口注册成功，继续运行。


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// 窗口类名，必须是已经注册了的窗口类
		"Break The Bricks!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// 窗口的style，这个表示为top-level window 
		CW_USEDEFAULT,			// 窗口水平位置default horizontal POINT 
		CW_USEDEFAULT,			// 窗口垂直位置default vertical POINT 
		CW_USEDEFAULT,			// 窗口宽度 default width 
		CW_USEDEFAULT,			// 窗口高度 default height 
		(HWND)NULL,				// 父窗口句柄 no owner window 
		(HMENU)NULL,			// 窗口菜单的句柄 use class menu 
		hinstance,				// 应用程序实例句柄 handle to application instance 
		(LPVOID)NULL);			// 指向附加数据的指针 no window-creation data 

	if (!hwnd)
	{
		// 窗口创建失败，消息框提示，并退出。
		MessageBox(NULL, "创建窗口失败", "错误！", MB_ICONERROR | MB_OK);
		return -1;
	}

	// 窗口创建成功，继续运行。

	// 显示窗口，WinMain函数的nCmdShow参数在这里发挥作用，一般都设置为SW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// 刷新窗口，向窗口发送一个WM_PAINT消息，使得窗口进行重绘。
	UpdateWindow(hwnd);


	// 以下进入消息循环。获取消息--翻译消息--分配消息（由窗口的消息处理函数来处理消息）
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0
		&& fGotMessage != -1)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

void GamePaint(HWND hwnd)
{

	HPEN hpen;
	//HBRUSH hbrush;
	HDC hdc, hdcmem, hdcBitmapSrc;
	HBITMAP hbmMem, hbmpFlower = LoadImage(NULL, BITMAP_FLOWER,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE), hbmOldFlower;

	HPEN hPenBoundary;  //画边界
	HPEN hOldPen;

	HBRUSH hbrushBrickNormal;  //画普通砖块
	HBRUSH hbrushBrickHard;  //画硬砖块
	HBRUSH hbrushBrickDiamond; //画钻石砖块
	HBRUSH hBrushBall;  //画小球
	HBRUSH hBrushBoard;  //画木板
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	BITMAP bmp;

	PGAME_COORD BallBody = GetBallPosition();
	PGAME_COORD OneBricks;
	PGAME_COORD Flower = GetFlower();
	PLIST brickPile;
	PGAME_COORD Board = GetBoard();
	int i, j, board_size = INIT_BOARD_LEN;  //木板的长度

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	// 创建需要用到的PEN和BRUSH
	hbrushBrickNormal = CreateSolidBrush(COLOR_BRICK_NORMAL); // RGB颜色，实心BRUSH
	hbrushBrickHard = CreateSolidBrush(COLOR_BRICK_HARD);
	hbrushBrickDiamond = CreateSolidBrush(COLOR_BRICK_DIAMOND);
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN， PS_NULL表示不可见
	hBrushBall = CreateSolidBrush(COLOR_BALL);
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
	hBrushBoard = CreateSolidBrush(COLOR_BOARD);

	//============

	// ===============
	HBITMAP hbmbackground = LoadImage(NULL, BITMAP_FILE_BK,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hbmbackground)
		MessageBox(NULL, "A", "A", NULL);
	HDC hdcmem1 = CreateCompatibleDC(hdc);
	HBITMAP hbmMem1 = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hdcmem1, hbmbackground);
	BitBlt(hdcmem,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcmem1,
		0, 0,
		SRCCOPY);
	//==============

	/*******************************************************************************
	* #############  画花花  ################
	*
	*******************************************************************************/
	//FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//HDC hdcmem2 = CreateCompatibleDC(hdc);
	//HBITMAP hbmMem2 = CreateCompatibleBitmap(hdc,
	//	rect.right - rect.left, rect.bottom - rect.top);
	////SelectObject(hdcmem2, hbmMem2);
	//hdcBitmapSrc = CreateCompatibleDC(hdc);
	SelectObject(hdcmem1, hbmpFlower);
	GetObject(hbmpFlower, sizeof(BITMAP), &bmp);
	StretchBlt(hdcmem, Flower->x * CELL_PIXEL + rectBoundary.left, Flower->y * CELL_PIXEL + rectBoundary.top,
		CELL_PIXEL, CELL_PIXEL, hdcmem1,
		0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

	///*******************************************************************************
	//* #############  画砖块  ################
	//*
	//*******************************************************************************/

	// 将画图需要用的PEN和BRUSH选择到DC中
	/*hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBrickNormal);*/
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	brickPile = GetBrick();
	PGAME_COORD p;
	pBRICK pb;

	// （椭）圆形，使用上面选择的PEN勾勒边框，BRUSH填充
	for (i = 0; i < ListSize(brickPile); i++)
	{
		pb = (pBRICK)malloc(sizeof(BRICK));
		pb = (pBRICK)ListGetAt(brickPile, i);
		OneBricks = pb->bCoord;
		if (pb->type == NORMAL){
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBrickNormal);
		}else if (pb->type == HARD){
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBrickHard);
		}else{
			hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBrickDiamond);
		}
		Rectangle(hdcmem,
			(OneBricks->x) * CELL_PIXEL + rectBoundary.left,
			(OneBricks->y)  * CELL_PIXEL + rectBoundary.top,
			(OneBricks->x + 1) *CELL_PIXEL + rectBoundary.left,
			(OneBricks->y + 1) *CELL_PIXEL + rectBoundary.top);
	}


	///*******************************************************************************
	/** #############  画球  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hBrushBall);

	Rectangle(hdcmem,
		BallBody->x * CELL_PIXEL + rectBoundary.left,
		BallBody->y * CELL_PIXEL + rectBoundary.top,
		(BallBody->x + 1)*CELL_PIXEL + rectBoundary.left,
		(BallBody->y + 1)*CELL_PIXEL + rectBoundary.top);

	///*******************************************************************************
	//* #############  画木板  ################
	//*
	//*******************************************************************************/
	SelectObject(hdcmem, hBrushBoard);
	Rectangle(hdcmem,
		Board->x * CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - Board->y * CELL_PIXEL,
		(Board->x + INIT_BOARD_LEN)*CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - (Board->y + 1)  * CELL_PIXEL);

	///*******************************************************************************
	//* #############  画墙  ################
	//*
	//*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	///*******************************************************************************
	//* #############  写一行字  ################
	//*
	//*******************************************************************************/

	// 创建了一个字体对象
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("MicroSoft YaHei"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Score %d level %d", GetScore(), GetGameState());
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  回收和释放资源  ################
	*
	*******************************************************************************/
	// 回收资源
	// DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hbrushBrickNormal);
	DeleteObject(hbrushBrickHard);
	DeleteObject(hbrushBrickDiamond);
	DeleteObject(hBrushBall);
	DeleteObject(hBrushBoard);
	DeleteObject(hpen);
	DeleteObject(hPenBoundary);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC 函数  ################
	* 释放占用的DC等系统资源。
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}

/*******************************************************************************
* ##########   根据游戏界面的坐标系大小设置游戏窗口大小    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// 左上角
	POINT ptRightBottom;	// 右下角
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// 计算上下左右角的位置
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// 计算好了，设置窗口大小。
	MoveWindow(hwnd,
		rectWindow.left - 15,
		rectWindow.top - 15,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // 保存边界和左右两边边框相等。
		rectBoundary.bottom + 120, //给积分信息留出显示空间。 //给积分信息留出显示空间。
		TRUE);
}


//主窗口绘制
void MainPaint(HWND hwnd)
{
	HPEN hpen;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;
	HPEN hPenBoundary;  //画边界
	HPEN hOldPen;

	HBRUSH hbrushChoice;  //画选项
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	// ===============
	HBITMAP hbmbackground= LoadImage(NULL, BITMAP_FILE_BK,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (!hbmbackground)
		MessageBox(NULL, "A", "A", NULL);
	HDC hdcmem1 = CreateCompatibleDC(hdc);
	HBITMAP hbmMem1 = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);
	SelectObject(hdcmem1, hbmbackground);
	BitBlt(hdcmem,
		rect.left, rect.top,
		rect.right - rect.left, rect.bottom - rect.top,
		hdcmem1,
		0, 0,
		SRCCOPY);
	//==============

	// 创建需要用到的PEN和BRUSH
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
	hbrushChoice = CreateSolidBrush(COLOR_CHOICE); // RGB颜色，实心BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN， PS_NULL表示不可见

    /*******************************************************************************
    * #############  画背景  ################
	*
	*******************************************************************************/
	//FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	///*******************************************************************************
	//* #############  画墙  ################
	//*
	//*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// 将PEN移动到需要绘制的方框的左上角
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// 画了一个方框。演示LineTo函数
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  画选项框  ################
	*
	*******************************************************************************/
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);
	SelectObject(hdcmem, hbrushChoice);
	Rectangle(hdcmem,
		INIT_LEVEL_1_X * CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - INIT_LEVEL_1_Y * CELL_PIXEL,
		(INIT_LEVEL_1_X + INIT_BOARD_LEN * 1.5)*CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - (INIT_LEVEL_1_Y + 2)  * CELL_PIXEL);

	/*SelectObject(hdcmem, hbrushChoice);*/
	Rectangle(hdcmem,
		INIT_LEVEL_2_X * CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - INIT_LEVEL_2_Y * CELL_PIXEL,
		(INIT_LEVEL_2_X + INIT_BOARD_LEN * 1.5)*CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - (INIT_LEVEL_2_Y + 2)  * CELL_PIXEL);

	/*SelectObject(hdcmem, hbrushChoice);*/
	Rectangle(hdcmem,
		INIT_LEVEL_3_X * CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - INIT_LEVEL_3_Y * CELL_PIXEL,
		(INIT_LEVEL_3_X + INIT_BOARD_LEN * 1.5)*CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - (INIT_LEVEL_3_Y + 2)  * CELL_PIXEL);
	//* #############  写一行字  ################
	//*
	//*******************************************************************************/

	// 创建了一个字体对象
	hFont = CreateFont(30, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("MicroSoft YaHei"));

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "NORMAL");
		SetBkMode(hdcmem, TRANSPARENT);
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 171, rectBoundary.bottom - 215,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "HARD");
		SetBkMode(hdcmem, TRANSPARENT);
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 188, rectBoundary.bottom - 154,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 将这个FONT对象放入DC中
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "HELL(NO");
		SetBkMode(hdcmem, TRANSPARENT);
		// 设置输出颜色
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// 输出字符串。
		TextOut(hdcmem, rectBoundary.left + 172, rectBoundary.bottom - 94,
			szSourceInfo, lstrlen(szSourceInfo));
		// 输出完成，将原来的字体对象放回DC中
		SelectObject(hdcmem, hOldFont);
	}

	// 在内存DC中画完，一次输出的窗口DC上。
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  回收和释放资源  ################
	*
	*******************************************************************************/
	// 回收资源
	// DeleteObject 释放GDI对象
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hPenBoundary);
	DeleteObject(hbrushChoice);
	DeleteObject(hpen);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC 函数  ################
	* 释放占用的DC等系统资源。
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}

void CreateMain(HWND hwnd, int boundary_x, int boundary_y)
{

	SetGameState(START);

	SetBoundary(boundary_x, boundary_y);
	PGAME_COORD pCoordBoundary = GetBoundary();

	// 设置游戏边界
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);


}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // 消息
	WPARAM wParam, // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
	LPARAM lParam) // 消息参数，不同的消息有不同的意义，详见MSDN中每个消息的文档
{
	// 注意，是switch-case, 每次这个函数被调用，只会落入到一个case中。
	switch (msg)
	{
		// 当窗口被创建时，收到的第一个消息就是WM_CREATE，
		// 一般收到这个消息处理过程中，可以用来进行一些初始化的工作
	case WM_CREATE:
		CreateMain(hwnd, MAX_X, MAX_Y);
		CreateThread(NULL, 0, StartPageBGM, NULL, 0, NULL);
		ReSizeGameWnd(hwnd);
		break;

	case WM_PAINT:
		MainPaint(hwnd);
		/*GamePaint(hwnd);*/
		break;

	case WM_KEYDOWN:

		OnKeyDown(wParam, hwnd);
		GamePaint(hwnd);
		break;

	case WM_LBUTTONDOWN:
		OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_TIMER:

		OnTimer(hwnd);
		GamePaint(hwnd);
		break;

	case WM_DESTROY:
		ExitProcess(0);
		break;

	default:
		break;
	}
	return DefWindowProc(hwnd,
		msg,
		wParam,
		lParam);
}

// 当用于操作时进行的逻辑处理。
// 本游戏只使用到了键盘左右键控制。
void OnKeyDown(DWORD vk, HWND hwnd)
{
	if (!gameState)
	{
		switch (vk)
		{
		case '1':
			PlaySound(NULL, NULL, NULL);
			CreateThread(NULL, 0, ChooseButton, NULL, 0, NULL);
			SetGameState(LEVEL_1);
			CreateGame(hwnd,
				INIT_TIMER_ELAPSE,
				ONE_LEVELS_SCORES,
				SPEEDUP_RATIO,
				MAX_X, MAX_Y,
				INIT_X, INIT_Y,
				INIT_DIR,
				INIT_BOARD_LEFT, INIT_BOARD_Y,
				INIT_BOARD_LEN,
				INIT_BRICK_X, INIT_BRICK_Y);
			CreateThread(NULL, 0, PlayMusic, NULL, 0, NULL);
			break;
		case '2':
			PlaySound(NULL, NULL, NULL);
			CreateThread(NULL, 0, ChooseButton, NULL, 0, NULL);
			SetGameState(LEVEL_2);
			CreateGame(hwnd,
				INIT_TIMER_ELAPSE,
				ONE_LEVELS_SCORES,
				SPEEDUP_RATIO,
				MAX_X, MAX_Y,
				INIT_X, INIT_Y,
				INIT_DIR,
				INIT_BOARD_LEFT, INIT_BOARD_Y,
				INIT_BOARD_LEN,
				INIT_BRICK_X, INIT_BRICK_Y);
			CreateThread(NULL, 0, PlayMusic2, NULL, 0, NULL);
			break;
		case '3':
			PlaySound(NULL, NULL, NULL);
			CreateThread(NULL, 0, ChooseButton, NULL, 0, NULL);
			SetGameState(LEVEL_3);
			CreateGame(hwnd,
				INIT_TIMER_ELAPSE,
				ONE_LEVELS_SCORES,
				SPEEDUP_RATIO,
				MAX_X, MAX_Y,
				INIT_X, INIT_Y,
				INIT_DIR,
				INIT_BOARD_LEFT, INIT_BOARD_Y,
				INIT_BOARD_LEN,
				INIT_BRICK_X, INIT_BRICK_Y);
			CreateThread(NULL, 0, PlayMusic3, NULL, 0, NULL);
			break;
		default:
			MessageBox(0, "Press 1 or 2 or 3 to choose game Mode!", "ATTENTION!!!", 0);
			ExitProcess(0);
			break;
		}
	}
	else
	{
		switch (vk) // virtual key value
		{
		case VK_LEFT:
			SetBoardDirection(LEFT);
			GamePaint(hwnd);
			break;
		case VK_RIGHT:
			SetBoardDirection(RIGHT);
			GamePaint(hwnd);
			break;
		case VK_ESCAPE:
			KillTimer(hwnd, 12340);
			ExitProcess(0);
			break;
		case VK_SPACE:
			if (!flag)
			{
				SetTimer(hwnd, 12340, 10000, NULL);
				flag = 1;
			}
			else
			{
				if (GetGameState() == 3)
				{
					SetTimer(hwnd, 12340, INIT_TIMER_ELAPSE / 3, NULL);
				}
				else
				{
					SetTimer(hwnd, 12340, INIT_TIMER_ELAPSE / 2, NULL);
				}
				flag = 0;
			}
		}
	}
	return;
}