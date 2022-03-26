#include<fbxsdk.h>
#include<fbxsdk/fileio/fbxiosettings.h>
#include<iostream>

//Function include
#include "Common.h"
#include "Global.h"
#include "Camera.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Pose.h"
#include "Animation.h"
#include "Light.h"
#include "Skeleton.h"
#include "GenericInfo.h"

#include "Link.h"
#include "Shape.h"
#include "Cache.h"


//Function Definition
void DisplayHierarchy(FbxScene* pScene);
void DisplayHierarchy(FbxNode* pNode, int pDepth);
void DisplayMetaData(FbxScene* pScene);
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayMarker(FbxNode* pNode);
void DisplayNurb(FbxNode* pNode);
void DisplayPatch(FbxNode* pNode);
void DisplayLodGroup(FbxNode* pNode);

void DisplayUserProperties(FbxObject* pObject);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayPivotsAndLimits(FbxNode* pNode);

static bool gVerbose = true;

//Create Fbx Sdk Manager
//FbxManager* lSdkManager = FbxManager::Create();
//FbxScene* lScene = FbxScene::Create(lSdkManager, "lScne");

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
	const char* importFileName = "Model//CubeAnimation_Light_Camera.FBX";

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

	FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

	if (gVerbose) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

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
		case FbxNodeAttribute::eMarker:
			DisplayMarker(pNode);
			break;

		case FbxNodeAttribute::eSkeleton:
			DisplaySkeleton(pNode);
			break;

		case FbxNodeAttribute::eMesh:
			DisplayMesh(pNode);
			break;

		case FbxNodeAttribute::eNurbs:
			DisplayNurb(pNode);
			break;

		case FbxNodeAttribute::ePatch:
			DisplayPatch(pNode);
			break;

		case FbxNodeAttribute::eCamera:
			DisplayCamera(pNode);
			break;

		case FbxNodeAttribute::eLight:
			DisplayLight(pNode);
			break;

		case FbxNodeAttribute::eLODGroup:
			DisplayLodGroup(pNode);
			break;
		}
	}

	DisplayUserProperties(pNode);
	DisplayTarget(pNode);
	DisplayPivotsAndLimits(pNode);
	DisplayTransformPropagation(pNode);
	DisplayGeometricTransform(pNode);

	for (i = 0; i < pNode->GetChildCount(); i++)
	{
		DisplayContent(pNode->GetChild(i));
	}
}
void DisplayMarker(FbxNode* pNode)
{
	FbxMarker* lMarker = (FbxMarker*)pNode->GetNodeAttribute();
	FbxString     lString;

	DisplayString("Marker Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lMarker);

	// Type
	lString = "    Marker Type: ";
	switch (lMarker->GetType())
	{
	case FbxMarker::eStandard:    lString += "Standard";    break;
	case FbxMarker::eOptical:     lString += "Optical";     break;
	case FbxMarker::eEffectorIK: lString += "IK Effector"; break;
	case FbxMarker::eEffectorFK: lString += "FK Effector"; break;
	}
	DisplayString(lString.Buffer());

	// Look
	lString = "    Marker Look: ";
	switch (lMarker->Look.Get())
	{
	default:
		break;
	case FbxMarker::eCube:         lString += "Cube";        break;
	case FbxMarker::eHardCross:   lString += "Hard Cross";  break;
	case FbxMarker::eLightCross:  lString += "Light Cross"; break;
	case FbxMarker::eSphere:       lString += "Sphere";      break;
	}
	DisplayString(lString.Buffer());

	// Size
	lString = FbxString("    Size: ") + FbxString(lMarker->Size.Get());
	DisplayString(lString.Buffer());

	// Color
	FbxDouble3 c = lMarker->Color.Get();
	FbxColor color(c[0], c[1], c[2]);
	DisplayColor("    Color: ", color);

	// IKPivot
	Display3DVector("    IKPivot: ", lMarker->IKPivot.Get());
}

void DisplayNurb(FbxNode* pNode){
	FbxNurbs* lNurbs = (FbxNurbs*)pNode->GetNodeAttribute();
	int i;

	DisplayString("Nurb Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lNurbs);

	const char* lSurfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayString("    Surface Mode: ", lSurfaceModes[lNurbs->GetSurfaceMode()]);

	int lControlPointsCount = lNurbs->GetControlPointsCount();
	FbxVector4* lControlPoints = lNurbs->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("    Control Point ", i);
		Display3DVector("        Coordinates: ", lControlPoints[i]);
		DisplayDouble("        Weight: ", lControlPoints[i][3]);
	}

	const char* lNurbTypes[] = { "Periodic", "Closed", "Open" };

	DisplayString("    Nurb U Type: ", lNurbTypes[lNurbs->GetNurbsUType()]);
	DisplayInt("    U Count: ", lNurbs->GetUCount());
	DisplayString("    Nurb V Type: ", lNurbTypes[lNurbs->GetNurbsVType()]);
	DisplayInt("    V Count: ", lNurbs->GetVCount());
	DisplayInt("    U Order: ", lNurbs->GetUOrder());
	DisplayInt("    V Order: ", lNurbs->GetVOrder());
	DisplayInt("    U Step: ", lNurbs->GetUStep());
	DisplayInt("    V Step: ", lNurbs->GetVStep());

	FbxString lString;
	int lUKnotCount = lNurbs->GetUKnotCount();
	int lVKnotCount = lNurbs->GetVKnotCount();
	int lUMultiplicityCount = lNurbs->GetUCount();
	int lVMultiplicityCount = lNurbs->GetVCount();
	double* lUKnotVector = lNurbs->GetUKnotVector();
	double* lVKnotVector = lNurbs->GetVKnotVector();
	int* lUMultiplicityVector = lNurbs->GetUMultiplicityVector();
	int* lVMultiplicityVector = lNurbs->GetVMultiplicityVector();

	lString = "    U Knot Vector: ";

	for (i = 0; i < lUKnotCount; i++)
	{
		lString += (float)lUKnotVector[i];

		if (i < lUKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    V Knot Vector: ";

	for (i = 0; i < lVKnotCount; i++)
	{
		lString += (float)lVKnotVector[i];

		if (i < lVKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    U Multiplicity Vector: ";

	for (i = 0; i < lUMultiplicityCount; i++)
	{
		lString += lUMultiplicityVector[i];

		if (i < lUMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    V Multiplicity Vector: ";

	for (i = 0; i < lVMultiplicityCount; i++)
	{
		lString += lVMultiplicityVector[i];

		if (i < lVMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	DisplayString("");

	DisplayTexture(lNurbs);
	DisplayMaterial(lNurbs);
	DisplayLink(lNurbs);
	DisplayShape(lNurbs);
	DisplayCache(lNurbs);
}

void DisplayPatch(FbxNode* pNode) {
	FbxNurbs* lNurbs = (FbxNurbs*)pNode->GetNodeAttribute();
	int i;

	DisplayString("Nurb Name: ", (char*)pNode->GetName());
	DisplayMetaDataConnections(lNurbs);

	const char* lSurfaceModes[] = { "Raw", "Low No Normals", "Low", "High No Normals", "High" };

	DisplayString("    Surface Mode: ", lSurfaceModes[lNurbs->GetSurfaceMode()]);

	int lControlPointsCount = lNurbs->GetControlPointsCount();
	FbxVector4* lControlPoints = lNurbs->GetControlPoints();

	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("    Control Point ", i);
		Display3DVector("        Coordinates: ", lControlPoints[i]);
		DisplayDouble("        Weight: ", lControlPoints[i][3]);
	}

	const char* lNurbTypes[] = { "Periodic", "Closed", "Open" };

	DisplayString("    Nurb U Type: ", lNurbTypes[lNurbs->GetNurbsUType()]);
	DisplayInt("    U Count: ", lNurbs->GetUCount());
	DisplayString("    Nurb V Type: ", lNurbTypes[lNurbs->GetNurbsVType()]);
	DisplayInt("    V Count: ", lNurbs->GetVCount());
	DisplayInt("    U Order: ", lNurbs->GetUOrder());
	DisplayInt("    V Order: ", lNurbs->GetVOrder());
	DisplayInt("    U Step: ", lNurbs->GetUStep());
	DisplayInt("    V Step: ", lNurbs->GetVStep());

	FbxString lString;
	int lUKnotCount = lNurbs->GetUKnotCount();
	int lVKnotCount = lNurbs->GetVKnotCount();
	int lUMultiplicityCount = lNurbs->GetUCount();
	int lVMultiplicityCount = lNurbs->GetVCount();
	double* lUKnotVector = lNurbs->GetUKnotVector();
	double* lVKnotVector = lNurbs->GetVKnotVector();
	int* lUMultiplicityVector = lNurbs->GetUMultiplicityVector();
	int* lVMultiplicityVector = lNurbs->GetVMultiplicityVector();

	lString = "    U Knot Vector: ";

	for (i = 0; i < lUKnotCount; i++)
	{
		lString += (float)lUKnotVector[i];

		if (i < lUKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    V Knot Vector: ";

	for (i = 0; i < lVKnotCount; i++)
	{
		lString += (float)lVKnotVector[i];

		if (i < lVKnotCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    U Multiplicity Vector: ";

	for (i = 0; i < lUMultiplicityCount; i++)
	{
		lString += lUMultiplicityVector[i];

		if (i < lUMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	lString = "    V Multiplicity Vector: ";

	for (i = 0; i < lVMultiplicityCount; i++)
	{
		lString += lVMultiplicityVector[i];

		if (i < lVMultiplicityCount - 1)
		{
			lString += ", ";
		}
	}

	lString += "\n";
	FBXSDK_printf(lString);

	DisplayString("");

	DisplayTexture(lNurbs);
	DisplayMaterial(lNurbs);
	DisplayLink(lNurbs);
	DisplayShape(lNurbs);
	DisplayCache(lNurbs);
}

void DisplayLodGroup(FbxNode* pNode){
	const char* lDisplayLevels[] = { "UseLOD", "Show", "Hide" };

	DisplayString("LodGroup Name: ", (char*)pNode->GetName());

	DisplayInt("    ", pNode->GetChildCount(), " Geometries");
	for (int i = 0; i < pNode->GetChildCount(); i++)
	{
		FbxNode* lChildNode = pNode->GetChild(i);
		DisplayString("        ", lChildNode->GetName());
	}

	FbxLODGroup* lLodGroupAttr = (FbxLODGroup*)pNode->GetNodeAttribute();
	DisplayBool("    MinMaxDistance Enabled: ", lLodGroupAttr->MinMaxDistance.Get());
	if (lLodGroupAttr->MinMaxDistance.Get())
	{
		DisplayDouble("        Min Distance: ", lLodGroupAttr->MinDistance.Get());
		DisplayDouble("        Max Distance: ", lLodGroupAttr->MaxDistance.Get());
	}
	DisplayBool("    Is World Space: ", lLodGroupAttr->WorldSpace.Get());

	DisplayString("    Thresholds ");
	for (int i = 0; i < lLodGroupAttr->GetNumThresholds(); i++)
	{
		FbxDistance lThreshVal;
		if (lLodGroupAttr->GetThreshold(i, lThreshVal))
			DisplayDouble("        ", lThreshVal.value());
	}

	DisplayString("    DisplayLevels");
	for (int i = 0; i < lLodGroupAttr->GetNumDisplayLevels(); i++)
	{
		FbxLODGroup::EDisplayLevel lLevel;
		if (lLodGroupAttr->GetDisplayLevel(i, lLevel))
			DisplayString("        ", lDisplayLevels[lLevel]);
	}
}

void DisplayUserProperties(FbxObject* pObject)
{
	int lCount = 0;
	FbxString lTitleStr = "    Property Count: ";

	FbxProperty lProperty = pObject->GetFirstProperty();
	while (lProperty.IsValid())
	{
		if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
			lCount++;

		lProperty = pObject->GetNextProperty(lProperty);
	}

	if (lCount == 0)
		return; // there are no user properties to display

	DisplayInt(lTitleStr.Buffer(), lCount);

	lProperty = pObject->GetFirstProperty();
	int i = 0;
	while (lProperty.IsValid())
	{
		if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
		{
			DisplayInt("        Property ", i);
			FbxString lString = lProperty.GetLabel();
			DisplayString("            Display Name: ", lString.Buffer());
			lString = lProperty.GetName();
			DisplayString("            Internal Name: ", lString.Buffer());
			DisplayString("            Type: ", lProperty.GetPropertyDataType().GetName());
			if (lProperty.HasMinLimit()) DisplayDouble("            Min Limit: ", lProperty.GetMinLimit());
			if (lProperty.HasMaxLimit()) DisplayDouble("            Max Limit: ", lProperty.GetMaxLimit());
			DisplayBool("            Is Animatable: ", lProperty.GetFlag(FbxPropertyFlags::eAnimatable));

			FbxDataType lPropertyDataType = lProperty.GetPropertyDataType();

			// BOOL
			if (lPropertyDataType.GetType() == eFbxBool)
			{
				DisplayBool("            Default Value: ", lProperty.Get<FbxBool>());
			}
			// REAL
			else if (lPropertyDataType.GetType() == eFbxDouble || lPropertyDataType.GetType() == eFbxFloat)
			{
				DisplayDouble("            Default Value: ", lProperty.Get<FbxDouble>());
			}
			// COLOR
			else if (lPropertyDataType.Is(FbxColor3DT) || lPropertyDataType.Is(FbxColor4DT))
			{
				FbxColor lDefault;
				char      lBuf[64];

				lDefault = lProperty.Get<FbxColor>();
				FBXSDK_sprintf(lBuf, 64, "R=%f, G=%f, B=%f, A=%f", lDefault.mRed, lDefault.mGreen, lDefault.mBlue, lDefault.mAlpha);
				DisplayString("            Default Value: ", lBuf);
			}
			// INTEGER
			else if (lPropertyDataType.GetType() == eFbxInt)
			{
				DisplayInt("            Default Value: ", lProperty.Get<FbxInt>());
			}
			// VECTOR
			else if (lPropertyDataType.GetType() == eFbxDouble3 || lPropertyDataType.GetType() == eFbxDouble4)
			{
				FbxDouble3 lDefault;
				char   lBuf[64];

				lDefault = lProperty.Get<FbxDouble3>();
				FBXSDK_sprintf(lBuf, 64, "X=%f, Y=%f, Z=%f", lDefault[0], lDefault[1], lDefault[2]);
				DisplayString("            Default Value: ", lBuf);
			}
			// LIST
			else if (lPropertyDataType.GetType() == eFbxEnum)
			{
				DisplayInt("            Default Value: ", lProperty.Get<FbxEnum>());
			}
			// UNIDENTIFIED
			else
			{
				DisplayString("            Default Value: UNIDENTIFIED");
			}
			i++;
		}

		lProperty = pObject->GetNextProperty(lProperty);
	}
}

void DisplayTarget(FbxNode* pNode)
{
	if (pNode->GetTarget() != NULL)
	{
		DisplayString("    Target Name: ", (char*)pNode->GetTarget()->GetName());
	}
}

void DisplayTransformPropagation(FbxNode* pNode) {
	FBXSDK_printf("    Transformation Propagation\n");

	// 
	// Rotation Space
	//
	EFbxRotationOrder lRotationOrder;
	pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

	FBXSDK_printf("        Rotation Space: ");

	switch (lRotationOrder)
	{
	case eEulerXYZ:
		FBXSDK_printf("Euler XYZ\n");
		break;
	case eEulerXZY:
		FBXSDK_printf("Euler XZY\n");
		break;
	case eEulerYZX:
		FBXSDK_printf("Euler YZX\n");
		break;
	case eEulerYXZ:
		FBXSDK_printf("Euler YXZ\n");
		break;
	case eEulerZXY:
		FBXSDK_printf("Euler ZXY\n");
		break;
	case eEulerZYX:
		FBXSDK_printf("Euler ZYX\n");
		break;
	case eSphericXYZ:
		FBXSDK_printf("Spheric XYZ\n");
		break;
	}

	//
	// Use the Rotation space only for the limits
	// (keep using eEulerXYZ for the rest)
	//
	FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
		pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


	//
	// Inherit Type
	//
	FbxTransform::EInheritType lInheritType;
	pNode->GetTransformationInheritType(lInheritType);

	FBXSDK_printf("        Transformation Inheritance: ");

	switch (lInheritType)
	{
	case FbxTransform::eInheritRrSs:
		FBXSDK_printf("RrSs\n");
		break;
	case FbxTransform::eInheritRSrs:
		FBXSDK_printf("RSrs\n");
		break;
	case FbxTransform::eInheritRrs:
		FBXSDK_printf("Rrs\n");
		break;
	}
}

void DisplayGeometricTransform(FbxNode* pNode)
{
	FbxVector4 lTmpVector;

	FBXSDK_printf("    Geometric Transformations\n");

	//
	// Translation
	//
	lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Rotation
	//
	lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Scaling
	//
	lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
	FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}

void DisplayPivotsAndLimits(FbxNode* pNode) {
	FbxVector4 lTmpVector;

	//
	// Pivots
	//
	FBXSDK_printf("    Pivot Information\n");

	FbxNode::EPivotState lPivotState;
	pNode->GetPivotState(FbxNode::eSourcePivot, lPivotState);
	FBXSDK_printf("        Pivot State: %s\n", lPivotState == FbxNode::ePivotActive ? "Active" : "Reference");

	lTmpVector = pNode->GetPreRotation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Pre-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetPostRotation(FbxNode::eSourcePivot);
	FBXSDK_printf("        Post-Rotation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetRotationPivot(FbxNode::eSourcePivot);
	FBXSDK_printf("        Rotation Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetRotationOffset(FbxNode::eSourcePivot);
	FBXSDK_printf("        Rotation Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetScalingPivot(FbxNode::eSourcePivot);
	FBXSDK_printf("        Scaling Pivot: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	lTmpVector = pNode->GetScalingOffset(FbxNode::eSourcePivot);
	FBXSDK_printf("        Scaling Offset: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

	//
	// Limits
	//
	bool		lIsActive, lMinXActive, lMinYActive, lMinZActive;
	bool		lMaxXActive, lMaxYActive, lMaxZActive;
	FbxDouble3	lMinValues, lMaxValues;

	FBXSDK_printf("    Limits Information\n");

	lIsActive = pNode->TranslationActive;
	lMinXActive = pNode->TranslationMinX;
	lMinYActive = pNode->TranslationMinY;
	lMinZActive = pNode->TranslationMinZ;
	lMaxXActive = pNode->TranslationMaxX;
	lMaxYActive = pNode->TranslationMaxY;
	lMaxZActive = pNode->TranslationMaxZ;
	lMinValues = pNode->TranslationMin;
	lMaxValues = pNode->TranslationMax;

	FBXSDK_printf("        Translation limits: %s\n", lIsActive ? "Active" : "Inactive");
	FBXSDK_printf("            X\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[0]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[0]);
	FBXSDK_printf("            Y\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[1]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[1]);
	FBXSDK_printf("            Z\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[2]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[2]);

	lIsActive = pNode->RotationActive;
	lMinXActive = pNode->RotationMinX;
	lMinYActive = pNode->RotationMinY;
	lMinZActive = pNode->RotationMinZ;
	lMaxXActive = pNode->RotationMaxX;
	lMaxYActive = pNode->RotationMaxY;
	lMaxZActive = pNode->RotationMaxZ;
	lMinValues = pNode->RotationMin;
	lMaxValues = pNode->RotationMax;

	FBXSDK_printf("        Rotation limits: %s\n", lIsActive ? "Active" : "Inactive");
	FBXSDK_printf("            X\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[0]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[0]);
	FBXSDK_printf("            Y\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[1]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[1]);
	FBXSDK_printf("            Z\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[2]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[2]);

	lIsActive = pNode->ScalingActive;
	lMinXActive = pNode->ScalingMinX;
	lMinYActive = pNode->ScalingMinY;
	lMinZActive = pNode->ScalingMinZ;
	lMaxXActive = pNode->ScalingMaxX;
	lMaxYActive = pNode->ScalingMaxY;
	lMaxZActive = pNode->ScalingMaxZ;
	lMinValues = pNode->ScalingMin;
	lMaxValues = pNode->ScalingMax;

	FBXSDK_printf("        Scaling limits: %s\n", lIsActive ? "Active" : "Inactive");
	FBXSDK_printf("            X\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[0]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxXActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[0]);
	FBXSDK_printf("            Y\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[1]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxYActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[1]);
	FBXSDK_printf("            Z\n");
	FBXSDK_printf("                Min Limit: %s\n", lMinZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Min Limit Value: %f\n", lMinValues[2]);
	FBXSDK_printf("                Max Limit: %s\n", lMaxZActive ? "Active" : "Inactive");
	FBXSDK_printf("                Max Limit Value: %f\n", lMaxValues[2]);
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






