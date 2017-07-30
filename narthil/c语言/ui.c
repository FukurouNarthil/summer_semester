// ���õ�ͷ�ļ�
#include <windows.h> // Windows���ڳ����̣���Ҫ����ͷ�ļ� Windows.h
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#include "ball_and_board.h"

//���˵�����ͼ
#define BITMAP_FILE_BK "C:\\Users\\V554L\\Documents\\Visual Studio 2017\\Projects\\test3\\test3\\bone.bmp"

//����
#define BITMAP_FLOWER "C:\\Users\\V554L\\Documents\\Visual Studio 2017\\Projects\\test3\\test3\\flower3.bmp"

// ��ͼʱʹ�õ����ص������
#define CELL_PIXEL			20

#define BOARD_MOVE_STEP	5		// ���ڷ�����������£�ÿ���ƶ������أ�

// ������ͼ����ɫ
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

// ��Ϸ�Ĳ��������� 
#define INIT_TIMER_ELAPSE	300	// ��ʼ��ʱ�����ڣ�ȷ����Ϸ��ʼ�ٶ�
#define	ONE_LEVELS_SCORES	10	// ÿ����һ����Ҫ�ļƷ�
#define SPEEDUP_RATIO		0.8 // �����Ժ�ʱ�����ڣ�ȷ����Ϸ�ٶȣ���ߵı�����
#define MAX_X		22	// ��Ϸ�����С
#define MAX_Y		23	// ��Ϸ�����С
#define INIT_X		11	// ��ĳ�ʼλ��
#define INIT_Y		20	// ��ĳ�ʼλ��
#define INIT_BRICK_X 1 //��һ��ש�����߽��λ��
#define INIT_BRICK_Y 1 //��һ��ת����ϱ߽��λ��
#define INIT_BOARD_LEFT 9  //�����˵��ʼλ��
#define INIT_BOARD_Y 2  //������±߿�ĸ߶�
#define INIT_BOARD_LEN  4  //��ĳ���
#define INIT_DIR	UP_LEFT	// ��ĳ�ʼ����
#define INIT_LEVEL_3_X  8
#define INIT_LEVEL_3_Y 3
#define INIT_LEVEL_2_X  8
#define INIT_LEVEL_2_Y 6
#define INIT_LEVEL_1_X  8
#define INIT_LEVEL_1_Y 9


// ȫ�ֱ���
HINSTANCE hinst; /// HINSTANCE��������ʾ��������ʵ���ľ����ĳЩAPI������ʹ�õ����������
RECT rectBoundary;
int flag = 0;

// ��������

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
//BOOL SpeedUp(HWND hwnd);

int WINAPI WinMain(
	HINSTANCE hinstance, // ����ʵ��������ڳ������У����̴������ɲ���ϵͳ��Ӧ�ó�����
	HINSTANCE hPrevInstance, // �����̵ĳ���ʵ�����
	LPSTR lpCmdLine,  // �����в�������λ����������C����main��������argc��argv,����û�а��ո�����з�
	int nCmdShow)   // ����ָ�������Ƿ���Ҫ��ʾ�Ĳ�����
{
	WNDCLASS wc;
	// ���ھ����hwnd�����������ڵľ�������������ֻ�õ���һ�����ڡ�
	HWND hwnd;

	MSG msg;
	int fGotMessage;

	hinst = hinstance;

	// Fill in the window class structure with parameters 
	// that describe the main window. 

	// ���������ʽ���������õ���ʽ��ʾ�����ڴ�С�仯����Ҫ�ػ�
	wc.style = CS_HREDRAW | CS_VREDRAW;

	// һ������ָ�룬�������������������Ϣ�� ��� MainWndProc������ע�͡�
	wc.lpfnWndProc = MainWndProc;


	// no extra class memory 
	wc.cbClsExtra = 0;
	// no extra window memory
	wc.cbWndExtra = 0;
	// handle to instance 
	wc.hInstance = hinstance;
	// hIcon��Ա����ָ�����ڵ�ͼ��
	// ����ֱ��ʹ��LoadIcon����������һ��ϵͳԤ�����ͼ�꣬������Ա��Ҳ�����Լ�����ͼ�ꡣ
	wc.hIcon = LoadIcon(NULL,
		IDI_APPLICATION);
	// Cursor������꣬�������趨����������ʽ��
	// ֱ��ʹ��LoadCursor API����������һ��ϵͳԤ����Ĺ����ʽ������IDC_CROSS,IDC_HAND����ʽ 
	wc.hCursor = LoadCursor(NULL,
		IDC_CROSS);
	// GetStockObject�Ĺ����Ǽ���һ��ϵͳԤ���壨��ջ�У���GDI����
	// ������ص���һ����ɫ�Ļ�ˢ���йػ�ˢ��GDI�������GDI˵����
	wc.hbrBackground = (HBRUSH)GetStockObject(
		WHITE_BRUSH);
	// ���ڵĲ˵�����Դ����
	wc.lpszMenuName = "MainMenu";
	// ����������һ�����֣��ڴ�������ʱ��Ҫ������֡�
	wc.lpszClassName = "MainWClass";

	// Register the window class. 

	if (!RegisterClass(&wc))
	{
		// ����ע��ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������classʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}
	// ����ע��ɹ����������С�


	// Create the main window. 

	hwnd = CreateWindow(
		"MainWClass",			// �����������������Ѿ�ע���˵Ĵ�����
		"Break The Bricks!",		// title-bar string 
		WS_OVERLAPPEDWINDOW,	// ���ڵ�style�������ʾΪtop-level window 
		CW_USEDEFAULT,			// ����ˮƽλ��default horizontal POINT 
		CW_USEDEFAULT,			// ���ڴ�ֱλ��default vertical POINT 
		CW_USEDEFAULT,			// ���ڿ�� default width 
		CW_USEDEFAULT,			// ���ڸ߶� default height 
		(HWND)NULL,				// �����ھ�� no owner window 
		(HMENU)NULL,			// ���ڲ˵��ľ�� use class menu 
		hinstance,				// Ӧ�ó���ʵ����� handle to application instance 
		(LPVOID)NULL);			// ָ�򸽼����ݵ�ָ�� no window-creation data 

	if (!hwnd)
	{
		// ���ڴ���ʧ�ܣ���Ϣ����ʾ�����˳���
		MessageBox(NULL, "��������ʧ��", "����", MB_ICONERROR | MB_OK);
		return -1;
	}

	// ���ڴ����ɹ����������С�

	// ��ʾ���ڣ�WinMain������nCmdShow���������﷢�����ã�һ�㶼����ΪSW_SHOW
	ShowWindow(hwnd, nCmdShow);

	// ˢ�´��ڣ��򴰿ڷ���һ��WM_PAINT��Ϣ��ʹ�ô��ڽ����ػ档
	UpdateWindow(hwnd);


	// ���½�����Ϣѭ������ȡ��Ϣ--������Ϣ--������Ϣ���ɴ��ڵ���Ϣ��������������Ϣ��
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

	HPEN hPenBoundary;  //���߽�
	HPEN hOldPen;

	HBRUSH hbrushBrickNormal;  //����ͨש��
	HBRUSH hbrushBrickHard;  //��Ӳש��
	HBRUSH hbrushBrickDiamond; //����ʯש��
	HBRUSH hBrushBall;  //��С��
	HBRUSH hBrushBoard;  //��ľ��
	HBRUSH hOldBrush;

	HFONT hFont, hOldFont;

	RECT rect;

	BITMAP bmp;

	PGAME_COORD BallBody = GetBallPosition();
	PGAME_COORD OneBricks;
	PGAME_COORD Flower = GetFlower();
	PLIST brickPile;
	PGAME_COORD Board = GetBoard();
	int i, j, board_size = INIT_BOARD_LEN;  //ľ��ĳ���

	GetClientRect(hwnd, &rect);

	hdc = GetDC(hwnd);

	hdcmem = CreateCompatibleDC(hdc);
	hbmMem = CreateCompatibleBitmap(hdc,
		rect.right - rect.left, rect.bottom - rect.top);

	SelectObject(hdcmem, hbmMem);

	// ������Ҫ�õ���PEN��BRUSH
	hbrushBrickNormal = CreateSolidBrush(COLOR_BRICK_NORMAL); // RGB��ɫ��ʵ��BRUSH
	hbrushBrickHard = CreateSolidBrush(COLOR_BRICK_HARD);
	hbrushBrickDiamond = CreateSolidBrush(COLOR_BRICK_DIAMOND);
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�
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
	* #############  ������  ################
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
	//* #############  ��ש��  ################
	//*
	//*******************************************************************************/

	// ����ͼ��Ҫ�õ�PEN��BRUSHѡ��DC��
	/*hOldBrush = (HBRUSH)SelectObject(hdcmem, hbrushBrickNormal);*/
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	brickPile = GetBrick();
	PGAME_COORD p;
	pBRICK pb;

	// ���֣�Բ�Σ�ʹ������ѡ���PEN���ձ߿�BRUSH���
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
	/** #############  ����  ################
	*
	*******************************************************************************/

	SelectObject(hdcmem, hBrushBall);

	Rectangle(hdcmem,
		BallBody->x * CELL_PIXEL + rectBoundary.left,
		BallBody->y * CELL_PIXEL + rectBoundary.top,
		(BallBody->x + 1)*CELL_PIXEL + rectBoundary.left,
		(BallBody->y + 1)*CELL_PIXEL + rectBoundary.top);

	///*******************************************************************************
	//* #############  ��ľ��  ################
	//*
	//*******************************************************************************/
	SelectObject(hdcmem, hBrushBoard);
	Rectangle(hdcmem,
		Board->x * CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - Board->y * CELL_PIXEL,
		(Board->x + INIT_BOARD_LEN)*CELL_PIXEL + rectBoundary.left,
		rectBoundary.bottom - (Board->y + 1)  * CELL_PIXEL);

	///*******************************************************************************
	//* #############  ��ǽ  ################
	//*
	//*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	///*******************************************************************************
	//* #############  дһ����  ################
	//*
	//*******************************************************************************/

	// ������һ���������
	hFont = CreateFont(48, 0, 0, 0, FW_DONTCARE, 0, 1, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("MicroSoft YaHei"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "Score %d level %d", GetScore(), GetGameState());
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 3, rectBoundary.bottom + 3,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  ���պ��ͷ���Դ  ################
	*
	*******************************************************************************/
	// ������Դ
	// DeleteObject �ͷ�GDI����
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
	* #############  ReleaseDC ����  ################
	* �ͷ�ռ�õ�DC��ϵͳ��Դ��
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}

/*******************************************************************************
* ##########   ������Ϸ���������ϵ��С������Ϸ���ڴ�С    ##########
*******************************************************************************/

void ReSizeGameWnd(HWND hwnd)
{
	POINT ptLeftTop;		// ���Ͻ�
	POINT ptRightBottom;	// ���½�
	RECT rectWindow;
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);

	// �����������ҽǵ�λ��
	ptLeftTop.x = rectBoundary.left;
	ptLeftTop.y = rectBoundary.top;
	ptRightBottom.x = rectBoundary.right;
	ptRightBottom.y = rectBoundary.bottom;
	ClientToScreen(hwnd, &ptLeftTop);
	ClientToScreen(hwnd, &ptRightBottom);

	GetWindowRect(hwnd, &rectWindow);
	// ������ˣ����ô��ڴ�С��
	MoveWindow(hwnd,
		rectWindow.left - 15,
		rectWindow.top - 15,
		ptLeftTop.x - rectWindow.left + ptRightBottom.x - rectWindow.left, // ����߽���������߱߿���ȡ�
		rectBoundary.bottom + 120, //��������Ϣ������ʾ�ռ䡣 //��������Ϣ������ʾ�ռ䡣
		TRUE);
}


//�����ڻ���
void MainPaint(HWND hwnd)
{
	HPEN hpen;
	HDC hdc, hdcmem;
	HBITMAP hbmMem;
	HPEN hPenBoundary;  //���߽�
	HPEN hOldPen;

	HBRUSH hbrushChoice;  //��ѡ��
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

	// ������Ҫ�õ���PEN��BRUSH
	hPenBoundary = CreatePen(0, 3, COLOR_BOUNDARY);
	hbrushChoice = CreateSolidBrush(COLOR_CHOICE); // RGB��ɫ��ʵ��BRUSH
	hpen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));  // PEN�� PS_NULL��ʾ���ɼ�

    /*******************************************************************************
    * #############  ������  ################
	*
	*******************************************************************************/
	//FillRect(hdcmem, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	hOldPen = (HPEN)SelectObject(hdcmem, hpen);

	///*******************************************************************************
	//* #############  ��ǽ  ################
	//*
	//*******************************************************************************/

	SelectObject(hdcmem, hPenBoundary);

	// ��PEN�ƶ�����Ҫ���Ƶķ�������Ͻ�
	MoveToEx(hdcmem, rectBoundary.left, rectBoundary.top, NULL);
	// ����һ��������ʾLineTo����
	LineTo(hdcmem, rectBoundary.left, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.bottom);
	LineTo(hdcmem, rectBoundary.right, rectBoundary.top);
	LineTo(hdcmem, rectBoundary.left, rectBoundary.top);

	/*******************************************************************************
	* #############  ��ѡ���  ################
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
	//* #############  дһ����  ################
	//*
	//*******************************************************************************/

	// ������һ���������
	hFont = CreateFont(30, 0, 0, 0, FW_DONTCARE, 0, 0, 0, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
		CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("MicroSoft YaHei"));

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "NORMAL");
		SetBkMode(hdcmem, TRANSPARENT);
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 171, rectBoundary.bottom - 215,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "HARD");
		SetBkMode(hdcmem, TRANSPARENT);
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 188, rectBoundary.bottom - 154,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// �����FONT�������DC��
	if (hOldFont = (HFONT)SelectObject(hdcmem, hFont))
	{
		CHAR szSourceInfo[1024];
		wsprintf(szSourceInfo, "HELL(NO");
		SetBkMode(hdcmem, TRANSPARENT);
		// ���������ɫ
		SetTextColor(hdcmem, COLOR_TEXT_START);
		// ����ַ�����
		TextOut(hdcmem, rectBoundary.left + 172, rectBoundary.bottom - 94,
			szSourceInfo, lstrlen(szSourceInfo));
		// �����ɣ���ԭ�����������Ż�DC��
		SelectObject(hdcmem, hOldFont);
	}

	// ���ڴ�DC�л��꣬һ������Ĵ���DC�ϡ�
	BitBlt(hdc,
		0, 0, rect.right - rect.left, rect.bottom - rect.top,
		hdcmem, 0, 0, SRCCOPY);

	/*******************************************************************************
	* #############  ���պ��ͷ���Դ  ################
	*
	*******************************************************************************/
	// ������Դ
	// DeleteObject �ͷ�GDI����
	DeleteObject(hbmMem);
	DeleteObject(hdcmem);
	DeleteObject(hPenBoundary);
	DeleteObject(hbrushChoice);
	DeleteObject(hpen);
	DeleteObject(hFont);
	/*******************************************************************************
	* #############  ReleaseDC ����  ################
	* �ͷ�ռ�õ�DC��ϵͳ��Դ��
	*******************************************************************************/
	ReleaseDC(hwnd, hdc);
}

void CreateMain(HWND hwnd, int boundary_x, int boundary_y)
{

	SetGameState(START);

	SetBoundary(boundary_x, boundary_y);
	PGAME_COORD pCoordBoundary = GetBoundary();

	// ������Ϸ�߽�
	rectBoundary.left = 10;
	rectBoundary.top = 10;
	rectBoundary.right = 10 + CELL_PIXEL*(pCoordBoundary->x + 1);
	rectBoundary.bottom = 10 + CELL_PIXEL*(pCoordBoundary->y + 1);


}

LONG CALLBACK MainWndProc(
	HWND hwnd, //
	UINT msg, // ��Ϣ
	WPARAM wParam, // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
	LPARAM lParam) // ��Ϣ��������ͬ����Ϣ�в�ͬ�����壬���MSDN��ÿ����Ϣ���ĵ�
{
	// ע�⣬��switch-case, ÿ��������������ã�ֻ�����뵽һ��case�С�
	switch (msg)
	{
		// �����ڱ�����ʱ���յ��ĵ�һ����Ϣ����WM_CREATE��
		// һ���յ������Ϣ��������У�������������һЩ��ʼ���Ĺ���
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

// �����ڲ���ʱ���е��߼�����
// ����Ϸֻʹ�õ��˼������Ҽ����ơ�
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