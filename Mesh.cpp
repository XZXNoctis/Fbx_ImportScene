#include "Mesh.h"
#include "Material.h"
#include "Texture.h"
#include "Link.h"
#include "Shape.h"
#include "Cache.h"


#define MAT_HEADER_LENGTH 200


////Used in DisplayMesh
void DisplayMesh(FbxNode* pNode) {
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

	DisplayString("Mesh Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lMesh);
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);
	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	DisplayTexture(lMesh);
	DisplayMaterialConnection(lMesh);
	DisplayLink(lMesh);
	DisplayShape(lMesh);
	DisplayCache(lMesh);

}

void DisplayMaterialConnection(FbxMesh* pMesh) {
	int i, l, lPolygonCount = pMesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayString("    Polygons Material Connections");

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++) {
		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) {
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame Mapping type,just out the material and texture mapping info once
	if (lIsAllSame) {
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++) {
			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				if (lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}

		//no material
		if (l == 0)
			DisplayString("        no material applied");
	}

	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);

			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if (lMatId >= 0)
				{
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}
	}
}

void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* header, int pMatId, int l) {
	if (!pMaterial)
		return;

	FbxString lConnectionString = "            Material %d -- ";
	//Show all the textures

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	DisplayTextureNames(lProperty, lConnectionString);

	//DiffuseFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Emissive Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	DisplayTextureNames(lProperty, lConnectionString);

	//EmissiveFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
	DisplayTextureNames(lProperty, lConnectionString);


	//Ambient Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	DisplayTextureNames(lProperty, lConnectionString);

	//AmbientFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Specular Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	DisplayTextureNames(lProperty, lConnectionString);

	//SpecularFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Shininess Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	DisplayTextureNames(lProperty, lConnectionString);

	//Bump Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
	DisplayTextureNames(lProperty, lConnectionString);

	//Normal Map Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	DisplayTextureNames(lProperty, lConnectionString);

	//Transparent Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
	DisplayTextureNames(lProperty, lConnectionString);

	//TransparencyFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Reflection Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	DisplayTextureNames(lProperty, lConnectionString);

	//ReflectionFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}