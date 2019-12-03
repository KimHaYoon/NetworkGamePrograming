#include "StageTime.h"
#include "Network.h"


CStageTime::CStageTime()
{
}


CStageTime::~CStageTime()
{
}

bool CStageTime::Init()
{
	SetPos(320, 505);
	return true;
}

void CStageTime::Input()
{
	CObj::Input();
}

void CStageTime::Update(const float & fTimeDelta)
{
	CObj::Update(fTimeDelta);

	m_fTime = GET_SINGLE(CNetwork)->GetStageLimitTime();
}

void CStageTime::Render(HDC hDC)
{
	CObj::Render(hDC);

	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(200, 100, 100));
	HFONT hFont = CreateFont(30, 0, 0, 0, 600, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("µÕ±Ù¸ð²Ã"));
	HFONT oldFont = (HFONT)SelectObject(hDC, hFont);
	string strTime = string(m_strTag + to_string((int)m_fTime));
	TextOut(hDC, m_tPos.x, m_tPos.y, strTime.c_str(), strTime.size());
	SelectObject(hDC, oldFont);
	DeleteObject(hFont);
}

void CStageTime::SetTime(float LimitTime)
{
	m_fTime = LimitTime;
}

float CStageTime::GetTime() const
{
	return m_fTime;
}

bool CStageTime::CheckTime()
{
	if (m_fTime <= 0.f)
		return true;

	return false;
}
