/************** ͷ�ļ� ****************/
#include <Windows.h>
#include "ball_and_board.h"

/************** �궨�� ****************/
// ��ʱ��ID��
#define TIMER_ID 12340
// �ж���Ϸ�Ƿ�ﵽ��������
#define IS_SPEEDUP(s) (s % dwOneLevelScores) == 0

/************** ȫ�ֱ��� ****************/
// ��ʱ����ʱ���
DWORD dwTimerElapse;
// ÿ���һ������ʱ��ʱ���������̵ı�����
DOUBLE dbLevelSpeedupRatio;
// �˱�����ʾ���ٸ���������һ������
DWORD dwOneLevelScores;

// ����Ʒ֣���ʼΪ0
int score = 0;
// ���漶�𣬳�ʼΪ0
int level = 0;

int GetScore()
{
	return score;
}
int GetLevel()
{
	return level;
}

// ��Ϸ�ĳ�ʼ����
// ������Ϸ���ڲ����ݽṹ��ϵͳ����
void CreateGame(HWND hwnd, // �����ھ��
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

	// ������Ϸ�ı߽�
	SetBoundary(boundary_x, boundary_y);

	// ����һ����ʱ��
	// ÿ���� dwTimerElapse ���룬hwnd���ڣ������ڣ��ͻ��յ�һ��WM_TIMER��Ϣ��
	// ��ʱ������������Ϸ���е���Ҫʱ���ߡ�
	// dwTimerElapse����Ӱ����Ϸ���еĿ����仯��
	SetTimer(hwnd, TIMER_ID, dwTimerElapse, NULL);

	CreateBall(init_dir, init_x, init_y);

	//����ľ��
	CreateBoard(init_board_x, init_board_y, init_board_len);

	//��������
	CreatrFlower();

	switch (GetGameState())
	{
	case LEVEL_1:
		//����ש��
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

// ��Ϸ���Ƶ�һ����Ҫ���̡�
// ����ʱ������ʱ���еĴ����߼���
void OnTimer(HWND hwnd)
{
	PGAME_COORD p = GetBallPosition();
	pBRICK pb = GetBrickPointer();
	int count = 0;
	/*��ʱ����ʱ�Ժ����ƶ�һ����
	�����Ժ��Ժ��״̬�����к�������*/
	switch (BallMove())
	{
		// ������Ѿ�����
	case BALL_DEAD:
		PlaySound(NULL, NULL, NULL);
		CreateThread(NULL, 0, GameOver, NULL, 0, NULL);
		// ���ȹرռ�ʱ���������ڼ�ʱ�����������ٴε��ñ�����
		KillTimer(hwnd, TIMER_ID);
		// Ȼ��֪ͨ��ң�Game Over�ˣ��˳����̡�
		MessageBox(0, "Game Over", "Game Over", 0);
		ExitProcess(0);
		break;
		// ��������ש��
	case BALL_BREAK_BRICK:
		// �Ʒ֣�
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
			// ���ȹرռ�ʱ���������ڼ�ʱ�����������ٴε��ñ�����
			KillTimer(hwnd, TIMER_ID);
			// Ȼ��֪ͨ��ң�Game Over�ˣ��˳����̡�
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
		// ���ȹرռ�ʱ���������ڼ�ʱ�����������ٴε��ñ�����
		KillTimer(hwnd, TIMER_ID);
		// Ȼ��֪ͨ��ң�Game Over�ˣ��˳����̡�
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