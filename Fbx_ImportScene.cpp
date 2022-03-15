#include<fbxsdk.h>
#include<fbxsdk/fileio/fbxiosettings.h>
#include<iostream>

//Create Fbx Sdk Manager
//FbxManager* lSdkManager = FbxManager::Create();
//FbxScene* lScene = FbxScene::Create(lSdkManager, "lScne");

//Function Definition
void DisplayGlobalLightSettings(FbxGlobalSettings* pGlobalSettings);
void DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings);
void DisplayHierarchy(FbxScene* pScene);
void DisplayHierarchy(FbxNode* pNode, int pDepth);
void DisplayMetaData(FbxScene* pScene);
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);

void DisplayMesh(FbxNode* pNode);
void DisplayControlsPoints(FbxMesh* pMesh);
void DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayPolygons(FbxMesh* pMesh);
void DisplayLink(FbxGeometry* pGeometry);

void DisplayPose(FbxScene* pScene);

void DisplayAnimation(FbxScene* pScene);
void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);
void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void(*DisplayCurve)(FbxAnimCurve* pCurve), void (*DisplayListCurve)(FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher);
void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayGenericInfo(FbxScene* pScene);
void DisplayGenericInfo(FbxNode* pNode, int pDepth);
void DisplayProperties(FbxObject* pObject);

void DisplaySkeleton(FbxNode* pNode);
//For Common
void DisplayMetaDataConnections(FbxObject* pNode);
void DisplayString(const char* pHeader, const char* pValue = "", const char* pSuffix = "");
void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");
void DisplayInt(const char* pHeader, int pValue, const char* pSuffix = "");
void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");
void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix = "");
void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix = "");

static bool gVerbose = true;

int main() {
	//FbxManager* lSdkManager = NULL;
	//FbxScene* lScene = NULL;

	FbxManager* lSdkManager = FbxManager::Create();
	//FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	//File Import
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "lImporter");

	//SetFileName
	const char* importFileName = "CubeAnimation_Light_Camera.FBX";

	bool lImportStatus = lImporter->Initialize(importFileName, -1, lSdkManager->GetIOSettings());

	if (!lImportStatus) {
		printf("Call to FbxImporter::Intialize()failed.\n");
		printf("Error returned:%s\n\n", lImporter->GetStatus().GetErrorString());
		gVerbose = false;
		exit(-1);
	}
	else {
		printf("%s Import Success.\n", importFileName);
	}




	//Create a new scene so it can be populated by the imported file.
	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");


	lImporter->Import(lScene);


	lImporter->Destroy();



	//File format version numbers to the populated
	int lFilemajor, lFileminor, lFileRevision;

	lImporter->GetFileVersion(lFilemajor, lFileminor, lFileRevision);
	if (!lImporter) {
		printf("FBX version number for file %s is %d.%d.%d",
			importFileName, lFilemajor, lFileminor, lFileRevision);
		printf("Error returned:%s\n\n", lImporter->GetStatus().GetErrorString());
		gVerbose = false;
		exit(-1);
	}
	else {
		printf("Import Success.\n");
	}


	//Display the scene
	DisplayMetaData(lScene);
	FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

	if (gVerbose) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

	FBXSDK_printf("\n\n---------------------\nGlobal Time Settings\n---------------------\n\n");

	if (gVerbose) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

	FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

	if (gVerbose) DisplayHierarchy(lScene);

	FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

	if (gVerbose) DisplayContent(lScene);

	FBXSDK_printf("\n\n----\nPose\n----\n\n");

	if (gVerbose) DisplayPose(lScene);

	FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

	if (gVerbose) DisplayAnimation(lScene);

	FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
	if (gVerbose) DisplayGenericInfo(lScene);






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
//For know the scene time

void DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings) {
	char lTimeString[256];

	DisplayString("Time Mode : ", FbxGetTimeModeName(pGlobalSettings->GetTimeMode()));

	FbxTimeSpan lTs;
	FbxTime lStart, lEnd;
	pGlobalSettings->GetTimelineDefaultTimeSpan(lTs);
	lStart = lTs.GetStart();
	lEnd = lTs.GetStop();
	DisplayString("Timeline default timespan:");
	DisplayString("		Start:", lStart.GetTimeString(lTimeString, FbxUShort(256)));
	DisplayString("		End:", lEnd.GetTimeString(lTimeString, FbxUShort(256)));
	DisplayString("	\n");
}

////For Scene
void DisplayHierarchy(FbxScene* pScene)
{
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();

	for (i = 0; i < lRootNode->GetChildCount(); i++)
	{
		DisplayHierarchy(lRootNode->GetChild(i), 0);
	}
}

void DisplayHierarchy(FbxNode* pNode, int pDepth)
{
	FbxString lString;
	int i;

	for (i = 0; i < pDepth; i++)
	{
		lString += "     ";
	}

	lString += pNode->GetName();
	lString += "\n";

	FBXSDK_printf(lString.Buffer());

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
	}
}

void DisplayContent(FbxScene* pScene) {
	FbxNode* lNode = pScene->GetRootNode();
	if (lNode) {
		for (int i = 0; i < lNode->GetChildCount(); i++) {
			DisplayContent(lNode->GetChild(i));
		}
	}
}
//For Node
void DisplayContent(FbxNode* pNode) {
	FbxNodeAttribute::EType lAttributeType;
	int i;
	if (pNode->GetNodeAttribute() == NULL)
	{
		FBXSDK_printf("Null Node Attribute\n\n");
	}
	else {
		lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());
		switch (lAttributeType) {
		default:
			break;
		case FbxNodeAttribute::eSkeleton:
			DisplaySkeleton(pNode);
			break;
		case FbxNodeAttribute::eMesh:
			DisplayMesh(pNode);
			break;
		}
	}
	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}

void DisplayGlobalLightSettings(FbxGlobalSettings* pGlobalSettings) {
	DisplayColor("Ambient Color:", pGlobalSettings->GetAmbientColor());
}
//
////Used in DisplayMesh
void DisplayMesh(FbxNode* pNode) {
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

	DisplayString("Mesh Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lMesh);
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);
	DisplayMaterialMapping(lMesh);
	DisplayLink(lMesh);

}
//For Skeleton
void DisplaySkeleton(FbxNode* pNode) {
	FbxSkeleton* lSkeleton = (FbxSkeleton*)pNode->GetNodeAttribute();
	DisplayString("Skeleton Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lSkeleton);

	const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

	DisplayString("    Type: ", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

	if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
	{
		DisplayDouble("    Limb Length: ", lSkeleton->LimbLength.Get());
	}
	else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
	{
		DisplayDouble("    Limb Node Size: ", lSkeleton->Size.Get());
	}
	else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
	{
		DisplayDouble("    Limb Root Size: ", lSkeleton->Size.Get());
	}

	DisplayColor("    Color: ", lSkeleton->GetLimbNodeColor());
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

void DisplayLink(FbxGeometry* pGeometry) {
	int i, j;
	int lSkinCount = 0;
	int lClusterCount = 0;
	FbxCluster* lCluster;

	lSkinCount = pGeometry->GetDeformerCount(FbxDeformer::eSkin);

	for (i = 0; i != lSkinCount; ++i) {
		for (j = 0; j != lClusterCount; ++j)
		{
			DisplayInt("    Cluster ", i);

			lCluster = ((FbxSkin*)pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);  

			const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

			DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

			if (lCluster->GetLink() != NULL)
			{
				DisplayString("        Name: ", (char*)lCluster->GetLink()->GetName());
			}

			FbxString lString1 = "        Link Indices: ";
			FbxString lString2 = "        Weight Values: ";

			int k, lIndexCount = lCluster->GetControlPointIndicesCount();
			int* lIndices = lCluster->GetControlPointIndices();
			double* lWeights = lCluster->GetControlPointWeights();

			for (k = 0; k < lIndexCount; k++)
			{
				lString1 += lIndices[k];
				lString2 += (float)lWeights[k];

				if (k < lIndexCount - 1)
				{
					lString1 += ", ";
					lString2 += ", ";
				}
			}

			lString1 += "\n";
			lString2 += "\n";

			FBXSDK_printf(lString1);
			FBXSDK_printf(lString2);

			DisplayString("");

			FbxAMatrix lMatrix;

			lMatrix = lCluster->GetTransformMatrix(lMatrix);
			Display3DVector("        Transform Translation: ", lMatrix.GetT());
			Display3DVector("        Transform Rotation: ", lMatrix.GetR());
			Display3DVector("        Transform Scaling: ", lMatrix.GetS());

			lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
			Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
			Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
			Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

			if (lCluster->GetAssociateModel() != NULL)
			{
				lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
				DisplayString("        Associate Model: ", (char*)lCluster->GetAssociateModel()->GetName());
				Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
				Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
				Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
			}

			DisplayString("");
		}
	}

}
//For show on poly
void DisplayPolygons(FbxMesh* pMesh) {
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	DisplayString("	Polygons");

	int vertexId = 0;
	for (i = 0; i < lPolygonCount; i++) {
		DisplayInt("		Polygon ", i);
		int l;

		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); i++) {
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex) {
					FBXSDK_sprintf(header, 100, "		Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					DisplayInt(header, polyGroupId);
					break;
				}
			default:
				DisplayString("        \"unsupported group assignment\"");
				break;
			}
		}

		int lPolygonCount = pMesh->GetPolygonSize(i);

		for (j = 0; j < lPolygonCount; j++) {
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);

			Display3DVector("	    Coordinates: ", lControlPoints[lControlPointIndex]);

			for (l = 0; i < pMesh->GetElementVertexColorCount(); i++) {
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "			Color vertex:");

				switch (leVtxc->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode()) {
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
						break;
					default:// other reference modes not shown here!
						break;
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
						break;
					default:// other reference modes not shown here!
						break;
					}
				}
				break;
				case FbxGeometryElement::eByPolygon:// doesn't make much sense for UVs
					break;
				case FbxGeometryElement::eAllSame:// doesn't make much sense for UVs
					break;
				case FbxGeometryElement::eNone: // doesn't make much sense for UVs
					break;
				default:
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");

				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leNormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}
			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}

			}			
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);

				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}
			vertexId++;

		}// for polygonSize
	} // for polygonCount

	//check visibility for the edges of the mesh
	for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
	{
		FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
		DisplayString(header);
		switch (leVisibility->GetMappingMode())
		{
		default:
			break;
			//should be eByEdge
		case FbxGeometryElement::eByEdge:
			//should be eDirect
			for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
			{
				DisplayInt("        Edge ", j);
				DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
			}

			break;
		}
	}
	DisplayString("");
}
//Used in Pose(so How to use that)
void DisplayPose(FbxScene* pScene) {
	int i, j, k, lPoseCount;
	FbxString lName;

	lPoseCount = pScene->GetPoseCount();

	for (i = 0; i < lPoseCount; i++) {
		FbxPose* lPose = pScene->GetPose(i);

		lName = lPose->GetName();
		DisplayString("Pose Name:", lName.Buffer());
	}
}

//Used in Animation
void DisplayAnimation(FbxScene* pScene) {
	int i;
	for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++) {
		FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

		FbxString lOutputString = "Animation Stack Name:";
		lOutputString += lAnimStack->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAnimStack, pScene->GetRootNode());
	}
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
	int l;
	int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	int nbAudioLayers = pAnimStack->GetMemberCount<FbxAudioLayer>();
	FbxString lOutputString;

	lOutputString = "   contains ";
	if (nbAnimLayers == 0 && nbAudioLayers == 0)
		lOutputString += "no layers";

	if (nbAnimLayers)
	{
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer";
		if (nbAnimLayers > 1)
			lOutputString += "s";
	}

	if (nbAudioLayers)
	{
		if (nbAnimLayers)
			lOutputString += " and ";

		lOutputString += nbAudioLayers;
		lOutputString += " Audio Layer";
		if (nbAudioLayers > 1)
			lOutputString += "s";
	}
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	for (l = 0; l < nbAnimLayers; l++)
	{
		FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l);

		lOutputString = "AnimLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAnimLayer, pNode, isSwitcher);
	}

	for (l = 0; l < nbAudioLayers; l++)
	{
		FbxAudioLayer* lAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(l);

		lOutputString = "AudioLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAudioLayer, isSwitcher);
		FBXSDK_printf("\n");
	}
}
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool) {
	int lClipCount;
	FbxString lOutputString;

	lClipCount = pAudioLayer->GetMemberCount<FbxAudio>();

	lOutputString = "     Name: ";
	lOutputString += pAudioLayer->GetName();
	lOutputString += "\n\n";
	lOutputString += "     Nb Audio Clips: ";
	lOutputString += lClipCount;
	lOutputString += "\n";
	FBXSDK_printf(lOutputString);

	for (int i = 0; i < lClipCount; i++)
	{
		FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
		lOutputString = "        Clip[";
		lOutputString += i;
		lOutputString += "]:\t";
		lOutputString += lClip->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
	int lModelCount;
	FbxString lOutputString;

	lOutputString = "     Node Name: ";
	lOutputString += pNode->GetName();
	lOutputString += "\n\n";
	FBXSDK_printf(lOutputString);

	DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
	FBXSDK_printf("\n");

	for (lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
	{
		DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
	}
}

void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)
{
	FbxAnimCurve* lAnimCurve = NULL;

	// Display general curves.
	if (!isSwitcher)
	{
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        TZ\n");
			DisplayCurve(lAnimCurve);
		}

		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        RZ\n");
			DisplayCurve(lAnimCurve);
		}

		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SX\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SY\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			FBXSDK_printf("        SZ\n");
			DisplayCurve(lAnimCurve);
		}
	}

	// Display curves specific to a light or marker.
	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

	if (lNodeAttribute)
	{
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Red\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Green\n");
			DisplayCurve(lAnimCurve);
		}
		lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
		if (lAnimCurve)
		{
			FBXSDK_printf("        Blue\n");
			DisplayCurve(lAnimCurve);
		}

		// Display curves specific to a light.
		FbxLight* light = pNode->GetLight();
		if (light)
		{
			lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Intensity\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Outer Angle\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = light->Fog.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Fog\n");
				DisplayCurve(lAnimCurve);
			}
		}

		// Display curves specific to a camera.
		FbxCamera* camera = pNode->GetCamera();
		if (camera)
		{
			lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View X\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Field of View Y\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Optical Center X\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Optical Center Y\n");
				DisplayCurve(lAnimCurve);
			}

			lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
			if (lAnimCurve)
			{
				FBXSDK_printf("        Roll\n");
				DisplayCurve(lAnimCurve);
			}
		}

		// Display curves specific to a geometry.
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			FbxGeometry* lGeometry = (FbxGeometry*)lNodeAttribute;

			int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
			for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeDeformerCount; ++lBlendShapeIndex)
			{
				FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);

				int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
				for (int lChannelIndex = 0; lChannelIndex < lBlendShapeChannelCount; ++lChannelIndex)
				{
					FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
					const char* lChannelName = lChannel->GetName();

					lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
					if (lAnimCurve)
					{
						FBXSDK_printf("        Shape %s\n", lChannelName);
						DisplayCurve(lAnimCurve);
					}
				}
			}
		}
	}

	// Display curves specific to properties
	FbxProperty lProperty = pNode->GetFirstProperty();
	while (lProperty.IsValid())
	{
		if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
		{
			FbxString lFbxFCurveNodeName = lProperty.GetName();
			FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

			if (!lCurveNode) {
				lProperty = pNode->GetNextProperty(lProperty);
				continue;
			}

			FbxDataType lDataType = lProperty.GetPropertyDataType();
			if (lDataType.GetType() == eFbxBool || lDataType.GetType() == eFbxDouble || lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
			{
				FbxString lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				};

				DisplayString(lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
						DisplayCurve(lAnimCurve);
				}
			}
			else if (lDataType.GetType() == eFbxDouble3 || lDataType.GetType() == eFbxDouble4 || lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
			{
				char* lComponentName1 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
				char* lComponentName2 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
				char* lComponentName3 = (lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE : (char*)"Z";
				FbxString      lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				}
				DisplayString(lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
					{
						DisplayString("        Component ", lComponentName1);
						DisplayCurve(lAnimCurve);
					}
				}

				for (int c = 0; c < lCurveNode->GetCurveCount(1U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(1U, c);
					if (lAnimCurve)
					{
						DisplayString("        Component ", lComponentName2);
						DisplayCurve(lAnimCurve);
					}
				}

				for (int c = 0; c < lCurveNode->GetCurveCount(2U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(2U, c);
					if (lAnimCurve)
					{
						DisplayString("        Component ", lComponentName3);
						DisplayCurve(lAnimCurve);
					}
				}
			}
			else if (lDataType.GetType() == eFbxEnum)
			{
				FbxString lMessage;

				lMessage = "        Property ";
				lMessage += lProperty.GetName();
				if (lProperty.GetLabel().GetLen() > 0)
				{
					lMessage += " (Label: ";
					lMessage += lProperty.GetLabel();
					lMessage += ")";
				};
				DisplayString(lMessage.Buffer());

				for (int c = 0; c < lCurveNode->GetCurveCount(0U); c++)
				{
					lAnimCurve = lCurveNode->GetCurve(0U, c);
					if (lAnimCurve)
						DisplayListCurve(lAnimCurve, &lProperty);
				}
			}
		}

		lProperty = pNode->GetNextProperty(lProperty);
	} // while

}


//for animation list

static int InterpolationFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant) return 1;
	if ((flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear) return 2;
	if ((flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic) return 3;
	return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard) return 1;
	if ((flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext) return 2;
	return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto) return 1;
	if ((flags & FbxAnimCurveDef::eTangentAutoBreak) == FbxAnimCurveDef::eTangentAutoBreak) return 2;
	if ((flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB) return 3;
	if ((flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser) return 4;
	if ((flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak) return 5;
	if ((flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak) return 6;
	return 0;
}

static int TangentweightFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone) return 1;
	if ((flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight) return 2;
	if ((flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft) return 3;
	return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
	if ((flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone) return 1;
	if ((flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight) return 2;
	if ((flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft) return 3;
	return 0;
}


void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
	static const char* interpolation[] = { "?", "constant", "linear", "cubic" };
	static const char* constantMode[] = { "?", "Standard", "Next" };
	static const char* cubicMode[] = { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
	static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

	FbxTime   lKeyTime;
	float   lKeyValue;
	char    lTimeString[256];
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " [ ";
		lOutputString += interpolation[InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount))];
		if ((pCurve->KeyGetInterpolation(lCount) & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			lOutputString += " | ";
			lOutputString += constantMode[ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount))];
		}
		else if ((pCurve->KeyGetInterpolation(lCount) & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			lOutputString += " | ";
			lOutputString += cubicMode[TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount))];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode())];
			lOutputString += " | ";
			lOutputString += tangentWVMode[TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode())];
		}
		lOutputString += " ]";
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
	FbxTime   lKeyTime;
	int     lKeyValue;
	char    lTimeString[256];
	FbxString lListValue;
	FbxString lOutputString;
	int     lCount;

	int lKeyCount = pCurve->KeyGetCount();

	for (lCount = 0; lCount < lKeyCount; lCount++)
	{
		lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
		lKeyTime = pCurve->KeyGetTime(lCount);

		lOutputString = "            Key Time: ";
		lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
		lOutputString += ".... Key Value: ";
		lOutputString += lKeyValue;
		lOutputString += " (";
		lOutputString += pProperty->GetEnumValue(lKeyValue);
		lOutputString += ")";

		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
	}
}


//Used in Generic
void DisplayGenericInfo(FbxScene* pScene) {
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();

	for (i = 0; i < lRootNode->GetChildCount(); i++) {
		DisplayGenericInfo(lRootNode->GetChild(i), 0);
	}

	//Other objects directly connected onto the scene
	for (i = 0; i < pScene->GetSrcObjectCount(); i++) {
		DisplayProperties(pScene->GetSrcObject(i));
	}
}


void DisplayGenericInfo(FbxNode* pNode, int pDepth) {
	FbxString lString;
	int i;

	for (i = 0; i < pDepth; i++) {
		lString += "		";
	}
	lString += pNode->GetName();
	lString += "\n";

	DisplayString(lString.Buffer());

	//Display generic info about that Node
	DisplayProperties(pNode);
	DisplayString("");
	for (i = 0; i < pNode->GetChildCount(); i++) {
		DisplayGenericInfo(pNode->GetChild(i), pDepth + 1);
	}
}


void DisplayProperties(FbxObject* pObject) {
	DisplayString("Name:", (char*)pObject->GetName());

	//Display all the properties
	int i, lCount = 0;
	FbxProperty lProperty = pObject->GetFirstProperty();
	while (lProperty.IsValid()) {
		lCount++;
		lProperty = pObject->GetNextProperty(lProperty);
	}

	FbxString lTitleStr = "     Property Count:";

	if (lCount == 0) {//there are no properties to display
		return;
	}

	DisplayInt(lTitleStr.Buffer(), lCount);

	i = 0;
	lProperty = pObject->GetFirstProperty();
	while (lProperty.IsValid()) {
		//exclude user properties

		FbxString lString;
		DisplayInt("	   Property ", i);
		lString = lProperty.GetLabel();
		DisplayString("		Display Name:", lString.Buffer());
		lString = lProperty.GetName();
		DisplayString("		Internal Name:", lString.Buffer());
		lString = lProperty.GetPropertyDataType().GetName();
		DisplayString("		Type:", lString.Buffer());
		if (lProperty.HasMinLimit())DisplayDouble("		Min Limit:", lProperty.GetMinLimit());
		if (lProperty.HasMaxLimit())DisplayDouble("		Max Limit:", lProperty.GetMaxLimit());
		DisplayBool("		Is Animatable:", lProperty.GetFlag(FbxPropertyFlags::eAnimatable));

		switch (lProperty.GetPropertyDataType().GetType()) {
		case eFbxBool:
			DisplayBool("		Default Value:", lProperty.Get<FbxBool>());
			break;
		case eFbxDouble:
			DisplayDouble("		Default Value:", lProperty.Get<FbxDouble>());
			break;
		case eFbxDouble4:
		{
			FbxColor lDefault;
			char lBuf[64];

			lDefault = lProperty.Get<FbxColor>();
			FBXSDK_sprintf(lBuf, 64, "R=%f,G=%f,B=%f,A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
			DisplayString("		Default Value:", lBuf);

		}
		break;
		case eFbxDouble3:
		{
			FbxDouble3 lDefault;
			char lBuf[64];

			lDefault = lProperty.Get<FbxDouble3>();
			FBXSDK_sprintf(lBuf, 64, "X=%f,Y=%f,Z=%f", lDefault[0], lDefault[1], lDefault[2]);
			DisplayString("		Default Value:", lBuf);
		}
		break;
		case eFbxInt:
			DisplayInt("		Default Value:", lProperty.Get<FbxInt>());
			break;
		case eFbxFloat:
			DisplayDouble("		Default Value:", lProperty.Get<FbxFloat>());
			break;
		case eFbxString:
			DisplayString("		Default Value:", lProperty.Get<FbxString>());
			break;
		default:
			DisplayString("		Default Value:Unidentified");
			break;
		}

		i++;
		lProperty = pObject->GetNextProperty(lProperty);
	}




}
//Use to show string
void DisplayString(const char* pHeader, const char* pValue /* = "" */, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
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



//Common Used
void DisplayMetaDataConnections(FbxObject* pObject) {
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
		DisplayString("    MetaData connections ");

	for (int i = 0; i < nbMetaData; i++)
	{
		FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		DisplayString("        Name: ", (char*)metaData->GetName());
	}
}
//Show X,Y,Z
void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix/* = "" */) {
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ",";
	lString += lFloatValue2;
	lString += ",";
	lString += lFloatValue3;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void DisplayInt(const char* pHeader, int pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix /* = "" */) {
	FbxString lString;
	FbxString lFloatValue = (float)pValue;

	lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
	lFloatValue = pValue <= HUGE_VAL ? "INFINITY" : lFloatValue.Buffer();

	lString = pHeader;
	lString += lFloatValue;
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}


void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix) {
	FbxString lString;

	lString = pHeader;
	lString += pValue ? "True" : "False";
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}

void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix) {
	FbxString lString;

	lString = pHeader;
	lString += (float)pValue.mRed;

	lString += " (red), ";
	lString += (float)pValue.mGreen;

	lString += " (green), ";
	lString += (float)pValue.mBlue;

	lString += " (blue)";
	lString += pSuffix;
	lString += "\n";
	FBXSDK_printf(lString);
}