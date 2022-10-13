#pragma once
#define NOMINMAX

#include <filesystem>
#include <windows.h>
#include <iostream>
#include <string>
#include <optional>


namespace ControllerMapper
{
    namespace Util
    {
        // Attempts to find game path through registry search 
        inline std::optional<std::string> GetGamePath() {
            const std::string gameExe = "Cyberpunk2077.exe";
            HKEY hKey = nullptr;
            if (RegOpenKeyExA(HKEY_CURRENT_USER,
                std::string(R"(Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\MuiCache)").c_str(),
                0, KEY_READ, &hKey) == ERROR_SUCCESS) {

                auto* valueNamePtr = new WCHAR[16383];
                DWORD valueNameSizePtr = 16383;
                DWORD i = 0;

                while (true) {
                    valueNamePtr[0] = '\0';
                    valueNameSizePtr = 16383;

                    if (RegEnumValueW(hKey, i++, valueNamePtr, &valueNameSizePtr, nullptr, nullptr, nullptr, nullptr) != ERROR_SUCCESS)
                    {
                      RegCloseKey(hKey);
                      break;
                    }

                    std::filesystem::path path(valueNamePtr);

                    if (path.filename().string().find(gameExe) != std::string::npos && std::filesystem::exists(path.remove_filename().append(gameExe))) {
                        RegCloseKey(hKey);
                        delete[] valueNamePtr;
                        return path.parent_path().parent_path().parent_path().string();
                    }
                }
                delete[] valueNamePtr;
            }
            return std::nullopt;
        }
    }
}