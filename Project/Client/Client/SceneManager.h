#pragma once

#include "main.h"

class CSceneManager
{
	class CScene* m_pCurScene;

public:
	bool Init();
	void Input();
	void Update( const float& fTimeDelta );
	void Render( HDC hDC );
	
	template <typename T>
	CScene* CreateScene( const string& strName )
	{
		SAFE_DELETE( m_pCurScene );
		GET_SINGLE( CObjectManager )->ClearObject();

		CScene*	pScene = new T;

		if ( !pScene->Init() )
		{
			delete pScene;
			pScene = NULL;
			return NULL;
		}

		m_pCurScene = pScene;

		return pScene;
	}
	
	DECLARE_SINGLE( CSceneManager )
};

