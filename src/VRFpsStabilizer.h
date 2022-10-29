#pragma once


#include "RE/Bethesda/TESForms.h"
#include "RE/Bethesda/Actor.h"

#include "openvr.h"
#include <iostream>
#include <string>
#include <fstream>
#include <thread>

#include <Windows.h>

#define MATH_PI 3.14159265358979323846

namespace VRFpsStabilizer
{
#define TOLERANCE 0.00001f

	enum LocationChoice
	{
		Default,
		Best,
		Good,
		Bad,
		Worst,
		None
	};

	extern int autoConfigEnabled;

	extern int checkSleepDuration;

	extern int frameCheckCount;

	extern float MinTargetFrameTime;
	extern float MaxTargetFrameTime;

	extern float frameTimeChangeThreshold;

	extern int waitTurnsToSwitchBackUp;
	extern int waitTurnsToSwitchBackDown;

	extern int angleToSwitchBackUp;

	extern int currentLevel;

	extern LocationChoice lastLocChoice;

	extern std::vector<std::string> dataLoadedCommands;
	extern std::vector<std::string> postLoadGameCommands;

	void loadLocationConfig();
	void loadConfig();

	void GameLoad();

	void RunDataLoadedCommands();
	void RunPostLoadGameCommands();
	void RunNewGameCommands();

	float GetPlayerHeadingAngle();

	void SetShadowDistance(float shadowDist);

	void SetInteriorShadowDistance(float shadowDist);

	void SetShadowMapResolution(int resolution);

	void SetiMinGrassSize(float iminGrassSize);

	void SetEnableTreeAnimations(bool enable);

	void SetMaxSkinnedTrees(unsigned int count);

	void SetEnableAutoDynamicResolution(bool enable);

	void SetLowestDynamicWidthRatio(float ratio);

	void SetLowestDynamicHeightRatio(float ratio);

	void SetRatioIncreasePerSeconds(float ratio);

	void SetRatioDecreasePerSeconds(float ratio);

	void SetRatioDecreasePerSeconds(float ratio);

	void SetResolutionChangeInterval(unsigned int interval);

	void SetDecreaseDRMilliseconds(float amount);

	void SetIncreaseDRMilliseconds(float amount);

	void SetDRClampOffset(float amount);

	void SetDRClampOffsetNeo(float amount);

	void RunIniSetting(std::string variableName, double variableValue);

	void ResetIniSettings();
	void StartFunctions();

	void CheckFrameTime();

	struct ConfigSetting
	{
		bool enb = false;
		bool console = false;
		bool toggle = false;

		std::string variableName;
		double variableValue;

		//enb related
		std::string filename;
		std::string category;
		std::string keyname;
		float valueFLOAT;
		int valueINT;
		bool valueBOOL;

		//console related
		std::string command;
	};

	std::vector<ConfigSetting>* GetSettingsListForLevel(int level);
	std::vector<ConfigSetting>* GetSettingsListForLocationChoice(LocationChoice locChoice);

	/*
	void SwitchToLevel(int level, double currentFrameTime);
	void SwitchToLocationSettings(LocationChoice locChoice, BGSLocation* curLoc);

	typedef uintptr_t(__fastcall* _ctor_1400B46A0)(uintptr_t a1_r8);
	extern RelocAddr<_ctor_1400B46A0> ctor_1401B46A0;

	typedef uintptr_t(__fastcall* _ctor_140154A80)(uintptr_t a1_r8);
	extern RelocAddr<_ctor_140154A80> ctor_140155A80;

	typedef uintptr_t(__fastcall* _SetGrassEnabled_1400B69B0)(uintptr_t a1_r8, bool a2_r1);
	extern RelocAddr<_SetGrassEnabled_1400B69B0> SetGrassEnabled_1401B69B0;

	typedef uintptr_t(__fastcall* _SetShadowDistance_14133B520)(float a1_r4);
	extern RelocAddr<_SetShadowDistance_14133B520> SetShadowDistance_14134B520;
*/

	struct ConditionalCommand
	{
		bool isInterior;
		float startTime;
		float endTime;
		std::string command;

		bool isImod = false;
		RE::TESImageSpaceModifier* imod;
		float strength;
		bool applyImod = true;

		bool InCombat = false;
		bool OutOfCombat = false;
	};

	//class taskApplyImod : public TaskDelegate
	//{
	//public:
	//	virtual void Run();
	//	virtual void Dispose();

	//	taskApplyImod(TESImageSpaceModifier* imod, float strength);
	//	TESImageSpaceModifier* m_imod;
	//	float m_strength;
	//};

	//class taskRemoveImod : public TaskDelegate
	//{
	//public:
	//	virtual void Run();
	//	virtual void Dispose();

	//	taskRemoveImod(TESImageSpaceModifier* imod);
	//	TESImageSpaceModifier* m_imod;
	//};

	//typedef void (*_ApplyImageSpaceModifier)(VMClassRegistry* registry, UInt32 stackId, TESImageSpaceModifier* imod, float afStrength);
	//extern RelocAddr<_ApplyImageSpaceModifier> ApplyImageSpaceModifier;

	//typedef void (*_RemoveImageSpaceModifier)(VMClassRegistry* registry, UInt32 stackId, TESImageSpaceModifier* imod);
	//extern RelocAddr<_RemoveImageSpaceModifier> RemoveImageSpaceModifier;

	bool ActorInCombat(RE::Actor* actor);

	typedef bool (*_IsInCombatNative)(RE::Actor* actor);

}
