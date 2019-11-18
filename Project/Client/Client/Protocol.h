#pragma once
#define  SERVERPORT 9000
// Ű�Է�
#define LEFT			'MOVE_LEFT'
#define RIGHT			'MOVE_RIGHT'
#define ATTACK			'BULLET_ATTACK'

// ���
#define SCENE_LOGO		'LogoScene'
#define SCENE_STAGE1	'Stage1Scene'
#define SCENE_STAGE2	'Stage2Scene'
#define SCENE_STAGE3	'Stage3Scene'

// �������� ���� ���ѽð��� ���� �� ���
typedef float GAMETIME;

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


	Player() {
		hp = 3;
		score = 0;
		bulletCount = 1;
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