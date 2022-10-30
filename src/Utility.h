#pragma once

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <random>
#include <iterator>

#include "dirent.h"

#include "RE/Bethesda/Settings.h"
#include "RE/NetImmerse/NiPoint3.h"



template <typename Iter, typename RandomGenerator>
Iter select_randomly(Iter start, Iter end, RandomGenerator& g)
{
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template <typename Iter>
Iter select_randomly(Iter start, Iter end)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return select_randomly(start, end, gen);
}

// trim from start (in place)
static inline void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(),
		s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s)
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s)
{
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s)
{
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s)
{
	trim(s);
	return s;
}

static inline std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	return tokens;
}

static inline std::vector<std::string> splitTrimmed(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		trim(token);
		if (token != "") {
			tokens.push_back(token);
		}
	}
	return tokens;
}

static inline std::string gettrimmed(const std::string& s)
{
	std::string temp = s;
	if (temp.front() == '"' && temp.back() == '"') {
		temp.erase(0, 1);
		temp.pop_back();
	}
	return temp;
}

static inline void skipComments(std::string& str)
{
	auto pos = str.find("#");
	if (pos != std::string::npos) {
		str.erase(pos);
	}
}

static inline void skipTactExtension(std::string& str)
{
	auto pos = str.find(".tact");
	if (pos != std::string::npos) {
		str.erase(pos);
	}
}

static inline std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	std::vector<std::string> names;

	DIR* directory = opendir(folder.c_str());
	struct dirent* direntStruct;

	if (directory != NULL) {
		while (direntStruct = readdir(directory)) {
			names.emplace_back(direntStruct->d_name);
		}
	}
	closedir(directory);

	return names;
}

static inline bool stringStartsWith(std::string str, std::string prefix)
{
	// std::string::find returns 0 if toMatch is found at starting
	if (str.find(prefix) == 0)
		return true;
	else
		return false;
}

static inline float randf(float lo, float hi)
{
	if (hi == 0)
		return 0;

	srand(time(NULL));
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = hi - lo;
	float r = random * diff;
	return lo + r;
}

static inline size_t randomGenerator(size_t min, size_t max)
{
	std::mt19937 rng;
	rng.seed(std::random_device()());
	//rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);

	return dist(rng);
}

static inline int randi(int lo, int hi)
{
	if (hi == 0)
		return 0;

	srand(time(NULL));
	int n = hi - lo + 1;
	int i = rand() % n;
	if (i < 0)
		i = -i;
	return lo + i;
}

static inline bool Contains(std::string str, std::string ministr)
{
	if (str.find(ministr) != std::string::npos) {
		return true;
	} else
		return false;
}

static std::string int_to_hex(std::uint8_t i)
{
	std::stringstream stream;
	stream << "0x"
		   << std::setfill('0') << std::setw(sizeof(std::uint8_t) * 2)
		   << std::hex << i;
	return stream.str();
}

static inline unsigned int stoui(const std::string& s)
{
	unsigned long lresult = stoul(s, 0, 10);
	unsigned int result = lresult;
	return result;
}

static inline int GetConfigSettingsValue(std::string line, std::string& variable)
{
	int value = 0;
	std::vector<std::string> splittedLine = split(line, '=');
	variable = "";
	if (splittedLine.size() > 1) {
		variable = splittedLine[0];
		trim(variable);

		std::string valuestr = splittedLine[1];
		trim(valuestr);
		value = std::stoi(valuestr);
	}

	return value;
}

static inline float GetConfigSettingsFloatValue(std::string line, std::string& variable)
{
	float value = 0;
	std::vector<std::string> splittedLine = split(line, '=');
	variable = "";
	if (splittedLine.size() > 1) {
		variable = splittedLine[0];
		trim(variable);

		std::string valuestr = splittedLine[1];
		trim(valuestr);
		value = strtof(valuestr.c_str(), 0);
	}

	return value;
}

static inline std::string GetConfigSettingsStringValue(std::string line, std::string& variable)
{
	std::string value = "";
	std::vector<std::string> splittedLine = split(line, '=');
	variable = "";
	if (splittedLine.size() > 0) {
		variable = splittedLine[0];
		trim(variable);

		if (splittedLine.size() > 1) {
			std::string valuestr = splittedLine[1];
			trim(valuestr);
			value = valuestr;
		}
	}

	return value;
}

static inline double GetConfigSettingsDoubleValue(std::string line, std::string& variable)
{
	double value = 0;
	std::vector<std::string> splittedLine = split(line, '=');
	variable = "";
	if (splittedLine.size() > 1) {
		variable = splittedLine[0];
		trim(variable);

		std::string valuestr = splittedLine[1];
		trim(valuestr);
		value = std::stod(valuestr.c_str());
	}

	return value;
}

static inline float GetAngleDifference(float angle1, float angle2)
{
	if ((angle1 < 90 && angle2 > 270)) {
		return (360 - angle2) + angle1;
	} else if (angle2 < 90 && angle1 > 270) {
		return (360 - angle1) + angle2;
	} else {
		return abs(angle1 - angle2);
	}
}

// check if mod index is valid (mod index is the upper 8 bits of form ID)
static inline bool IsValidModIndex(std::uint32_t modIndex)
{
	return modIndex > 0 && modIndex != 0xFF;
}

// get mod index from a normal form ID 32 bit unsigned
static inline std::uint32_t GetModIndex(std::uint32_t formId)
{
	return formId >> 24;
}

// get base formID (without mod index)
static inline std::uint32_t GetBaseFormID(std::uint32_t formId)
{
	return formId & 0x00FFFFFF;
}

//// Converts the std::uint32_t bits of a FormID to a full FormID depending on plugin type
//static inline std::uint32_t GetFullFormID(const ModInfo* modInfo, std::uint32_t formLower)
//{
//#ifdef RUNTIME_VR_VERSION_1_4_15
//	return (modInfo->modIndex << 24) | formLower;
//#elif RUNTIME_VERSION_1_5_97 || RUNTIME_VERSION_1_5_80 || RUNTIME_VERSION_1_5_73
//	return !modInfo->IsLight() ? UInt32(modInfo->modIndex) << 24 | (formLower & 0xFFFFFF) : 0xFE000000 | (UInt32(modInfo->lightIndex) << 12) | (formLower & 0xFFF);
//#elif RUNTIME_VERSION_1_5_62
//	return !(UInt32(modInfo->lightIndex) > 0) ? UInt32(modInfo->modIndex) << 24 | (formLower & 0xFFFFFF) : 0xFE000000 | (UInt32(modInfo->lightIndex) << 12) | (formLower & 0xFFF);
//#else
//	return !(std::uint32_t(modInfo->pad47C[1]) > 0) ? std::uint32_t(modInfo->modIndex) << 24 | (formLower & 0xFFFFFF) : 0xFE000000 | (std::uint32_t(modInfo->pad47C[1]) << 12) | (formLower & 0xFFF);
//#endif
//}

//Erase all Occurrences of given substring from main string.
static inline std::string eraseAllSubStr(std::string mainStr, const std::string& toErase)
{
	std::string newStr = mainStr;
	size_t pos = std::string::npos;
	// Search for the substring in string in a loop until nothing is found
	while ((pos = newStr.find(toErase)) != std::string::npos) {
		// If found then erase it from string
		newStr.erase(pos, toErase.length());
	}
	return newStr;
}

static inline RE::Setting* GetINISetting(const char* name) {
	RE::Setting* setting = RE::INISettingCollection::GetSingleton()->GetPtr(name);
	if (!setting)
		setting = RE::INIPrefSettingCollection::GetSingleton()->GetPtr(name);

	return setting;
}


// Names should be the full INI setting name followed by a colon and it's category (Prog's code)
// For example: bAlwaysShowHands:VR
static inline float vlibGetSetting(const char* name)
{
	RE::Setting* setting = GetINISetting(name);
	float value;
	if (!setting)
		return -1;
	value = setting->GetFloat();
	if (value) {
		return value;
	}
	return -1;
}

static inline float distanceNoSqrt(RE::NiPoint3 po1, RE::NiPoint3 po2)
{
	float x = po1.x - po2.x;
	float y = po1.y - po2.y;
	float z = po1.z - po2.z;
	return x * x + y * y + z * z;
}

static inline std::uint32_t getHex(std::string hexstr)
{
	return (std::uint32_t)strtoul(hexstr.c_str(), 0, 16);
}

template <typename I>
static inline std::string num2hex(I w, size_t hex_len = sizeof(I) << 1)
{
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');
	for (size_t i = 0, j = (hex_len - 1) * 4; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0f];
	return rc;
}
