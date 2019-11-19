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

void CLogoObj::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );
}

void CLogoObj::Render( HDC hDC )
{
	CObj::Render( hDC );
}
