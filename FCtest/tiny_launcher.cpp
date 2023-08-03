#include "tiny_launcher.h"

namespace FCtest {

    bool TinyLauncher::isExecutable(const std::wstring& filename) {
        size_t dotPos = filename.find_last_of('.');
        if (dotPos == std::string::npos) {
            return false;
        }
        std::wstring extension = filename.substr(dotPos + 1);
        for (auto& c : extension) {
            c = std::tolower(c);
        }
        return (extension == L"exe");
    }

    void TinyLauncher::launch(const std::wstring& filename, const std::wstring& filepath, const std::wstring& parameters) {
        if (isExecutable(filename)) {
            if (reinterpret_cast<int>(ShellExecuteW(nullptr, L"open", (filepath + filename).c_str(), parameters.c_str(), nullptr, SW_SHOWNORMAL)) > 32) {
                std::cout << "File executed successfully." << "\n";
            }
            else {
                throw std::runtime_error("execution of file failed.");
            }
        }
        else {
            throw std::runtime_error("file is not executable.");
        }
    }

} // namespace FCtest