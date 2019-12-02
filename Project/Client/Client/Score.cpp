#include "Score.h"

CScore::CScore()
{
}


CScore::~CScore()
{
}

bool CScore::Init()
{
	return true;
}

void CScore::Input()
{
	CObj::Input();
}

void CScore::Update( const float& fTimeDelta )
{
	CObj::Update( fTimeDelta );

	m_fScore += (10 * fTimeDelta);
	if (m_fScore >= 99999.f)
		m_fScore = 0.f;
}

void CScore::Render( HDC hDC )
{
	CObj::Render( hDC );

	string strScore{ };

	int Score = (int)m_fScore;
	int temp = Score / 10000;
	int tempScore[5]{};
	tempScore[0] = temp;
	temp = Score / 1000;
	tempScore[1] = (temp % 10);
	temp = Score / 100;
	tempScore[2] = temp % 100;
	tempScore[2] = (tempScore[2] % 10);
	temp = Score / 10;
	tempScore[3] = temp % 1000;
	tempScore[3] = tempScore[3] % 100;
	tempScore[3] = (tempScore[3] % 10);
	temp = Score % 10;
	tempScore[4] = temp;

	for(int i = 0; i < 5; ++i )
		strScore.push_back( tempScore[i] + 48);

	SetBkMode( hDC, TRANSPARENT );
	SetTextColor( hDC, RGB( 255, 255, 255 ) );
	HFONT hFont = CreateFont( 40, 0, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT( "µÕ±Ù¸ð²Ã" ) );
	HFONT oldFont = ( HFONT )SelectObject( hDC, hFont );
	TextOut( hDC, m_tPos.x, m_tPos.y, strScore.c_str(), strScore.size() );
	SelectObject( hDC, oldFont );
	DeleteObject( hFont );
}

void CScore::SetScore( int iScore )
{
	m_fScore = (float)iScore;
}
