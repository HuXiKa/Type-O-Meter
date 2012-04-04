#include "systemactionhandler.h"

SystemActionHandler::SystemActionHandler() :
    QObject()
{
    // Assign to null
    m_KeyboardHook = NULL;
    m_MouseHook = NULL;
}


LRESULT CALLBACK SystemActionHandler::keypressProcedure(int nCode, WPARAM wParam, LPARAM /*lParam*/)
{
    // Check for a key down press
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYUP)
        {
            emit SystemActionHandler::instance()->keyPressed();
        }
        //reconnect
        instance()->setKeyboardConnected( false );
        instance()->setKeyboardConnected( true );
    }

    return false;
}

LRESULT CALLBACK SystemActionHandler::mouseProcedure(int nCode, WPARAM wParam, LPARAM /*lParam*/)
{
    // Check for a key down press
    if (nCode == HC_ACTION)
    {
        if(wParam == WM_LBUTTONUP){
            emit SystemActionHandler::instance()->mousePressed();
        }
        //reconnect
        instance()->setMouseConnected( false );
        instance()->setMouseConnected( true );
    }
    return false;
}

bool SystemActionHandler::keyboardConnected()
{
    return m_KeyboardHook;
}

bool SystemActionHandler::mouseConnected()
{
    return m_MouseHook;
}

bool SystemActionHandler::setKeyboardConnected(bool state)
{
    if(state && m_KeyboardHook == NULL)
    {
        m_KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keypressProcedure, GetModuleHandle(NULL), 0);

        return m_KeyboardHook;
    }
    else
    {
        UnhookWindowsHookEx(m_KeyboardHook);
        m_KeyboardHook = NULL;

        return m_KeyboardHook;
    }
}

bool SystemActionHandler::setMouseConnected(bool state)
{
    if(state && m_MouseHook == NULL)
    {
        m_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProcedure, GetModuleHandle(NULL), 0);

        return m_MouseHook;
    }
    else
    {
        UnhookWindowsHookEx(m_MouseHook);
        m_MouseHook = NULL;

        return m_MouseHook;
    }
}

SystemActionHandler* SystemActionHandler::instance()
{
    static SystemActionHandler* pKeyboardReadWriteInstance = new SystemActionHandler();
    return pKeyboardReadWriteInstance;
}
