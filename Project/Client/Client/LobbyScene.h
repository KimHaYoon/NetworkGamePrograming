#pragma once
#include "Scene.h"
class CLobbyScene :
	public CScene
{
private:
	GAME_STATE	m_eGameState;
public:
	CLobbyScene();
	virtual ~CLobbyScene();


	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update( const float& fTimeDelta ) override;
	virtual void Render( HDC hDC ) override;
};

