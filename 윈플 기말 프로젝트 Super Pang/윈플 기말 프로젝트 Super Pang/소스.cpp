#include "stdafx.h"
#include "resource.h"
#include "Player.h"
#include "Define.h"

static RECT rt; // 윈도우 사이즈

HINSTANCE g_hInst;
LPCTSTR lpszClass = "SUPER PANG";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevinstance, LPSTR lpszCmdParam, int nCmdshow) {
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClass);

	RECT rcWindow = { 0, 0, WINCX, WINCY};

	AdjustWindowRect(&rcWindow, WS_OVERLAPPEDWINDOW, false);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, 300, 0, 
		rcWindow.right, rcWindow.bottom, NULL, (HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdshow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) 
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;
}

typedef enum DirOne
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR1_END
}DIR_ONE;
typedef enum DirTwo
{
	DIR_UP,
	DIR_DOWN,
	DIR2_END
}DIR_TWO;

typedef struct tagFrame
{
	int NowFrame;
	int MaxFrame;
}FRAME;

typedef struct tagTile   //타일
{
	int X; // X좌표
	int Y; // Y좌표

	int CX;  // 타일의 가로 폭
	int CY;	 // 타일의 세로 폭
	int type; // 0없음 1부서짐 2안부서짐

	FRAME tFrame;

	RECT rc;  //타일 렉트

	void SetRect(void)  // 타이머에서 움직일 때마다 갱신
	{
		rc.left = X - CX / 2;
		rc.top = Y - CY / 2;
		rc.right = X + CX / 2;
		rc.bottom = Y + CY / 2;
	}
}TILE;

typedef struct tagBall
{
	int X; // X좌표
	int Y; // Y좌표

	int R; //반지름 
	int type; // 볼의타입(0 R:15 / 1 R:30 / 2 R:50)
	int gravity;

	DIR_ONE DirLR;
	DIR_TWO DirUD;

	RECT rc;

	void SetRect(void)  // 타이머에서 움직일 때마다 갱신해주려고 씀
	{
		rc.left = X - (R - 3);
		rc.top = Y - (R - 3);
		rc.right = X + (R - 3);
		rc.bottom = Y + (R - 3);
	}
}BALL;


LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam)
{
	static int logo_count = 0; // Press Enter Key 카운트
	static RECT rcTemp = { 0, 0, 0, 0 }; //충돌전용 RECT
	static RECT Missile_P1[5], Missile_P2[5]; // 2P니 난이도를 위해서 한발만 가지고 사용하기로
	static CImage img, blackimg, Logo, Key, Missile, timer, Number, Number2, Alphabet, Number3, Mitem, img2, GameOver, Ending, img3, Life;
	static HBITMAP hBit, hBlock, hBall1, hBall2, hBall3, hBrokenBlock, oldBit;
	static HDC hdc, mem1dc, blockdc, ball1dc, ball2dc, ball3dc, brokenblockdc;
	static BOOL Start; // false면 로고띄우기 true면 게임시작
	static BOOL Stop = false, Stop_item = false;
	static BOOL T; // 시간이 다되었을때
	static BOOL M = false;
	static BOOL H; // 생명추가
	static RECT timer_item; // 시간정지 아이템
	static RECT Missile_item, Life_item;
	static HPEN hPen, oldPen;
	static int Time_item_count = 0; // 시간 정지 카운트
	static int Time_count, check_time; // 시간 카운트
	static int Time1_count;
	static int Stage, stage_count, gameover_count;
	static BOOL Gameover, End;
	PAINTSTRUCT ps;

	//플레이어
	static Player myPlayer[PLAYER_END];

	//타일
	static const int TILECNT_ONE = 3;
	static TILE TileOne[TILECNT_ONE];

	//볼
	static BALL ball[4][4];

	switch (IMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		SetTimer(hWnd, 1, 60, NULL);
		SetTimer(hWnd, 2, 40, NULL);		//충돌전용타이머
		SetTimer(hWnd, 3, 50, NULL);		//공움직임 전용 타이머
		Number.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP38));
		Number3.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP41));
		Mitem.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP42));
		Life.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP43));
		Alphabet.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP39));
		Number2.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP40));
		img.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		img2.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP45));
		img3.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP48));
		blackimg.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		Logo.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		Key.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		Missile.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP29));
		timer.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP35));
		hBlock = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP30));
		hBall1 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP31));
		hBall2 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP32));
		hBall3 = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP33));
		hBrokenBlock = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP34));
		GameOver.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP46));
		Ending.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP47));
		Stage = 1;

		myPlayer[PLAYER_1].Initalize(g_hInst, PLAYER_1);
		myPlayer[PLAYER_2].Initalize(g_hInst, PLAYER_2);

		myPlayer[PLAYER_1].SetPlay(true);

		T = false;
		Start = false;

		//타일
		TileOne[0].X = 200;
		TileOne[0].Y = 400;
		TileOne[1].X = 800;
		TileOne[1].Y = 400;
		TileOne[2].X = 500;
		TileOne[2].Y = 300;
		for (int i = 0; i < TILECNT_ONE; ++i)
		{
			TileOne[i].tFrame.MaxFrame = 5;
			TileOne[i].tFrame.NowFrame = 0;

			TileOne[i].type = 1;
			TileOne[i].CX = 100;
			TileOne[i].CY = 40;
			TileOne[i].SetRect();
		}

		for (int j = 0; j < 4; ++j)
		{
			for (int i = 0; i < 4; ++i)
			{
				ball[j][i].type = 4;
			}
		}

		//볼(사이즈 50, 30, 15)
		ball[0][0].X = 200;
		ball[0][0].Y = 250;
		ball[0][0].R = 25;
		ball[0][0].type = 2;
		ball[0][0].SetRect();
		ball[0][0].DirLR = DIR_RIGHT;
		ball[0][0].DirUD = DIR_DOWN;
		ball[0][0].gravity = 0;

		ball[1][0].X = 800;
		ball[1][0].Y = 250;
		ball[1][0].R = 25;
		ball[1][0].type = 2;
		ball[1][0].SetRect();
		ball[1][0].DirLR = DIR_RIGHT;
		ball[1][0].DirUD = DIR_DOWN;
		ball[1][0].gravity = 0;

		PlaySound("Opening.wav", NULL, SND_ASYNC | SND_LOOP);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		mem1dc = CreateCompatibleDC(hdc);
		SelectObject(mem1dc, hBit);

		BitBlt(hdc, 0, 0, rt.right, rt.bottom, mem1dc, 0, 0, SRCCOPY);

		DeleteDC(mem1dc);
		EndPaint(hWnd, &ps);
		break;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			hdc = GetDC(hWnd);
			if (hBit == NULL)
			{
				hBit = CreateCompatibleBitmap(hdc, rt.right, rt.bottom);
			}
			mem1dc = CreateCompatibleDC(hdc);
			blockdc = CreateCompatibleDC(hdc);
			ball1dc = CreateCompatibleDC(hdc);
			ball2dc = CreateCompatibleDC(hdc);
			ball3dc = CreateCompatibleDC(hdc);
			brokenblockdc = CreateCompatibleDC(hdc);
			SelectObject(mem1dc, hBit);
			SelectObject(blockdc, hBlock);
			SelectObject(ball1dc, hBall1);
			SelectObject(ball2dc, hBall2);
			SelectObject(ball3dc, hBall3);
			SelectObject(brokenblockdc, hBrokenBlock);

			if (Start) // 엔터키를 누르고나서.
			{
				blackimg.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, blackimg.GetWidth(), blackimg.GetHeight(), SRCCOPY); // 검은 배경
				if (Stage == 1)
				{
					img.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY); // 스테이지 출력
				}
				else if (Stage == 2)
				{
					img2.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img2.GetWidth(), img2.GetHeight(), SRCCOPY); // 스테이지 출력
				}
				else if (Stage == 3)
				{
					img3.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img3.GetWidth(), img3.GetHeight(), SRCCOPY); // 스테이지 출력
				}

				for (int i = 0; i < 5; ++i)
				{
					for (int player = PLAYER_1; player < PLAYER_END; ++player)
					{
						if (myPlayer[player].GetBulletShoot(i)) // 작살
						{
							RECT position = myPlayer[player].GetBulletPosition(i);
							int height = myPlayer[player].GetBulletHeight(i);
							int imageheight = myPlayer[player].GetBulletImageHeight(i);

							Missile.TransparentBlt(mem1dc, position.left, position.top, 30, height, 0, 0, 30, imageheight, RGB(0, 0, 0));
						}
					}
				}

				for (int player = PLAYER_1; player < PLAYER_END; ++player)
				{
					myPlayer[player].Update(g_hInst, mem1dc);
					if (!myPlayer[player].GetPlay())
					{
						if (gameover_count < 3 && player == PLAYER_1)
						{
							Alphabet.TransparentBlt(mem1dc, 15, STAGE_Y + 45, 50, 56, 6 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 65, STAGE_Y + 45, 50, 56, 0 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 115, STAGE_Y + 45, 50, 56, 12 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 165, STAGE_Y + 45, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 15, STAGE_Y + 95, 50, 56, 14 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 65, STAGE_Y + 95, 50, 56, 20 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 115, STAGE_Y + 95, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, 165, STAGE_Y + 95, 50, 56, 17 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
						}
						else if (gameover_count < 3 && player == PLAYER_2)
						{
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 300, STAGE_Y + 45, 50, 56, 6 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 350, STAGE_Y + 45, 50, 56, 0 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 400, STAGE_Y + 45, 50, 56, 12 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 450, STAGE_Y + 45, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 300, STAGE_Y + 95, 50, 56, 14 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 350, STAGE_Y + 95, 50, 56, 20 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 400, STAGE_Y + 95, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
							Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 450, STAGE_Y + 95, 50, 56, 17 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
						}
					}

				}

				if(T)
					timer.TransparentBlt(mem1dc, 480, 660, 60, 60, 0, 0, 30, 30, RGB(255, 0, 255));

				//스테이지 글씨 출력
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 125, STAGE_Y + 25, 50, 56, 18 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 85, STAGE_Y + 25, 50, 56, 19 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 50, STAGE_Y + 25, 50, 56, 0 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 5, STAGE_Y + 25, 50, 56, 6 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 40, STAGE_Y + 25, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				// Number3.TransparentBlt(mem1dc, rt.right / 2 + 75, STAGE_Y + 25, 45, 50, 8 * (Number3.GetWidth() / 10), 0, Number3.GetWidth() / 10, Number3.GetHeight(), RGB(255, 0, 255));
				Number2.TransparentBlt(mem1dc, rt.right / 2 + 80, STAGE_Y + 25, 45, 50, Stage * (Number2.GetWidth() / 10), 0, Number2.GetWidth() / 10, Number2.GetHeight(), RGB(255, 0, 255));// Stage Number 출력

				//Time
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 125, STAGE_Y + 80, 50, 56, 19 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 90, STAGE_Y + 80, 50, 56, 8 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 55, STAGE_Y + 80, 50, 56, 12 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 10, STAGE_Y + 80, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Number.TransparentBlt(mem1dc, 585, 607, 30, 36, (9 - Time_count) * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255)); // 시간 충력
				if(Stage == 1)
					Number.TransparentBlt(mem1dc, 550, 607, 30, 36, (STAGE1_LIMIT_TIME - Time1_count) * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
				else if(Stage == 2)
					Number.TransparentBlt(mem1dc, 550, 607, 30, 36, (STAGE2_LIMIT_TIME - Time1_count) * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
				else if(Stage == 3)
					Number.TransparentBlt(mem1dc, 550, 607, 30, 36, (STAGE3_LIMIT_TIME - Time1_count) * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));


				if (Stop_item)
				{
					timer.TransparentBlt(mem1dc, timer_item.left, timer_item.top, 30, 30, 0, 0, 30, 30, RGB(255, 0, 255));
				}
				if (M)
				{
					Mitem.StretchBlt(mem1dc, Missile_item.left, Missile_item.top, 30, 30, 0, 0, Mitem.GetWidth(), Mitem.GetHeight(), SRCCOPY);
				}
				if (H)
				{
					Life.TransparentBlt(mem1dc, Life_item.left, Life_item.top, 30, 30, 0, 0, Life.GetWidth(), Life.GetHeight(), RGB(27, 24, 24));
				}

				//타일출력
				for (int i = 0; i < TILECNT_ONE; ++i)
				{
					switch (TileOne[i].type)
					{
					case 0:
						break;
					case 1:
						TransparentBlt(mem1dc,
							TileOne[i].rc.left, TileOne[i].rc.top,
							TileOne[i].CX, TileOne[i].CY,
							brokenblockdc,
							55 * TileOne[i].tFrame.NowFrame, 0,
							55 * (TileOne[i].tFrame.NowFrame + 1), 26,
							RGB(0, 0, 0));
						break;
					case 2:
						TransparentBlt(mem1dc,
							TileOne[i].rc.left, TileOne[i].rc.top,
							TileOne[i].CX, TileOne[i].CY,
							blockdc,
							0, 0,
							51, 28,
							RGB(0, 0, 0));
						break;
					}
				}

				//공출력
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(mem1dc, hPen);
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						switch (ball[j][i].type)
						{
						case 0:
							if (Stop)
							{
								Ellipse(mem1dc, ball[j][i].rc.left, ball[j][i].rc.top, ball[j][i].rc.left + 16, ball[j][i].rc.top + 16);
							}
							TransparentBlt(mem1dc,
								ball[j][i].rc.left, ball[j][i].rc.top,
								2 * ball[j][i].R, 2 * ball[j][i].R,
								ball3dc,
								0, 0,
								15, 15,
								RGB(0, 0, 0));
							break;
						case 1:
							if (Stop)
							{
								Ellipse(mem1dc, ball[j][i].rc.left, ball[j][i].rc.top, ball[j][i].rc.left + 31, ball[j][i].rc.top + 31);
							}
							TransparentBlt(mem1dc,
								ball[j][i].rc.left, ball[j][i].rc.top,
								2 * ball[j][i].R, 2 * ball[j][i].R,
								ball2dc,
								0, 0,
								30, 30,
								RGB(0, 0, 0));
							break;
						case 2:
							if (Stop)
							{
								Ellipse(mem1dc, ball[j][i].rc.left, ball[j][i].rc.top, ball[j][i].rc.left + 51, ball[j][i].rc.top + 51);
							}
							TransparentBlt(mem1dc,
								ball[j][i].rc.left, ball[j][i].rc.top,
								2 * ball[j][i].R, 2 * ball[j][i].R,
								ball1dc,
								0, 0,
								50, 50,
								RGB(0, 0, 0));
							break;
						}
					}
				}
				SelectObject(mem1dc, oldPen);
				DeleteObject(hPen);
			}
			else // 처음 시작할때
			{
				if (Gameover)
				{
					GameOver.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, GameOver.GetWidth(), GameOver.GetHeight(), SRCCOPY);
				}
				else if (End)
				{
					Ending.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, Ending.GetWidth(), Ending.GetHeight(), SRCCOPY);
				}
				else
				{
					Logo.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, Logo.GetWidth(), Logo.GetHeight(), SRCCOPY); // 로고 출력
					if (logo_count >= 3 && logo_count <= 4)
					{
						Key.TransparentBlt(mem1dc, 380, 600, Key.GetWidth(), Key.GetHeight(), 0, 0, Key.GetWidth(), Key.GetHeight(), RGB(0, 0, 0));
					}
					logo_count++;
					logo_count = logo_count % 5; // Enter Key를 누르라는 문구가 나오는 주기
					Number.TransparentBlt(mem1dc, 685, 607, 20, 26, 2 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 705, 607, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 725, 607, 20, 26, 1 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 745, 607, 20, 26, 5 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 765, 607, 20, 26, 1 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 785, 607, 20, 26, 8 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 805, 607, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 825, 607, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 845, 607, 20, 26, 3 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 865, 607, 20, 26, 8 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 890, 598, 40, 48, 9 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 925, 598, 40, 48, 7 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 960, 598, 40, 48, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));


					Number.TransparentBlt(mem1dc, 685, 648, 20, 26, 2 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 705, 648, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 725, 648, 20, 26, 1 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 745, 648, 20, 26, 5 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 765, 648, 20, 26, 1 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 785, 648, 20, 26, 8 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 805, 648, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 825, 648, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 845, 648, 20, 26, 3 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Number.TransparentBlt(mem1dc, 865, 648, 20, 26, 0 * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 890, 636, 40, 48, 11 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 925, 636, 40, 48, 3 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
					Alphabet.TransparentBlt(mem1dc, 960, 636, 40, 48, 6 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				}
			}

			if (Start)
			{ // 플레이어 2의 버튼
				if (P2_Play)
				{
					if (GetAsyncKeyState(VK_LEFT) & 0x8000)
					{
						if (P2.left - SPEED > 0)
						{
							P2.left -= SPEED;
							P2.right -= SPEED;
							Right2 = false; // 왼쪽을 바라보게
						}
						Move2 = true;
					}
					if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
					{
						if (P2.right + SPEED < rt.right)
						{
							P2.left += SPEED;
							P2.right += SPEED;
							Right2 = true; // 오른쪽을 바라보게
						}
						Move2 = true; // 애니메이션 활성화
					}
					if (GetAsyncKeyState(VK_NUMPAD0) & 1) // 플레이어 2가 작살발사버튼을 누를때
					{
						for (int i = 0; i < P2_mCount; ++i)
						{
							if (!M2[i])
							{
								Missile_P2[i].left = P2.left + 20; // 작살 그림의 폭이 30, 플레이어 그림의 폭은 70
								Missile_P2[i].top = P2.top;
								Missile_P2[i].right = P2.right - 20;
								Missile_P2[i].bottom = P2.bottom;
								M_height2[i] = Missile_P2[i].bottom - Missile_P2[i].top; // 작살의 길이
								height2[i] = 150; // 작살 그림에서의 길이
								M2[i] = true;
								Shot2 = true;
								break;
							}
						}
					}
				}

				// 플레이어 1의 버튼
				if (P1_Play)
				{
					if (GetAsyncKeyState('A') & 0x8000)
					{
						if (P1.left - SPEED > 0)
						{
							P1.left -= SPEED;
							P1.right -= SPEED;
							Right1 = false;
						}
						Move1 = true;
					}
					if (GetAsyncKeyState('D') & 0x8000)
					{
						if (P1.right + SPEED < rt.right)
						{
							P1.left += SPEED;
							P1.right += SPEED;
							Right1 = true;
						}
						Move1 = true;
					}
					if (GetAsyncKeyState('G') & 1) // 플레이어1이 작살발사 버튼을 누를때
					{
						for (int i = 0; i < P1_mCount; ++i)
						{
							if (!M1[i])
							{
								Missile_P1[i].left = P1.left + 20;
								Missile_P1[i].top = P1.top;
								Missile_P1[i].right = P1.right - 20;
								Missile_P1[i].bottom = P1.bottom;
								M_height1[i] = Missile_P1[i].bottom - Missile_P1[i].top;
								height1[i] = 150;
								M1[i] = true;
								Shot1 = true;
								break;
							}
						}
					}
				}

				// 작살 //
				for (int i = 0; i < 5; ++i)
				{
					if (M1[i]) // 플레이어1의 작살 이동 타이머
					{
						M_height1[i] += SPEED; // 작살의 길이
						Missile_P1[i].top -= SPEED; // 작살의 y1좌표의 위치
						height1[i] += M_SPEED; // 작살 그림에서 얼마나 출력할건지
						if (Missile_P1[i].top < 25)
						{
							Missile_P1[i].top = 0;
							Missile_P1[i].left = 0;
							Missile_P1[i].bottom = 0;
							Missile_P1[i].right = 0;
							M1[i] = false; // 작살 타이머 정지
						}
					}
					if (M2[i]) // 플레이어 2의 작살 이동 타이머
					{
						M_height2[i] += SPEED;
						Missile_P2[i].top -= SPEED;
						height2[i] += M_SPEED;
						if (Missile_P2[i].top < 25)
						{
							Missile_P2[i].top = 0;
							Missile_P2[i].left = 0;
							Missile_P2[i].bottom = 0;
							Missile_P2[i].right = 0;
							M2[i] = false;
						}
					}
				}
			}

			if (Stop_item) // 아이템 낙하
			{
				if (timer_item.bottom < STAGE_Y)
				{
					timer_item.top += 10;
					timer_item.bottom += 10;
				}
			}
			if (M) // 아이템 낙하
			{
				if (Missile_item.bottom < STAGE_Y)
				{
					Missile_item.top += 10;
					Missile_item.bottom += 10;
				}
			}
			if (H) // 아이템 낙하
			{
				if (Life_item.bottom < STAGE_Y)
				{
					Life_item.top += 10;
					Life_item.bottom += 10;
				}
			}

			gameover_count++;
			gameover_count = gameover_count % 6;
			DeleteDC(mem1dc);
			DeleteDC(blockdc);
			DeleteDC(ball1dc);
			DeleteDC(ball2dc);
			DeleteDC(ball3dc);
			DeleteDC(brokenblockdc);
			ReleaseDC(hWnd, hdc);
			break;

		case 2:
			//작살 타일충돌 + 타일프레임
			for (int i = 0; i < TILECNT_ONE; ++i)
			{
				RECT rcTemp;
				if ((TileOne[i].type == 1) && (TileOne[i].tFrame.NowFrame != 0))
				{
					++TileOne[i].tFrame.NowFrame;

					if (TileOne[i].tFrame.NowFrame > TileOne[i].tFrame.MaxFrame)
						TileOne[i].type = 0;
				}

				if ((TileOne[i].type != 0) && (TileOne[i].tFrame.NowFrame == 0))
				{
					for (int a = 0; a < 5; ++a)
					{
						if (IntersectRect(&rcTemp, &TileOne[i].rc, &Missile_P1[a]))
						{
							if (TileOne[i].type == 1)
								TileOne[i].tFrame.NowFrame = 1;

							// 플레이어1 작살과 타일의 충돌
							int item = rand() % 10;
							if (item == 0 && Stop_item == false && Stop == false && TileOne[i].type == 1) // 10퍼센트 확률로 시간정지 아이템 등장
							{// 필드에 시간정지 아이템이 없고 시간정지가 아닐때만 등장
								timer_item.top = Missile_P1[a].top;
								timer_item.left = Missile_P1[a].left;
								timer_item.right = timer_item.left + 30;
								timer_item.bottom = timer_item.top + 30;
								Stop_item = true;
							}
							if (item >= 1 && item <= 2 && M == false && TileOne[i].type == 1) // 10퍼센트 확률로 작살갯수 추가 아이템 등장
							{
								Missile_item.top = Missile_P1[a].top;
								Missile_item.left = Missile_P1[a].left;
								Missile_item.right = Missile_item.left + 30;
								Missile_item.bottom = Missile_item.top + 30;
								M = true;
							}
							if (item == 3 && H == false && TileOne[i].type == 1)
							{
								Life_item.top = Missile_P2[a].top;
								Life_item.left = Missile_P2[a].left;
								Life_item.right = Life_item.left + 30;
								Life_item.bottom = Life_item.top + 30;
								H = true;
							}
							Missile_P1[a].top = 0;
							Missile_P1[a].left = 0;
							Missile_P1[a].bottom = 0;
							Missile_P1[a].right = 0;
							M1[a] = false; // 작살 타이머 정지
						}
						if (IntersectRect(&rcTemp, &TileOne[i].rc, &Missile_P2[a]))
						{
							if (TileOne[i].type == 1)
								TileOne[i].tFrame.NowFrame = 1;

							// 플레이어2 작살과 타일의 충돌
							int item = rand() % 10;
							if (item == 0 && Stop_item == false && Stop == false && TileOne[i].type == 1) // 10퍼센트 확률로 시간정지 아이템 등장
							{// 필드에 시간정지 아이템이 없고 시간정지가 아닐때만 등장
								timer_item.top = Missile_P2[a].top;
								timer_item.left = Missile_P2[a].left;
								timer_item.right = timer_item.left + 30;
								timer_item.bottom = timer_item.top + 30;
								Stop_item = true;
							}
							if (item >= 1 && item <= 2 && M == false && TileOne[i].type == 1) // 10퍼센트 확률로 작살갯수 추가 아이템 등장
							{
								Missile_item.top = Missile_P2[a].top;
								Missile_item.left = Missile_P2[a].left;
								Missile_item.right = Missile_item.left + 30;
								Missile_item.bottom = Missile_item.top + 30;
								M = true;
							}
							if (item == 3&& H == false && TileOne[i].type == 1)
							{
								Life_item.top = Missile_P2[a].top;
								Life_item.left = Missile_P2[a].left;
								Life_item.right = Life_item.left + 30;
								Life_item.bottom = Life_item.top + 30;
								H = true;
							}
							Missile_P2[a].top = 0;
							Missile_P2[a].left = 0;
							Missile_P2[a].bottom = 0;
							Missile_P2[a].right = 0;
							M2[a] = false;
						}
					}
				}
			}

			//공과 타일의 충돌
			for (int a = 0; a < 4; ++a)
			{
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						//충돌하는 타일일때만
						if ((TileOne[j].type != 0) && (TileOne[j].tFrame.NowFrame == 0))
						{
							float fDistanceX = fabs((float)TileOne[j].X - (float)ball[a][i].X);
							float fDistanceY = fabs((float)TileOne[j].Y - (float)ball[a][i].Y);

							//충돌
							if ((fDistanceX < (TileOne[j].CX / 2 + ball[a][i].R)) && (fDistanceY < (TileOne[j].CY / 2 + ball[a][i].R)))
							{
								if (fabs(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX) == fabs(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY))
								{
									if (ball[a][i].DirUD == DIR_UP)
									{
										ball[a][i].Y += (int)(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY);
										ball[a][i].DirUD = DIR_DOWN;
									}
									else if (ball[a][i].DirUD == DIR_DOWN)
									{
										ball[a][i].Y -= (int)(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY);
										ball[a][i].DirUD = DIR_UP;
									}

									if (ball[a][i].DirLR == DIR_LEFT)
									{
										ball[a][i].X += (int)(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX);
										ball[a][i].DirLR = DIR_RIGHT;
									}
									else if (ball[a][i].DirLR == DIR_RIGHT)
									{
										ball[a][i].X -= (int)(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX);
										ball[a][i].DirLR = DIR_LEFT;
									}
								}
								else if (fabs(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX) < fabs(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY))
								{
									if (ball[a][i].DirLR == DIR_LEFT)
									{
										ball[a][i].X += (int)(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX);
										ball[a][i].DirLR = DIR_RIGHT;
									}
									else if (ball[a][i].DirLR == DIR_RIGHT)
									{
										ball[a][i].X -= (int)(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX);
										ball[a][i].DirLR = DIR_LEFT;
									}
								}
								else if (fabs(TileOne[j].CX / 2 + ball[a][i].R - fDistanceX) > fabs(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY))
								{
									if (ball[a][i].DirUD == DIR_UP)
									{
										ball[a][i].Y += (int)(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY);
										ball[a][i].DirUD = DIR_DOWN;
									}
									else if (ball[a][i].DirUD == DIR_DOWN)
									{
										ball[a][i].Y -= (int)(TileOne[j].CY / 2 + ball[a][i].R - fDistanceY);
										ball[a][i].DirUD = DIR_UP;
									}
								}
							}
						}
					}
				}
			}

			//공과 플레이어 충돌
			for (int j = 0; j < 4; ++j)
			{
				for (int i = 0; i < 4; ++i)
				{
					RECT rcTemp;
					if (IntersectRect(&rcTemp, &ball[j][i].rc, &P1) && P1_Play && ball[j][i].type != 4)
					{
						if (!P1_attack)
						{
							P1_Life--;
							if (P1_Life == 0)
							{
								P1_Play = false;
								if (P2_Play == false)
								{
									Gameover = true;
									Start = false;
									PlaySound(NULL, 0, 0);
									PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
								}
								score1 = 0;
								P1_mCount = 1;
							}
							P1_attack = true;
						}
					}
					if (IntersectRect(&rcTemp, &ball[j][i].rc, &P2) && P2_Play && ball[j][i].type != 4)
					{
						if (!P2_attack)
						{
							P2_Life--;
							if (P2_Life == 0)
							{
								P2_Play = false;
								if (P1_Play == false)
								{
									Gameover = true;
									Start = false;
									PlaySound(NULL, 0, 0);
									PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
								}
								score2 = 0;
								P2_mCount = 1;
							}
							P2_attack = true;
						}
					}
				}
			}
			

			// 아이템과 플레이어 충돌
			if(Stop == false && Stop_item == true)
			{
				RECT rcTemp;
				if (IntersectRect(&rcTemp, &timer_item, &P1) && P1_Play)
				{
					Stop = true;
					timer_item.top = 0;
					timer_item.left = 0;
					timer_item.right = 0;
					timer_item.bottom = 0;
					score1 += 200;
					Stop_item = false;
					T = true;
					PlaySound(NULL, 0, 0);
					PlaySound("Clock.wav", NULL, SND_ASYNC);
				}
				if (IntersectRect(&rcTemp, &timer_item, &P2) && P2_Play)
				{
					Stop = true;
					timer_item.top = 0;
					timer_item.left = 0;
					timer_item.right = 0;
					timer_item.bottom = 0;
					score2 += 200;
					Stop_item = false;
					T = true;
					PlaySound(NULL, 0, 0);
					PlaySound("Clock.wav", NULL, SND_ASYNC);
				}
			}
			if (M == true)
			{
				RECT rcTemp;
				if (IntersectRect(&rcTemp, &Missile_item, &P1) && P1_Play)
				{
					M = false;
					Missile_item.top = 0;
					Missile_item.left = 0;
					Missile_item.right = 0;
					Missile_item.bottom = 0;
					score1 += 50;
					if (P1_mCount < 5)
						P1_mCount++;
				}
				if (IntersectRect(&rcTemp, &Missile_item, &P2) && P2_Play)
				{
					M = false;
					Missile_item.top = 0;
					Missile_item.left = 0;
					Missile_item.right = 0;
					Missile_item.bottom = 0;
					score2 += 50;
					if (P2_mCount < 5)
						P2_mCount++;
				}
			}
			if (H == true)
			{
				RECT rcTemp;
				if (IntersectRect(&rcTemp, &Life_item, &P1) && P1_Play)
				{
					H = false;
					Life_item.top = 0;
					Life_item.left = 0;
					Life_item.right = 0;
					Life_item.bottom = 0;
					if (P1_Life < 3 && P1_Play)
						P1_Life++;
					score1 += 50;
				}
				if (IntersectRect(&rcTemp, &Life_item, &P2) && P2_Play)
				{
					H = false;
					Life_item.top = 0;
					Life_item.left = 0;
					Life_item.right = 0;
					Life_item.bottom = 0;
					if (P2_Life < 3 && P2_Play)
						P2_Life++;
					score1 += 50;
				}
			}

			//공과 작살 충돌
			for (int j = 0; j < 4; ++j)
			{
				for (int i = 0; i < 4; ++i)
				{
					RECT rcTemp;
					for (int a = 0; a < 5; ++a)
					{
						if (IntersectRect(&rcTemp, &ball[j][i].rc, &Missile_P1[a]) && ball[j][i].type != 4)
						{
							//플레이어1의 작살과 공의 충돌
							int item = rand() % 10;
							if (item == 0 && Stop_item == false && Stop == false) // 10퍼센트 확률로 시간정지 아이템 등장
							{// 필드에 시간정지 아이템이 없고 시간정지가 아닐때만 등장
								timer_item.top = ball[j][i].Y;
								timer_item.left = ball[j][i].X;
								timer_item.right = timer_item.left + 30;
								timer_item.bottom = timer_item.top + 30;
								Stop_item = true;
							}
							if (item >= 1 && item <= 2 && M == false) // 20퍼센트 확률로 작살갯수 추가 아이템 등장
							{
								Missile_item.top = ball[j][i].Y;
								Missile_item.left = ball[j][i].X;
								Missile_item.right = Missile_item.left + 30;
								Missile_item.bottom = Missile_item.top + 30;
								M = true;
							}
							if (item == 3 && H == false)
							{
								Life_item.top = ball[j][i].Y;
								Life_item.left = ball[j][i].X;
								Life_item.right = Life_item.left + 30;
								Life_item.bottom = Life_item.top + 30;
								H = true;
							}
							Missile_P1[a].top = 0;
							Missile_P1[a].left = 0;
							Missile_P1[a].bottom = 0;
							Missile_P1[a].right = 0;
							M1[a] = false;

							score1 += 50;

							switch (ball[j][i].type)
							{
							case 0:
								score1 += 100;
								ball[j][i].type = 4;
								break;
							case 1:
								if (i == 0)
								{
									ball[j][0].X = ball[j][0].X;
									ball[j][0].Y = ball[j][0].Y;
									ball[j][0].R = 8;
									ball[j][0].type = 0;
									ball[j][0].SetRect();
									ball[j][0].DirLR = DIR_RIGHT;
									ball[j][0].DirUD = ball[j][0].DirUD;
									ball[j][0].gravity = ball[j][0].gravity;

									ball[j][1].X = ball[j][0].X;
									ball[j][1].Y = ball[j][0].Y;
									ball[j][1].R = 8;
									ball[j][1].type = 0;
									ball[j][1].SetRect();
									ball[j][1].DirLR = DIR_LEFT;
									ball[j][1].DirUD = ball[j][0].DirUD;
									ball[j][1].gravity = ball[j][0].gravity;
								}
								else if (i == 2)
								{
									ball[j][2].X = ball[j][2].X;
									ball[j][2].Y = ball[j][2].Y;
									ball[j][2].R = 8;
									ball[j][2].type = 0;
									ball[j][2].SetRect();
									ball[j][2].DirLR = DIR_RIGHT;
									ball[j][2].DirUD = ball[j][2].DirUD;
									ball[j][2].gravity = ball[j][2].gravity;

									ball[j][3].X = ball[j][2].X;
									ball[j][3].Y = ball[j][2].Y;
									ball[j][3].R = 8;
									ball[j][3].type = 0;
									ball[j][3].SetRect();
									ball[j][3].DirLR = DIR_LEFT;
									ball[j][3].DirUD = ball[j][2].DirUD;
									ball[j][3].gravity = ball[j][2].gravity;
								}
								break;
							case 2:
								ball[j][0].X = ball[j][0].X;
								ball[j][0].Y = ball[j][0].Y;
								ball[j][0].R = 15;
								ball[j][0].type = 1;
								ball[j][0].SetRect();
								ball[j][0].DirLR = DIR_RIGHT;
								ball[j][0].DirUD = ball[j][0].DirUD;
								ball[j][0].gravity = ball[j][0].gravity;

								ball[j][2].X = ball[j][0].X;
								ball[j][2].Y = ball[j][0].Y;
								ball[j][2].R = 15;
								ball[j][2].type = 1;
								ball[j][2].SetRect();
								ball[j][2].DirLR = DIR_LEFT;
								ball[j][2].DirUD = ball[j][0].DirUD;
								ball[j][2].gravity = ball[j][0].gravity;
								break;
							}
						}
						if (IntersectRect(&rcTemp, &ball[j][i].rc, &Missile_P2[a]) && ball[j][i].type != 4)
						{
							// 플레이어 2의 작살과 공의 충돌
							int item = rand() % 10;
							if (item == 0 && Stop_item == false && Stop == false) // 10퍼센트 확률로 시간정지 아이템 등장
							{// 필드에 시간정지 아이템이 없고 시간정지가 아닐때만 등장
								timer_item.top = ball[j][i].Y;
								timer_item.left = ball[j][i].X;
								timer_item.right = timer_item.left + 30;
								timer_item.bottom = timer_item.top + 30;
								Stop_item = true;
							}
							if (item >= 1 && item <= 2 && M == false) // 10퍼센트 확률로 작살갯수 추가 아이템 등장
							{
								Missile_item.top = ball[j][i].Y;
								Missile_item.left = ball[j][i].X;
								Missile_item.right = Missile_item.left + 30;
								Missile_item.bottom = Missile_item.top + 30;
								M = true;
							}
							if (item == 3 && H == false)
							{
								Life_item.top = ball[j][i].Y;
								Life_item.left = ball[j][i].X;
								Life_item.right = Life_item.left + 30;
								Life_item.bottom = Life_item.top + 30;
								H = true;
							}

							Missile_P2[a].top = 0;
							Missile_P2[a].left = 0;
							Missile_P2[a].bottom = 0;
							Missile_P2[a].right = 0;
							M2[a] = false;

							score2 += 50;

							switch (ball[j][i].type)
							{
							case 0:
								score2 += 100;
								ball[j][i].type = 4;
								break;
							case 1:
								if (i == 0)
								{
									ball[j][0].X = ball[j][0].X;
									ball[j][0].Y = ball[j][0].Y;
									ball[j][0].R = 8;
									ball[j][0].type = 0;
									ball[j][0].SetRect();
									ball[j][0].DirLR = DIR_RIGHT;
									ball[j][0].DirUD = ball[j][0].DirUD;
									ball[j][0].gravity = ball[j][0].gravity;

									ball[j][1].X = ball[j][0].X;
									ball[j][1].Y = ball[j][0].Y;
									ball[j][1].R = 8;
									ball[j][1].type = 0;
									ball[j][1].SetRect();
									ball[j][1].DirLR = DIR_LEFT;
									ball[j][1].DirUD = ball[j][0].DirUD;
									ball[j][1].gravity = ball[j][0].gravity;
								}
								else if (i == 2)
								{
									ball[j][2].X = ball[j][2].X;
									ball[j][2].Y = ball[j][2].Y;
									ball[j][2].R = 8;
									ball[j][2].type = 0;
									ball[j][2].SetRect();
									ball[j][2].DirLR = DIR_RIGHT;
									ball[j][2].DirUD = ball[j][2].DirUD;
									ball[j][2].gravity = ball[j][2].gravity;

									ball[j][3].X = ball[j][2].X;
									ball[j][3].Y = ball[j][2].Y;
									ball[j][3].R = 8;
									ball[j][3].type = 0;
									ball[j][3].SetRect();
									ball[j][3].DirLR = DIR_LEFT;
									ball[j][3].DirUD = ball[j][2].DirUD;
									ball[j][3].gravity = ball[j][2].gravity;
								}
								break;
							case 2:
								ball[j][0].X = ball[j][0].X;
								ball[j][0].Y = ball[j][0].Y;
								ball[j][0].R = 15;
								ball[j][0].type = 1;
								ball[j][0].SetRect();
								ball[j][0].DirLR = DIR_RIGHT;
								ball[j][0].DirUD = ball[j][0].DirUD;
								ball[j][0].gravity = ball[j][0].gravity;

								ball[j][2].X = ball[j][0].X;
								ball[j][2].Y = ball[j][0].Y;
								ball[j][2].R = 15;
								ball[j][2].type = 1;
								ball[j][2].SetRect();
								ball[j][2].DirLR = DIR_LEFT;
								ball[j][2].DirUD = ball[j][0].DirUD;
								ball[j][2].gravity = ball[j][0].gravity;
								break;
							}
						}

					}
				}
			}

			//점수 계산
			int temp;
			temp = score1 / 10000;
			P1_score[0] = temp;
			temp = score1 / 1000;
			P1_score[1] = temp % 10;
			temp = score1 / 100;
			P1_score[2] = temp % 100;
			P1_score[2] = P1_score[2] % 10;
			temp = score1 / 10;
			P1_score[3] = temp % 1000;
			P1_score[3] = P1_score[3] % 100;
			P1_score[3] = P1_score[3] % 10;
			temp = score1 % 10;
			P1_score[4] = temp;

			temp = score2 / 10000;
			P2_score[4] = temp;
			temp = score2 / 1000;
			P2_score[3] = temp % 10;
			temp = score2 / 100;
			P2_score[2] = temp % 100;
			P2_score[2] = P2_score[2] % 10;
			temp = score2 / 10;
			P2_score[1] = temp % 1000;
			P2_score[1] = P2_score[1] % 100;
			P2_score[1] = P2_score[1] % 10;
			temp = score2 % 10;
			P2_score[0] = temp;

			break;
		case 3:
			//공 움직임
			if (!Stop) // 타이머
			{
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						//중력
						if (ball[j][i].R != 0 && Start)
						{
							ball[j][i].gravity = (ball[j][i].Y - 140) / 10; // 낙하속도

							if (ball[j][i].DirLR == DIR_LEFT)
							{
								ball[j][i].X -= 4;

								if (ball[j][i].X - ball[j][i].R < 30) // 왼쪽 벽에 충돌
									ball[j][i].DirLR = DIR_RIGHT;
							}
							else if (ball[j][i].DirLR == DIR_RIGHT)
							{
								ball[j][i].X += 4;

								if (ball[j][i].X + ball[j][i].R > rt.right - 30) // 오른쪽 벽에 충돌
									ball[j][i].DirLR = DIR_LEFT;
							}

							if (ball[j][i].DirUD == DIR_DOWN)
							{
								ball[j][i].Y += ball[j][i].gravity;

								if (ball[j][i].Y + ball[j][i].R > STAGE_Y - 25) // 바닥에 충돌
									ball[j][i].DirUD = DIR_UP;
							}
							else if (ball[j][i].DirUD == DIR_UP)
							{
								ball[j][i].Y -= ball[j][i].gravity;

								if (ball[j][i].Y - ball[j][i].R < 150) // 최대 높이 값
									ball[j][i].DirUD = DIR_DOWN;
							}

							ball[j][i].SetRect();
						}
					}
				}
			}
			break;
		case 4:
			if (Start)
			{
				if (!T)
				{
					check_time++;
					Time_count++;
					if (Time_count == 10)
					{
						switch (Stage) {
						case 1:
							if (STAGE1_LIMIT_TIME - Time1_count != 0)
							{
								Time1_count++;
								Time1_count = Time1_count % 10;
							}
							break;
						case 2:
							if (STAGE2_LIMIT_TIME - Time1_count != 0)
							{
								Time1_count++;
								Time1_count = Time1_count % 10;
							}
						case 3:
							if (STAGE3_LIMIT_TIME - Time1_count != 0)
							{
								Time1_count++;
								Time1_count = Time1_count % 10;
							}
						}
					}
					switch (Stage) {
					case 1:
						if (Time_count == 9 && STAGE1_LIMIT_TIME - Time1_count == 0)
						{
							T = true; // 제한시간 종료
							Gameover = true;
							Start = false;
							PlaySound(NULL, 0, 0);
							PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						break;
					case 2:
						if (Time_count == 9 && STAGE2_LIMIT_TIME - Time1_count == 0)
						{
							T = true; // 제한시간 종료
							Gameover = true;
							Start = false;
							PlaySound(NULL, 0, 0);
							PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						break;
					case 3:
						if (Time_count == 9 && STAGE3_LIMIT_TIME - Time1_count == 0)
						{
							T = true; // 제한시간 종료
							Gameover = true;
							Start = false;
							PlaySound(NULL, 0, 0);
							PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						break;
					}
					Time_count = Time_count % 10;
				}
				if (Stop)
				{
					Time_item_count++;
					if (Time_item_count == 5)
					{
						Time_item_count = 0;
						Stop = false;
						T = false;
						PlaySound(NULL, 0, 0);
						if(Stage == 1)
							PlaySound("Stage1.wav", NULL, SND_ASYNC | SND_LOOP);
						else if(Stage == 2)
							PlaySound("Stage2.wav", NULL, SND_ASYNC | SND_LOOP);
						else if(Stage == 3)
							PlaySound("Stage3.wav", NULL, SND_ASYNC | SND_LOOP);
					}
				}

				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						if (ball[j][i].type != 4)
						{
							stage_count++;
						}
					}
				}
				if (stage_count == 0)
				{
					if (Stage < STAGE_LIMIT)
					{
						Stage++; // 스테이지 2 생성
						P1_mCount = 1;
						P2_mCount = 1;
						Time_count = 0;
						Time1_count = 0;
						if (Stage == 2)
						{
							ball[0][0].X = 200;
							ball[0][0].Y = 200;
							ball[0][0].R = 25;
							ball[0][0].type = 2;
							ball[0][0].SetRect();
							ball[0][0].DirLR = DIR_RIGHT;
							ball[0][0].DirUD = DIR_DOWN;
							ball[0][0].gravity = 0;

							ball[1][0].X = 800;
							ball[1][0].Y = 200;
							ball[1][0].R = 25;
							ball[1][0].type = 2;
							ball[1][0].SetRect();
							ball[1][0].DirLR = DIR_LEFT;
							ball[1][0].DirUD = DIR_DOWN;
							ball[1][0].gravity = 0;

							ball[2][0].X = 400;
							ball[2][0].Y = 250;
							ball[2][0].R = 25;
							ball[2][0].type = 2;
							ball[2][0].SetRect();
							ball[2][0].DirLR = DIR_RIGHT;
							ball[2][0].DirUD = DIR_DOWN;
							ball[2][0].gravity = 0;

							ball[3][0].X = 600;
							ball[3][0].Y = 250;
							ball[3][0].R = 25;
							ball[3][0].type = 2;
							ball[3][0].SetRect();
							ball[3][0].DirLR = DIR_LEFT;
							ball[3][0].DirUD = DIR_DOWN;
							ball[3][0].gravity = 0;

							TileOne[0].X = 250;
							TileOne[0].Y = 300;
							TileOne[1].X = 490;
							TileOne[1].Y = 200;
							TileOne[2].X = rt.right - 250;
							TileOne[2].Y = 300;
							for (int i = 0; i < TILECNT_ONE; ++i)
							{
								if (i == 1)
								{
									TileOne[i].tFrame.MaxFrame = 5;
									TileOne[i].tFrame.NowFrame = 0;

									TileOne[i].type = 2;
									TileOne[i].CX = 570;
									TileOne[i].CY = 40;
									TileOne[i].SetRect();
								}
								else
								{
									TileOne[i].tFrame.MaxFrame = 5;
									TileOne[i].tFrame.NowFrame = 0;

									TileOne[i].type = 2;
									TileOne[i].CX = 40;
									TileOne[i].CY = 220;
									TileOne[i].SetRect();
								}
							}
							PlaySound(NULL, 0, 0);
							PlaySound("Stage2.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						else if (Stage == 3)
						{
							ball[0][0].X = 200;
							ball[0][0].Y = 200;
							ball[0][0].R = 25;
							ball[0][0].type = 2;
							ball[0][0].SetRect();
							ball[0][0].DirLR = DIR_RIGHT;
							ball[0][0].DirUD = DIR_DOWN;
							ball[0][0].gravity = 0;

							ball[1][0].X = 800;
							ball[1][0].Y = 200;
							ball[1][0].R = 25;
							ball[1][0].type = 2;
							ball[1][0].SetRect();
							ball[1][0].DirLR = DIR_LEFT;
							ball[1][0].DirUD = DIR_DOWN;
							ball[1][0].gravity = 0;

							ball[2][0].X = 400;
							ball[2][0].Y = 250;
							ball[2][0].R = 25;
							ball[2][0].type = 2;
							ball[2][0].SetRect();
							ball[2][0].DirLR = DIR_RIGHT;
							ball[2][0].DirUD = DIR_DOWN;
							ball[2][0].gravity = 0;

							ball[3][0].X = 600;
							ball[3][0].Y = 250;
							ball[3][0].R = 25;
							ball[3][0].type = 2;
							ball[3][0].SetRect();
							ball[3][0].DirLR = DIR_LEFT;
							ball[3][0].DirUD = DIR_DOWN;
							ball[3][0].gravity = 0;

							TileOne[0].X = 200;
							TileOne[0].Y = 350;
							TileOne[1].X = rt.right / 2;
							TileOne[1].Y = 350;
							TileOne[2].X = rt.right - 200;
							TileOne[2].Y = 350;
							for (int i = 0; i < TILECNT_ONE; ++i)
							{
								TileOne[i].tFrame.MaxFrame = 5;
								TileOne[i].tFrame.NowFrame = 0;

								TileOne[i].type = 2;
								TileOne[i].CX = 200;
								TileOne[i].CY = 40;
								TileOne[i].SetRect();
							}
							PlaySound(NULL, 0, 0);
							PlaySound("Stage3.wav", NULL, SND_ASYNC | SND_LOOP);
						}
					}
					else if (Stage == STAGE_LIMIT)
					{
						End = true;
						Start = false;
						PlaySound(NULL, 0, 0);
						PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
					}
				}
				stage_count = 0;

				if (P1_Play)
					score1 += 10;
				if (P2_Play)
					score2 += 10;
			}

			break;
		}
		InvalidateRgn(hWnd, NULL, false);
		break;
	case WM_LBUTTONDOWN:
		if (!Start && (Gameover || End))
		{
			PostQuitMessage(0);
		}
		break;
	case WM_KEYDOWN:
		
		if (!Start && wParam == VK_RETURN && check_time == 0)
		{
			SetTimer(hWnd, 4, 1000, NULL);		//시간 재는 타이머
			Start = true; // 눌러야지 화면이 넘어감
			PlaySound(NULL, 0, 0);
			PlaySound("Stage1.wav", NULL, SND_ASYNC | SND_LOOP);
		}

		if (Start && (wParam == 49 || wParam == VK_NUMPAD1) && !P1_Play) // 플레이어 1 추가/부활
		{
			P1_Play = true;
			P1_Life = 3;
			P1_attack = false;
			P1_mCount = 1;
		}
		else if (Start && (wParam == 50 || wParam == VK_NUMPAD2) && !P2_Play) // 플레이어 2 추가/부활
		{
			P2_Play = true;
			P2_Life = 3;
			P2_attack = false;
			P2_mCount = 1;
		}
		else if (Start && (wParam == 51 || wParam == VK_NUMPAD3) && P1_Play) // 플레이어1 체력회복 치트
		{
			P1_Life = 3;
		}
		else if (Start && (wParam == 52 || wParam == VK_NUMPAD4) && P2_Play) // 플레이어2 체력회복 치트
		{
			P2_Life = 3;
		}
		else if (Start && (wParam == 53 || wParam == VK_NUMPAD5) && P1_Play) // 플레이어1 작살 치트
		{
			P1_mCount = 5;
		}
		else if (Start && (wParam == 54 || wParam == VK_NUMPAD6) && P2_Play) // 플레이어2 작살 치트
		{
			P2_mCount = 5;
		}
		else if (Start && (wParam == 55 || wParam == VK_NUMPAD7))
		{
			Time1_count = 0;
			Time_count = 0;
		}
		if(wParam == VK_ESCAPE)
			PostQuitMessage(0);

		break;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, IMessage, wParam, lParam);
}

void P1_Animation(int x, int y, HDC mem1dc, BOOL Move, BOOL Right)
{
	CImage Player1[5];
	static int RM1_count, LM1_count, S1_count;

	if (Shot1)
	{
		S1_count++;
		S1_count = S1_count % 2; // 쏘는 자세 애니메이션 카운트
		Player1[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		Player1[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));

		Player1[S1_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player1[S1_count].GetWidth(), Player1[S1_count].GetHeight(), RGB(0, 255, 0));
		if (S1_count % 2 == 1)
		{
			Shot1 = false;
		}
		return; // 리턴이없으면 이동애니메이션까지 작동하기때문에 여기서 끊음
	}

	if (Right) // 오른쪽이동
	{
		RM1_count++;
		RM1_count = RM1_count % 5; // 오른쪽 애니메이션
		Player1[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		Player1[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		Player1[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		Player1[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		Player1[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

		if (Move)
		{
			Player1[RM1_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player1[RM1_count].GetWidth(), Player1[RM1_count].GetHeight(), RGB(0, 255, 0));
		}
		else // 정지할때
			Player1[0].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player1[RM1_count].GetWidth(), Player1[RM1_count].GetHeight(), RGB(0, 255, 0));
	}
	else // 왼쪽이동
	{
		LM1_count++;
		LM1_count = LM1_count % 5; // 왼쪽 애니메이션
		Player1[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		Player1[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		Player1[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		Player1[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		Player1[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));

		if (Move)
		{
			Player1[LM1_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player1[LM1_count].GetWidth(), Player1[LM1_count].GetHeight(), RGB(0, 255, 0));
		}
		else // 정지할때
		{
			Player1[4].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player1[LM1_count].GetWidth(), Player1[LM1_count].GetHeight(), RGB(0, 255, 0));
		}
	}
}

void P2_Animation(int x, int y, HDC mem1dc, BOOL Move, BOOL Right)
{
	CImage Player2[5];
	static int RM2_count, LM2_count, S2_count;

	if (Shot2)
	{
		S2_count++;
		S2_count = S2_count % 2;
		Player2[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP27));
		Player2[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP28));

		Player2[S2_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player2[S2_count].GetWidth(), Player2[S2_count].GetHeight(), RGB(0, 255, 0));
		if (S2_count % 2 == 1)
		{
			Shot2 = false;
		}
		return; // 리턴이없으면 이동애니메이션까지 작동하기때문에 여기서 끊음
	}

	if (Right) // 오른쪽이동
	{
		RM2_count++;
		RM2_count = RM2_count % 5;
		Player2[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP22));
		Player2[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP23));
		Player2[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP24));
		Player2[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP25));
		Player2[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP26));

		if (Move)
		{
			Player2[RM2_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player2[RM2_count].GetWidth(), Player2[RM2_count].GetHeight(), RGB(0, 255, 0));
		}
		else
			Player2[0].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player2[RM2_count].GetWidth(), Player2[RM2_count].GetHeight(), RGB(0, 255, 0));
	}
	else // 왼쪽이동
	{
		LM2_count++;
		LM2_count = LM2_count % 5;
		Player2[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP17));
		Player2[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP18));
		Player2[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP19));
		Player2[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP20));
		Player2[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP21));

		if (Move)
		{
			Player2[LM2_count].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player2[LM2_count].GetWidth(), Player2[LM2_count].GetHeight(), RGB(0, 255, 0));
		}
		else
		{
			Player2[4].TransparentBlt(mem1dc, x, y, 70, 70, 0, 0, Player2[LM2_count].GetWidth(), Player2[LM2_count].GetHeight(), RGB(0, 255, 0));
		}
	}
}