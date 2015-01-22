#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d9.h>
#include "Utils\Named.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include <assert.h>

class CTexture : public CNamed
{
protected:
	LPDIRECT3DTEXTURE9 m_Texture;
	std::string m_FileName;
	virtual bool LoadFile();
	void Unload();

	//////
	unsigned int m_Width, m_Height;
	IDirect3DSurface9	* m_DepthStencilRenderTargetTexture, *m_OldRenderTarget, *m_RenderTargetTexture, *m_OldDepthStencilRenderTarget;
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
	

	////////////////////////////////////////////////////////////////7
	enum TPoolType {
		DEFAULT=0,
		SYSTEMMEM
	};
	enum TUsageType {
		DYNAMIC=0,
		RENDERTARGET
	};
	enum TTextureType {
		TGA=0,
		JPG,
		BMP
	};
	enum TFormatType {
		A8R8G8B8=0,
		R8G8B8,
		X8R8G8B8,
		R32F
	};
	bool Create(const std::string &Name, unsigned int Width, unsigned int
	Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType,
	TFormatType FormatType)
	{
		setName(Name);
		D3DPOOL l_Pool=D3DPOOL_DEFAULT;
		DWORD l_UsageType=D3DUSAGE_DYNAMIC;
		D3DFORMAT l_Format=D3DFMT_A8R8G8B8;
		bool l_CreateDepthStencilSurface=false;
		switch(UsageType)
		{
		case DYNAMIC:
			l_UsageType=D3DUSAGE_DYNAMIC;
			break;
		case RENDERTARGET:
			l_CreateDepthStencilSurface=true;
			l_UsageType=D3DUSAGE_RENDERTARGET;
			break;
		}
		switch(PoolType)
		{
		case DEFAULT:
			l_Pool=D3DPOOL_DEFAULT;
			break;
		case SYSTEMMEM:
			l_Pool=D3DPOOL_SYSTEMMEM;
			break;
		}
		switch(FormatType)
		{
		case A8R8G8B8:
			l_Format=D3DFMT_A8R8G8B8;
			break;
		case R8G8B8:
			l_Format=D3DFMT_R8G8B8;
			break;
		case X8R8G8B8:
			l_Format=D3DFMT_X8R8G8B8;
			break;
		case R32F:
			l_Format=D3DFMT_R32F;
			break;
		}
		HRESULT hr=GRAPHM->GetDevice()->CreateTexture(
		Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,
		&m_Texture,NULL);
		if(l_CreateDepthStencilSurface)
		{
			GRAPHM->GetDevice()->
			CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8,
			D3DMULTISAMPLE_NONE, 0, TRUE,
			&m_DepthStencilRenderTargetTexture, NULL);
			assert(m_DepthStencilRenderTargetTexture!=NULL);
		}
		assert(m_Texture!=NULL);
		assert(hr==D3D_OK);
		m_Width=Width;
		m_Height=Height;
		return hr!=D3D_OK;
	}

	void Deactivate(size_t Stage);
	bool SetAsRenderTarget(size_t IdStage=0);
	void UnsetAsRenderTarget(size_t IdStage=0);
	void CaptureFrameBuffer(size_t IdStage);
	CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType);

};



#endif