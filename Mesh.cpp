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


////To tell the player what is this
void DisplayMetaData(FbxScene* pScene) {
	FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo) {
		FBXSDK_printf("SceneInfo:%s", sceneInfo->GetName());
		FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
		FBXSDK_printf("		Title:%s\n", sceneInfo->mTitle.Buffer());
		FBXSDK_printf("		Subject:%s\n", sceneInfo->mSubject.Buffer());
		FBXSDK_printf("		Author:%s\n", sceneInfo->mAuthor.Buffer());
		FBXSDK_printf("		Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		FBXSDK_printf("		Revision: %s\n", sceneInfo->mRevision.Buffer());
		FBXSDK_printf("		Comment: %s\n", sceneInfo->mComment.Buffer());
	}
}

void DisplayControlsPoints(FbxMesh* pMesh) {
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	DisplayString("	Control Points");
	for (i = 0; i < lControlPointsCount; i++) {
		DisplayInt("		Control Point", i);
		Display3DVector("            Coordinates: ", lControlPoints[i]);
		for (int j = 0; j < pMesh->GetElementNormalCount(); j++) {
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];
				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
					Display3DVector(header, leNormals->GetDirectArray().GetAt(i));
			}
		}
	}

	DisplayString("");
}

void DisplayMaterialMapping(FbxMesh* pMesh)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };

	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if (pMesh) {
		lNode = pMesh->GetNode();
		if (lNode)
			lMtrlCount = lNode->GetMaterialCount();
	}

	for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
		if (leMat)
		{
			char header[100];
			FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
			DisplayString(header);


			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
			DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

			int lMaterialCount = 0;
			FbxString lString;

			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lMaterialCount = lMtrlCount;
			}

			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int i;

				lString = "           Indices: ";

				int lIndexArrayCount = leMat->GetIndexArray().GetCount();
				for (i = 0; i < lIndexArrayCount; i++)
				{
					lString += leMat->GetIndexArray().GetAt(i);

					if (i < lIndexArrayCount - 1)
					{
						lString += ", ";
					}
				}

				lString += "\n";

				FBXSDK_printf(lString);
			}
		}
	}

	DisplayString("");
}

void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString)
{
	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (lLayeredTextureCount > 0)
	{
		for (int j = 0; j < lLayeredTextureCount; ++j)
		{
			FbxLayeredTexture* lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
			pConnectionString += " Texture ";

			for (int k = 0; k < lNbTextures; ++k)
			{
				//lConnectionString += k;
				pConnectionString += "\"";
				pConnectionString += (char*)lLayeredTexture->GetName();
				pConnectionString += "\"";
				pConnectionString += " ";
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " on layer ";
			pConnectionString += j;
		}
		pConnectionString += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

		if (lNbTextures > 0)
		{
			pConnectionString += " Texture ";
			pConnectionString += " ";

			for (int j = 0; j < lNbTextures; ++j)
			{
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if (lTexture)
				{
					pConnectionString += "\"";
					pConnectionString += (char*)lTexture->GetName();
					pConnectionString += "\"";
					pConnectionString += " ";
				}
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " |";
		}
	}
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