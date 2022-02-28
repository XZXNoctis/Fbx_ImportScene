#include<fbxsdk.h>
#include<fbxsdk/fileio/fbxiosettings.h>
#include<iostream>

//Create Fbx Sdk Manager
FbxManager* lSdkManager = FbxManager::Create();



int main() {

	FbxIOSettings* ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);

	//File Import
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"lImporter");

	//SetFileName
	const char* importFileName = "Noctis.FBX";

	bool lImportStatus = lImporter->Initialize(importFileName, -1, lSdkManager->GetIOSettings());

	if (!lImportStatus) {
		printf("Call to FbxImporter::Intialize()failed.\n");
		printf("Error returned:%s\n\n",lImporter->GetStatus().GetErrorString());
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
			importFileName,lFilemajor, lFileminor, lFileRevision);
		printf("Error returned:%s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}
	else {
		printf("Import Success.\n");
	}

}
