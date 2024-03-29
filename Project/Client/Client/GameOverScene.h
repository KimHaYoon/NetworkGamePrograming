#pragma once
#include "Scene.h"
class CGameOverScene :
	public CScene
{
public:
	CGameOverScene();
	virtual ~CGameOverScene();

	// CScene을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float & fTimeDelta) override;
	virtual void Render(HDC hDC) override;
};

