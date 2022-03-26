#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Common.h"

//Used be in texture
void DisplayTexture(FbxGeometry* pGeometry);
void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode);
void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);

#endif // !_TEXTURE_H
