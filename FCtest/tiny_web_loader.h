#pragma once
//WinAPI
#include <Windows.h>
#include <WinInet.h>

//std
#include <fstream>

namespace FCtest {

	class TinyWebLoader	{
	public:
		TinyWebLoader();
		~TinyWebLoader();

		void load(LPCWSTR url, std::wstring& filename, std::wstring& filepath);
	private:
		HINTERNET hInternet{};
	};

} // namespace FCtest