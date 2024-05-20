#pragma once

class Settings
{
public:
    [[nodiscard]] static Settings *GetSingleton()
    {
        static Settings singleton;
        return &singleton;
    }

    static void SetAttackData(RE::BGSAttackData *attackData, attackdata &data);
    static void InsertAttackDataRecords(RE::TESRace* race, event_map &events);

    // static void InsertAttackDataRecords(RE::TESRace *race, event_map &events);

   

    // json JSONSettings;
    // void LoadGameSettings();
    // void LoadINI(const wchar_t *a_path);
    // void LoadJSON(const wchar_t *a_path);
    // void LoadSettings();

private:
    Settings() = default;
    Settings(const Settings &) = delete;
    Settings(Settings &&) = delete;

    ~Settings() = default;

    Settings &operator=(const Settings &) = delete;
    Settings &operator=(Settings &&) = delete;
};