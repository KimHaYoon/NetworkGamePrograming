#pragma once
#include "Scene.h"
class CEndingScene :
	public CScene
{
public:
	CEndingScene();
	virtual ~CEndingScene();

	// CScene을(를) 통해 상속됨
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float & fTimeDelta) override;
	virtual void Render(HDC hDC) override;
};

