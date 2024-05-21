#include <shlobj.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
#include <filesystem>
#include "picojson\picojson.h"
#include "settings.h"



// truncated name warning
#pragma warning(disable: 4503)

typedef std::vector<std::pair<std::string, picojson::value>> attackdata;
typedef std::map<std::string, attackdata> event_map;
typedef std::map<std::string, event_map> setting_map;

static setting_map s_setting;


inline static void Chomp(std::string & str, int c = '\r')
{
	std::size_t len = str.length();
	if (len > 0 && str[len - 1] == c)
	{
		str.resize(len - 1);
	}
}


inline static void ToLower(std::string & str)
{
	for (auto &n : str)
	{
		n = tolower(n);
	}
}


static std::string ToString(uint32_t id)
{
	auto dhnd = RE::TESDataHandler::GetSingleton();
	uint8_t modIndex = id >> 24;
	if (modIndex >= dhnd->modList.loadedMods.count)
		return std::string("XXXXXX");

	std::stringstream ss;

	if (modIndex) {
		std::string modName = dhnd->modList.loadedMods[modIndex]->name;
		ToLower(modName);
		ss << modName << " ";
	}

	for (uint32_t i = 0; i < dhnd->modList.loadedMods.count; ++i) {
	}

	ss << std::hex << std::setfill('0') << std::setw(6) << (id & 0x00FFFFFF);
	return ss.str();
}


static bool ParseJson(std::string jsonFileName, picojson::value &val)
{
	std::stringstream ss;
	std::ifstream fileStream(jsonFileName);

	if (!fileStream)
	{
		return false;
	}

	ss << fileStream.rdbuf();

	std::string err = picojson::parse(val, ss);
	if (!err.empty())
	{
		return false;
	}
	if (!val.is<picojson::object>())
	{
		return false;
	}

	return true;
}


static void RemapJson(picojson::value &json, setting_map &setting)
{
	if (!json.is<picojson::object>())
		return;

	for (auto &kv : json.get<picojson::object>())
	{
		picojson::value &v = kv.second;
		if (!v.is<picojson::object>())
			continue;

		std::string key = kv.first;
		if (key.empty())
			continue;
		ToLower(key);

		picojson::object &events_in = v.get<picojson::object>();
		event_map &events_out = setting[key];

		for (auto &event : events_in)
		{
			picojson::value &value = event.second;
			if (!value.is<picojson::object>())
				continue;

			std::string eventName = event.first;
			if (eventName.empty())
				continue;

			picojson::object &attackdata_in = value.get<picojson::object>();
			attackdata &attackdata_out = events_out[eventName];
			attackdata_out.reserve(attackdata_in.size());

			for (auto &data : attackdata_in)
			{
				std::string name = data.first;
				if (name.empty())
					continue;
				ToLower(name);

				attackdata_out.push_back(std::make_pair(name, data.second));
			}
		}
	}
}


void Settings::SetAttackData(RE::BGSAttackData* attackData, attackdata &data)
{
	for (auto &val : data)
	{
		std::string &valName = val.first;

		if (valName == "damagemult") {
			attackData->data.attackChance = val.second.get<double>();
		}
		else if (valName == "attackchance") {
			attackData->data.attackChance = val.second.get<double>();
		}
		else if (valName == "attackangle") {
			attackData->data.attackAngle = val.second.get<double>();
		}
		else if (valName == "strikeangle") {
			attackData->data.strikeAngle = val.second.get<double>();
		}
		else if (valName == "stagger") {
			attackData->data.staggerOffset = val.second.get<double>();
		}
		else if (valName == "knockdown") {
			attackData->data.knockDown = val.second.get<double>();
		}
		else if (valName == "recoverytime") {
			attackData->data.recoveryTime = val.second.get<double>();
		}
		else if (valName == "staminamult") {
			attackData->data.staminaMult = val.second.get<double>();
		}
		else if (valName == "ignoreweapon") {
			attackData->data.flags.get()<RE::AttackData::AttackFlag::kIgnoreWeapon>val.second.get<bool>();
		}
		else if (valName == "bashattack") {
			attackData->data.flags.get()<RE::AttackData::AttackFlag::kBashAttack>val.second.get<bool>();
		}
		else if (valName == "powerattack") {
			attackData->data.flags.get()<RE::AttackData::AttackFlag::kPowerAttack>val.second.get<bool>();
		}
		else if (valName == "leftattack") {
			attackData->data.flags.get()<RE::AttackData::AttackFlag::kChargeAttack>val.second.get<bool>();
		}
		else if (valName == "rotatingattack") {
			attackData->data.flags.get()<RE::AttackData::AttackFlag::kRotatingAttack>val.second.get<bool>();
		}
		else {
			
		}
	}
}


void Settings::InsertAttackDataRecords(RE::TESRace* race, event_map &events)
{
	for (auto &ev : events)
	{
		RE::BSFixedString eventName(ev.first.c_str());

		auto adm = (RE::BGSAttackDataMap*)race->attackDataMap.get();
		auto attackData = (RE::BGSAttackData*)adm->attackDataMap.contains(eventName);

		if (!attackData) {
			RE::BGSAttackData *ptr = attackData;
			RE::BSTSmartPointerIntrusiveRefCount<RE::BGSAttackData>::Acquire(ptr);
			ptr->event = eventName;
			eventName.c_str
			// adm.insert(eventName, ptr);

			// RE::BSTSmartPointerIntrusiveRefCount::IncRef<>(ptr)
			// adm.insert(attackData->eventName, ptr)
		}

		AttackAnimationArrayMap

		// if (!attackData) {
		// 	attackData = adm->attackDataMap.
		// }

		// if (attackData) {
		// 	SetAttackData(attackData, ev.second);
		// }

		// RE::BGSAttackDataPtr

		// RE::BGSAttackDataMap *adm = race->faceRelatedData<RE::TESRace::FaceRelatedData::Morph::unk08>->As<RE::BGSAttackData>();

		// RE::BSGAttackData *attackData = adm->Get(eventName);

		// if (!attackData)
		// {
		// 	attackData = adm->Add(eventName);
		// }

		// if (attackData)
		// {
		// 	SetAttackData(attackData, ev.second);
		// }
	}
}

static void InitAttackDataJson()
{
	std::string baseDir("Meshes\\AttackData");

	// read json files
    std::filesystem::path p(std::string("Data\\") + baseDir);
	std::for_each(std::filesystem::directory_iterator(p), std::filesystem::directory_iterator(),
		[&baseDir](const std::filesystem::path& p)
	{
		if (!std::filesystem::is_regular_file(p))
			return;

		std::string ext = p.extension().string();
		if (_stricmp(ext.c_str(), ".json") != 0)
			return;

		std::string fileName = "Data\\" + baseDir + "\\" + p.filename().string();

		
		// read json
		picojson::value val;
		if (ParseJson(fileName, val))
		{
			RemapJson(val, s_setting);
		}
	});
}

typedef bool(*_TESRaceLoadForm)(RE::TESRace * race, int64_t unk1);
REL::Relocation<_TESRaceLoadForm> TESRaceOriginalLoadForm(0x00384BC0);
REL::Relocation<uintptr_t> vtbl_TESRaceLoadForm(0x015A30E8); // vtable[6]

bool HookedLoadForm(RE::TESRace * race, int64_t unk1)
{

	bool result = TESRaceOriginalLoadForm(race, unk1);

	if (result)
	{
		_MESSAGE("successfully loaded race with formid %08X and name %s", race->formID, race->fullName.GetName());
	}

	if (!race->attackData.unk08.ptr)
	{
		_MESSAGE("race has no attack data, returning");
		return result;
	}

	RE::BGSAttackDataMap * adm = (RE::BGSAttackDataMap *)race->attackData.unk08.ptr;



	std::string keys[3];

	keys[0] = race->behaviorGraph[0].GetModelName();
	if (race->editorId)
	{

		keys[1] = race->editorId;
	}
	keys[2] = ToString(race->formID);

	for (std::string &key : keys)
	{
		if (key.empty())
			continue;

		ToLower(key);

		auto it = s_setting.find(key);
		if (it != s_setting.end())
		{
			Settings::InsertAttackDataRecords(race, it->second);
		}
	}
	


	return result;
}



extern "C" {
	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\InsertAttackData64.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("InsertAttackData64 Plugin");

		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = "InsertAttackData64 plugin";
		info->version = 1;

		if (skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");
			return false;
		}
		else if (skse->runtimeVersion != RUNTIME_VERSION_1_5_73)
		{
			_MESSAGE("unsupported runtime version %08X", skse->runtimeVersion);
			return false;
		}

		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse) {
		_MESSAGE("Hooking TESRace::LoadForm");
		SafeWrite64(vtbl_TESRaceLoadForm.GetUIntPtr(), uintptr_t(HookedLoadForm));
		_MESSAGE("Patched");

		_MESSAGE("Reading attack data json");
		InitAttackDataJson();
		_MESSAGE("Done");
		return true;
	}
};

RE::BGSAttackData *attackData = RE::NiTMallocInterface::Allocate<RE::BGSAttackData>();

// == RE::AttackData::AttackFlag::kIgnoreWeapon);

RE::BGSAttackData *RE::BGSAttackDataMap::Get(const RE::BSFixedString &eventName)
{
	auto iter = map.find(eventName);
	return (iter != map.end()) ? iter->value : nullptr;
}

RE::BGSAttackData *RE::BGSAttackDataMap::Add(const RE::BSFixedString &eventName)
{
	RE::BSFixedString evName(eventName.c_str()); // refcount+1

	

	RE::BGSAttackData *ptr = RE::BGSAttackData::Create();
	ptr->IncRef();
	if (ptr)
	{
		ptr->eventName = evName;
		map.insert(evName, ptr);
	}

	return ptr;
}

RE::BGSAttackData *RE::BGSAttackDataMap::Add(RE::BGSAttackData *attackData)
{
	if (attackData)
	{
		RE::BGSAttackData *ptr = attackData;

		map.insert(attackData->eventName, ptr);
	}

	return attackData;
}