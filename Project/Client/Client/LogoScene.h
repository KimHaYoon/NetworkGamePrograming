#pragma once
#include "Scene.h"
class CLogoScene :
	public CScene
{
public:
	CLogoScene();
	virtual ~CLogoScene();

	// CScene��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update() override;
	virtual void Render( HDC hDC ) override;
};

