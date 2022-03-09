#include<fbxsdk.h>
#include<fbxsdk/fileio/fbxiosettings.h>
#include<iostream>

//Create Fbx Sdk Manager
FbxManager* lSdkManager = FbxManager::Create();
FbxScene* lScene = FbxScene::Create(lSdkManager, "lScne");

//Function Definition
void DisplayHierarchy(FbxScene* pScene);
void DisplayHierarchy(FbxNode* pNode, int pDepth);
void DisplayMetaData(FbxScene* pScene);
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayMesh(FbxNode* pNode);
void DisplayGenericInfo(FbxScene* pScene);
void DisplayGenericInfo(FbxNode* pNode, int pDepth);
void DisplayProperties(FbxObject* pObject);
void DisplayMetaDataConnections(FbxObject* pNode);
void DisplayControlsPoints(FbxMesh* pMesh);
//For Common
void DisplayString(const char* pHeader, const char* pValue = "", const char* pSuffix = "");
void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");
void DisplayInt(const char* pHeader, int pValue, const char* pSuffix = "");
void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");
void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix = "");

static bool gVerbose = true;

int main() {

	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	//File Import
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "lImporter");

	//SetFileName
	const char* importFileName = "Cube.FBX";

	bool lImportStatus = lImporter->Initialize(importFileName, -1, lSdkManager->GetIOSettings());

	if (!lImportStatus) {
		printf("Call to FbxImporter::Intialize()failed.\n");
		printf("Error returned:%s\n\n", lImporter->GetStatus().GetErrorString());
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
		exit(-1);
	}
	else {
		printf("Import Success.\n");
	}


	//Display the scene
	DisplayMetaData(lScene);

	FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

	if (gVerbose) DisplayHierarchy(lScene);

	FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

	if (gVerbose) DisplayContent(lScene);

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
//
//
//
////Used in DisplayMesh
void DisplayMesh(FbxNode* pNode) {
	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

	DisplayString("Mesh Name: ", (char*) pNode->GetName());
	//DisplayMetaDataConnections(lMesh);
	DisplayControlsPoints(lMesh);


}
//Used in Generic
void DisplayGenericInfo(FbxScene* pScene) {
	int i;
	FbxNode* lRootNode = pScene->GetRootNode();

	for (i = 0; i < lRootNode->GetChildNameCount(); i++) {
		DisplayGenericInfo(lRootNode->GetChild(i), 0);
	}

	//Other objects directly connected onto the scene
	for (i = 0; i < pScene->GetSrcObjectCount();i++) {
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
		DisplayInt("	   Property ",i);
		lString = lProperty.GetLabel();
		DisplayString("		Display Name:", lString.Buffer());
		lString = lProperty.GetName();
		DisplayString("		Internal Name:", lString.Buffer());
		lString - lProperty.GetPropertyDataType().GetName();
		DisplayString("		Type:", lString.Buffer());
		if (lProperty.HasMinLimit())DisplayDouble("		Min Limit:", lProperty.GetMinLimit());
		if (lProperty.HasMaxLimit())DisplayDouble("		Max Limit:", lProperty.GetMaxLimit());
		DisplayBool("		Is Animatable:",lProperty.GetFlag(FbxPropertyFlags::eAnimatable));

		switch (lProperty.GetPropertyDataType().GetType()) {
		case eFbxBool:
			DisplayBool("		Default Value:",lProperty.Get<FbxBool>());
			break;
		case eFbxDouble:
			DisplayDouble("		Default Value:", lProperty.Get<FbxDouble>());
			break;
		case eFbxDouble4:
		{
			FbxColor lDefault;
			char lBuf[64];

			lDefault = lProperty.Get<FbxColor>();
			FBXSDK_sprintf(lBuf, 64, "R=%f,G=%f,B=%f,A=%f",lDefault.mRed,lDefault.mGreen,lDefault.mBlue,lDefault.mAlpha);
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

//void DisplayMetaDataConnections(FbxObject* pObject) {
//
//}

void DisplayControlsPoints(FbxMesh* pMesh) {
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	DisplayString("	Control Points");
	for (i = 0; i < lControlPointsCount; i++) {
		DisplayInt("		Control Point",i);
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