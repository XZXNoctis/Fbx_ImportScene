#include "Global.h"

void DisplayGlobalLightSettings(FbxGlobalSettings* pGlobalSettings);
void DisplayGlobalCameraSettings(FbxGlobalSettings* pGlobalSettings);
void DisplayGlobalTimeSettings(FbxGlobalSettings* pGlobalSettings);

void DisplayGlobalLightSettings(FbxGlobalSettings* pGlobalSettings) {
	DisplayColor("Ambient Color:", pGlobalSettings->GetAmbientColor());
}
void DisplayGlobalCameraSettings(FbxGlobalSettings* pGlobalSettings)
{
	DisplayString("Default Camera: ", pGlobalSettings->GetDefaultCamera());
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