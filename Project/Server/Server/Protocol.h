#pragma once
#define  SERVERPORT 9000
// 키입력
#define LEFT			'MOVE_LEFT'
#define RIGHT			'MOVE_RIGHT'
#define ATTACK			'BULLET_ATTACK'

// 서버에서 게임 제한시간을 받을 때 사용
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

// 게임 제한시간
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

	int maxFrame;

	Player() {
		hp = 3;
		score = 0;
		bulletCount = 1;
		maxFrame = 5;
		gameStart = true;
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
	char type;
	int radius;
}BALLINFO;

typedef struct ServerBallInfo
{
	BALLINFO info;
	bool	xDir;
	bool	yDir;
}SERVERBALLINFO;

typedef struct Item
{
	int x;
	int y;
	char type;
}ITEMINFO;

typedef struct Tile
{
	int id;
	int x;
	int y;

	int cx; // 가로폭
	int cy; // 세로폭

	int type; // 0 없음 1 부서짐 2 안부서짐
	int color; // 0 블루 1 옐로우

	int nowFrame;
	int maxFrame;
}TILEINFO;
#pragma pack (pop)  // 위에 선언한 패킹상태를 원상태로 돌리는것