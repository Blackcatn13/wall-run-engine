#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <d3d9.h>
#include "Utils\Named.h"
#include "Core\Core.h"
#include "GraphicsManager.h"
#include <assert.h>
#include <string>

class CTexture : public CNamed
{
protected:
    IDirect3DBaseTexture9* m_Texture;
    std::string m_FileName;
    virtual bool LoadFile();
	virtual bool LoadFileGUI();
    void Unload();
    bool m_reloadable;
	bool m_DepthStencil;

    //////
    unsigned int m_Width, m_Height;
    IDirect3DSurface9	* m_DepthStencilRenderTargetTexture, *m_OldRenderTarget, *m_RenderTargetTexture, *m_OldDepthStencilRenderTarget;
public:
    CTexture();
    ~CTexture()
    {
        Unload();
    }
    const std::string & GetFileName() const
    {
        return m_FileName;
    }
    bool Load(const std::string &FileName);
	bool LoadGUI(const std::string &FileName);
    bool Reload();
    void Activate(size_t StageId);


    ////////////////////////////////////////////////////////////////7
    enum TPoolType {
        DEFAULT = 0,
        SYSTEMMEM
    };
    enum TUsageType {
        DYNAMIC = 0,
        RENDERTARGET
    };
    enum TTextureType {
        TGA = 0,
        JPG,
        BMP
    };
    enum TFormatType {
        A8R8G8B8 = 0,
        R8G8B8,
        X8R8G8B8,
        R32F
    };
    bool Create(const std::string &Name, unsigned int Width, unsigned int
                Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType,
                TFormatType FormatType, bool new_stencil = false); //UsageType RenderTarget, 0


    void Deactivate(size_t Stage);
    bool SetAsRenderTarget(size_t IdStage = 0);
    void UnsetAsRenderTarget(size_t IdStage = 0);
    void CaptureFrameBuffer(size_t IdStage);
    CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType);
    int GetWidth()
    {
        return m_Width;
    }
    int GetHeight()
    {
        return m_Height;
    }
    bool SaveToFile(const std::string &Filename);

};



#endif