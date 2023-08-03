#include "application.h"

namespace FCtest {

	Application::Application(int argc, char* argv[]) : loader(), launcher() {
		if (argc < 2) {
			throw std::runtime_error("Not enough arguments");
		}
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wstr = converter.from_bytes(argv[1]);
		url = new wchar_t[wstr.size() + 1];
		wcscpy_s(const_cast<LPWSTR>(url), wstr.size() + 1, wstr.c_str());
		findFileName();
		filepath = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(argv[2]);
		findParameters(argc, argv);
	}

	void Application::findFileName() {
		boost::wregex regex(L"\\/([^\\/]+)$");
		boost::wsmatch match;
		std::wstring wurl(url);

		if (boost::regex_search(wurl, match, regex)) {
			filename = match[1].str();
			std::wcout << "Filename found: " << filename << "\n";
		}
		else {
			throw std::runtime_error("Filename not found.");
		}
	}

	void Application::findParameters(int argc, char* argv[]) {
		std::string parameters;
		for (int i = 0; i < argc; i++) {
			parameters += " ";
			parameters += argv[i];
		}
		this->parameters = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>().from_bytes(parameters.c_str());
	}

	void Application::run() {
#ifdef PARAM_TEST
		filename = L"test.exe"; // set name of file you want to run with params
		filepath = L""; // set path to file
		parameters = L"Kiyv Lviv Odessa Kharkiv Dnipro"; // set parameters
		std::wcout << "\nParameters: " << parameters << std::endl;
#endif
#ifndef PARAM_TEST
		loader.load(url, filename, filepath);
#endif
		launcher.launch(filename, filepath, parameters);
	}

} // namespace FCtest