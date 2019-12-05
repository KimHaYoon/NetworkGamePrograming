#include "ObjectManager.h"
#include "Obj.h"

DEFINITION_SINGLE(CObjectManager)

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
	list<class CObj*>::iterator	iter = m_ObjList.begin();
	list<class CObj*>::iterator	iterEnd = m_ObjList.end();

	while ( iter != iterEnd )
	{
		SAFE_DELETE( *iter );
		++iter;
	}

	m_ObjList.clear();
}

bool CObjectManager::Init()
{
	return true;
}

void CObjectManager::Input()
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd; )
	{
		( *iter )->Input( );

		if ( !( *iter )->GetAlive() )
		{
			SAFE_DELETE( *iter );
			m_ObjList.erase( iter );
		}

		++iter;
	}
}

void CObjectManager::Update( const float& fTimeDelta )
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		( *iter )->Update( fTimeDelta );		
		
		if ( !( *iter )->GetAlive() )
		{
			SAFE_DELETE( *iter );
			m_ObjList.erase( iter );
		}

		++iter;
	}
}

void CObjectManager::Render(HDC hDC)
{
	list<CObj*>::iterator	iter;
	list<CObj*>::iterator	iterEnd = m_ObjList.end();

	for ( iter = m_ObjList.begin(); iter != iterEnd;)
	{
		( *iter )->Render( hDC );		
		
		if ( !( *iter )->GetAlive() )
		{
			SAFE_DELETE( *iter );
			m_ObjList.erase( iter );
		}

		++iter;
	}
}

CObj * CObjectManager::FindObject( const string & strName )
{
	auto iter = find_if( m_ObjList.begin(), m_ObjList.end(), [&strName](CObj* a) {
		if ( a->GetTag() == strName )
			return a;
	} );

	if ( iter == m_ObjList.end() )
		return NULL;

	return *iter;
}

void CObjectManager::ClearObject()
{

	list<class CObj*>::iterator	iter = m_ObjList.begin();
	list<class CObj*>::iterator	iterEnd = m_ObjList.end();

	while ( iter != iterEnd )
	{
		SAFE_DELETE( *iter );
		++iter;
	}

	m_ObjList.clear();
}
