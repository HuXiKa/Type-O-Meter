#ifndef TYPEOMETERWIDGET_H
#define TYPEOMETERWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include "windows.h"

namespace Ui {
class TypeOMeterWidget;
}

class TypeOMeterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TypeOMeterWidget(QWidget *parent = 0);
    ~TypeOMeterWidget();
    void paintEvent(QPaintEvent *);
    /**
      Returns the elapsed time since the current session started.
    */
    int getElapsedTime() { return -QTime::currentTime().msecsTo(m_SessionStartTime); }

public slots:    
    void registerMouseClick() {
        m_TotalMousePressCount++;
        m_SessionMousePressCount++;
        if(m_SessionStartTime.elapsed() > SHIFT_TIME)
            m_NextSessionKeyPressCount++;
    }

    void registerKeypress() {
        m_TotalKeyPressCount++;
        m_SessionKeyPressCount++;
        if(m_SessionStartTime.elapsed() > SHIFT_TIME)
            m_NextSessionKeyPressCount++;
    }

    void displayAPM();

    void restartTime();
signals:
    void APMChanged(int APM);
    void totalAPMChanged(int APM);
private:
    Ui::TypeOMeterWidget *ui;
    static const int TICK_TIME = 20;
    static const int RESTART_TIME = 6000;
    static const int SHIFT_TIME = RESTART_TIME / 2;
    int m_NextSessionKeyPressCount;
    int m_NextSessionMousePressCount;
    /**
      Key presses in the current session.
    */
    int m_SessionKeyPressCount;
    int m_SessionMousePressCount;
    int m_TotalKeyPressCount;
    int m_TotalMousePressCount;
    QTime m_StartTime;
    QTime m_SessionStartTime;
    QTimer m_Ticker;
    int m_AvarageAPM;
    int m_SessionAPM;
};

#endif // TYPEOMETERWIDGET_H
