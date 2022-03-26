#include "Skeleton.h"

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