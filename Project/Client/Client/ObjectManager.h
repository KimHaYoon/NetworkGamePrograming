#pragma once

#include "main.h"

class CObjectManager
{
private:
	list<class CObj*>	m_ObjList;

public:
	bool Init();
	void Input();
	void Update( const float& fTimeDelta );
	void Render( HDC hDC );

public:
	template <typename T>
	CObj* CreateObject(const string& strName)
	{
		CObj*	pObj = new T;
		
		if (!pObj->Init())
		{
			delete pObj;
			pObj = NULL;
			return NULL;
		}

		pObj->SetTag( strName );
		m_ObjList.push_back( pObj );

		return pObj;
	}

	CObj* FindObject( const string& strName );

	void ClearObject();

	DECLARE_SINGLE(CObjectManager)
};

