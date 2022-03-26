#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "Common.h"

void DisplayAnimation(FbxScene* pScene);
void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);
void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve)(FbxAnimCurve* pCurve), void (*DisplayListCurve)(FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher);
void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

#endif // !_ANIMATION_H

