#include "Link.h"

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