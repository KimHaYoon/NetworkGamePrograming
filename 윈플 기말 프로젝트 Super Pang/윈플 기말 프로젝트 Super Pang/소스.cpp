#include "stdafx.h"
#include "resource.h"
#include "Player.h"
#include "Define.h"
#include "Ball.h"

static RECT rt; // ������ ������

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

//typedef enum DirOne
//{
//	DIR_LEFT,
//	DIR_RIGHT,
//	DIR1_END
//}DIR_ONE;
//typedef enum DirTwo
//{
//	DIR_UP,
//	DIR_DOWN,
//	DIR2_END
//}DIR_TWO;

typedef struct tagFrame
{
	int NowFrame;
	int MaxFrame;
}FRAME;

typedef struct tagTile   //Ÿ��
{
	int X; // X��ǥ
	int Y; // Y��ǥ

	int CX;  // Ÿ���� ���� ��
	int CY;	 // Ÿ���� ���� ��
	int type; // 0���� 1�μ��� 2�Ⱥμ���

	FRAME tFrame;

	RECT rc;  //Ÿ�� ��Ʈ

	void SetRect(void)  // Ÿ�̸ӿ��� ������ ������ ����
	{
		rc.left = X - CX / 2;
		rc.top = Y - CY / 2;
		rc.right = X + CX / 2;
		rc.bottom = Y + CY / 2;
	}
}TILE;

//typedef struct tagBall
//{
//	int X; // X��ǥ
//	int Y; // Y��ǥ
//
//	int R; //������ 
//	int type; // ����Ÿ��(0 R:15 / 1 R:30 / 2 R:50)
//	int gravity;
//
//	DIR_ONE DirLR;
//	DIR_TWO DirUD;
//
//	RECT rc;
//
//	void SetRect(void)  // Ÿ�̸ӿ��� ������ ������ �������ַ��� ��
//	{
//		rc.left = X - (R - 3);
//		rc.top = Y - (R - 3);
//		rc.right = X + (R - 3);
//		rc.bottom = Y + (R - 3);
//	}
//}BALL;


LRESULT CALLBACK WndProc(HWND hWnd, UINT IMessage, WPARAM wParam, LPARAM lParam)
{
	static int logo_count = 0; // Press Enter Key ī��Ʈ
	static RECT rcTemp = { 0, 0, 0, 0 }; //�浹���� RECT
	static CImage img, blackimg, Logo, Key, Missile, timer, Number, Number2, Alphabet, Number3, Mitem, img2, GameOver, Ending, img3, Life;
	static HBITMAP hBit, hBlock, hBall1, hBall2, hBall3, hBrokenBlock, oldBit;
	static HDC hdc, mem1dc, blockdc, ball1dc, ball2dc, ball3dc, brokenblockdc;
	static BOOL Start; // false�� �ΰ���� true�� ���ӽ���
	static BOOL Stop = false, Stop_item = false;
	static BOOL T; // �ð��� �ٵǾ�����
	static BOOL M = false;
	static BOOL H; // �����߰�
	static RECT timer_item; // �ð����� ������
	static RECT Missile_item, Life_item;
	static HPEN hPen, oldPen;
	static int Time_item_count = 0; // �ð� ���� ī��Ʈ
	static int Time_count, check_time; // �ð� ī��Ʈ
	static int Time1_count;
	static int Stage, stage_count, gameover_count;
	static BOOL Gameover, End;
	PAINTSTRUCT ps;

	//�÷��̾�
	static Player myPlayer[PLAYER_END];

	//Ÿ��
	static const int TILECNT_ONE = 3;
	static TILE TileOne[TILECNT_ONE];

	//��
	static Ball ball[4][4];

	switch (IMessage)
	{
	case WM_CREATE:
		srand((unsigned)time(NULL));
		GetClientRect(hWnd, &rt);
		SetTimer(hWnd, 1, 60, NULL);
		SetTimer(hWnd, 2, 40, NULL);		//�浹����Ÿ�̸�
		SetTimer(hWnd, 3, 50, NULL);		//�������� ���� Ÿ�̸�
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

		//Ÿ��
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
				ball[j][i].Initialize();
			}
		}

		//��(������ 50, 30, 15)
		ball[0][0].Initialize(200, 250, 25, 2, DIR_RIGHT, DIR_DOWN);
		ball[1][0].Initialize(800, 250, 25, 2, DIR_RIGHT, DIR_DOWN);

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

			if (Start) // ����Ű�� ��������.
			{
				blackimg.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, blackimg.GetWidth(), blackimg.GetHeight(), SRCCOPY); // ���� ���
				if (Stage == 1)
				{
					img.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img.GetWidth(), img.GetHeight(), SRCCOPY); // �������� ���
				}
				else if (Stage == 2)
				{
					img2.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img2.GetWidth(), img2.GetHeight(), SRCCOPY); // �������� ���
				}
				else if (Stage == 3)
				{
					img3.StretchBlt(mem1dc, 0, 0, rt.right, STAGE_Y + 25, 0, 0, img3.GetWidth(), img3.GetHeight(), SRCCOPY); // �������� ���
				}

				for (int i = 0; i < 5; ++i)
				{
					for (int player = PLAYER_1; player < PLAYER_END; ++player)
					{
						if (myPlayer[player].GetBulletShoot(i)) // �ۻ�
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

				//�������� �۾� ���
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 125, STAGE_Y + 25, 50, 56, 18 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 85, STAGE_Y + 25, 50, 56, 19 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 50, STAGE_Y + 25, 50, 56, 0 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 5, STAGE_Y + 25, 50, 56, 6 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 + 40, STAGE_Y + 25, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				// Number3.TransparentBlt(mem1dc, rt.right / 2 + 75, STAGE_Y + 25, 45, 50, 8 * (Number3.GetWidth() / 10), 0, Number3.GetWidth() / 10, Number3.GetHeight(), RGB(255, 0, 255));
				Number2.TransparentBlt(mem1dc, rt.right / 2 + 80, STAGE_Y + 25, 45, 50, Stage * (Number2.GetWidth() / 10), 0, Number2.GetWidth() / 10, Number2.GetHeight(), RGB(255, 0, 255));// Stage Number ���

				//Time
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 125, STAGE_Y + 80, 50, 56, 19 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 90, STAGE_Y + 80, 50, 56, 8 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 55, STAGE_Y + 80, 50, 56, 12 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Alphabet.TransparentBlt(mem1dc, rt.right / 2 - 10, STAGE_Y + 80, 50, 56, 4 * (Alphabet.GetWidth() / 25), 0, Alphabet.GetWidth() / 25, Alphabet.GetHeight(), RGB(255, 0, 255));
				Number.TransparentBlt(mem1dc, 585, 607, 30, 36, (9 - Time_count) * (Number.GetWidth() / 10), 0, Number.GetWidth() / 10, Number.GetHeight(), RGB(255, 0, 255)); // �ð� ���
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

				//Ÿ�����
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

				//�����
				hPen = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
				oldPen = (HPEN)SelectObject(mem1dc, hPen);
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						RECT rc = ball[j][i].GetCollisionBox();
						int r = ball[j][i].GetRadius();
						switch (ball[j][i].GetType())
						{
						case 0:
							if (Stop)
							{
								Ellipse(mem1dc, rc.left, rc.top, rc.left + 16, rc.top + 16);
							}
							TransparentBlt(mem1dc,
								rc.left, rc.top,
								2 * r, 2 * r,
								ball3dc,
								0, 0,
								15, 15,
								RGB(0, 0, 0));
							break;
						case 1:
							if (Stop)
							{
								Ellipse(mem1dc, rc.left, rc.top, rc.left + 31, rc.top + 31);
							}
							TransparentBlt(mem1dc,
								rc.left, rc.top,
								2 * r, 2 * r,
								ball2dc,
								0, 0,
								30, 30,
								RGB(0, 0, 0));
							break;
						case 2:
							if (Stop)
							{
								Ellipse(mem1dc, rc.left, rc.top, rc.left + 51, rc.top + 51);
							}
							TransparentBlt(mem1dc,
								rc.left, rc.top,
								2 * r, 2 * r,
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
			else // ó�� �����Ҷ�
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
					Logo.StretchBlt(mem1dc, 0, 0, rt.right, rt.bottom, 0, 0, Logo.GetWidth(), Logo.GetHeight(), SRCCOPY); // �ΰ� ���
					if (logo_count >= 3 && logo_count <= 4)
					{
						Key.TransparentBlt(mem1dc, 380, 600, Key.GetWidth(), Key.GetHeight(), 0, 0, Key.GetWidth(), Key.GetHeight(), RGB(0, 0, 0));
					}
					logo_count++;
					logo_count = logo_count % 5; // Enter Key�� ������� ������ ������ �ֱ�
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
			{ // �÷��̾� 2�� ��ư
				for (int i = PLAYER_1; i < PLAYER_END; ++i)
				{
					myPlayer[i].Move_Update();
				}

				// �ۻ� //
				for (int i = PLAYER_1; i < PLAYER_END; ++i)
				{
					myPlayer[i].Bullet_Update();
				}
			}

			if (Stop_item) // ������ ����
			{
				if (timer_item.bottom < STAGE_Y)
				{
					timer_item.top += 10;
					timer_item.bottom += 10;
				}
			}
			if (M) // ������ ����
			{
				if (Missile_item.bottom < STAGE_Y)
				{
					Missile_item.top += 10;
					Missile_item.bottom += 10;
				}
			}
			if (H) // ������ ����
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
			//�ۻ� Ÿ���浹 + Ÿ��������

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
					for (int player = PLAYER_1; player < PLAYER_END; ++player)
					{
						for (int a = 0; a < 5; ++a)
						{
							RECT bullet = myPlayer[player].GetBulletPosition(a);
							if (IntersectRect(&rcTemp, &TileOne[i].rc, &bullet))
							{
								if (TileOne[i].type == 1)
									TileOne[i].tFrame.NowFrame = 1;

								// �÷��̾�1 �ۻ�� Ÿ���� �浹
								int item = rand() % 10;
								if (item == 0 && Stop_item == false && Stop == false && TileOne[i].type == 1) // 10�ۼ�Ʈ Ȯ���� �ð����� ������ ����
								{// �ʵ忡 �ð����� �������� ���� �ð������� �ƴҶ��� ����
									timer_item.top = bullet.top;
									timer_item.left = bullet.left;
									timer_item.right = timer_item.left + 30;
									timer_item.bottom = timer_item.top + 30;
									Stop_item = true;
								}
								if (item >= 1 && item <= 2 && M == false && TileOne[i].type == 1) // 10�ۼ�Ʈ Ȯ���� �ۻ찹�� �߰� ������ ����
								{
									Missile_item.top = bullet.top;
									Missile_item.left = bullet.left;
									Missile_item.right = Missile_item.left + 30;
									Missile_item.bottom = Missile_item.top + 30;
									M = true;
								}
								if (item == 3 && H == false && TileOne[i].type == 1)
								{
									Life_item.top = bullet.top;
									Life_item.left = bullet.left;
									Life_item.right = Life_item.left + 30;
									Life_item.bottom = Life_item.top + 30;
									H = true;
								}
								bullet.top = 0;
								bullet.left = 0;
								bullet.bottom = 0;
								bullet.right = 0;
								myPlayer[player].SetBulletPosition(bullet, a);
								myPlayer[player].SetBulletShot(true, a); // �ۻ� Ÿ�̸� ����
							}
						}
					}
				}
			}

			//���� Ÿ���� �浹
			for (int a = 0; a < 4; ++a)
			{
				for (int i = 0; i < 4; ++i)
				{
					for (int j = 0; j < 3; ++j)
					{
						//�浹�ϴ� Ÿ���϶���
						if ((TileOne[j].type != 0) && (TileOne[j].tFrame.NowFrame == 0))
						{
							float fDistanceX = fabs((float)TileOne[j].X - (float)ball[a][i].GetX());
							float fDistanceY = fabs((float)TileOne[j].Y - (float)ball[a][i].GetY());

							//�浹
							int r = ball[a][i].GetRadius();
							if ((fDistanceX < (TileOne[j].CX / 2 + r)) && (fDistanceY < (TileOne[j].CY / 2 + r)))
							{
								if (fabs(TileOne[j].CX / 2 + r - fDistanceX) == fabs(TileOne[j].CY / 2 + r - fDistanceY))
								{
									DIR_ONE DirLR = ball[a][i].GetDirLR();
									DIR_TWO DirUD = ball[a][i].GetDirUD();

									if (DirUD == DIR_UP)
									{
										int y = ball[a][i].GetY();
										y += (int)(TileOne[j].CY / 2 + r - fDistanceY);
										ball[a][i].SetY(y);
										ball[a][i].SetDirUD(DIR_DOWN);
									}
									else if (DirUD == DIR_DOWN)
									{
										int y = ball[a][i].GetY();
										y -= (int)(TileOne[j].CY / 2 + r - fDistanceY);
										ball[a][i].SetY(y);
										ball[a][i].SetDirUD(DIR_UP);
									}

									if (DirLR == DIR_LEFT)
									{
										int x = ball[a][i].GetX();
										x += (int)(TileOne[j].CX / 2 + r - fDistanceX);
										ball[a][i].SetX(x);
										ball[a][i].SetDirLR(DIR_RIGHT);
									}
									else if (DirLR == DIR_RIGHT)
									{
										int x = ball[a][i].GetX();
										x -= (int)(TileOne[j].CX / 2 + r - fDistanceX);
										ball[a][i].SetX(x);
										ball[a][i].SetDirLR(DIR_LEFT);
									}
								}
								else if (fabs(TileOne[j].CX / 2 + r - fDistanceX) < fabs(TileOne[j].CY / 2 + r - fDistanceY))
								{
									DIR_ONE DirLR = ball[a][i].GetDirLR();
									if (DirLR == DIR_LEFT)
									{
										int x = ball[a][i].GetX();
										x += (int)(TileOne[j].CX / 2 + r - fDistanceX);
										ball[a][i].SetX(x);
										ball[a][i].SetDirLR(DIR_RIGHT);
									}
									else if (DirLR == DIR_RIGHT)
									{
										int x = ball[a][i].GetX();
										x -= (int)(TileOne[j].CX / 2 + r - fDistanceX);
										ball[a][i].SetX(x);
										ball[a][i].SetDirLR(DIR_LEFT);
									}
								}
								else if (fabs(TileOne[j].CX / 2 + r - fDistanceX) > fabs(TileOne[j].CY / 2 + r - fDistanceY))
								{
									DIR_TWO DirUD = ball[a][i].GetDirUD();
									if (DirUD == DIR_UP)
									{
										int y = ball[a][i].GetY();
										y += (int)(TileOne[j].CY / 2 + r - fDistanceY);
										ball[a][i].SetY(y);
										ball[a][i].SetDirUD(DIR_DOWN);
									}
									else if (DirUD == DIR_DOWN)
									{
										int y = ball[a][i].GetY();
										y -= (int)(TileOne[j].CY / 2 + r - fDistanceY);
										ball[a][i].SetY(y);
										ball[a][i].SetDirUD(DIR_UP);
									}
								}
							}
						}
					}
				}
			}

			//���� �÷��̾� �浹
			for (int player = PLAYER_1; player < PLAYER_END; ++player)
			{
				RECT player_position = myPlayer[player].GetPosition();
				bool play = myPlayer[player].GetPlay();
				bool attack = myPlayer[player].GetAttack();
				int life = myPlayer[player].GetLife();
				int score = myPlayer[player].GetScore();

				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						RECT rcTemp;
						if (IntersectRect(&rcTemp, &ball[j][i].GetCollisionBox(), &player_position) && play && ball[j][i].GetType() != 4)
						{
							if (!attack)
							{
								myPlayer[player].SetLife(life - 1);
								if (life == 0)
								{
									myPlayer[player].SetPlay(false);
									if (myPlayer[(player + 1) % PLAYER_END].GetPlay() == false)
									{
										Gameover = true;
										Start = false;
										PlaySound(NULL, 0, 0);
										PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
									}
									myPlayer[player].SetScore(0);
									myPlayer[player].SetBulletCount(1);
								}
								myPlayer[player].SetAttack(true);
							}
						}
					}
				}
			}
			

			// �����۰� �÷��̾� �浹
			for (int player = PLAYER_1; player < PLAYER_END; ++player)
			{
				RECT player_position = myPlayer[player].GetPosition();
				bool play = myPlayer[player].GetPlay();
				int score = myPlayer[player].GetScore();
				if (Stop == false && Stop_item == true)
				{
					RECT rcTemp;
					if (IntersectRect(&rcTemp, &timer_item, &player_position) && play)
					{
						Stop = true;
						timer_item.top = 0;
						timer_item.left = 0;
						timer_item.right = 0;
						timer_item.bottom = 0;
						myPlayer[player].SetScore(score + 200);
						Stop_item = false;
						T = true;
						PlaySound(NULL, 0, 0);
						PlaySound("Clock.wav", NULL, SND_ASYNC);
					}
				}
				if (M == true)
				{
					RECT rcTemp;
					if (IntersectRect(&rcTemp, &Missile_item, &player_position) && play)
					{
						M = false;
						Missile_item.top = 0;
						Missile_item.left = 0;
						Missile_item.right = 0;
						Missile_item.bottom = 0;
						myPlayer[player].SetScore(score + 50);
						int bullet_count = myPlayer[player].GetBulletCount();
						if (bullet_count < 5)
							myPlayer[player].SetBulletCount(bullet_count + 1);
					}
				}
				if (H == true)
				{
					RECT rcTemp;
					if (IntersectRect(&rcTemp, &Life_item, &player_position) && play)
					{
						H = false;
						Life_item.top = 0;
						Life_item.left = 0;
						Life_item.right = 0;
						Life_item.bottom = 0;
						int life = myPlayer[player].GetLife();
						if (life < 3 && play)
							myPlayer[player].SetLife(life + 1);
						myPlayer[player].SetScore(score + 50);
					}
				}
			}

			//���� �ۻ� �浹
			for (int player = PLAYER_1; player < PLAYER_END; ++player)
			{
				int score = myPlayer[player].GetScore();
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						RECT rcTemp;
						for (int a = 0; a < 5; ++a)
						{
							RECT bullet = myPlayer[player].GetBulletPosition(a);
							if (IntersectRect(&rcTemp, &ball[j][i].GetCollisionBox(), &bullet) && ball[j][i].GetType() != 4)
							{
								//�÷��̾�1�� �ۻ�� ���� �浹
								int item = rand() % 10;
								if (item == 0 && Stop_item == false && Stop == false) // 10�ۼ�Ʈ Ȯ���� �ð����� ������ ����
								{// �ʵ忡 �ð����� �������� ���� �ð������� �ƴҶ��� ����
									timer_item.top = ball[j][i].GetY();
									timer_item.left = ball[j][i].GetX();
									timer_item.right = timer_item.left + 30;
									timer_item.bottom = timer_item.top + 30;
									Stop_item = true;
								}
								if (item >= 1 && item <= 2 && M == false) // 20�ۼ�Ʈ Ȯ���� �ۻ찹�� �߰� ������ ����
								{
									Missile_item.top = ball[j][i].GetY();
									Missile_item.left = ball[j][i].GetX();
									Missile_item.right = Missile_item.left + 30;
									Missile_item.bottom = Missile_item.top + 30;
									M = true;
								}
								if (item == 3 && H == false)
								{
									Life_item.top = ball[j][i].GetY();
									Life_item.left = ball[j][i].GetX();
									Life_item.right = Life_item.left + 30;
									Life_item.bottom = Life_item.top + 30;
									H = true;
								}
								bullet.top = 0;
								bullet.left = 0;
								bullet.bottom = 0;
								bullet.right = 0;
								myPlayer[player].SetBulletPosition(bullet, a);
								myPlayer[player].SetBulletShot(false, a);

								myPlayer[player].SetScore(score + 50);

								switch (ball[j][i].GetType())
								{
								case 0:
									myPlayer[player].SetScore(score + 100);
									ball[j][i].SetType(4);
									break;
								case 1:
									if (i == 0)
									{
										ball[j][0].SetPosition(ball[j][0].GetX(), ball[j][0].GetY(), 8, 0, ball[j][0].GetGravity(), DIR_RIGHT, ball[i][j].GetDirUD());

										ball[j][1].SetPosition(ball[j][0].GetX(), ball[j][0].GetY(), 8, 0, ball[j][0].GetGravity(), DIR_LEFT, ball[i][j].GetDirUD());
									}
									else if (i == 2)
									{
										ball[j][2].SetPosition(ball[j][2].GetX(), ball[j][2].GetY(), 8, 0, ball[j][2].GetGravity(), DIR_RIGHT, ball[j][2].GetDirUD());

										ball[j][3].SetPosition(ball[j][2].GetX(), ball[j][2].GetY(), 8, 0, ball[j][2].GetGravity(), DIR_LEFT, ball[j][2].GetDirUD());
									}
									break;
								case 2:
									ball[j][0].SetPosition(ball[j][0].GetX(), ball[j][0].GetY(), 15, 1, ball[j][0].GetGravity(), DIR_RIGHT, ball[j][0].GetDirUD());

									ball[j][2].SetPosition(ball[j][0].GetX(), ball[j][0].GetY(), 15, 1, ball[j][0].GetGravity(), DIR_LEFT, ball[j][0].GetDirUD());
									break;
								}
							}
						}
					}
				}
			}

			break;
		case 3:
			//�� ������
			if (!Stop) // Ÿ�̸�
			{
				for (int j = 0; j < 4; ++j)
				{
					for (int i = 0; i < 4; ++i)
					{
						//�߷�
						if (ball[j][i].GetRadius() != 0 && Start)
						{
							ball[j][i].Update();
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
							T = true; // ���ѽð� ����
							Gameover = true;
							Start = false;
							PlaySound(NULL, 0, 0);
							PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						break;
					case 2:
						if (Time_count == 9 && STAGE2_LIMIT_TIME - Time1_count == 0)
						{
							T = true; // ���ѽð� ����
							Gameover = true;
							Start = false;
							PlaySound(NULL, 0, 0);
							PlaySound("Ending.wav", NULL, SND_ASYNC | SND_LOOP);
						}
						break;
					case 3:
						if (Time_count == 9 && STAGE3_LIMIT_TIME - Time1_count == 0)
						{
							T = true; // ���ѽð� ����
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
						if (ball[j][i].GetType() != 4)
						{
							stage_count++;
						}
					}
				}
				if (stage_count == 0)
				{
					if (Stage < STAGE_LIMIT)
					{
						Stage++; // �������� 2 ����
						for (int player = PLAYER_1; player < PLAYER_END; ++player)
						{
							myPlayer[player].SetBulletCount(1);
						}
						Time_count = 0;
						Time1_count = 0;
						if (Stage == 2)
						{
							ball[0][0].SetPosition(200, 200, 25, 2, 0, DIR_RIGHT, DIR_DOWN);

							ball[1][0].SetPosition(800, 200, 25, 2, 0, DIR_LEFT, DIR_DOWN);
							
							ball[2][0].SetPosition(400, 250, 25, 2, 0, DIR_RIGHT, DIR_DOWN);

							ball[3][0].SetPosition(600, 250, 25, 2, 0, DIR_LEFT, DIR_DOWN);

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
							ball[0][0].SetPosition(200, 200, 25, 2, 0, DIR_RIGHT, DIR_DOWN);

							ball[1][0].SetPosition(800, 200, 25, 2, 0, DIR_LEFT, DIR_DOWN);

							ball[2][0].SetPosition(400, 250, 25, 2, 0, DIR_RIGHT, DIR_DOWN);

							ball[3][0].SetPosition(600, 250, 25, 2, 0, DIR_LEFT, DIR_DOWN);

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
			SetTimer(hWnd, 4, 1000, NULL);		//�ð� ��� Ÿ�̸�
			Start = true; // �������� ȭ���� �Ѿ
			PlaySound(NULL, 0, 0);
			PlaySound("Stage1.wav", NULL, SND_ASYNC | SND_LOOP);
		}

		if (Start && (wParam == 49 || wParam == VK_NUMPAD1) && !myPlayer[PLAYER_1].GetPlay()) // �÷��̾� 1 �߰�/��Ȱ
		{
			myPlayer[PLAYER_1].CHEAT_Reborn();
		}
		else if (Start && (wParam == 50 || wParam == VK_NUMPAD2) && !myPlayer[PLAYER_2].GetPlay()) // �÷��̾� 2 �߰�/��Ȱ
		{
			myPlayer[PLAYER_2].CHEAT_Reborn();
		}
		else if (Start && (wParam == 51 || wParam == VK_NUMPAD3) && myPlayer[PLAYER_1].GetPlay()) // �÷��̾�1 ü��ȸ�� ġƮ
		{
			myPlayer[PLAYER_1].CHEAT_MAXLIFE();
		}
		else if (Start && (wParam == 52 || wParam == VK_NUMPAD4) && myPlayer[PLAYER_2].GetPlay()) // �÷��̾�2 ü��ȸ�� ġƮ
		{
			myPlayer[PLAYER_2].CHEAT_MAXLIFE();
		}
		else if (Start && (wParam == 53 || wParam == VK_NUMPAD5) && myPlayer[PLAYER_1].GetPlay()) // �÷��̾�1 �ۻ� ġƮ
		{
			myPlayer[PLAYER_1].CHEAT_MAXBULLET();
		}
		else if (Start && (wParam == 54 || wParam == VK_NUMPAD6) && myPlayer[PLAYER_2].GetPlay()) // �÷��̾�2 �ۻ� ġƮ
		{
			myPlayer[PLAYER_2].CHEAT_MAXBULLET();
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