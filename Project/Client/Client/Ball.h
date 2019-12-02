#pragma once
#include "Obj.h"
#define DIR_RIGHT false
#define DIR_LEFT true
#define DIR_UP false
#define DIR_DOWN true

// Type
// Radius 25 = A
// Radius 15 = B
// Radius 8 = C

class CBall :
	public CObj
{
	BALLINFO m_tInfo;

	bool m_bDirLR;
	bool m_bDirUD;

	int m_nGravity; // 낙하속도

public:
	CBall();
	virtual ~CBall();

	// CObj을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetBallInfo(BALLINFO tInfo);
	void SetGravity(int nGravity);
	void SetDirLR(bool LR);
	void SetDirUD(bool UD);

	BALLINFO GetBallInfo();
	int GetGravity();
	bool GetDirLR();
	bool GetDirUD();
};

