#ifndef _GENERICINFO_H
#define _GENERICINFO_H
#include "Common.h"

void DisplayGenericInfo(FbxScene* pScene);
void DisplayGenericInfo(FbxNode* pNode, int pDepth);
void DisplayProperties(FbxObject* pObject);

#endif // !_GENERICINFO_H
