#include "Input.h"

DEFINITION_SINGLE(CInput)

CInput::CInput()	:
	m_pCreate(NULL)
{
}

CInput::~CInput()
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();
	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
	
	m_mapKey.clear();
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	
	// �÷��̾�
	CreateKey(VK_LEFT, "MoveLeft");
	CreateKey(VK_RIGHT, "MoveRight");
	CreateKey(VK_UP, "MoveUp");
	CreateKey(VK_DOWN, "MoveDown");

	return true;
}

void CInput::Update()
{
	unordered_map<string, PKEYINFO>::iterator	iter;
	unordered_map<string, PKEYINFO>::iterator	iterEnd = m_mapKey.end();

	for (iter = m_mapKey.begin(); iter != iterEnd; ++iter)
	{
		int	iPushCount = 0;
		for (size_t i = 0; i < iter->second->vecKey.size(); ++i)
		{
			if (GetAsyncKeyState(iter->second->vecKey[i]) & 0x8000)
				++iPushCount;
		}

		// ���Ϳ� �ִ� Ű�� ��� ������ ���
		if (iPushCount == iter->second->vecKey.size())
		{
			if (!iter->second->bDown && !iter->second->bPush)
				iter->second->bDown = true;

			else
			{
				iter->second->bPush = true;
				iter->second->bDown = false;
			}
		}

		// ���� �����ӿ� �� Ű�� ������ �־��� ���
		else if (iter->second->bDown || iter->second->bPush)
		{
			iter->second->bUp = true;
			iter->second->bDown = false;
			iter->second->bPush = false;
		}

		else if (iter->second->bUp)
		{
			iter->second->bUp = false;
		}
	}
}

PKEYINFO CInput::FindKey(const string & strKey)	const
{
	unordered_map<string, PKEYINFO>::const_iterator	iter = m_mapKey.find(strKey);

	if (iter == m_mapKey.end())
		return NULL;

	return iter->second;
}

bool CInput::KeyDown(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bDown;
}

bool CInput::KeyPush(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bPush;
}

bool CInput::KeyUp(const string & strKey) const
{
	PKEYINFO pKey = FindKey(strKey);

	if (!pKey)
		return false;

	return pKey->bUp;
}
