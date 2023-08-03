#include "tiny_web_loader.h"
//std
#include <iostream>
#include <vector>

namespace FCtest {

    TinyWebLoader::TinyWebLoader() {
        hInternet = InternetOpen(L"FCtest", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) {
            throw std::runtime_error("Failed to create internet handle");
        }
    }

    void TinyWebLoader::load(LPCWSTR url, std::wstring& filename, std::wstring& filepath) {
        HINTERNET hUrl = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_RELOAD, 0);
        if (!hUrl) {
            throw std::runtime_error("Failed to open url");
        }

        std::ofstream file(filepath + filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("failed to open file for writing");
            InternetCloseHandle(hUrl);
        }

        std::vector<char> buffer(1024);
        DWORD bytesRead = 0;
        std::cout << "Downloading file..." << "\n";
        while (InternetReadFile(hUrl, buffer.data(), sizeof(buffer), &bytesRead) && bytesRead > 0) {
            file.write(buffer.data(), bytesRead);
        }

        file.close();
        InternetCloseHandle(hUrl);
        std::cout << "File downloaded successfully." << "\n";
    }

    TinyWebLoader::~TinyWebLoader() {
        InternetCloseHandle(hInternet);
    }

} // namespace FCtest