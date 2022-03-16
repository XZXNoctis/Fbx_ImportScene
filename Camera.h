#ifndef _CAMERA_H
#define _CAMERA_H

#include "Common.h"

void DisplayCamera(FbxNode* pNode);
void DisplayCamera(FbxCamera* pCamera, char* pName, FbxNode* pTargetNode = NULL, FbxNode* pTargetUpNode = NULL);
void DisplayDefaultAnimationValues(FbxCamera* pCamera);
void DisplayRenderOptions(FbxCamera* pCamera);
void DisplayCameraViewOptions(FbxCamera* pCamera);
void DisplayBackgroundProperties(FbxCamera* pCamera);
void DisplayApertureAndFilmControls(FbxCamera* pCamera);
void DisplayViewingAreaControls(FbxCamera* pCamera);
void DisplayCameraPositionAndOrientation(FbxCamera* pCamera, FbxNode* pTargetNode, FbxNode* pUpTargetNode);

#endif _CAMERA_H
