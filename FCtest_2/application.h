#pragma once
//WinAPI
#include <Windows.h>
#include <AtlBase.h>
#include <AtlCom.h>
#include <comdef.h>
#include <UIAutomation.h>
//std
#include <stdlib.h>
#include <iostream>
#include <functional>
#include <regex>
#include <boost/regex.hpp>
namespace FCtest {

    class Application {
    public:
        Application();
        ~Application();

        void run();
    private:

        //url field methods
        bool isElementFocused(IUIAutomationElement* element);
        void invokeEditField(IUIAutomationElement* element);
        void setEditField(IUIAutomationElement* editElement, const std::wstring text);
        void SendEnter();
        void modifyUrl(std::wstring& url, const std::wstring text);
        void changeUrl();

        //members
        HWND firefoxWindow;
        CComQIPtr<IUIAutomation> uia;
        CComPtr<IUIAutomationElement> root;
        CComPtr<IUIAutomationCondition> condition;
        CComPtr<IUIAutomationElement> edit;

        //callback
        using EnterCallback = std::function<void()>;
        static HHOOK g_keyboardHook;
        static EnterCallback g_enterCallback;

        bool SetKeyboardHook(const EnterCallback& callback);
        bool UnhookKeyboard();
        static LRESULT CALLBACK KeyboardHookCallback(int nCode, WPARAM wParam, LPARAM lParam);
    };

}