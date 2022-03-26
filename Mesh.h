#ifndef _MESH_H
#define _MESH_H

#include "Common.h"

void DisplayMesh(FbxNode* pNode);
void DisplayControlsPoints(FbxMesh* pMesh);
void DisplayPolygons(FbxMesh* pMesh);
void DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayMaterialConnection(FbxMesh* pMesh);
void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* header, int pMatId, int l);
void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString);

#endif // !_MESH_H
