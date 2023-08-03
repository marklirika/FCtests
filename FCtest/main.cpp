#pragma comment(lib, "Wininet.lib")
#include "application.h"

int main(int argc, char* argv[]) {
#ifdef NCONSOLE
    const char* url = "https://zoom.us/client/latest/ZoomInstaller.exe";
    const char* path = "C:/";
    const char* arg1 = "Kyiv";
    const char* arg2 = "Lviv";
    const char* arg3 = "Odessa";
    const char* arg4 = "Kharkiv";
    char* my_argv[] = {
        const_cast<char*>("Fast Corporate Test.exe"),
        const_cast<char*>(url),
        const_cast<char*>(path),
        const_cast<char*>(arg1),
        const_cast<char*>(arg2),
        const_cast<char*>(arg3),
        const_cast<char*>(arg4),
        nullptr
    };
    argc = sizeof(my_argv) / sizeof(char*) - 1;
    argv = my_argv;
#endif
    try {
        FCtest::Application app(argc, argv);
        app.run();
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
        return -100731;
    }
    return 0;
}
