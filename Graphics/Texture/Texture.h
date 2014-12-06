#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d9.h>

class CTexture
{
protected:
	LPDIRECT3DTEXTURE9 m_Texture;
	std::string m_FileName;
	virtual bool LoadFile();
	void Unload();
public:
	CTexture();
	~CTexture()
	{
		Unload();
	}
	const std::string & GetFileName() const {return m_FileName; }
	bool Load(const std::string &FileName);
	bool Reload();
	void Activate(size_t StageId);
};

#endif