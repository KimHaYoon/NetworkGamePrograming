#pragma once
#include "Obj.h"
class CStageTime :
	public CObj
{
	float m_fTime;
public:
	CStageTime();
	virtual ~CStageTime();

	// CObj��(��) ���� ��ӵ�
	virtual bool Init() override;
	virtual void Input() override;
	virtual void Update(const float& fTimeDelta);
	virtual void Render(HDC hDC);

	void SetTime(float LimitTime);
	float GetTime() const;
	bool CheckTime();
};

