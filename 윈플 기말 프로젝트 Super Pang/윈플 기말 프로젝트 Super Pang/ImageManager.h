#pragma once
class ImageManager
{
private:
	ImageManager();
	~ImageManager();

	static ImageManager* m_pInstance;

public:
	ImageManager* GetInstance();

	void Destroy_Instance();
};

