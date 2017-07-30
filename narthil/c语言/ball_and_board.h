#include <Windows.h>
#include "LinkList.h"

//判断游戏状态
typedef enum _state
{
	START,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
}GameState;

int gameState;

//球的状态
#define BALL_MOVE 0
#define BALL_DEAD 1
#define BALL_BREAK_BRICK 2
#define BALL_HIT_BOUNDARY_LEFT 3
#define BALL_HIT_BOUNDARY_RIGHT 4
#define BALL_HIT_BOARD 5
#define BALL_ERROR 6
#define BALL_HIT_BOUNDARY_TOP 7
#define BALL_HIT_EMPTY_BRICK 8
#define BALL_GET_FLOWER 9

//球的方向
typedef enum _dirction
{
	UP_LEFT,
	UP_RIGHT,
	UP_STRAIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	DOWN_STRAIGHT,
}ballDirection;

//板的方向
typedef enum _direction
{
	LEFT,
	RIGHT,
}boardDirection;

//砖块的状态
#define NORMAL 1
#define HARD 2
#define DIAMOND 3

// 游戏坐标
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//砖块
typedef struct _brick
{
	PGAME_COORD bCoord;
	int type;
}BRICK, *pBRICK;

//音效设置
DWORD WINAPI PlayMusic(LPVOID lpParam);
DWORD WINAPI PlayMusic2(LPVOID lpParam);
DWORD WINAPI PlayMusic3(LPVOID lpParam);
DWORD WINAPI BreakBrick(LPVOID lpParam);
DWORD WINAPI HitBoard(LPVOID lpParam);
DWORD WINAPI ChooseButton(LPVOID lpParam);
DWORD WINAPI GameOver(LPVOID lpParam);
DWORD WINAPI StartPageBGM(LPVOID lpParam);
DWORD WINAPI HitFirstTime(LPVOID lpParam);

//设置游戏状态
void SetGameState(int state);

//获得游戏状态
int GetGameState();


// 设置边界坐标
void SetBoundary(int x, int y);

// 获得边界坐标
PGAME_COORD GetBoundary();

//获得砖块坐标
PLIST GetBrick();

//改变砖块的属性
void SetBrickType(int type);

// 创建球
void CreateBall(ballDirection dir, int head_x, int head_y);

//球移动一步，判断是继续前进，还是打到砖块或者墙壁或者木板，或者球死亡
int BallMove();

//判断球是否击中砖块
int isBallBreakBrick();

//返回砖块指针
pBRICK GetBrickPointer();

//判断周围砖块
//int isBrick(int x, int y);

// 销毁球，释放内存资源。
void DestroyBall();

// 改变球移动的方向
void SetDirction(int, ballDirection);

// 获得当前球的方向
ballDirection GetBallDirection();

//获得当前球的坐标
PGAME_COORD GetBallPosition();

//创建板
void CreateBoard(int init_board_x, int init_board_y, int init_board_len);

//改变板移动的方向
void SetBoardDirection(boardDirection);

//获得当前板的坐标
PGAME_COORD GetBoard();

//销毁板
void DestroyBoard();

//创建花花
void CreatrFlower();

//返回花花坐标
PGAME_COORD GetFlower();

//销毁花花
void DestroyFlower();

int isBallGetFlower();

//创建砖块
void CreateBricks(int init_brick_x, int init_brick_y);
void CreateBricks_2(int init_brick_x, int init_brick_y);
void CreateBricks_3(int init_brick_x, int init_brick_y);

//销毁砖块
void DestroyBrick(int x, int y);

//获得某砖块的坐标
PLIST GetBrick();

//GameOperator.c 中的接口函数

int GetScore();
int GetLevel();
void CreateGame(HWND hwnd,
	DWORD dwInitTimerElapse,
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y,
	ballDirection init_dir,
	int init_board_x, int init_board_y,
	int init_borad_len,
	int init_brick_x, int init_brick_y);
void OnTimer(HWND hwnd);
void OnKeyDown(DWORD vk, HWND hwnd);