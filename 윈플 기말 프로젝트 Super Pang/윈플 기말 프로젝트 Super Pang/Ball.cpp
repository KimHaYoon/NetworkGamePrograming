#include "stdafx.h"
#include "Ball.h"
#include "Define.h"

Ball::Ball()
{
}

Ball::~Ball()
{
}

HRESULT Ball::Initialize()
{
	m_nX = 0;
	m_nX = 0;
	m_nY = 0;
	m_nR = 0;
	m_nType = 4;
	m_nGravity = 0;
	SetRect();
	m_DirLR = DIR_RIGHT;
	m_DirUD = DIR_DOWN;

	return S_OK;
}

HRESULT Ball::Initialize(int x, int y, int r, int type, DIR_ONE dirLR, DIR_TWO dirUD)
{
	m_nX = x;
	m_nY = y;
	m_nR = r;
	m_nType = type;
	m_nGravity = 0;
	SetRect();
	m_DirLR = dirLR;
	m_DirUD = dirUD;

	return S_OK;
}

int Ball::Update()
{
	m_nGravity = (m_nY - 140) / 10; // 낙하속도

	if (m_DirLR == DIR_LEFT)
	{
		m_nX -= 4;

		if (m_nX - m_nR < 30) // 왼쪽 벽에 충돌
			m_DirLR = DIR_RIGHT;
	}
	else if (m_DirLR == DIR_RIGHT)
	{
		m_nX += 4;

		if (m_nX + m_nR > WINCX - 30) // 오른쪽 벽에 충돌
			m_DirLR = DIR_LEFT;
	}

	if (m_DirUD == DIR_DOWN)
	{
		m_nY += m_nGravity;

		if (m_nY + m_nR > STAGE_Y - 25) // 바닥에 충돌
			m_DirUD = DIR_UP;
	}
	else if (m_DirUD == DIR_UP)
	{
		m_nY -= m_nGravity;

		if (m_nY - m_nR < 150) // 최대 높이 값
			m_DirUD = DIR_DOWN;
	}

	SetRect();
	return 0;
}

int Ball::Render()
{
	return 0;
}

void Ball::SetPosition(int x, int y, int r, int type, int gravity, DIR_ONE dirLR, DIR_TWO dirUD)
{
	m_nX = x;
	m_nY = y;
	m_nR = r;
	m_nType = type;
	m_nGravity = gravity;
	SetRect();
	m_DirLR = dirLR;
	m_DirUD = dirUD;
}

void Ball::SetRect()  // 타이머에서 움직일 때마다 갱신해주려고 씀
{
	m_rcCollisionBox.left = m_nX - (m_nR - 3);
	m_rcCollisionBox.top = m_nY - (m_nR - 3);
	m_rcCollisionBox.right = m_nX + (m_nR - 3);
	m_rcCollisionBox.bottom = m_nY + (m_nR - 3);
}

void Ball::SetX(int x)
{
	m_nX = x;
}

void Ball::SetY(int y)
{
	m_nY = y;
}

void Ball::SetRadius(int r)
{
	m_nR = r;
}

void Ball::SetType(int type)
{
	m_nType = type;
}

void Ball::SetGravity(int gravity)
{
	m_nGravity = gravity;
}

void Ball::SetDirLR(DIR_ONE dir)
{
	m_DirLR = dir;
}

void Ball::SetDirUD(DIR_TWO dir)
{
	m_DirUD = dir;
}

int Ball::GetX()
{
	return m_nX;
}

int Ball::GetY()
{
	return m_nY;
}

int Ball::GetRadius()
{
	return m_nR;
}

int Ball::GetType()
{
	return m_nType;
}

int Ball::GetGravity()
{
	return m_nGravity;
}

DIR_ONE Ball::GetDirLR()
{
	return m_DirLR;
}

DIR_TWO Ball::GetDirUD()
{
	return m_DirUD;
}

RECT Ball::GetCollisionBox()
{
	return m_rcCollisionBox;
}
