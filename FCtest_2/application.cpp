#include "application.h"


namespace FCtest {

    //static members
    HHOOK Application::g_keyboardHook = nullptr;
    Application::EnterCallback Application::g_enterCallback;

    Application::Application() : firefoxWindow{} {
        CoInitialize(nullptr);
        firefoxWindow = FindWindowEx(0, firefoxWindow, L"MozillaWindowClass", NULL);
        if (FAILED(uia.CoCreateInstance(CLSID_CUIAutomation)) || !uia) {
            throw std::runtime_error("failed to create uia instance");
        }

        if (FAILED(uia->ElementFromHandle(firefoxWindow, &root)) || !root) {
            throw std::runtime_error("failed to find root");
        }

        uia->CreatePropertyCondition(UIA_ControlTypePropertyId, CComVariant(0xC354), &condition);
        if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &edit))
            || !edit) {
            throw std::runtime_error("failed to find addressbar");
        }
    }

    Application::~Application() {
        CoUninitialize();
    }

    void Application::invokeEditField(IUIAutomationElement* element) {
        CComPtr<IUIAutomationInvokePattern> invokePattern;
        if (SUCCEEDED(element->GetCurrentPattern(UIA_InvokePatternId, (IUnknown**)&invokePattern))) {
            invokePattern->Invoke();
        }
    }

    void Application::setEditField(IUIAutomationElement* editElement, const std::wstring text) {
        CComPtr<IUIAutomationValuePattern> valuePattern;
        if (SUCCEEDED(editElement->GetCurrentPattern(UIA_ValuePatternId, (IUnknown**)&valuePattern))) {
            valuePattern->SetValue(CComBSTR(text.c_str()));
        }
    }

    void Application::SendEnter() {
        SendMessage(firefoxWindow, WM_KEYDOWN, VK_RETURN, 0);
        SendMessage(firefoxWindow, WM_KEYUP, VK_RETURN, 0);
    }

    void Application::modifyUrl(std::wstring& url, const std::wstring text) {
        boost::wregex protocolRegex(L"(https?)://");
        boost::wsmatch protocolMatch;
        if (boost::regex_search(url, protocolMatch, protocolRegex)) {
            std::wstring protocol = protocolMatch.str(1);
            url = boost::regex_replace(url, protocolRegex, L"");

            boost::wregex domainRegex(L"www\\.");
            url = boost::regex_replace(url, domainRegex, L"");

            url = protocol + L"://" + text + L"." + url;
        }
        else {
            boost::wregex domainRegex(L"www\\.");
            url = boost::regex_replace(url, domainRegex, L"");

            url = text + L"." + url;
        }
    }

    void Application::changeUrl() {
        Sleep(500);
        if (!firefoxWindow) {
            throw std::runtime_error("Error 0");
        }

        if (!IsWindowVisible(firefoxWindow)) {
            throw std::runtime_error("Error 1");
        }

        CComPtr<IUIAutomationElement> addressBar;
        if (FAILED(root->FindFirst(TreeScope_Descendants, condition, &addressBar)) || !addressBar) {
            throw std::runtime_error("Error 3");
        }

        CComVariant url;
        addressBar->GetCurrentPropertyValue(UIA_ValueValuePropertyId, &url);
        std::wstring URL = std::wstring(url.bstrVal);
        invokeEditField(addressBar);
        modifyUrl(URL, L"Love");
        setEditField(addressBar, URL);
        SendEnter();
    }

    bool Application::SetKeyboardHook(const EnterCallback& callback) {
        g_enterCallback = callback;
        g_keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHookCallback, nullptr, 0);
        return g_keyboardHook != nullptr;
    }

    bool Application::UnhookKeyboard() {
        if (g_keyboardHook) {
            bool success = UnhookWindowsHookEx(g_keyboardHook);
            g_keyboardHook = nullptr;
            return success;
        }
        return false;
    }

    LRESULT CALLBACK Application::KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
            KBDLLHOOKSTRUCT* kbStruct = (KBDLLHOOKSTRUCT*)lParam;
            if (kbStruct->vkCode == VK_RETURN) {
                HWND activeWindow = GetForegroundWindow();
                wchar_t className[256];
                GetClassNameW(activeWindow, className, 256);
                if (wcscmp(className, L"MozillaWindowClass") == 0) {
                    std::wcout << L"Enter pressed in browser!\n";
                    if (g_enterCallback) {
                        g_enterCallback();
                    }
                }
            }
        }
        return CallNextHookEx(g_keyboardHook, nCode, wParam, lParam);
    }

    void Application::run() {
        SetKeyboardHook([this]() {
            this->changeUrl();
            });

        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        UnhookKeyboard();
    }
}