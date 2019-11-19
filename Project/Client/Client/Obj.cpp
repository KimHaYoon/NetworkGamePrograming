#include "Obj.h"
#include "Texture.h"
#include "ResourcesManager.h"

CObj::CObj()
{
}

CObj::~CObj()
{
}

CTexture * CObj::GetTexture() const
{
	return nullptr;
}

void CObj::SetScene( CScene * pScene )
{
}

void CObj::SetTexture( const string & strKey, const wchar_t * pFullPath, bool bColorKey, COLORREF dwColorKey )
{
	if ( pFullPath )
		m_pTexture = GET_SINGLE( CResourcesManager )->LoadTexture( strKey, pFullPath, bColorKey, dwColorKey );

	else
		m_pTexture = GET_SINGLE( CResourcesManager )->FindTexture( strKey );
}

bool CObj::Init()
{
	return true;
}

void CObj::Input()
{
}

void CObj::Update( const float& fTimeDelta )
{
}

void CObj::Render( HDC hDC )
{
	POS	tPos = m_tPos;

	if ( tPos.x + m_tSize.x< 0 ||
		tPos.x  > WINX ||
		tPos.y + m_tSize.y < 0 ||
		tPos.y  > WINY )
		return;

	if ( m_pTexture )
	{
		if ( !m_pTexture->GetColorKeyEnable() )
		{
			// BitBlt �Լ��� Ư�� DC�� ����� �Ǵ� DC�� ������ִ� �Լ��̴�.
			// 1�� ���ڴ� ����� �Ǵ� DC�� ���� DC���ٰ� ����ϰ� �ȴ�.
			// 2��, 3�� ���ڴ� x, y�ε� �� ���� ��� ����� DC���� ��ǥ�� �������ش�.
			// ��ǥ�� �� ��� ���� �ȴ�.
			// 4��, 5�� ���ڴ� ����, ������ ��� ũ���̴�.
			// 2, 3�� ������ ��ǥ�κ��� 4��, 5�� ������ ����, ���� ũ�⸸ŭ
			// 6�����ڿ� ������ DC�� �׷�����.
			// 7, 8�� ���ڴ� �̹����� ��� �������� �����Ѵ�.
			int	x, y;
			x = tPos.x;
			y = tPos.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;

			BitBlt( hDC, x, y, iWidth, iHeight, m_pTexture->GetMemDC(), iImageX, iImageY, SRCCOPY );
		}

		else
		{
			// ColorKey�� �����ϰ� ����Ѵ�.
			int	x, y;
			x = tPos.x;
			y = tPos.y;

			UINT	iWidth, iHeight;
			iWidth = m_pTexture->GetWidth();
			iHeight = m_pTexture->GetHeight();

			UINT	iImageX = 0, iImageY = 0;
			
			if(m_tSize.x > 0 && m_tSize.y > 0 )
				TransparentBlt( hDC, x, y, m_tSize.x, m_tSize.y,
					m_pTexture->GetMemDC(), iImageX, iImageY, iWidth, iHeight,
					m_pTexture->GetColorKey() );
			else
				TransparentBlt( hDC, x, y, iWidth, iHeight,
					m_pTexture->GetMemDC(), iImageX, iImageY, iWidth, iHeight,
					m_pTexture->GetColorKey() );
		}
	}
}
