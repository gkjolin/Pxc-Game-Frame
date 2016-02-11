#include "SpriteProduct.h"
#include "SpriteLine.h"
#include "PublicDefinitions/CoreConstantsDef.h"

#define SPRITE_LOADING_DRAW_ID CoreConstantsDef::Assets_Consts::c_iAssets_LoadingDrawID

CSpriteLine::CSpriteLine()
{
	m_pLoadingDraw = NULL;
}

CSpriteLine::~CSpriteLine()
{
	//
}

bool CSpriteLine::Init(const char* szFileName)
{
	bool bRet = CBaseLine::Init(szFileName);
	m_pLoadingDraw = Fetch(SPRITE_LOADING_DRAW_ID, CBaseProduct::ESharedProduct);
	return bRet;
}

void CSpriteLine::Release()
{
	Discard(m_pLoadingDraw);
	CBaseLine::Release();
}

CSpriteProduct* CSpriteLine::Fetch(int iID, CBaseProduct::ELoadType eLoadType, bool bAsyn)
{
	CSpriteProduct* pProduct = static_cast<CSpriteProduct*>(FetchProduct(iID, eLoadType, bAsyn));
	if (pProduct)
		pProduct->SetLine(this);
	return pProduct;
}

void CSpriteLine::DrawLoading(cocos2d::Vec4& v4Trans, cocos2d::Renderer* renderer, const cocos2d::Mat4& parentTrans, uint32_t parentFlags)
{
	if (!m_pLoadingDraw)
		return;
	m_pLoadingDraw->SetTransform(v4Trans);
	m_pLoadingDraw->Draw(renderer, parentTrans, parentFlags);
}

CBaseProduct* CSpriteLine::CreateProduct()
{
	return new CSpriteProduct();
}