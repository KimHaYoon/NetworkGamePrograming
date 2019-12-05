#pragma once
#define  SERVERPORT 9000
// Ű�Է�
#define LEFT			'MOVE_LEFT'
#define RIGHT			'MOVE_RIGHT'
#define ATTACK			'BULLET_ATTACK'

// �� ����
#define DIR_RIGHT false
#define DIR_LEFT true
#define DIR_UP false
#define DIR_DOWN true

// �������� ���� ���ѽð��� ���� �� ���
typedef float GAMETIME;

enum GAME_STATE
{
	GAME_WAIT,
	GAME_START,
	GAME_STAGE1,
	GAME_STAGE2,
	GAME_STAGE3,
	GAME_END
};

// ���� ���ѽð�
#define LIMIT_GAMETIME 70;

#pragma pack(push ,1)

typedef struct Player
{
	int id;
	int x;
	int y;
	int hp;
	int score;
	int bulletCount;
	bool gameStart;
	bool invincibile;

	int maxFrame;

	Player() {
		hp = 3;
		score = 0;
		bulletCount = 1;
		maxFrame = 5;
		gameStart = true;
		invincibile = false;
	}

}PLAYERINFO;

typedef struct Keys
{
	int playerid;
	bool left;
	bool right;
	bool space;

	Keys() {
		left = false;
		right = false;
		space = false;
	}
}PLAYERKEYINFO;

typedef struct ServerPlayerInfo
{
	PLAYERINFO info;
	SOCKET socket;
	PLAYERKEYINFO keys;
	float	invincibileTime;
}SERVERPLAYER;

typedef struct Bullet
{
	int id;
	int x;
	int y;
	int height;
	bool shot;

	Bullet()
	{
		shot = false;
	}
}BULLETINFO;

typedef struct Ball
{
	int x;
	int y;
	int type;
	int radius;
}BALLINFO;

typedef struct ServerBallInfo
{
	BALLINFO info;
	bool	xDir;
	bool	yDir;
	bool	live;
}SERVERBALLINFO;

typedef struct Item
{
	int x;
	int y;
	int type;
}ITEMINFO;

typedef struct Tile
{
	int id;
	int x;
	int y;

	int cx; // ������
	int cy; // ������

	int type; // 0 ���� 1 �μ��� 2 �Ⱥμ���
	int color; // 0 ��� 1 ���ο�

	bool animation;
	int maxFrame;
}TILEINFO;
#pragma pack (pop)  // ���� ������ ��ŷ���¸� �����·� �����°�