#pragma once

#include "main.h"

class CSceneManager
{
	class CScene* m_pCurScene;

public:
	bool Init();
	void Input();
	void Update();
	void Render( HDC hDC );

	void SceneChange( class CScene* pScene );
	
	DECLARE_SINGLE( CSceneManager )
};

