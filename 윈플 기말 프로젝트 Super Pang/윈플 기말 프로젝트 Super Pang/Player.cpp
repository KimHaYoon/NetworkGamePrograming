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
	for (int i = PLAYER_1; i < PLAYER_END; ++i)
	{
		if (i == PLAYER_1)
		{
			m_nID = PLAYER_1;
			m_imgFace.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP36));

			m_rcPosition.top = STAGE_Y - 70;
			m_rcPosition.left = 50;
			m_rcPosition.right = 120;
			m_rcPosition.bottom = STAGE_Y;

			break;
		}
		else if (i == PLAYER_2)
		{
			m_nID = PLAYER_2;
			m_imgFace.LoadFromResource(g_hInst, MAKEINTRESOURCE(IDB_BITMAP37));

			m_rcPosition.top = STAGE_Y - 70;
			m_rcPosition.left = rt.right - 120;
			m_rcPosition.right = rt.right - 50;
			m_rcPosition.bottom = STAGE_Y;

			break;
		}
	}

	m_bMove = false;
	m_bDir = LEFT;
	m_bAttack = false;

	m_nLife = P_LIFE;

	m_bPlay = false;

	m_nBullet_Count = 1;


	return S_OK;
}

void Player::Player_Animation(HINSTANCE g_hInst, HDC memDC)
{
}

int Player::Update(HINSTANCE g_hInst, HDC memDC)
{
	if (m_bPlay)
	{
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
			m_imgFace.TransparentBlt(memDC, rt.right - 150, STAGE_Y + 25, 150, 150, 0, 0, 150, 150, RGB(255, 0, 0));

			for (int i = 0; i < P_LIFE; ++i)
			{
				if (i < m_nLife)
					m_imgLife.TransparentBlt(memDC, (rt.right - 150) - ((i + 1) * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
				else
					m_imgDeathLife.TransparentBlt(memDC, (rt.right - 150) - ((i + 1) * 50), STAGE_Y + 75, 50, 50, 0, 0, 30, 30, RGB(27, 24, 24));
			}

			for (int i = 0; i < m_nBullet_Count; ++i)
			{
				m_imgBulletIcon.StretchBlt(memDC, rt.right - (30 * i) - 30, 700, 30, 30, 0, 0, m_imgBulletIcon.GetWidth(), m_imgBulletIcon.GetHeight(), SRCCOPY);
			}
			for (int i = 0; i < 5; ++i)
			{
				m_imgNumber.TransparentBlt(memDC, (rt.right - 150) - ((i + 1) * 30), STAGE_Y + 35, 30, 36, m_nScoreImage[i] * (m_imgNumber.GetWidth() / 10), 0, m_imgNumber.GetWidth() / 10, m_imgNumber.GetHeight(), RGB(255, 0, 255));
			}
		}

		m_bMove = false;
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
