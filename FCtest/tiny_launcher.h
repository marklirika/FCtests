#pragma once
#include <Windows.h>

//std
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

namespace FCtest {

	class TinyLauncher {
	public:
		void launch(const std::wstring& filename, const std::wstring& filepath, const std::wstring& parameters);
	private:
		bool isExecutable(const std::wstring& filename);
	};

} // namespace FCtest