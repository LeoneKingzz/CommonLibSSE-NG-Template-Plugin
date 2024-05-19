// #pragma once
// #include <SimpleIni.h>
// #include <nlohmann/json.hpp>
// using json = nlohmann::json;





// class Settings
// {
// public:
//     [[nodiscard]] static Settings *GetSingleton()
//     {
//         static Settings singleton;
//         return &singleton;
//     }

//     json JSONSettings;

   





    
//     void LoadGameSettings();
//     void LoadINI(const wchar_t *a_path);
//     void LoadJSON(const wchar_t *a_path);
//     void LoadSettings();

// private:
//     Settings() = default;
//     Settings(const Settings &) = delete;
//     Settings(Settings &&) = delete;

//     ~Settings() = default;

//     Settings &operator=(const Settings &) = delete;
//     Settings &operator=(Settings &&) = delete;
// };