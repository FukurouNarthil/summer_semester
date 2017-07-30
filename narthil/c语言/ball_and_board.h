#include <Windows.h>
#include "LinkList.h"

//�ж���Ϸ״̬
typedef enum _state
{
	START,
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
}GameState;

int gameState;

//���״̬
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

//��ķ���
typedef enum _dirction
{
	UP_LEFT,
	UP_RIGHT,
	UP_STRAIGHT,
	DOWN_LEFT,
	DOWN_RIGHT,
	DOWN_STRAIGHT,
}ballDirection;

//��ķ���
typedef enum _direction
{
	LEFT,
	RIGHT,
}boardDirection;

//ש���״̬
#define NORMAL 1
#define HARD 2
#define DIAMOND 3

// ��Ϸ����
typedef struct _GAME_COORD
{
	short x;
	short y;
}GAME_COORD, *PGAME_COORD;

//ש��
typedef struct _brick
{
	PGAME_COORD bCoord;
	int type;
}BRICK, *pBRICK;

//��Ч����
DWORD WINAPI PlayMusic(LPVOID lpParam);
DWORD WINAPI PlayMusic2(LPVOID lpParam);
DWORD WINAPI PlayMusic3(LPVOID lpParam);
DWORD WINAPI BreakBrick(LPVOID lpParam);
DWORD WINAPI HitBoard(LPVOID lpParam);
DWORD WINAPI ChooseButton(LPVOID lpParam);
DWORD WINAPI GameOver(LPVOID lpParam);
DWORD WINAPI StartPageBGM(LPVOID lpParam);
DWORD WINAPI HitFirstTime(LPVOID lpParam);

//������Ϸ״̬
void SetGameState(int state);

//�����Ϸ״̬
int GetGameState();


// ���ñ߽�����
void SetBoundary(int x, int y);

// ��ñ߽�����
PGAME_COORD GetBoundary();

//���ש������
PLIST GetBrick();

//�ı�ש�������
void SetBrickType(int type);

// ������
void CreateBall(ballDirection dir, int head_x, int head_y);

//���ƶ�һ�����ж��Ǽ���ǰ�������Ǵ�ש�����ǽ�ڻ���ľ�壬����������
int BallMove();

//�ж����Ƿ����ש��
int isBallBreakBrick();

//����ש��ָ��
pBRICK GetBrickPointer();

//�ж���Χש��
//int isBrick(int x, int y);

// �������ͷ��ڴ���Դ��
void DestroyBall();

// �ı����ƶ��ķ���
void SetDirction(int, ballDirection);

// ��õ�ǰ��ķ���
ballDirection GetBallDirection();

//��õ�ǰ�������
PGAME_COORD GetBallPosition();

//������
void CreateBoard(int init_board_x, int init_board_y, int init_board_len);

//�ı���ƶ��ķ���
void SetBoardDirection(boardDirection);

//��õ�ǰ�������
PGAME_COORD GetBoard();

//���ٰ�
void DestroyBoard();

//��������
void CreatrFlower();

//���ػ�������
PGAME_COORD GetFlower();

//���ٻ���
void DestroyFlower();

int isBallGetFlower();

//����ש��
void CreateBricks(int init_brick_x, int init_brick_y);
void CreateBricks_2(int init_brick_x, int init_brick_y);
void CreateBricks_3(int init_brick_x, int init_brick_y);

//����ש��
void DestroyBrick(int x, int y);

//���ĳש�������
PLIST GetBrick();

//GameOperator.c �еĽӿں���

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