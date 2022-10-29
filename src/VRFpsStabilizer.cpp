#include "VRFpsStabilizer.h"
#include "Utility.h"
#include "Menu.h"

#pragma warning(disable : 4505 4189 4100)

namespace VRFpsStabilizer
{
	vr::IVRCompositor* ivrCompositor = nullptr;
	
	//To find an address, you need to search the name in Strings window of IDA and find variables or functions(for example: For shadow) to set the parameter.

	//dword_141ED3918 = g_fLODFadeOutMultItems;
	//dword_141ED3910[0] = g_fLODFadeOutMultObjects;
	//dword_141ED3914 = g_fLODFadeOutMultObjects;
	//dword_141ED3920 = g_fLODFadeOutMultObjects;
	//dword_141ED3924 = g_fLODFadeOutMultObjects;
	//dword_141ED3930 = g_fLODFadeOutMultObjects;
	//dword_141ED3938 = g_fLODFadeOutMultSkyCell;
	//dword_141ED391C = g_fLODFadeOutMultActors;

	//RelocPtr<float> g_fLODFadeOutMultItems(0x1ED3918);
	//RelocPtr<float> g_fLODFadeOutMultActors(0x1ED391C);
	//RelocPtr<float> g_fLODFadeOutMultObjects_1(0x1ED3910);
	//RelocPtr<float> g_fLODFadeOutMultObjects_2(0x1ED3914);
	//RelocPtr<float> g_fLODFadeOutMultObjects_3(0x1ED3920);
	//RelocPtr<float> g_fLODFadeOutMultObjects_4(0x1ED3924);
	//RelocPtr<float> g_fLODFadeOutMultObjects_5(0x1ED3930);

	//RelocPtr<float> g_fShadowDistance_1(0x1ED6530);
	//RelocPtr<float> g_fShadowDistance_2(0x36ED280);
	//RelocPtr<float> g_fShadowDistanceExterior_3(0x36ED280);

	//RelocAddr<_SetShadowDistance_14134B520> SetShadowDistance_14134B520(0x134B520);

	//RelocPtr<int> g_iShadowMapResolution_1(0x1ED6308);
	//RelocPtr<int> g_iShadowMapResolution_2(0x36ED2A8);

	//RelocPtr<bool> g_isInterior(0x34230C4);

	//RelocPtr<int> g_iMinGrassSize(0x1E702E0);

	//RelocPtr<float> g_fGrassStartFadeDistance(0x1E70298);
	//RelocPtr<float> g_fGrassFadeRange(0x1E702B0);
	//RelocPtr<float> g_fGrassMaxStartFadeDistance(0x1E70280);

	//RelocPtr<unsigned int> g_uiMaxSkinnedTreesToRender(0x1EA7540);

	//RelocPtr<bool> g_bEnableTreeAnimations(0x1EA74E0);

	//RelocPtr<bool> g_bTreesReceiveShadows(0x1EA7570);

	//RelocPtr<bool> g_bEnableAutoDynamicResolution(0x1EABEE8);
	//RelocPtr<int> g_bEnableAutoDynamicResolution_2(0x3186D28);
	//RelocPtr<float> g_fLowestDynamicWidthRatio(0x1EC7210);
	//RelocPtr<float> g_fLowestDynamicHeightRatio(0x1EC7228);
	//RelocPtr<float> g_fRatioIncreasePerSeconds(0x1EC7240);
	//RelocPtr<float> g_fRatioDecreasePerSeconds(0x1EC7258);
	//RelocPtr<unsigned int> g_uiResolutionChangeInterval(0x1EC7270);
	//RelocPtr<float> g_fDecreaseDRMilliseconds(0x1EC7288);
	//RelocPtr<float> g_fIncreaseDRMilliseconds(0x1EC72A0);
	//RelocPtr<float> g_fDRClampOffset(0x1EC72B8);
	//RelocPtr<float> g_fDRClampOffsetNeo(0x1EC72C8);

	//RelocPtr<float> g_fDRCurrentWidth(0x3186D14);
	//RelocPtr<float> g_fDRCurrentHeight(0x3186D18);

	//RelocPtr<UInt64> g_qword_142FEC210(0x2FEC210);

	//RelocPtr<UInt64> g_GrassManager(0x1F85130);

	//RelocPtr<UInt64> g_qword_142FEB6F8(0x2FEB6F8);

	//RelocAddr<_ctor_1401B46A0> ctor_1401B46A0(0x1B46A0);

	//RelocAddr<_ctor_140155A80> ctor_140155A80(0x155A80);

	//RelocAddr<_SetGrassEnabled_1401B69B0> SetGrassEnabled_1401B69B0(0x1B69B0);

	//Sky** g_SkyPtr = nullptr;

	//ini settings
	int enableLog = 1;
	int autoConfigEnabled = 1;
	int checkSleepDuration = 12;
	float MaxTargetFrameTime = 9.5f;
	float MinTargetFrameTime = 8.5f;
	float frameTimeChangeThreshold = 0.5f;
	int waitTurnsToSwitchBackUp = 80;
	int waitTurnsToSwitchBackDown = 80;
	int angleToSwitchBackUp = 30;
	int cpuLateStartAngleToSwitchBackUp = 20;
	float cpuLateStartDistanceSqrSwitchBackUp = 10000;
	int maxLevel = 0;

	int grassChange = 1;
	int LODcitychange = 0;
	int LODinteriorchange = 0;

	int AdjustActorLODFade = 0;
	int MinfLODFadeOutMultActors = 6;
	int MaxfLODFadeOutMultActors = 15;
	int AdjustObjectLODFade = 0;
	int MinfLODFadeOutMultObjects = 5;
	int MaxfLODFadeOutMultObjects = 15;

	int RiftenLODFadeOutModifierActors = -3;
	int RiftenLODFadeOutModifierObjects = -1;
	int OtherCitiesLODFadeOutModifierActors = -2;
	int OtherCitiesLODFadeOutModifierObjects = -1;

	float CPULateStartChangeThreshold = 2.0f;
	int CPULateStartCheckFrameCount = 30;
	int CPULateStartMinFrameCountToReduce = 4;
	int CPULateStartMaxFrameCountToIncrease = 4;

	int LODFadeOutValueUpWait = 30;
	int LODFadeOutValueDownWait = 10;
	int CheckCPUToo = 0;

	int AdjustDynamicResolution = 1;
	int DynamicResolutionStartLevel = 0;
	float MinDynamicResolutionRatio = 1.2f;
	float MaxDynamicResolutionRatio = 1.6f;

	//
	int frameCheckCount = 5;

	int currentLowPerfFrameCount = 0;
	int currentHighPerfFrameCount = 0;

	float lastHeadingAngle = 0;
	float lastCpuLateStartHeadingAngle = 0;
	RE::NiPoint3 lastCpuLateStartPosition;

	int CurrentActorLODFade = 0;
	int CurrentObjectLODFade = 0;

	int currentLevel = 0;

	double currentLevelFrameTime = MaxTargetFrameTime;  //This value will be set for levels higher than 0, and will be used to try changing current level when it drops below this frametime.

	int waitingToGoDown = 0;
	int waitingToGoUp = 0;

	int waitingToGoDownLODFade = 0;
	int waitingToGoUpLODFade = 0;

	bool lastDirectionPositive = false;

	//dr
	int frameCheckCountDR = 5;
	double currentFrameTimeDR = MaxTargetFrameTime;  //This value will be set for levels higher than 0, and will be used to try changing current level when it drops below this frametime.
	int waitingToGoDownDR = 0;
	int waitingToGoUpDR = 0;
	float lastHeadingAngleDR = 0;
	float currentDRRatio = 0.0f;
	int currentLowPerfFrameCountDR = 0;
	int currentHighPerfFrameCountDR = 0;
	bool lastDirectionPositiveDR = false;
	//

	std::unordered_map<int, std::vector<ConfigSetting>> LevelMap;

	std::unordered_map<std::string, bool> ToggleMap;
	std::unordered_map<std::string, int> ToggleLevelMap;

	std::vector<std::string> dataLoadedCommands;
	std::vector<std::string> postLoadGameCommands;
	std::vector<std::string> afterLoadGameCommands;
	std::vector<std::string> newGameCommands;
	std::vector<std::string> conditionalCommands;

	std::vector<ConditionalCommand> conditionalCommandsParsed;
	std::vector<bool> conditionalCommandsApplied;

	std::vector<std::uint32_t> notExteriorWorlds = { 0x69857, 0x1EE62, 0x20DCB, 0x1FAE2, 0x34240, 0x50015, 0x2C965, 0x29AB7, 0x4F838, 0x3A9D6, 0x243DE, 0xC97EB, 0xC350D, 0x1CDD3, 0x1CDD9, 0x21EDB, 0x1E49D, 0x2B101, 0x2A9D8, 0x20BFE };
	//

	bool thereAreCombatChangeConditions = false;

	//Location config:
	int locationConfigEnabled = 0;

	LocationChoice lastLocChoice = LocationChoice::None;

	std::unordered_map<std::string, LocationChoice> LocationMap;

	std::unordered_map<LocationChoice, std::vector<ConfigSetting>> LocationChoiceConfigMap;

	// TODO : get console working

	const size_t cmdBufferSize = 255;
	char cmdBuffer[cmdBufferSize];

	void RunDataLoadedCommandsFunc()
	{
		Sleep(2000);

		for (int i = 0; i < dataLoadedCommands.size(); i++) {
			sprintf_s(cmdBuffer, cmdBufferSize, "%s", dataLoadedCommands[i].c_str());
	//		CSkyrimConsole::RunCommand(cmdBuffer);
			RE::Console::ExecuteCommand(cmdBuffer);
			logger::info("Executed command: %s", dataLoadedCommands[i].c_str());
		}
	}

	void RunDataLoadedCommands()
	{
		std::thread t0(RunDataLoadedCommandsFunc);
		t0.detach();
	}

	void RunAfterLoadGameCommands()
	{
		RE::TESObjectCELL* cell = nullptr;

		const auto g_thePlayer = RE::PlayerCharacter::GetSingleton();
		while (true) {
			if (!g_thePlayer || !g_thePlayer->loadedData) {
				Sleep(5000);
				continue;
			}

			cell = g_thePlayer->parentCell;

			if (!cell) {
				Sleep(3000);
				continue;
			}

			for (int i = 0; i < afterLoadGameCommands.size(); i++) {
				sprintf_s(cmdBuffer, cmdBufferSize, "%s", afterLoadGameCommands[i].c_str());
	//			CSkyrimConsole::RunCommand(cmdBuffer);
				RE::Console::ExecuteCommand(cmdBuffer);
				logger::info("Executed command after load game: %s", afterLoadGameCommands[i].c_str());
			}
			break;
		}
	}

	void RunPostLoadGameCommands()
	{
		for (int i = 0; i < postLoadGameCommands.size(); i++) {
			sprintf_s(cmdBuffer, cmdBufferSize, "%s", postLoadGameCommands[i].c_str());
	//		CSkyrimConsole::RunCommand(cmdBuffer);
			RE::Console::ExecuteCommand(cmdBuffer);
			logger::info("Executed command post load game: %s", postLoadGameCommands[i].c_str());
		}

		std::thread t3(RunAfterLoadGameCommands);
		t3.detach();
	}

	void RunNewGameCommands()
	{
		for (int i = 0; i < newGameCommands.size(); i++) {
			sprintf_s(cmdBuffer, cmdBufferSize, "%s", newGameCommands[i].c_str());
	//		CSkyrimConsole::RunCommand(cmdBuffer);
			RE::Console::ExecuteCommand(cmdBuffer);
			logger::info("Executed command new game: %s", newGameCommands[i].c_str());
		}
	}

	void ParseConditionalCommands()
	{
		conditionalCommandsParsed.clear();
		conditionalCommandsApplied.clear();
		for (int i = 0; i < conditionalCommands.size(); i++) {
			std::vector<std::string> commandSplitted = split(conditionalCommands[i], '|');
			if (commandSplitted.size() == 4 || commandSplitted.size() == 2) {
				ConditionalCommand cc;
				cc.isInterior = strcmp(commandSplitted[0].c_str(), "Interior") == 0 || strcmp(commandSplitted[0].c_str(), "interior") == 0;
				cc.InCombat = strcmp(commandSplitted[0].c_str(), "InCombat") == 0 || strcmp(commandSplitted[0].c_str(), "incombat") == 0;
				cc.OutOfCombat = strcmp(commandSplitted[0].c_str(), "OutOfCombat") == 0 || strcmp(commandSplitted[0].c_str(), "outofcombat") == 0;

				if (commandSplitted.size() == 4) {
					cc.startTime = strtof(commandSplitted[1].c_str(), 0);
					cc.endTime = strtof(commandSplitted[2].c_str(), 0);
					cc.command = commandSplitted[3];
				} else if (commandSplitted.size() == 2) {
					cc.command = commandSplitted[1];
				}

				if (cc.InCombat || cc.OutOfCombat) {
					thereAreCombatChangeConditions = true;
				}

				if (cc.command.rfind("IMOD>", 0) == 0)  //Starts with
				{
					cc.isImod = true;

					std::string str = eraseAllSubStr(cc.command, "IMOD>");
					std::string variableName;
					double variableValue = GetConfigSettingsDoubleValue(str, variableName);

					cc.strength = variableValue;

					std::transform(variableName.begin(), variableName.end(), variableName.begin(), ::tolower);

					trim(variableName);

					std::vector<std::string> splittedByPlugin = split(variableName, '-');

					std::string pluginNumber = "";

					if (splittedByPlugin.size() > 0) {
						RE::TESDataHandler* dataHandler = RE::TESDataHandler::GetSingleton();
						std::optional<std::uint8_t> modIndex = dataHandler->GetModIndex(splittedByPlugin[0].c_str());

						if (modIndex && modIndex != 255) {
							if (splittedByPlugin.size() > 1) {
								//pluginNumber = num2hex(modIndex, 2);

								//std::uint32_t formId = getHex((pluginNumber + splittedByPlugin[1]).c_str());

								//RE::TESForm* form = RE::TESForm::GetFormByID(formId);

								//if (form != nullptr) {
								//	RE::TESImageSpaceModifier* ismForm = DYNAMIC_CAST(form, TESForm, TESImageSpaceModifier);
								//	if (ismForm != nullptr) {
								//		cc.imod = ismForm;
								//	}
								//}
							}
						}
					}
				}

				conditionalCommandsParsed.emplace_back(cc);
				conditionalCommandsApplied.emplace_back(false);
			}
		}
	}

	void loadConfig()
	{
	//	SafeWriteJump(addressStart.GetUIntPtr(), addressEnd.GetUIntPtr());

			std::string filepath = "Data\\SKSE\\Plugins\\VRFpsStabilizer.ini";

			std::ifstream file(filepath);

			if (!file.is_open()) {
				transform(filepath.begin(), filepath.end(), filepath.begin(), ::tolower);
				file.open(filepath);
			}

			if (file.is_open()) {
				std::string line;
				std::string currentSetting;
				while (std::getline(file, line)) {
					//trim(line);
					skipComments(line);
					trim(line);
					if (line.length() > 0) {
						if (line.substr(0, 1) == "[") {
							//newsetting
							currentSetting = line;
						} else {
							if (currentSetting == "[DataLoaded]") {
								dataLoadedCommands.emplace_back(line);
							} else if (currentSetting == "[PostLoadGame]") {
								postLoadGameCommands.emplace_back(line);
							} else if (currentSetting == "[AfterLoadGame]") {
								afterLoadGameCommands.emplace_back(line);
							} else if (currentSetting == "[NewGame]") {
								newGameCommands.emplace_back(line);
							} else if (currentSetting == "[Conditional]") {
								conditionalCommands.emplace_back(line);
							} else if (currentSetting == "[Settings]") {
								std::string variableName;
								int variableValue = GetConfigSettingsValue(line, variableName);

								if (variableName == "AutoConfigEnabled") {
									autoConfigEnabled = variableValue;
								} else if (variableName == "CheckSleepDuration") {
									checkSleepDuration = variableValue;
								} else if (variableName == "EnableLog") {
									enableLog = variableValue;
								} else if (variableName == "LevelChangeBackUpWait") {
									waitTurnsToSwitchBackUp = variableValue;
								} else if (variableName == "LevelChangeBackDownWait") {
									waitTurnsToSwitchBackDown = variableValue;
								} else if (variableName == "LevelChangeBackUpAngle") {
									angleToSwitchBackUp = variableValue;
								} else if (variableName == "GrassChange") {
									grassChange = variableValue;
								} else if (variableName == "LODcitychange") {
									LODcitychange = variableValue;
								} else if (variableName == "CheckCpuToo") {
									CheckCPUToo = variableValue;
								} else if (variableName == "LODinteriorchange") {
									LODinteriorchange = variableValue;
								} else if (variableName == "AdjustActorLODFade") {
									AdjustActorLODFade = variableValue;
								} else if (variableName == "MinfLODFadeOutMultActors") {
									MinfLODFadeOutMultActors = variableValue;
								} else if (variableName == "MaxfLODFadeOutMultActors") {
									MaxfLODFadeOutMultActors = variableValue;
								} else if (variableName == "AdjustObjectLODFade") {
									AdjustObjectLODFade = variableValue;
								} else if (variableName == "MinfLODFadeOutMultObjects") {
									MinfLODFadeOutMultObjects = variableValue;
								} else if (variableName == "MaxfLODFadeOutMultObjects") {
									MaxfLODFadeOutMultObjects = variableValue;
								} else if (variableName == "RiftenLODFadeOutModifierActors") {
									RiftenLODFadeOutModifierActors = variableValue;
								} else if (variableName == "RiftenLODFadeOutModifierObjects") {
									RiftenLODFadeOutModifierObjects = variableValue;
								} else if (variableName == "OtherCitiesLODFadeOutModifierActors") {
									OtherCitiesLODFadeOutModifierActors = variableValue;
								} else if (variableName == "OtherCitiesLODFadeOutModifierObjects") {
									OtherCitiesLODFadeOutModifierObjects = variableValue;
								} else if (variableName == "CPULateStartCheckFrameCount") {
									CPULateStartCheckFrameCount = variableValue;
									if (CPULateStartCheckFrameCount > 100) {
										CPULateStartCheckFrameCount = 100;
									}
								} else if (variableName == "CPULateStartMinFrameCountToReduce") {
									CPULateStartMinFrameCountToReduce = variableValue;
								} else if (variableName == "CPULateStartMaxFrameCountToIncrease") {
									CPULateStartMaxFrameCountToIncrease = variableValue;
								} else if (variableName == "LODFadeOutValueUpWait") {
									LODFadeOutValueUpWait = variableValue;
								} else if (variableName == "LODFadeOutValueDownWait") {
									LODFadeOutValueDownWait = variableValue;
								} else if (variableName == "MaxTargetFrameTime") {
									MaxTargetFrameTime = GetConfigSettingsFloatValue(line, variableName);
								} else if (variableName == "MinTargetFrameTime") {
									MinTargetFrameTime = GetConfigSettingsFloatValue(line, variableName);
								} else if (variableName == "FrameTimeChangeThreshold") {
									frameTimeChangeThreshold = GetConfigSettingsFloatValue(line, variableName);
								} else if (variableName == "CPULateStartChangeThreshold") {
									CPULateStartChangeThreshold = GetConfigSettingsFloatValue(line, variableName);
								} else if (variableName == "CPULateStartAngleToSwitchBackUp ") {
									cpuLateStartAngleToSwitchBackUp = variableValue;
								} else if (variableName == "CPULateStartDistanceToSwitchBackUp") {
									cpuLateStartDistanceSqrSwitchBackUp = GetConfigSettingsFloatValue(line, variableName);
									cpuLateStartDistanceSqrSwitchBackUp = cpuLateStartDistanceSqrSwitchBackUp * cpuLateStartDistanceSqrSwitchBackUp;
								} else if (variableName == "AdjustDynamicResolution") {
									AdjustDynamicResolution = variableValue;
								} else if (variableName == "DynamicResolutionStartLevel") {
									DynamicResolutionStartLevel = variableValue;
								} else if (variableName == "MinDynamicResolutionRatio") {
									MinDynamicResolutionRatio = GetConfigSettingsFloatValue(line, variableName);
								} else if (variableName == "MaxDynamicResolutionRatio") {
									MaxDynamicResolutionRatio = GetConfigSettingsFloatValue(line, variableName);
								}
							} else if (stringStartsWith(currentSetting, "[Level")) {
								std::string variableNameStr;
								std::string variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);
								std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
								ConfigSetting cs;

								std::string levelStr = eraseAllSubStr(currentSetting, "[Level");
								levelStr = eraseAllSubStr(levelStr, "]");
								int level = std::stoi(levelStr);

								//if (variableNameStr.rfind("ENB>", 0) == 0)  //Starts with
								//{
								//	cs.enb = true;
								//	std::string enbStr = eraseAllSubStr(variableNameStr, "ENB>");
								//	ConfigLineSplitterENB(enbStr, cs);
								//	if (variableValueStr == "true" || variableValueStr == "false") {
								//		cs.valueBOOL = variableValueStr == "true";
								//	} else if (Contains(variableValueStr, ".")) {
								//		cs.valueFLOAT = GetConfigSettingsFloatValue(line, variableNameStr);
								//	} else {
								//		cs.valueINT = GetConfigSettingsValue(line, variableNameStr);
								//	}
								//} 
								if (variableNameStr.rfind("CONSOLE>", 0) == 0)  //Starts with
								{
									cs.console = true;
									cs.command = eraseAllSubStr(variableNameStr, "CONSOLE>");
								} else if (variableNameStr.rfind("TOGGLE>", 0) == 0)  //Starts with
								{
									cs.toggle = true;
									cs.command = eraseAllSubStr(variableNameStr, "TOGGLE>");
									ToggleMap[cs.command] = false;
									ToggleLevelMap[cs.command] = level;
								} else {
									std::string variableName;
									double variableValue = GetConfigSettingsDoubleValue(line, variableName);

									cs.enb = false;
									cs.variableName = variableName;
									cs.variableValue = variableValue;
								}

								if (LevelMap.find(level) == LevelMap.end()) {
									LevelMap[level] = std::vector<ConfigSetting>{ cs };
								} else {
									LevelMap[level].emplace_back(cs);
								}

								if (level > maxLevel)
									maxLevel = level;
							}
						}
					}
				}
				ParseConditionalCommands();

				if (AdjustDynamicResolution && maxLevel > DynamicResolutionStartLevel) {
					for (int i = 0; i <= maxLevel; i++) {
						ConfigSetting cs;
						cs.enb = false;
						cs.toggle = false;
						cs.console = false;
						cs.variableName = "fLowestDynamicWidthRatio:Display";
						if (i < DynamicResolutionStartLevel) {
							cs.variableValue = 1.0;
						} else {
							cs.variableValue = MaxDynamicResolutionRatio - ((MaxDynamicResolutionRatio - MinDynamicResolutionRatio) * ((float)(i - DynamicResolutionStartLevel) / (maxLevel - DynamicResolutionStartLevel)));
						}
						LevelMap[i].emplace_back(cs);

						cs.variableName = "fLowestDynamicHeightRatio:Display";
						LevelMap[i].emplace_back(cs);
					}
				}
			}
		
	}

	void loadLocationConfig()
	{

			std::string filepath = "Data\\SKSE\\Plugins\\VRFpsStabilizerLocation.ini";

			std::ifstream file(filepath);

			if (!file.is_open()) {
				transform(filepath.begin(), filepath.end(), filepath.begin(), ::tolower);
				file.open(filepath);
			}

			if (file.is_open()) {
				std::string line;
				std::string currentSetting;
				while (std::getline(file, line)) {
					//trim(line);
					skipComments(line);
					trim(line);
					if (line.length() > 0) {
						//LOG_ERR("Parsing line: %s", line.c_str());
						if (line.substr(0, 1) == "[") {
							//newsetting
							currentSetting = line;
						} else {
							if (currentSetting == "[Settings]") {
								std::string variableName;
								std::string variableValueStr = GetConfigSettingsStringValue(line, variableName);

								if (variableName == "Enabled") {
									locationConfigEnabled = GetConfigSettingsValue(line, variableName);
									;
								} else if (variableName == "Best") {
									trim(variableValueStr);

									if (variableValueStr != "") {
										std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
										std::vector<std::string> splitted = splitTrimmed(variableValueStr, ',');
										for (int i = 0; i < splitted.size(); i++) {
											if (splitted[i] != "") {
												LocationMap[splitted[i]] = LocationChoice::Best;
											}
										}
									}
								} else if (variableName == "Good") {
									std::string variableNameStr;
									variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);

									trim(variableValueStr);

									if (variableValueStr != "") {
										std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
										std::vector<std::string> splitted = splitTrimmed(variableValueStr, ',');
										for (int i = 0; i < splitted.size(); i++) {
											if (splitted[i] != "") {
												LocationMap[splitted[i]] = LocationChoice::Good;
											}
										}
									}
								} else if (variableName == "Default") {
									std::string variableNameStr;
									variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);

									trim(variableValueStr);

									if (variableValueStr != "") {
										std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
										std::vector<std::string> splitted = splitTrimmed(variableValueStr, ',');
										for (int i = 0; i < splitted.size(); i++) {
											if (splitted[i] != "") {
												LocationMap[splitted[i]] = LocationChoice::Default;
											}
										}
									}
								} else if (variableName == "Bad") {
									std::string variableNameStr;
									variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);

									trim(variableValueStr);

									if (variableValueStr != "") {
										std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
										std::vector<std::string> splitted = splitTrimmed(variableValueStr, ',');

										for (int i = 0; i < splitted.size(); i++) {
											if (splitted[i] != "") {
												LocationMap[splitted[i]] = LocationChoice::Bad;
											}
										}
									}
								} else if (variableName == "Worst") {
									std::string variableNameStr;
									variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);

									trim(variableValueStr);

									if (variableValueStr != "") {
										std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
										std::vector<std::string> splitted = splitTrimmed(variableValueStr, ',');
										for (int i = 0; i < splitted.size(); i++) {
											if (splitted[i] != "") {
												LocationMap[splitted[i]] = LocationChoice::Worst;
											}
										}
									}
								}
							} else if (currentSetting == "[Default]" || currentSetting == "[Best]" || currentSetting == "[Good]" || currentSetting == "[Bad]" || currentSetting == "[Worst]") {
								std::string variableNameStr;
								std::string variableValueStr = GetConfigSettingsStringValue(line, variableNameStr);
								std::transform(variableValueStr.begin(), variableValueStr.end(), variableValueStr.begin(), ::tolower);
								ConfigSetting cs;

								//if (variableNameStr.rfind("ENB>", 0) == 0)  //Starts with
								//{
								//	cs.enb = true;
								//	std::string enbStr = eraseAllSubStr(variableNameStr, "ENB>");
								//	ConfigLineSplitterENB(enbStr, cs);
								//	if (variableValueStr == "true" || variableValueStr == "false") {
								//		cs.valueBOOL = variableValueStr == "true";
								//	} else if (Contains(variableValueStr, ".")) {
								//		cs.valueFLOAT = GetConfigSettingsFloatValue(line, variableNameStr);
								//	} else {
								//		cs.valueINT = GetConfigSettingsValue(line, variableNameStr);
								//	}
								//} else 
								if (variableNameStr.rfind("CONSOLE>", 0) == 0)  //Starts with
								{
									cs.console = true;
									cs.command = eraseAllSubStr(variableNameStr, "CONSOLE>");
								} else {
									std::string variableName;
									double variableValue = GetConfigSettingsDoubleValue(line, variableName);

									cs.enb = false;
									cs.variableName = variableName;
									cs.variableValue = variableValue;
								}

								LocationChoice lc = LocationChoice::None;

								if (currentSetting == "[Default]") {
									lc = LocationChoice::Default;
								} else if (currentSetting == "[Best]") {
									lc = LocationChoice::Best;
								} else if (currentSetting == "[Good]") {
									lc = LocationChoice::Good;
								} else if (currentSetting == "[Bad]") {
									lc = LocationChoice::Bad;
								} else if (currentSetting == "[Worst]") {
									lc = LocationChoice::Worst;
								}

								if (lc != LocationChoice::None) {
									if (LocationChoiceConfigMap.find(lc) == LocationChoiceConfigMap.end()) {
										LocationChoiceConfigMap[lc] = std::vector<ConfigSetting>{ cs };
									} else {
										LocationChoiceConfigMap[lc].emplace_back(cs);
									}
								}
							}
						}
					}
				}
			}
	
	}

	void InitSystem()
	{
		ivrCompositor = vr::VRCompositor();
		if (ivrCompositor)
			logger::info("VR compositor is alive.");
		else
			logger::critical("No VR compositor found.");
	}

	void ToggleSwitch(int level)
	{
		for (auto& toggleLevel : ToggleLevelMap) {
			if (level < toggleLevel.second)  //This toggle must be off
			{
				if (ToggleMap[toggleLevel.first] == true) {
					sprintf_s(cmdBuffer, cmdBufferSize, "%s", toggleLevel.first.c_str());
					//CSkyrimConsole::RunCommand(cmdBuffer);
					RE::Console::ExecuteCommand(cmdBuffer);
				}
			} else  //This toggle must be on
			{
				if (ToggleMap[toggleLevel.first] == false) {
					sprintf_s(cmdBuffer, cmdBufferSize, "%s", toggleLevel.first.c_str());
				//	CSkyrimConsole::RunCommand(cmdBuffer);
					RE::Console::ExecuteCommand(cmdBuffer);
				}
			}
		}
	}

	void Resetter()
	{
		if (autoConfigEnabled != 0) {
			Sleep(5000);
			logger::critical("Setting ini settings to Level 0 values...");
			if (LevelMap.find(0) != LevelMap.end()) {
				for (int i = 0; i < LevelMap[0].size(); i++) {
					if (LevelMap[0][i].console) {
						sprintf_s(cmdBuffer, cmdBufferSize, "%s", LevelMap[0][i].command.c_str());
				//		CSkyrimConsole::RunCommand(cmdBuffer);
						RE::Console::ExecuteCommand(cmdBuffer);
					} else if (LevelMap[0][i].enb == false && LevelMap[0][i].toggle == false) {
						RE::Setting* csetting = GetINISetting(LevelMap[0][i].variableName.c_str());
						if (csetting) {
							csetting->SetFloat(LevelMap[0][i].variableValue);
						}
					}
				}
				currentLevel = 0;
			} else {
				logger::info("Cannot find Level 0 values...");
			}

			ToggleSwitch(0);
		}
	}
	
	void ResetIniSettings()
	{
		std::thread t0(Resetter);
		t0.detach();
	}

	bool IsInTimePeriod(float startTime, float endTime)
	{
		float gametime = RE::Calendar::GetSingleton()->gameHour->value;
		if (startTime > endTime) {
			return (gametime >= startTime && gametime < endTime + 24.0f);
		} else {
			return (gametime >= startTime && gametime < endTime);
		}
	}


	void RunConditionalCommand(ConditionalCommand cc)
	{
		if (cc.isImod) {
			//if (cc.imod != nullptr) {
			//	if (cc.strength > TOLERANCE) {
			//		g_task->AddTask(new taskApplyImod(cc.imod, cc.strength));
			//		LOG("Applied imod. %x", cc.imod->formID);
			//	} else {
			//		g_task->AddTask(new taskRemoveImod(cc.imod));
			//		LOG("Removed imod. %x", cc.imod->formID);
			//	}
			//}
		} else {
			if (cc.command.rfind("ENB>", 0) == 0)  //Starts with
			{
				return;
			} else if (cc.command.rfind("TOGGLE>", 0) == 0)  //Starts with
			{
				return;
			} else if (cc.command.rfind("CONSOLE>", 0) == 0)  //Starts with
			{
				std::string consoleStr = eraseAllSubStr(cc.command, "CONSOLE>");
			//	SkyrimConsole::RunCommand(consoleStr.c_str());
				RE::Console::ExecuteCommand(consoleStr.c_str());
			} else if (cc.command.rfind("INI>", 0) == 0)  //Starts with
			{
				std::string iniStr = eraseAllSubStr(cc.command, "INI>");
				std::string variableName;
				double variableValue = GetConfigSettingsDoubleValue(iniStr, variableName);

				RunIniSetting(variableName, variableValue);
			} else {
		//		CSkyrimConsole::RunCommand(cc.command.c_str());
				RE::Console::ExecuteCommand(cc.command.c_str());
			}
		}
	}

	//BGSLocation* GetCurLocationFromCoordinates()
	//{
	//	if ((*g_thePlayer) != nullptr) {
	//		if ((*g_thePlayer)->pos.y <= 16 * 4096 && (*g_thePlayer)->pos.y >= -12 * 4096 && (*g_thePlayer)->pos.x >= -53 * 4096 && (*g_thePlayer)->pos.x <= -20 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x016769), TESForm, BGSLocation);  //Reach
	//		} else if ((*g_thePlayer)->pos.y <= 14 * 4096 && (*g_thePlayer)->pos.y >= -11 * 4096 && (*g_thePlayer)->pos.x >= 16 * 4096 && (*g_thePlayer)->pos.x <= 46 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676A), TESForm, BGSLocation);  //Eastmarch
	//		} else if ((*g_thePlayer)->pos.y <= 37 * 4096 && (*g_thePlayer)->pos.y >= 14 * 4096 && (*g_thePlayer)->pos.x >= 17 * 4096 && (*g_thePlayer)->pos.x <= 46 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676B), TESForm, BGSLocation);  //Winterhold
	//		} else if ((*g_thePlayer)->pos.y <= -11 * 4096 && (*g_thePlayer)->pos.y >= -33 * 4096 && (*g_thePlayer)->pos.x >= 1 * 4096 && (*g_thePlayer)->pos.x <= 54 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676C), TESForm, BGSLocation);  //Rift
	//		} else if ((*g_thePlayer)->pos.y <= 32 * 4096 && (*g_thePlayer)->pos.y >= 6 * 4096 && (*g_thePlayer)->pos.x >= -4 * 4096 && (*g_thePlayer)->pos.x <= 17 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676D), TESForm, BGSLocation);  //Pale
	//		} else if ((*g_thePlayer)->pos.y <= 24 * 4096 && (*g_thePlayer)->pos.y >= 8 * 4096 && (*g_thePlayer)->pos.x >= -19 * 4096 && (*g_thePlayer)->pos.x <= -4 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676E), TESForm, BGSLocation);  //Hjaalmarch
	//		} else if ((*g_thePlayer)->pos.y <= -9 * 4096 && (*g_thePlayer)->pos.y >= -28 * 4096 && (*g_thePlayer)->pos.x >= -24 * 4096 && (*g_thePlayer)->pos.x <= 1 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x01676F), TESForm, BGSLocation);  //Falkreath
	//		} else if ((*g_thePlayer)->pos.y <= 37 * 4096 && (*g_thePlayer)->pos.y >= 13 * 4096 && (*g_thePlayer)->pos.x >= -44 * 4096 && (*g_thePlayer)->pos.x <= -12 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x016770), TESForm, BGSLocation);  //Haafingar
	//		} else if ((*g_thePlayer)->pos.y <= 10 * 4096 && (*g_thePlayer)->pos.y >= -13 * 4096 && (*g_thePlayer)->pos.x >= -27 * 4096 && (*g_thePlayer)->pos.x <= 17 * 4096) {
	//			return DYNAMIC_CAST(LookupFormByID(0x016771), TESForm, BGSLocation);  //Whiterun
	//		}
	//	}
	//}


	void ChangeFunction()
	{
		RE::TESObjectCELL* cell = nullptr;

		bool lodON = true;
		bool cellChanged = true;
		bool firstTime = true;
		bool firstTimeLoc = true;
		bool lastAppliedCombatState = false;
		bool combatStateChanged = false;

		bool latestCellInterior = false;

		RE::TESObjectCELL* curCell = nullptr;

	//	g_SkyPtr = RelocPtr<RE::Sky*>(0x2FC62C8);

		const auto g_thePlayer = RE::PlayerCharacter::GetSingleton();
		while (true) {
			if (!g_thePlayer || !g_thePlayer->loadedData) {
				Sleep(5000);
				continue;
			}

			cell = g_thePlayer->parentCell;

			if (!cell) {
				Sleep(3000);
				continue;
			}

	//		const auto skyPtr = *g_SkyPtr;

			if (/*!isGameStopped() && */ curCell != cell) {
				//cell change
				Sleep(1000);
				curCell = cell;

				cellChanged = true;

				latestCellInterior = (!(cell->worldSpace) || (std::find(notExteriorWorlds.begin(), notExteriorWorlds.end(), cell->worldSpace->formID) != notExteriorWorlds.end()));

				if (LODcitychange == 1 || LODinteriorchange == 1) {
					bool newLODState = true;
					//Interiors and Cities with LOD data
					if (cell->worldSpace != nullptr && cell->worldSpace->parentWorld != nullptr && cell->worldSpace->parentUseFlags.all(RE::TESWorldSpace::PARENT_USE_FLAG::kClimate)) {
						if ((LODinteriorchange == 1 && (cell->cellFlags.all(RE::TESObjectCELL::Flag::kInterior))) || LODcitychange == 1) {
							newLODState = false;
						}
					}

					if ((lodON) && !newLODState) {
						lodON = false;
				//		CSkyrimConsole::RunCommand("tll");
						RE::Console::ExecuteCommand("tll");
						logger::info("Turning LOD off.");
					} else if ((!lodON) && newLODState) {
						lodON = true;
						/*CSkyrimConsole::RunCommand("tll");*/
						RE::Console::ExecuteCommand("tll");
						logger::info("Turning LOD on.");
					}
				}

				if (grassChange == 1) {
					//LOG_ERR("Cell changed. Currently grass is %s. New cell %s worldspace and parentworld is %s. ", grassOn ? "ON" : "OFF", cell->unk120 != nullptr ? "has" : "doesn't have", (cell->unk120 != nullptr && cell->unk120->parentWorld == nullptr) ? "NULL so we turn the grass ON now" : "not null");
					bool newGrassState = false;

					if (cell->worldSpace != nullptr)  //Has worldspace
					{
						if (cell->worldSpace->parentWorld == nullptr)  //Exteriors (excluding inside cities)
						{
							newGrassState = true;
						}
					}
					bool isGrassEnabled = true;
					//__int64 v0_r8 = *(__int64*)((std::uint64_t)(*g_GrassManager) + 104);
					//if (v0_r8) {
					//	isGrassEnabled = ((*(BYTE*)(v0_r8 + 268)) & 1) == 0;
					//	//LOG_ERR("Grass is currently %s.", isGrassEnabled ? "enabled" : "disabled");
					//}

					if (isGrassEnabled && !newGrassState) {
				//		CSkyrimConsole::RunCommand("tg");
						RE::Console::ExecuteCommand("tg");

						logger::info("Turning grass off.");
					} else if ((!isGrassEnabled) && newGrassState) {
				//		CSkyrimConsole::RunCommand("tg");
						RE::Console::ExecuteCommand("tg");

						logger::info("Turning grass on.");
					}
				}
			}

			//if (locationConfigEnabled && cellChanged) {
			//	cellChanged = false;
			//	RE::BGSLocation* curLoc = g_thePlayer->currentLocation;
			//	if (curLoc == nullptr || curLoc->formType.all(RE::ENUM_FORM_ID::kLCTN) || RE::TESForm::GetFormByID(curLoc->formID) == nullptr) {
			//		curLoc = GetCurLocationFromCoordinates();
			//	}
			//	bool foundNewLocChoice = false;
			//	bool isLocChoiceDifferent = false;
			//	if (curLoc != nullptr || firstTimeLoc) {
			//		if (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) {
			//			std::string curLocName = "000";

			//			if (curLoc->fullName.GetName() != nullptr) {
			//				curLocName = curLoc->fullName.GetName();
			//				std::transform(curLocName.begin(), curLocName.end(), curLocName.begin(), ::tolower);
			//			}
			//			auto it = LocationMap.find(curLocName);

			//			if (it != LocationMap.end()) {
			//				foundNewLocChoice = true;
			//				LocationChoice curLocChoice = it->second;
			//				if (curLocChoice != lastLocChoice) {
			//					isLocChoiceDifferent = true;
			//					//Change ini settings
			//					lastLocChoice = curLocChoice;
			//				}
			//			} else if (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) {
			//				if (curLoc->parentLoc->fullName.GetName() != nullptr) {
			//					curLocName = curLoc->parentLoc->fullName.GetName();
			//					std::transform(curLocName.begin(), curLocName.end(), curLocName.begin(), ::tolower);
			//				}
			//				auto itp = LocationMap.find(curLocName);

			//				if (itp != LocationMap.end()) {
			//					foundNewLocChoice = true;
			//					LocationChoice parentLocChoice = itp->second;
			//					if (parentLocChoice != lastLocChoice) {
			//						isLocChoiceDifferent = true;
			//						//Change ini settings
			//						lastLocChoice = parentLocChoice;
			//					}
			//				} else if (curLoc->parentLoc->parentLoc != nullptr && curLoc->parentLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->parentLoc->formID) != nullptr) {
			//					if (curLoc->parentLoc->parentLoc->fullName.GetName() != nullptr) {
			//						curLocName = curLoc->parentLoc->parentLoc->fullName.GetName();
			//						std::transform(curLocName.begin(), curLocName.end(), curLocName.begin(), ::tolower);
			//					}
			//					auto itpp = LocationMap.find(curLocName);

			//					if (itpp != LocationMap.end()) {
			//						foundNewLocChoice = true;
			//						LocationChoice parentparentLocChoice = itpp->second;
			//						if (parentparentLocChoice != lastLocChoice) {
			//							isLocChoiceDifferent = true;
			//							//Change ini settings
			//							lastLocChoice = parentparentLocChoice;
			//						}
			//					}
			//				}
			//			}
			//		}

			//		if (foundNewLocChoice && isLocChoiceDifferent) {
			//			SwitchToLocationSettings(lastLocChoice, curLoc);
			//		} else if (!foundNewLocChoice && firstTimeLoc) {
			//			if (lastLocChoice != LocationChoice::Worst) {
			//				lastLocChoice = LocationChoice::Worst;
			//				SwitchToLocationSettings(LocationChoice::Worst, curLoc);
			//			}
			//		} else if (!foundNewLocChoice) {
			//			if (lastLocChoice != LocationChoice::Default) {
			//				lastLocChoice = LocationChoice::Default;
			//				SwitchToLocationSettings(LocationChoice::Default, curLoc);
			//			}
			//		}

			//		if (firstTimeLoc)
			//			firstTimeLoc = false;

			//		//LOG_ERR("Current Location: %x - %s -- Parent Loc: %x - %s", curLoc->formID, curLoc->fullName.GetName(), curLoc->parentLoc != nullptr ? curLoc->parentLoc->formID : 0, curLoc->parentLoc != nullptr ? curLoc->parentLoc->fullName.GetName() : "None");
			//	} else {
			//		cellChanged = true;
			//	}
			//}

			if (thereAreCombatChangeConditions) {
				combatStateChanged = false;
				bool isInCombat = ActorInCombat(g_thePlayer);
				if (firstTime) {
					firstTime = false;
					combatStateChanged = true;
					lastAppliedCombatState = isInCombat;
				}

				if (lastAppliedCombatState != isInCombat) {
					combatStateChanged = true;
					lastAppliedCombatState = isInCombat;
				}

				if (combatStateChanged && conditionalCommandsParsed.size() > 0) {
					for (int i = 0; i < conditionalCommandsParsed.size(); i++) {
						if (conditionalCommandsParsed[i].InCombat || conditionalCommandsParsed[i].OutOfCombat) {
							if ((conditionalCommandsParsed[i].InCombat && isInCombat) || (conditionalCommandsParsed[i].OutOfCombat && !isInCombat)) {
								if (conditionalCommandsParsed[i].startTime < TOLERANCE && conditionalCommandsParsed[i].endTime < TOLERANCE)  //don't check time
								{
									if (conditionalCommandsApplied[i] == false) {
										RunConditionalCommand(conditionalCommandsParsed[i]);
										conditionalCommandsApplied[i] = true;
									}
								} else if (IsInTimePeriod(conditionalCommandsParsed[i].startTime, conditionalCommandsParsed[i].endTime))  //time checks out
								{
									if (conditionalCommandsApplied[i] == false) {
										RunConditionalCommand(conditionalCommandsParsed[i]);
										conditionalCommandsApplied[i] = true;
									}
								} else  //not in time period
								{
									if (conditionalCommandsApplied[i]) {
										conditionalCommandsApplied[i] = false;
									}
								}
							} else  //event doesn't match
							{
								if (conditionalCommandsApplied[i]) {
									conditionalCommandsApplied[i] = false;
								}
							}
						}
					}
				}
			}

			if (conditionalCommandsParsed.size() > 0) {
				for (int i = 0; i < conditionalCommandsParsed.size(); i++) {
					if (!conditionalCommandsParsed[i].InCombat && !conditionalCommandsParsed[i].OutOfCombat) {
						if ((latestCellInterior && conditionalCommandsParsed[i].isInterior) || (!latestCellInterior && !conditionalCommandsParsed[i].isInterior))  //space or event matches
						{
							if (conditionalCommandsParsed[i].startTime < TOLERANCE && conditionalCommandsParsed[i].endTime < TOLERANCE)  //don't check time
							{
								if (conditionalCommandsApplied[i] == false) {
									RunConditionalCommand(conditionalCommandsParsed[i]);
									conditionalCommandsApplied[i] = true;
								}
							} else if (IsInTimePeriod(conditionalCommandsParsed[i].startTime, conditionalCommandsParsed[i].endTime))  //time checks out
							{
								if (conditionalCommandsApplied[i] == false) {
									RunConditionalCommand(conditionalCommandsParsed[i]);
									conditionalCommandsApplied[i] = true;
								}
							} else  //not in time period
							{
								if (conditionalCommandsApplied[i]) {
									conditionalCommandsApplied[i] = false;
								}
							}
						} else  //space doesn't match
						{
							if (conditionalCommandsApplied[i]) {
								conditionalCommandsApplied[i] = false;
							}
						}
					}
				}
			}

			Sleep(1000);
		}
	}


	//void SwitchToLocationSettings(LocationChoice locChoice, BGSLocation* curLoc)
	//{
	//	std::vector<ConfigSetting>* SettingsList = GetSettingsListForLocationChoice(locChoice);

	//	if (SettingsList != nullptr) {
	//		for (int i = 0; i < SettingsList->size(); i++) {
	//			if (SettingsList->at(i).enb)
	//				continue;

	//			if (SettingsList->at(i).console) {
	//				sprintf_s(cmdBuffer, cmdBufferSize, "%s", SettingsList->at(i).command.c_str());
	//				CSkyrimConsole::RunCommand(cmdBuffer);
	//			} else if (SettingsList->at(i).enb == false && SettingsList->at(i).toggle == false) {
	//				RunIniSetting(SettingsList->at(i).variableName, SettingsList->at(i).variableValue);
	//			}
	//		}

	//		if (enableLog > 0) {
	//			if (locChoice == LocationChoice::Default)
	//				LOG("Switching to Default settings. Current Location : % x - % s -- Parent Loc : % x - % s", curLoc != nullptr ? curLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) ? curLoc->fullName.GetName() : "null", (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->fullName.GetName() : "None");
	//			else if (locChoice == LocationChoice::Best)
	//				LOG("Switching to Best settings. Current Location : % x - % s -- Parent Loc : % x - % s", curLoc != nullptr ? curLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) ? curLoc->fullName.GetName() : "null", (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->fullName.GetName() : "None");
	//			else if (locChoice == LocationChoice::Good)
	//				LOG("Switching to Good settings. Current Location : % x - % s -- Parent Loc : % x - % s", curLoc != nullptr ? curLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) ? curLoc->fullName.GetName() : "null", (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->fullName.GetName() : "None");
	//			else if (locChoice == LocationChoice::Bad)
	//				LOG("Switching to Bad settings. Current Location : % x - % s -- Parent Loc : % x - % s", curLoc != nullptr ? curLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) ? curLoc->fullName.GetName() : "null", (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->fullName.GetName() : "None");
	//			else if (locChoice == LocationChoice::Worst)
	//				LOG("Switching to Worst settings. Current Location : % x - % s -- Parent Loc : % x - % s", curLoc != nullptr ? curLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) ? curLoc->fullName.GetName() : "null", (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->formID : 0, (curLoc != nullptr && curLoc->formType == kFormType_Location && LookupFormByID(curLoc->formID) != nullptr) && (curLoc->parentLoc != nullptr && curLoc->parentLoc->formType == kFormType_Location && LookupFormByID(curLoc->parentLoc->formID) != nullptr) ? curLoc->parentLoc->fullName.GetName() : "None");
	//		}
	//	}
	//}


	void StartFunctions()
	{
		if (autoConfigEnabled != 0) {
			InitSystem();

			if (LevelMap.find(0) != LevelMap.end()) {
				for (int i = 0; i < LevelMap[0].size(); i++) {
					if (LevelMap[0][i].console) {
						sprintf_s(cmdBuffer, cmdBufferSize, "%s", LevelMap[0][i].command.c_str());
					//	CSkyrimConsole::RunCommand(cmdBuffer);
						RE::Console::ExecuteCommand(cmdBuffer);
					} else if (LevelMap[0][i].enb == false && LevelMap[0][i].toggle == false) {
						RE::Setting* csetting = GetINISetting(LevelMap[0][i].variableName.c_str());
						if (csetting) {
							csetting->SetFloat(LevelMap[0][i].variableValue);
						}
					}
				}
				currentLevel = 0;
			} else {
				logger::critical("Cannot find Level 0 values...");
			}

		//	ToggleSwitch(0);

			if (AdjustDynamicResolution) {
				//SetEnableAutoDynamicResolution(true);
				SetRatioDecreasePerSeconds(0.07f);
				SetRatioIncreasePerSeconds(0.07f);
				SetLowestDynamicHeightRatio(MaxDynamicResolutionRatio);
				SetLowestDynamicWidthRatio(MaxDynamicResolutionRatio);
				//SetResolutionChangeInterval(0.01);
				//SetDecreaseDRMilliseconds(10);
				//SetIncreaseDRMilliseconds(10);
				//SetDRClampOffset(4.0f);
				//SetDRClampOffsetNeo(4.0f);
				currentFrameTimeDR = MaxTargetFrameTime;
				currentDRRatio = MaxDynamicResolutionRatio;
				logger::info("Setting dynamic resolution settings to their max.");
			} else {
				//SetEnableAutoDynamicResolution(false);
			}

			std::thread t1(CheckFrameTime);
			t1.detach();

			/*std::thread t2(ChangeFadeOutValues);
			t2.detach();*/
		}

		if (enableLog == 0) {
			logger::critical("Logging is disabled. Set EnableLog=1 in the ini to enable it.");
		}

		if (grassChange != 0 || LODinteriorchange != 0 || LODcitychange != 0 || conditionalCommandsParsed.size() > 0 || locationConfigEnabled != 0) {
			std::thread t3(ChangeFunction);
			t3.detach();
		}
	}

	std::vector<ConfigSetting>* GetSettingsListForLevel(int level)
	{
		return &LevelMap[level];
	}

	std::vector<ConfigSetting>* GetSettingsListForLocationChoice(LocationChoice locChoice)
	{
		return &LocationChoiceConfigMap[locChoice];
	}

	RE::NiPoint3 GetPlayerPosition()
	{
		RE::NiPoint3 pos;
		auto g_thePlayer = RE::PlayerCharacter::GetSingleton();
		if (g_thePlayer && g_thePlayer->loadedData) {
			return g_thePlayer->loadedData->data3D->world.translate;
		}
		return pos;
	}

	float GetPlayerHeadingAngle()
	{
		auto g_thePlayer = RE::PlayerCharacter::GetSingleton();
		float headingAngle = 0.0f;
		if (g_thePlayer && g_thePlayer->loadedData) {
			float heading = 0.0;
			float attitude = 0.0;
			float bank = 0.0;
			RE::NiMatrix3 playerrotation = g_thePlayer->loadedData->data3D->world.rotate;
			playerrotation.ToEulerAnglesXYZ(&heading, &attitude, &bank);

			float playerHeading = heading * (180 / MATH_PI);

			float playerAttitude = attitude * (180 / MATH_PI);

			if (playerHeading == -180) {
				headingAngle = 180 - playerAttitude;
			} else {
				if (playerAttitude < 0) {
					headingAngle = 360 + playerAttitude;
				} else {
					headingAngle = playerAttitude;
				}
			}
		}
		return headingAngle;
	}

	void SetShadowDistance(float shadowDist)
	{
//		SetShadowDistance_14134B520(shadowDist);
	}

	void SetInteriorShadowDistance(float shadowDist)
	{
		//if ((*g_isInterior)) {
		//	(*g_fShadowDistanceExterior_3) = shadowDist;
		//	SetShadowDistance_14134B520(shadowDist);
		//}
	}

	void SetShadowMapResolution(int resolution)
	{
		//(*g_iShadowMapResolution_1) = resolution;
		//(*g_iShadowMapResolution_2) = resolution;
	}

	void SetiMinGrassSize(float iminGrassSize)
	{
		//if ((*g_iMinGrassSize)) {
		//	(*g_iMinGrassSize) = (int)iminGrassSize;
			//CSkyrimConsole::RunCommand("tg");
			//CSkyrimConsole::RunCommand("tg");
	//	}
	}

	void SetEnableTreeAnimations(bool enable)
	{
		//*g_bEnableTreeAnimations = enable;
		//*(BYTE*)(g_qword_142FEC210 + 2) = enable;
	}

	void SetTreesReceiveShadows(bool enable)
	{
//		*g_bTreesReceiveShadows = enable;
	}

	void SetMaxSkinnedTrees(unsigned int count)
	{
//		*g_uiMaxSkinnedTreesToRender = count;
	}

	void SetEnableAutoDynamicResolution(bool enable)
	{
		//*g_bEnableAutoDynamicResolution = enable;
		//*g_bEnableAutoDynamicResolution_2 = enable ? 0 : 1;
	}

	void SetLowestDynamicWidthRatio(float ratio)
	{
//		*g_fLowestDynamicWidthRatio = ratio;
	}

	void SetLowestDynamicHeightRatio(float ratio)
	{
//		*g_fLowestDynamicHeightRatio = ratio;
	}

	void SetRatioIncreasePerSeconds(float ratio)
	{
//		*g_fRatioIncreasePerSeconds = ratio;
	}

	void SetRatioDecreasePerSeconds(float ratio)
	{
//		*g_fRatioDecreasePerSeconds = ratio;
	}

	void SetResolutionChangeInterval(unsigned int interval)
	{
//		*g_uiResolutionChangeInterval = interval;
	}

	void SetDecreaseDRMilliseconds(float amount)
	{
//		*g_fDecreaseDRMilliseconds = amount;
	}

	void SetIncreaseDRMilliseconds(float amount)
	{
//		*g_fIncreaseDRMilliseconds = amount;
	}

	void SetDRClampOffset(float amount)
	{
//		*g_fDRClampOffset = amount;
	}

	void SetDRClampOffsetNeo(float amount)
	{
//		*g_fDRClampOffsetNeo = amount;
	}

	void RunIniSetting(std::string variableName, double variableValue)
	{
		if (strcmp(variableName.c_str(), "fShadowDistance:Display") == 0) {
			SetShadowDistance(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "fInteriorShadowDistance:Display") == 0) {
			SetInteriorShadowDistance(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "iMinGrassSize:Grass") == 0) {
			SetiMinGrassSize(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "bEnableTreeAnimations:Trees") == 0) {
			SetEnableTreeAnimations(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "bTreesReceiveShadows:Display") == 0) {
			SetTreesReceiveShadows(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "uiMaxSkinnedTreesToRender:Trees") == 0) {
			SetMaxSkinnedTrees(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "iShadowMapResolution:Display") == 0) {
			SetShadowMapResolution(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "bEnableAutoDynamicResolution:Display") == 0) {
			SetEnableAutoDynamicResolution(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "fLowestDynamicWidthRatio:Display") == 0) {
			SetLowestDynamicWidthRatio(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else if (strcmp(variableName.c_str(), "fLowestDynamicHeightRatio:Display") == 0) {
			SetLowestDynamicHeightRatio(variableValue);
			logger::info("New setting (%s): %g", variableName.c_str(), variableValue);
		} else {
			RE::Setting* csetting = GetINISetting(variableName.c_str());
			if (csetting) {
				csetting->SetFloat(variableValue);

				double out = -1;

				out = csetting->GetFloat();
				if (csetting) {
					logger::info("New setting (%s): %g", variableName.c_str(), out);
				} else {
					logger::info("cannot set setting (%s)", variableName.c_str());
				}
			} else {
				logger::info("setting not found (%s)", variableName.c_str());
			}
		}
	}

	void SwitchToLevel(int level, double currentFrameTime)
	{
		currentLevel = level;
		waitingToGoUp = 0;
		waitingToGoDown = 0;

		lastHeadingAngle = GetPlayerHeadingAngle();

		std::vector<ConfigSetting>* SettingsList = GetSettingsListForLevel(level);

		if (SettingsList != nullptr) {
			for (int i = 0; i < SettingsList->size(); i++) {
				if (SettingsList->at(i).enb)
					continue;

				if (SettingsList->at(i).console) {
					sprintf_s(cmdBuffer, cmdBufferSize, "%s", SettingsList->at(i).command.c_str());
					//CSkyrimConsole::RunCommand(cmdBuffer);
					RE::Console::ExecuteCommand(cmdBuffer);
				} else if (SettingsList->at(i).enb == false && SettingsList->at(i).toggle == false) {
					RunIniSetting(SettingsList->at(i).variableName, SettingsList->at(i).variableValue);
				}
			}

			ToggleSwitch(level);

			currentLevelFrameTime = currentFrameTime;
			logger::info("Switching to level %d - Angle:%g", level, lastHeadingAngle);
		}
	}

	void SwitchDR(float newRatio, double frameTime)
	{
		currentDRRatio = newRatio;
		waitingToGoUpDR = 0;
		waitingToGoDownDR = 0;

		lastHeadingAngleDR = GetPlayerHeadingAngle();

		SetLowestDynamicWidthRatio(newRatio);
		SetLowestDynamicHeightRatio(newRatio);

		currentFrameTimeDR = frameTime;
		logger::info("Setting DR to %g - Angle:%g", newRatio, lastHeadingAngle);
	}

	void CheckFrameTime()
	{
		vr::Compositor_FrameTiming pFrameTimingArray[100];

		CurrentActorLODFade = (int)vlibGetSetting("fLODFadeOutMultActors:LOD");
		CurrentObjectLODFade = (int)vlibGetSetting("fLODFadeOutMultObjects:LOD");

		auto g_thePlayer = RE::PlayerCharacter::GetSingleton();
		while (true) {
		//	if (!isGameStopped()) {
			if (!isInMenu()) {
				if (ivrCompositor != nullptr) {
					vr::Compositor_FrameTiming pFrameTiming;
					vr::Compositor_FrameTiming* pFrameTimings = pFrameTimingArray;
					pFrameTiming.m_nSize = sizeof(vr::Compositor_FrameTiming);
					pFrameTimings[0].m_nSize = sizeof(vr::Compositor_FrameTiming);
					if (ivrCompositor != nullptr && ivrCompositor->GetFrameTiming(&pFrameTiming)) {
						//Checking cpu late starts
						bool cpuLateStart = false;
						int lateStartCount = 0;
						std::uint32_t filledCount = ivrCompositor->GetFrameTimings(pFrameTimings, CPULateStartCheckFrameCount);
						float lateStartAmount = 0;
						float averageLateStartAmount = 0;
						int modifierActors = 0;
						int modifierObjects = 0;

						RE::TESObjectCELL* cell = g_thePlayer->parentCell;

						//if (cell != nullptr && cell->worldSpace != nullptr && cell->worldSpace->parentWorld != nullptr && !(cell->cellFlags & 0x1)) {
						//	if (cell->worldSpace->formID == 0x16BB4)  //Riften city
						//	{
						//		modifierActors = RiftenLODFadeOutModifierActors;
						//		modifierObjects = RiftenLODFadeOutModifierObjects;
						//	} else if (cell->worldSpace->parentUseFlags)  //Other cities
						//	{
						//		modifierActors = OtherCitiesLODFadeOutModifierActors;
						//		modifierObjects = OtherCitiesLODFadeOutModifierObjects;
						//	}
						//}

						if (pFrameTimings != nullptr) {
							for (int i = 0; i < CPULateStartCheckFrameCount; i++) {
								if (pFrameTimings[i].m_flWaitGetPosesCalledMs > pFrameTimings[i].m_flNewPosesReadyMs + CPULateStartChangeThreshold) {
									lateStartAmount += pFrameTimings[i].m_flWaitGetPosesCalledMs - pFrameTimings[i].m_flNewPosesReadyMs;
									lateStartCount++;
								}
								averageLateStartAmount += pFrameTimings[i].m_flWaitGetPosesCalledMs - pFrameTimings[i].m_flNewPosesReadyMs;
							}
						}

						if (lateStartCount >= CPULateStartMinFrameCountToReduce) {
							lateStartAmount = lateStartAmount / lateStartCount;
							//_MESSAGE("CPU late start count:%d - avg amount: %g!!!", lateStartCount, lateStartAmount);
							cpuLateStart = true;
						} else {
							averageLateStartAmount = averageLateStartAmount / (float)CPULateStartCheckFrameCount;
							//_MESSAGE("CPU fineeeeeeeeeeeeeeee.");
						}

						//CPU stuff
						if (AdjustActorLODFade || AdjustObjectLODFade) {
							if (cpuLateStart) {
								waitingToGoDownLODFade++;
								if (waitingToGoDownLODFade > LODFadeOutValueDownWait) {
									waitingToGoDownLODFade = 0;
									waitingToGoUpLODFade = 0;
									//decrease lod fade values
									if (AdjustActorLODFade) {
										if (CurrentActorLODFade > MinfLODFadeOutMultActors + modifierActors) {
											CurrentActorLODFade--;
									//		(*g_fLODFadeOutMultActors) = (float)CurrentActorLODFade;
											logger::info("Late Start. Setting fLODFadeOutMultActors to %d", CurrentActorLODFade);
										}
									}
									if (AdjustObjectLODFade) {
										if (CurrentObjectLODFade > MinfLODFadeOutMultObjects + modifierObjects) {
											CurrentObjectLODFade--;
											//(*g_fLODFadeOutMultObjects_1) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_2) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_3) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_4) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_5) = (float)CurrentObjectLODFade;
											logger::info("Late Start. Setting fLODFadeOutMultObjects to %d", CurrentObjectLODFade);
										}
									}
									lastCpuLateStartHeadingAngle = GetPlayerHeadingAngle();
									lastCpuLateStartPosition = GetPlayerPosition();
								}
								//_MESSAGE("Late Start-> Amount: %g - LatestAmount: %g - CurObjLODFade:%d - CurActorLODFade:%d - Waiting: %d", lateStartAmount, pFrameTiming.m_flWaitGetPosesCalledMs - pFrameTiming.m_flNewPosesReadyMs, CurrentObjectLODFade, CurrentActorLODFade, 10-waitingToGoUpLODFade);
							} else if (lateStartCount == CPULateStartMaxFrameCountToIncrease && averageLateStartAmount < -CPULateStartChangeThreshold && (GetAngleDifference(GetPlayerHeadingAngle(), lastCpuLateStartHeadingAngle) > static_cast<float>(cpuLateStartAngleToSwitchBackUp) && distanceNoSqrt(GetPlayerPosition(), lastCpuLateStartPosition) > cpuLateStartDistanceSqrSwitchBackUp)) {
								waitingToGoUpLODFade++;
								if (waitingToGoUpLODFade > LODFadeOutValueUpWait) {
									waitingToGoUpLODFade = 0;
									//check to increase back
									if (AdjustActorLODFade) {
										if (CurrentActorLODFade < MaxfLODFadeOutMultActors + modifierActors) {
											CurrentActorLODFade++;
									//		(*g_fLODFadeOutMultActors) = (float)CurrentActorLODFade;
											logger::info("No Late Start. Setting fLODFadeOutMultActors to %d", CurrentActorLODFade);
										}
									}
									if (AdjustObjectLODFade) {
										if (CurrentObjectLODFade < MaxfLODFadeOutMultObjects + modifierObjects) {
											CurrentObjectLODFade++;
											//(*g_fLODFadeOutMultObjects_1) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_2) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_3) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_4) = (float)CurrentObjectLODFade;
											//(*g_fLODFadeOutMultObjects_5) = (float)CurrentObjectLODFade;
											logger::info("No Late Start. Setting fLODFadeOutMultObjects to %d", CurrentObjectLODFade);
										}
									}
								}
								//_MESSAGE("No Late Start-> averageLateStartAmount: %g - CurObjLODFade:%d - CurActorLODFade:%d - Waiting: %d", averageLateStartAmount, CurrentObjectLODFade, CurrentActorLODFade, LODFadeOutValueReduceUpWait -waitingToGoUpLODFade);
							}
						}

						//Dynamic Resolution Stuff
						//if (AdjustDynamicResolution)
						//{
						//	//LOG("Current DR: %g - %g", *g_fDRCurrentWidth, *g_fDRCurrentHeight);
						//	if (pFrameTiming.m_flTotalRenderGpuMs < MinTargetFrameTime)
						//	{
						//		if (pFrameTiming.m_flTotalRenderGpuMs < currentFrameTimeDR - frameTimeChangeThreshold)
						//		{
						//			waitingToGoDownDR++;
						//			if (currentHighPerfFrameCountDR < frameCheckCountDR)
						//			{
						//				currentHighPerfFrameCountDR++;
						//			}
						//			else
						//			{
						//				currentHighPerfFrameCountDR = 0;
						//				currentLowPerfFrameCountDR = 0;

						//				if (currentDRRatio > MinDynamicResolutionRatio)
						//				{
						//					if (!lastDirectionPositiveDR || (waitingToGoDownDR >= waitTurnsToSwitchBackDown && GetAngleDifference(GetPlayerHeadingAngle(), lastHeadingAngleDR) > static_cast<float>(angleToSwitchBackUp)))
						//					{
						//						lastDirectionPositiveDR = false;
						//						SwitchDR(currentDRRatio - RatioChangeStep, pFrameTiming.m_flTotalRenderGpuMs);
						//						lastHeadingAngle = GetPlayerHeadingAngle();
						//					}
						//				}
						//			}
						//		}
						//	}
						//	else if (pFrameTiming.m_flTotalRenderGpuMs > MaxTargetFrameTime)
						//	{
						//		waitingToGoUpDR++;
						//		if (currentLowPerfFrameCountDR < frameCheckCountDR)
						//		{
						//			currentLowPerfFrameCountDR++;
						//		}
						//		else
						//		{
						//			currentLowPerfFrameCountDR = 0;
						//			currentHighPerfFrameCountDR = 0;

						//			if (currentDRRatio < MaxDynamicResolutionRatio)
						//			{
						//				if (lastDirectionPositiveDR || (waitingToGoUpDR >= waitTurnsToSwitchBackUp))
						//				{
						//					lastDirectionPositiveDR = true;
						//					SwitchDR(currentDRRatio + RatioChangeStep, pFrameTiming.m_flTotalRenderGpuMs);
						//				}
						//			}
						//		}
						//	}
						//}

						//GPU stuff
						if (pFrameTiming.m_flTotalRenderGpuMs < MinTargetFrameTime && (!cpuLateStart || CheckCPUToo == 0)) {
							if (pFrameTiming.m_flTotalRenderGpuMs < currentLevelFrameTime - frameTimeChangeThreshold && (!cpuLateStart || CheckCPUToo == 0)) {
								waitingToGoDown++;
								if (currentHighPerfFrameCount < frameCheckCount) {
									currentHighPerfFrameCount++;
								} else {
									currentHighPerfFrameCount = 0;
									currentLowPerfFrameCount = 0;
									//_MESSAGE("Last direction: %s - waiting to go down: %d", lastDirectionPositive ? "Positive" : "Negative", waitingToGoDown);

									if (currentLevel > 0) {
										if (!lastDirectionPositive || (waitingToGoDown >= waitTurnsToSwitchBackDown && GetAngleDifference(GetPlayerHeadingAngle(), lastHeadingAngle) > static_cast<float>(angleToSwitchBackUp))) {
											lastDirectionPositive = false;
											SwitchToLevel(currentLevel - 1, pFrameTiming.m_flTotalRenderGpuMs);
										}
									}
								}
							}
						} else if (pFrameTiming.m_flTotalRenderGpuMs > MaxTargetFrameTime || (cpuLateStart && CheckCPUToo == 1)) {
							waitingToGoUp++;
							if (currentLowPerfFrameCount < frameCheckCount) {
								currentLowPerfFrameCount++;
							} else {
								currentLowPerfFrameCount = 0;
								currentHighPerfFrameCount = 0;
								//_MESSAGE("Last direction: %s - waiting to go up: %d", lastDirectionPositive ? "Positive" : "Negative", waitingToGoUp);

								if (currentLevel < maxLevel) {
									if (lastDirectionPositive || (waitingToGoUp >= waitTurnsToSwitchBackUp)) {
										lastDirectionPositive = true;
										SwitchToLevel(currentLevel + 1, pFrameTiming.m_flTotalRenderGpuMs);
									}
								}
							}
						}
					}
				} else {
					logger::critical("Cannot access VR Compositor. Are you sure you are using the right version?");
					InitSystem();
				}
				Sleep(checkSleepDuration);
			} else {
				Sleep(1000);
			}
		}
	}

	bool ActorInCombat(RE::Actor* actor)
	{
		const bool combat = actor->IsInCombat();
		return combat;
	}


}
