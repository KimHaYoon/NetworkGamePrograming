#pragma once
#define  SERVERPORT 9000
// Ű�Է�
#define LEFT			'MOVE_LEFT'
#define RIGHT			'MOVE_RIGHT'
#define ATTACK			'BULLET_ATTACK'

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
	int dir;
	int hp;
	int score;
	int bulletCount;
	bool gameStart;
	bool moveAnimation;
	bool shoot;

	int maxFrame;
	int nowFrame;

	Player() {
		hp = 3;
		score = 0;
		bulletCount = 1;
		maxFrame = 5;
		nowFrame = 0;
		gameStart = true;
		moveAnimation = false;
		shoot = false;
	}

}PLAYERINFO;

typedef struct Bullet
{
	int id;
	int x;
	int y;
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
	int dir;
	char type;
	int radius;
}BALLINFO;

typedef struct Item
{
	int x;
	int y;
	char type;
}ITEMINFO;


#pragma pack (pop)  // ���� ������ ��ŷ���¸� �����·� �����°�