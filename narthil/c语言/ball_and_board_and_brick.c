
#include "ball_and_board.h"

/************* ȫ�ֱ��� ****************/
// ש��
PLIST brick;
pBRICK pbrick;
//��
GAME_COORD ball_pos;
// ���ƶ�����
ballDirection ball_dir;
// ��Ϸ�߽磨�������£�����Ϊ0,0��
GAME_COORD boundary;

//���ƶ�����
boardDirection board_dir;
//������
GAME_COORD board_pos;
//�峤��
int board_len;

//������
GAME_COORD flower_pos;

//������ש���ɧ����
int Rabbit[12][21] = 
{ 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };

/*********** �ڲ��������� **************/
/// �ж����������Ƿ���ȡ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two);

/**************��������****************/
DWORD WINAPI PlayMusic(LPVOID lpParam)
{
	PlaySound(TEXT("GOATBED - DMMd re BGM12.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return 0;
}
DWORD WINAPI PlayMusic2(LPVOID lpParam)
{
	PlaySound(TEXT("GOATBED - DMMd re BGM01.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return 0;
}
DWORD WINAPI PlayMusic3(LPVOID lpParam)
{
	PlaySound(TEXT("GOATBED - DMMd re BGM03.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return 0;
}
DWORD WINAPI BreakBrick(LPVOID lpParam)
{
	mciSendString("close break", 0, 0, 0);
	mciSendString(TEXT("open BreakBrick.wav alias break "), NULL, 0, NULL);
	mciSendString("play break ", 0, 0, 0);
}
DWORD WINAPI HitBoard(LPVOID lpParam)
{
	mciSendString("close hit", 0, 0, 0);
	mciSendString(TEXT("open HitBoard.wav alias hit "), NULL, 0, NULL);
	mciSendString("play hit ", 0, 0, 0);
}
DWORD WINAPI ChooseButton(LPVOID lpParam)
{
	mciSendString("close choose", 0, 0, 0);
	mciSendString(TEXT("open ChoiceButton.wav alias choose "), NULL, 0, NULL);
	mciSendString("play choose ", 0, 0, 0);
}
DWORD WINAPI GameOver(LPVOID lpParam)
{
	mciSendString("close dead", 0, 0, 0);
	mciSendString(TEXT("open GameOver.wav alias dead "), NULL, 0, NULL);
	mciSendString("play dead ", 0, 0, 0);
}
DWORD WINAPI StartPageBGM(LPVOID lpParam)
{
	PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	return 0;
}
DWORD WINAPI HitFirstTime(LPVOID lpParam)
{
	mciSendString("close hitOne", 0, 0, 0);
	mciSendString(TEXT("open CANNOT.wav alias hitOne "), NULL, 0, NULL);
	mciSendString("play hitOne ", 0, 0, 0);
}


// �ж��������� GAME_COORD �Ƿ��غ�
int CoordEqual(PGAME_COORD one, PGAME_COORD two)
{
	if (one->x == two->x && one->y == two->y)
		return 1;
	return 0;
}

//������Ϸ״̬
void SetGameState(int state)
{
	gameState = state;
}

//�����Ϸ״̬
int GetGameState()
{
	return gameState;
}


// ��ñ߽�����
PGAME_COORD GetBoundary()
{
	return &boundary;
}
// ���ñ߽�����
void SetBoundary(int x, int y)
{
	boundary.x = x;
	boundary.y = y;
}

//������
void CreateBoard(int init_board_x, int init_board_y, int init_board_len)
{
	board_pos.x = init_board_x;
	board_pos.y = init_board_y;
	board_len = init_board_len;
}
//���ð���ƶ�����
void SetBoardDirection(boardDirection bd)
{
	board_dir = bd;
	if (bd == LEFT)
	{
		if (board_pos.x != boundary.x - 22)
		{
			board_pos.x--;
		}
	}
	else
	{
		if (board_pos.x != boundary.x - board_len + 1)
		{
			board_pos.x++;
		}
	}
}
//��ð嵱ǰ����
PGAME_COORD GetBoard()
{
	return &board_pos;
}

//��������
void CreatrFlower()
{
	PGAME_COORD p;
	int i, j;
	switch (GetGameState())
	{
	case 1:
		flower_pos.x = rand() % 21 + 1;
		flower_pos.y = rand() % 3 + 1;
		break;
	case 2:
		flower_pos.x = rand() % 21 + 1;
		flower_pos.y = rand() % 3 + 1;
		break;
	case 3:
		i = flower_pos.x = rand() % 21 + 1;
		j = flower_pos.y = rand() % 12 + 1;
		if (!Rabbit[i][j])
		{
			if (i <= 8)
			{
				for(i; Rabbit[i][j] = 0; i++)
				{
					if (Rabbit[i][j])
					{
						break;
					}
				}
				flower_pos.x = i;
			}
			else if (i >= 17)
			{
				for (i; Rabbit[i][j] = 0; i--)
				{
					if (Rabbit[i][j])
					{
						break;
					}
				}
				flower_pos.x = i;
			}
		}
		break;
	}
	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	pbrick = (pBRICK)malloc(sizeof(BRICK));
	p->x = flower_pos.x;
	p->y = flower_pos.y;
	pbrick->bCoord = p;
	pbrick->type = NORMAL;
	ListPushBack(brick, pbrick);
}

//���ػ�������
PGAME_COORD GetFlower()
{
	return &flower_pos;
}

//���ٻ���
void DestroyFlower()
{
	PGAME_COORD p1 = GetBallPosition();
	PGAME_COORD p2 = GetFlower();
	int size = ListSize(brick);
	int i;
	for (i = 0; i < size; i++)
	{
		if (CoordEqual(p1, p2))
		{
			(pBRICK)ListDeleteAt(brick, i);
			/*free((PGAME_COORD)ListDeleteAt(brick, i));*/
			break;
		}
	}
}

int isBallGetFlower()
{
	PGAME_COORD p1 = GetBallPosition();
	PGAME_COORD p2 = GetFlower();
	if (CoordEqual(p1, p2))
		return 1;
	return 0;
}

//����ĳ��ש��ָ��
pBRICK GetBrickPointer()
{
	return pbrick;
}
//����ש��
void CreateBricks(int init_brick_x, int init_brick_y)
{
	PGAME_COORD p;
	brick = ListCreate(0);
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 21; j++)
		{
			p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
			pbrick = (pBRICK)malloc(sizeof(BRICK));
			p->x = init_brick_x + j;
			p->y = init_brick_y + i;
			if (!CoordEqual(p, GetFlower()))
			{
				pbrick->bCoord = p;
				pbrick->type = NORMAL;
				ListPushBack(brick, pbrick);
			}
			
		}

	return;
}

//����ש��
void DestroyBrick(int x, int y)
{
	PGAME_COORD p = GetBallPosition();
	p->x = x;
	p->y = y;
	int size = ListSize(brick);
	int i;
	for (i = 0; i < size; i++)
	{
		pbrick = (pBRICK)ListGetAt(brick, i);
		if (CoordEqual(p, pbrick->bCoord))
		{
			(pBRICK)ListDeleteAt(brick, i);
			/*free((PGAME_COORD)ListDeleteAt(brick, i));*/
			break;
		}

	}

}
//����ש�����ڵ�����
PLIST GetBrick()
{
	return brick;
}
//�ı�ש�������
void SetBrickType(int type)
{
	pbrick->type = type;
}
void CreateBricks_2(int init_brick_x, int init_brick_y)
{
	PGAME_COORD p;
	brick = ListCreate(0);
	int i, j;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 21; j++)
		{
			p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
			pbrick = (pBRICK)malloc(sizeof(BRICK));
			p->x = init_brick_x + j;
			p->y = init_brick_y + i;
			if (!CoordEqual(p, GetFlower()))
			{
				pbrick->bCoord = p;
				pbrick->type = rand() % 3 + 1;
				ListPushBack(brick, pbrick);
			}
		}
}
void CreateBricks_3(int init_brick_x, int init_brick_y)
{
	PGAME_COORD p;
	brick = ListCreate(0);
	int i, j;
	for (i = 0; i < 12; i++)
		for (j = 0; j < 21; j++)
		{
			if (Rabbit[i][j] == 1)
			{
				p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
				pbrick = (pBRICK)malloc(sizeof(BRICK));
				p->x = init_brick_x + j;
				p->y = init_brick_y + i;
				if (!CoordEqual(p, GetFlower()))
				{
					pbrick->bCoord = p;
					pbrick->type = rand() % 3 + 1;
					ListPushBack(brick, pbrick);
				}
			}
		}
}

//������
void CreateBall(ballDirection dir, int head_x, int head_y)
{
	ball_pos.x = head_x;
	ball_pos.y = head_y;
	ball_dir = dir;
}
//��õ�ǰ�������
PGAME_COORD GetBallPosition()
{
	return &ball_pos;
}
//���ƶ�һ�����ж��Ǽ���ǰ�������Ǵ�ש�����ǽ�ڻ���ľ�壬����������
int BallMove()
{
	switch (ball_dir)
	{
	case UP_LEFT:
		ball_pos.x--;
		ball_pos.y--;
		break;
	case UP_RIGHT:
		ball_pos.x++;
		ball_pos.y--;
		break;
	case DOWN_LEFT:
		ball_pos.x--;
		ball_pos.y++;
		break;
	case DOWN_RIGHT:
		ball_pos.x++;
		ball_pos.y++;
		break;
	case UP_STRAIGHT:
		ball_pos.y--;
		break;
	case DOWN_STRAIGHT:
		ball_pos.y++;
		break;
	default:
		break;
	}

	if (ball_pos.y > 21 && ball_pos.x != 0 && ball_pos.x != 22)
	{
		return BALL_DEAD;
	}
	else if (ball_pos.x <= 0)
	{
		return BALL_HIT_BOUNDARY_LEFT;
	}
	else if (ball_pos.x >= 22)
	{
		return BALL_HIT_BOUNDARY_RIGHT;
	}
	else if (ball_pos.y == 20 && ball_pos.x < board_pos.x + board_len && ball_pos.x != 0 && ball_pos.x != 22)
	{
		return BALL_HIT_BOARD;
	}
	else if (isBallBreakBrick())
	{
		return BALL_BREAK_BRICK;

	}
	else if (isBallGetFlower())
	{
		return BALL_GET_FLOWER;
	}
	else if (ball_pos.y == 0)
	{
		return BALL_HIT_BOUNDARY_TOP;
	}
	/*else if (isBallHitEmptyBrick(ball_pos.x, ball_pos.y))
	{
		return BALL_HIT_EMPTY_BRICK;
	}*/
	else
	{
		return BALL_MOVE;
	}
}
//��õ�ǰ��ķ���
ballDirection GetBallDirection()
{
	return ball_dir;
}
//�ı���ķ���
void SetDirction(int state, ballDirection bd)
{
	switch (state)
	{
	case 2:
		if (bd == UP_LEFT)
		{
			ball_dir = DOWN_LEFT;
		}
		else if (bd == UP_RIGHT)
		{
			ball_dir = DOWN_RIGHT;
		}
		else if (bd == DOWN_LEFT)
		{
			ball_dir = UP_LEFT;
		}
		else if (bd == UP_STRAIGHT)
		{
			ball_dir = rand() % 3 + 3;
		}
		else
		{
			ball_dir = UP_RIGHT;
		}
		break;
	case 3:
		if (bd == UP_LEFT)
		{
			ball_dir = UP_RIGHT;
		}
		else if (bd == DOWN_LEFT)
		{
			ball_dir = DOWN_RIGHT;
		}
		break;
	case 4:
		if (bd == UP_RIGHT)
		{
			ball_dir = UP_LEFT;
			ball_dir = UP_LEFT;
		}
		else if (bd == DOWN_RIGHT)
		{
			ball_dir = DOWN_LEFT;
		}
		break;
	case 5:
		ball_dir = rand() % 3;
		break;
	case 7:
		if (bd == UP_LEFT)
		{
			ball_dir = DOWN_LEFT;
		}
		else if (bd == UP_RIGHT)
		{
			ball_dir = DOWN_RIGHT;
		}
		else if (bd == UP_STRAIGHT)
		{
			ball_dir = rand() % 3 + 3;
		}
		break;
	case 9:
		if (bd == UP_LEFT)
		{
			ball_dir = DOWN_LEFT;
		}
		else if (bd == UP_RIGHT)
		{
			ball_dir = DOWN_RIGHT;
		}
		else if (bd == DOWN_LEFT)
		{
			ball_dir = UP_LEFT;
		}
		else if (bd == UP_STRAIGHT)
		{
			ball_dir = rand() % 3 + 3;
		}
		else
		{
			ball_dir = UP_RIGHT;
		}
		break;
	default:
		break;
	}
}
//�ж�С���Ƿ����ש��
int isBallBreakBrick()
{
	int i, len = ListSize(brick);
	PGAME_COORD p;
	p = (PGAME_COORD)malloc(sizeof(GAME_COORD));
	for (i = 0; i < len; i++)
	{
		pbrick = (pBRICK)ListGetAt(brick, i);
		p = pbrick->bCoord;
		if (ball_pos.x == p->x && ball_pos.y == p->y)
		{
			return pbrick->type;
			break;
		}
	}
	return 0;
}

