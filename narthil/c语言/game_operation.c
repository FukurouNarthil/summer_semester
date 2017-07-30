/************** 头文件 ****************/
#include <Windows.h>
#include "ball_and_board.h"

/************** 宏定义 ****************/
// 计时器ID。
#define TIMER_ID 12340
// 判断游戏是否达到升级条件
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0

/************** 全局变量 ****************/
// 计时器到时间隔
DWORD dwTimerElapse;
// 每提高一级，计时器时钟周期缩短的比例。
DOUBLE dbLevelSpeedupRatio;
// 此变量表示多少个积分上升一个级别
DWORD dwOneLevelScores;

// 保存计分，初始为0
int score = 0;
// 保存级别，初始为0
int level = 0;

int GetScore()
{
	return score;
}
int GetLevel()
{
	return level;
}

// 游戏的初始化，
// 创建游戏的内部数据结构和系统对象。
void CreateGame(HWND hwnd, // 主窗口句柄
	DWORD dwInitTimerElapse, //
	unsigned int one_level_scores,
	DOUBLE level_speedup_ratio,
	int boundary_x, int boundary_y,
	int init_x, int init_y,
	ballDirection init_dir,
	int init_board_x, int init_board_y,
	int init_board_len,
	int init_brick_x, int init_brick_y)
{
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);
	srand(ft.dwLowDateTime);

	dbLevelSpeedupRatio = level_speedup_ratio;
	dwTimerElapse = dwInitTimerElapse / 2;
	dwOneLevelScores = one_level_scores;

	// 设置游戏的边界
	SetBoundary(boundary_x, boundary_y);

	// 创建一个计时器
	// 每经过 dwTimerElapse 毫秒，hwnd窗口（主窗口）就会收到一个WM_TIMER消息。
	// 计时器是驱动本游戏进行的主要时间线。
	// dwTimerElapse变量影响游戏进行的快慢变化。
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);

	CreateBall(init_dir, init_x, init_y);

	//创建木板
	CreateBoard(init_board_x, init_board_y, init_board_len);

	//创建花花
	CreatrFlower();

	switch (GetGameState())
	{
	case LEVEL_1:
		//创建砖块
		CreateBricks(init_brick_x, init_brick_y);
		break;
	case LEVEL_2:
		CreateBricks_2(init_brick_x, init_brick_y);
		break;
	case LEVEL_3:
		SetTimer(hwnd, TIMER_ID, dwInitTimerElapse / 3, NULL);
		CreateBricks_3(init_brick_x, init_brick_y);
		break;
	}

}

// 游戏控制的一个主要流程。
// 当计时器发生时进行的处理逻辑。
void OnTimer(HWND hwnd)
{
	PGAME_COORD p = GetBallPosition();
	pBRICK pb = GetBrickPointer();
	int count = 0;
	/*计时器到时以后，球移动一步。
	根据以后以后的状态，进行后续处理。*/
	switch (BallMove())
	{
		// 如果球已经死了
	case BALL_DEAD:
		PlaySound(NULL, NULL, NULL);
		CreateThread(NULL, 0, GameOver, NULL, 0, NULL);
		// 首先关闭计时器，避免在计时器的驱动下再次调用本函数
		KillTimer(hwnd, TIMER_ID);
		// 然后通知玩家，Game Over了，退出进程。
		MessageBox(0, "Game Over", "Game Over", 0);
		ExitProcess(0);
		break;
		// 如果球打到了砖块
	case BALL_BREAK_BRICK:
		// 计分；
		if (isBallBreakBrick() == NORMAL)
		{
			CreateThread(NULL, 0, BreakBrick, NULL, 0, NULL);
			SetDirction(BALL_BREAK_BRICK, GetBallDirection());
			GamePaint(hwnd);
			DestroyBrick(p->x, p->y);
			score++;
		}
		else if (isBallBreakBrick() == HARD)
		{
			CreateThread(NULL, 0, HitFirstTime, NULL, 0, NULL);
			SetDirction(BALL_BREAK_BRICK, GetBallDirection());
			SetBrickType(NORMAL);
			GamePaint(hwnd);
			score++;
			count++;
			if (count == 2)
			{
				CreateThread(NULL, 0, BreakBrick, NULL, 0, NULL);
				SetDirction(BALL_BREAK_BRICK, GetBallDirection());
				GamePaint(hwnd);
				DestroyBrick(p->x, p->y);
				score += 2;
			}
		}
		else if (isBallBreakBrick() == DIAMOND)
		{
			CreateThread(NULL, 0, HitFirstTime, NULL, 0, NULL);
			SetDirction(BALL_BREAK_BRICK, GetBallDirection());
			SetBrickType	(HARD);
			GamePaint(hwnd);
			score++;
			count++;
			if (count == 2)
			{
				CreateThread(NULL, 0, HitFirstTime, NULL, 0, NULL);
				SetDirction(BALL_BREAK_BRICK, GetBallDirection());
				SetBrickType(NORMAL);
				GamePaint(hwnd);
				score++;
			}
			if (count == 3)
			{
				CreateThread(NULL, 0, BreakBrick, NULL, 0, NULL);
				SetDirction(BALL_BREAK_BRICK, GetBallDirection());
				GamePaint(hwnd);
				DestroyBrick(p->x, p->y);
				score += 5;
			}
		}
		if (!ListSize(GetBrick()))
		{
			PlaySound(NULL, NULL, NULL);
			CreateThread(NULL, 0, GameOver, NULL, 0, NULL);
			// 首先关闭计时器，避免在计时器的驱动下再次调用本函数
			KillTimer(hwnd, TIMER_ID);
			// 然后通知玩家，Game Over了，退出进程。
			MessageBox(0, "You Win!", "Game Over", 0);
			ExitProcess(0);
		}
		break;
	case BALL_MOVE:
		break;
	case BALL_HIT_BOARD:
		CreateThread(NULL, 0, HitBoard, NULL, 0, NULL);
		SetDirction(BALL_HIT_BOARD, GetBallDirection());
		break;
	case BALL_HIT_BOUNDARY_LEFT:
		SetDirction(BALL_HIT_BOUNDARY_LEFT, GetBallDirection());
		break;
	case BALL_HIT_BOUNDARY_RIGHT:
		SetDirction(BALL_HIT_BOUNDARY_RIGHT, GetBallDirection());
		break;
	case BALL_HIT_BOUNDARY_TOP:
		SetDirction(BALL_HIT_BOUNDARY_TOP, GetBallDirection());
		break;
		
	case BALL_GET_FLOWER:
		DestroyFlower();
		GamePaint(hwnd);
		PlaySound(NULL, NULL, NULL);
		CreateThread(NULL, 0, GameOver, NULL, 0, NULL);
		// 首先关闭计时器，避免在计时器的驱动下再次调用本函数
		KillTimer(hwnd, TIMER_ID);
		// 然后通知玩家，Game Over了，退出进程。
		PGAME_COORD p1 = GetBallPosition();
		PGAME_COORD p2 = GetFlower();
		/*DestroyFlower();*/
		MessageBox(0, "You Win!", "Game Over", 0);
		ExitProcess(0);
		/*DestroyFlower();*/
		break;
	default:
		break;
	}

	return;

}