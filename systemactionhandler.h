#ifndef SYSTEMACTIONHANDLER_H
#define SYSTEMACTIONHANDLER_H

#include <QObject>
#include <Windows.h>

/*
  huxika
  */
class SystemActionHandler : public QObject
{

Q_OBJECT
public:
    // Returns singleton instance
    static SystemActionHandler * instance();

    // Returns whether the keyboard hook is connected
    bool keyboardConnected();
    // Returns whether the keyboard hook is connected
    bool mouseConnected();

    // Connects / Disconnects the keyboard hook
    bool setKeyboardConnected(bool state);
    // Connects / Disconnects the mouse hook
    bool setMouseConnected(bool state);

signals:
    // Broadcasts a key has been pressed
    void keyPressed();
    // Broadcasts the mouse has been pressed
    void mousePressed();

private:
    // Keyboard hook
    HHOOK m_KeyboardHook;
    // Mouse hook
    HHOOK m_MouseHook;

    // Class constructor
    SystemActionHandler();

    // Identifies hook activity
    static LRESULT CALLBACK mouseProcedure(int nCode, WPARAM wParam, LPARAM lParam);
    // Identifies hook activity
    static LRESULT CALLBACK keypressProcedure(int nCode, WPARAM wParam, LPARAM lParam);

};

#endif // SYSTEMKEYBOARDREADWRITE_H
