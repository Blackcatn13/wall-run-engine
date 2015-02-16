#include "RenderableCommands\CDrawQuadRendererCommand.h"

void CDrawQuadRendererCommand::DrawColoredQuad2DTexturedInPixelsByEffectTechnique(CRenderManager *RM, CEffectTechnique *EffectTechnique, RECT Rect, CColor Color, CTexture *Texture,
														float U0=0.0f, float V0=0.0f, float U1=1.0f, float V1=1.0f)
{
	EffectTechnique->BeginRender();
	Máster en Creación de Videojuegos
	LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
	if(l_Effect!=NULL)
	{
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
		UINT l_NumPasses;
		l_Effect->Begin(&l_NumPasses, 0);
		for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
		{
			l_Effect->BeginPass(iPass);
			RM->DrawColoredQuad2DTexturedInPixels(Rect,Color,Texture, U0,
			V0, U1, V1);
			l_Effect->EndPass();
		}
		l_Effect->End();
	}
}

^/*crear la función DrawColoredQuad2DTexturedInPixels en GrapchisManager DrawColoredQuad2DTexturedInPixels(Rect,Color,Texture, U0,
			V0, U1, V1);

/*
technique DrawQuadTechnique
{
	pass p0
	{
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 DrawQuadPS();
	}
}*/