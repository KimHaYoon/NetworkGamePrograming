#include "stdafx.h"
#include "Player.h"
#include "Define.h"
#include "resource.h"
#include "Extern.h"

Player::Player()
{
}


Player::~Player()
{
}

HRESULT Player::Initalize(HINSTANCE g_hInst, int number)
{
	m_imgLife.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP43));
	m_imgDeathLife.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP44));
	m_imgNumber.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP40));
	m_imgBulletIcon.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP42));

	if (number == PLAYER_1)
	{
		m_nID = PLAYER_1;
		m_imgFace.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));

		m_rcPosition.top = STAGE_Y - 70;
		m_rcPosition.left = 50;
		m_rcPosition.right = 120;
		m_rcPosition.bottom = STAGE_Y;

	}
	else if (number == PLAYER_2)
	{
		m_nID = PLAYER_2;
		m_imgFace.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));

		m_rcPosition.top = STAGE_Y - 70;
		m_rcPosition.left = WINCX - 120;
		m_rcPosition.right = WINCX - 50;
		m_rcPosition.bottom = STAGE_Y;

	}

	m_bMove = false;
	m_bDir = LEFT;
	m_bAttack = false;

	m_nLife = P_LIFE;
	m_fSpeed = SPEED;

	m_bPlay = false;

	m_nBullet_Count = 1;


	return S_OK;
}

void Player::Player_Animation(HINSTANCE g_hInst, HDC memDC)
{
	if (m_bShoot)
	{
		S_count++;
		S_count = S_count % 2; // 쏘는 자세 애니메이션 카운트
		m_imgAnimation[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP15));
		m_imgAnimation[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP16));

		m_imgAnimation[S_count].TransparentBlt(memDC, m_rcPosition.left, m_rcPosition.top, 70, 70, 0, 0, m_imgAnimation[S_count].GetWidth(), m_imgAnimation[S_count].GetHeight(), RGB(0, 255, 0));
		if (S_count % 2 == 1)
		{
			m_bShoot = false;
		}
		return; // 리턴이없으면 이동애니메이션까지 작동하기때문에 여기서 끊음
	}

	if (m_bDir == RIGHT) // 오른쪽이동
	{
		RM_count++;
		RM_count = RM_count % 5; // 오른쪽 애니메이션
		m_imgAnimation[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		m_imgAnimation[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		m_imgAnimation[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		m_imgAnimation[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		m_imgAnimation[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));

		if (m_bMove)
		{
			m_imgAnimation[RM_count].TransparentBlt(memDC, m_rcPosition.left, m_rcPosition.top, 70, 70, 0, 0, m_imgAnimation[RM_count].GetWidth(), m_imgAnimation[RM_count].GetHeight(), RGB(0, 255, 0));
		}
		else // 정지할때
			m_imgAnimation[0].TransparentBlt(memDC, m_rcPosition.left, m_rcPosition.top, 70, 70, 0, 0, m_imgAnimation[RM_count].GetWidth(), m_imgAnimation[RM_count].GetHeight(), RGB(0, 255, 0));
	}
	else // 왼쪽이동
	{
		LM_count++;
		LM_count = LM_count % 5; // 왼쪽 애니메이션
		m_imgAnimation[0].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
		m_imgAnimation[1].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		m_imgAnimation[2].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		m_imgAnimation[3].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));
		m_imgAnimation[4].LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP14));

		if (m_bMove)
		{
			m_imgAnimation[LM_count].TransparentBlt(memDC, m_rcPosition.left, m_rcPosition.top, 70, 70, 0, 0, m_imgAnimation[LM_count].GetWidth(), m_imgAnimation[LM_count].GetHeight(), RGB(0, 255, 0));
		}
		else // 정지할때
		{
			m_imgAnimation[4].TransparentBlt(memDC, m_rcPosition.left, m_rcPosition.top, 70, 70, 0, 0, m_imgAnimation[LM_count].GetWidth(), m_imgAnimation[LM_count].GetHeight(), RGB(0, 255, 0));
		}
	}
}

int Player::Update(HINSTANCE g_hInst, HDC memDC)
{
	if (m_bPlay)
	{
		//점수 계산
		int temp;
		temp = m_nScore / 10000;
		m_nScoreImage[0] = temp;
		temp = m_nScore / 1000;
		m_nScoreImage[1] = temp % 10;
		temp = m_nScore / 100;
		m_nScoreImage[2] = temp % 100;
		m_nScoreImage[2] = m_nScoreImage[2] % 10;
		temp = m_nScore / 10;
		m_nScoreImage[3] = temp % 1000;
		m_nScoreImage[3] = m_nScoreImage[3] % 100;
		m_nScoreImage[3] = m_nScoreImage[3] % 10;
		temp = m_nScore % 10;
		m_nScoreImage[4] = temp;

		if (m_bAttack)
		{
			m_nInvincible_Time++;
			if (m_nInvincible_Time % 2 == 0)
				Player_Animation(g_hInst, memDC);
			if (m_nInvincible_Time == 30)
			{
				m_nInvincible_Time = 0;
				m_bAttack = false;
			}
		}
		else
		{
			Player_Animation(g_hInst, memDC);
		}
		if (m_nID == PLAYER_1) // UI
		{
			m_imgFace.TransparentBlt(memDC, 0, STAGE_Y + 25, 150, 150, 0, 0, 150, 150, RGB(255, 0, 0));

			for(int i = 0; i < P_LIFE; ++i)
			{
				if (i < m_nLife)
					m_imgLife.TransparentBlt(memDC, 150 + (i * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
				else
					m_imgDeathLife.TransparentBlt(memDC, 150 + (i * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
			}

			for (int i = 0; i < m_nBullet_Count; ++i)
			{
				m_imgBulletIcon.StretchBlt(memDC, i * 30, 700, 30, 30, 0, 0, m_imgBulletIcon.GetWidth(), m_imgBulletIcon.GetHeight(), SRCCOPY);
			}
			for (int i = 0; i < 5; ++i)
			{
				m_imgNumber.TransparentBlt(memDC, 150 + (i * 30), STAGE_Y + 35, 30, 36, m_nScoreImage[i] * (m_imgNumber.GetWidth() / 10), 0, m_imgNumber.GetWidth() / 10, m_imgNumber.GetHeight(), RGB(255, 0, 255));
			}
		}
		else if (m_nID == PLAYER_2)
		{
			m_imgFace.TransparentBlt(memDC, WINCX - 150, STAGE_Y + 25, 150, 150, 0, 0, 150, 150, RGB(255, 0, 0));

			for (int i = 0; i < P_LIFE; ++i)
			{
				if (i < m_nLife)
					m_imgLife.TransparentBlt(memDC, (WINCX - 150) - ((i + 1) * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
				else
					m_imgDeathLife.TransparentBlt(memDC, (WINCX - 150) - ((i + 1) * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
			}

			for (int i = 0; i < m_nBullet_Count; ++i)
			{
				m_imgBulletIcon.StretchBlt(memDC, WINCX - (30 * i) - 30, 700, 30, 30, 0, 0, m_imgBulletIcon.GetWidth(), m_imgBulletIcon.GetHeight(), SRCCOPY);
			}
			for (int i = 0; i < 5; ++i)
			{
				m_imgNumber.TransparentBlt(memDC, (WINCX - 150) - ((i + 1) * 30), STAGE_Y + 35, 30, 36, m_nScoreImage[i] * (m_imgNumber.GetWidth() / 10), 0, m_imgNumber.GetWidth() / 10, m_imgNumber.GetHeight(), RGB(255, 0, 255));
			}
		}

		m_bMove = false;

		m_nScore += 1;
	}
	return 0;
}

int Player::Move_Update()
{
	if (m_nID == PLAYER_1)
	{
		if (GetAsyncKeyState('A') & 0x8000)
		{
			if (m_rcPosition.left - m_fSpeed > 0)
			{
				m_rcPosition.left -= m_fSpeed;
				m_rcPosition.right -= m_fSpeed;
				m_bDir = LEFT;
			}
			m_bMove = true;
		}

		if (GetAsyncKeyState('D') & 0x8000)
		{
			if (m_rcPosition.right + m_fSpeed < WINCX)
			{
				m_rcPosition.left += m_fSpeed;
				m_rcPosition.right += m_fSpeed;
				m_bDir = RIGHT;
			}
			m_bMove = true;
		}

		if (GetAsyncKeyState('G') && 1) // 플레이어1이 작살발사 버튼을 누를때
		{
			for (int i = 0; i < m_nBullet_Count; ++i)
			{
				if (!m_bBullet_Shot[i])
				{
					m_rcBullet[i].left = m_rcPosition.left + 20;
					m_rcBullet[i].top = m_rcPosition.top;
					m_rcBullet[i].right = m_rcPosition.right - 20;
					m_rcBullet[i].bottom = m_rcPosition.bottom;

					m_nBullet_Height[i] = m_rcBullet[i].bottom - m_rcBullet[i].top;
					m_nBulletImage_Height[i] = 150;
					m_bBullet_Shot[i] = true;
					m_bShoot = true;
				}
			}
		}
	}
	else
	{
		if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		{
			if (m_rcPosition.left - m_fSpeed > 0)
			{
				m_rcPosition.left -= m_fSpeed;
				m_rcPosition.right -= m_fSpeed;
				m_bDir = LEFT;
			}
			m_bMove = true;
		}

		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (m_rcPosition.right + m_fSpeed < WINCX)
			{
				m_rcPosition.left += m_fSpeed;
				m_rcPosition.right += m_fSpeed;
				m_bDir = RIGHT;
			}
			m_bMove = true;
		}

		if (GetAsyncKeyState(VK_NUMPAD0) && 1) // 플레이어1이 작살발사 버튼을 누를때
		{
			for (int i = 0; i < m_nBullet_Count; ++i)
			{
				if (!m_bBullet_Shot[i])
				{
					m_rcBullet[i].left = m_rcPosition.left + 20;
					m_rcBullet[i].top = m_rcPosition.top;
					m_rcBullet[i].right = m_rcPosition.right - 20;
					m_rcBullet[i].bottom = m_rcPosition.bottom;

					m_nBullet_Height[i] = m_rcBullet[i].bottom - m_rcBullet[i].top;
					m_nBulletImage_Height[i] = 150;
					m_bBullet_Shot[i] = true;
					m_bShoot = true;
				}
			}
		}
	}

	return 0;
}

int Player::Bullet_Update()
{
	for (int i = 0; i < 5; ++i)
	{
		if (m_bBullet_Shot[i]) // 플레이어1의 작살 이동 타이머
		{
			m_nBullet_Height[i] += SPEED; // 작살의 길이
			m_rcBullet[i].top -= SPEED; // 작살의 y1좌표의 위치
			m_nBulletImage_Height[i] += M_SPEED; // 작살 그림에서 얼마나 출력할건지
			if (m_rcBullet[i].top < 25)
			{
				m_rcBullet[i].top = 0;
				m_rcBullet[i].left = 0;
				m_rcBullet[i].bottom = 0;
				m_rcBullet[i].right = 0;
				m_bBullet_Shot[i] = false; // 작살 타이머 정지
			}
		}
	}
	return 0;
}

int Player::Render(HDC hDC)
{
	return 0;
}

int Player::BulletRender(HDC hDC)
{
	return 0;
}

void Player::CHEAT_MAXLIFE()
{
	m_nLife = 3;
}

void Player::CHEAT_MAXBULLET()
{
	m_nBullet_Count = 5;
}

void Player::CHEAT_Reborn()
{
	m_bPlay = true;
	m_nLife = 3;
	m_bAttack = false;
	m_nBullet_Count = 1;
}

void Player::IncreaseBulletCount()
{
	if (m_nBullet_Count + 1 < 5)
		m_nBullet_Count++;
}

void Player::IncreaseInvincibleTime()
{
	m_nInvincible_Time++;
}




void Player::SetPlay(bool play)
{
	m_bPlay = play;
}

void Player::SetLife(int life)
{
	m_nLife = life;
}

void Player::SetShoot(bool shoot)
{
	m_bShoot = shoot;
}

void Player::SetPosition(RECT rt)
{
	m_rcPosition = rt;
}

void Player::SetAttack(bool attack)
{
	m_bAttack = attack;
}

void Player::SetDir(bool dir)
{
	m_bDir = dir;
}

void Player::SetMove(bool move)
{
	m_bMove = move;
}

void Player::SetBulletShot(bool bulletshot, int index)
{
	m_bBullet_Shot[index] = bulletshot;
}

void Player::SetScore(int score)
{
	m_nScore = score;
}

void Player::SetBulletPosition(RECT rt, int index)
{
	m_rcBullet[index] = rt;
}

void Player::SetBulletHeight(int height, int index)
{
	m_nBullet_Height[index] = height;
}

void Player::SetBulletImageHeight(int height, int index)
{
	m_nBulletImage_Height[index] = height;
}

void Player::SetInvincibleTime(int time)
{
	m_nInvincible_Time = time;
}

void Player::SetBulletCount(int bullet_count)
{
	m_nBullet_Count = bullet_count;
}

bool Player::GetPlay()
{
	return m_bPlay;
}

int Player::GetLife()
{
	return m_nLife;
}

bool Player::GetShoot()
{
	return m_bShoot;
}

RECT Player::GetPosition()
{
	return m_rcPosition;
}

bool Player::GetAttack()
{
	return m_bAttack;
}

bool Player::GetDir()
{
	return m_bDir;
}

bool Player::GetMove()
{
	return m_bMove;
}

bool Player::GetBulletShoot(int index)
{
	return m_bBullet_Shot[index];
}

int Player::GetScore()
{
	return m_nScore;
}

int Player::GetScore(int index)
{
	return m_nScoreImage[index];
}

RECT Player::GetBulletPosition(int index)
{
	return m_rcBullet[index];
}

int Player::GetBulletHeight(int index)
{
	return m_nBullet_Height[index];
}

int Player::GetBulletImageHeight(int index)
{
	return m_nBulletImage_Height[index];
}

int Player::GetInvincibleTime()
{
	return m_nInvincible_Time;
}

int Player::GetBulletCount()
{
	return m_nBullet_Count;
}
