#pragma once
#define  SERVERPORT 9000
// 키입력
#define LEFT			'MOVE_LEFT'
#define RIGHT			'MOVE_RIGHT'
#define ATTACK			'BULLET_ATTACK'

// 장면
#define SCENE_LOGO		'LogoScene'
#define SCENE_STAGE1	'Stage1Scene'
#define SCENE_STAGE2	'Stage2Scene'
#define SCENE_STAGE3	'Stage3Scene'

// 서버에서 게임 제한시간을 받을 때 사용
typedef float GAMETIME;

// 게임 제한시간
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


#pragma pack (pop)  // 위에 선언한 패킹상태를 원상태로 돌리는것