#pragma once
#include "Scene.h"
class CLobbyScene :
	public CScene
{
private:
	bool	m_bGameStart;
public:
	CLobbyScene();
	virtual ~CLobbyScene();


	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

