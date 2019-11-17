#include "LogoObj.h"
#include "ObjectManager.h"



CLogoObj::CLogoObj()
{
}


CLogoObj::~CLogoObj()
{
}

bool CLogoObj::Init()
{
	SetTexture( "Logo", L"Texture/Logo/Logo.bmp" );

	return true;
}

void CLogoObj::Input()
{
	CObj::Input();
}

void CLogoObj::Update( DWORD dwTime )
{
	CObj::Update( dwTime );
}

void CLogoObj::Render( HDC hDC )
{
	CObj::Render( hDC );
}
