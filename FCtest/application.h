#pragma once
#include "tiny_web_loader.h"
#include "tiny_launcher.h"
#include <codecvt>
#include <boost/regex.hpp>
#include <boost/locale.hpp>


namespace FCtest {

	class Application {
	public:
		Application(int argc, char* argv[]);
		Application() = delete;
		Application(const Application& Application) = delete;
		Application& operator=(const Application& Application) = delete;
		Application(Application&& Application) = delete;
		Application& operator=(Application&& Application) = delete;
		~Application() {}

		void run();
	private:
		void findFileName();
		void findParameters(int argc, char* argv[]);

		LPCWSTR url = nullptr;
		std::wstring parameters;

		std::wstring filename;
		std::wstring filepath;

		TinyWebLoader loader{};
		TinyLauncher launcher{};
	};

} // namespace FCtest