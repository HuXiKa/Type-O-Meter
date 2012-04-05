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
    /*
                      60 sec
  ------------------------------------------------
  |                                          |

  eltelt 3 s, ezalatt 2 key és 1 mouse
  akkor 60 sec alatt

  60000   x
  3000   2+1

  3 / 2 + 1 = 60 / x
  60000 / (delayed) * (2+1) = x
  */
public:
    explicit TypeOMeterWidget(QWidget *parent = 0);
    ~TypeOMeterWidget();
    void paintEvent(QPaintEvent *);
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

    void displayAPM(){
        int difference = getElapsedTime();
        int apm = 0;
        if (difference > 0)
            apm = 60000 / difference * (m_SessionKeyPressCount + m_SessionMousePressCount);

        //qDebug() << "KeyPress: " << m_SessionKeyPressCount << " mouse: " << m_SessionMousePressCount << " apm: " << apm << " difference: " << difference;
        emit APMChanged(apm);
        int avarageAPM = ((double)(m_TotalKeyPressCount + m_TotalMousePressCount) / m_StartTime.elapsed()) * 60000;
        emit totalAPMChanged(avarageAPM);
    }

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
    int m_SessionKeyPressCount;
    int m_SessionMousePressCount;
    int m_TotalKeyPressCount;
    int m_TotalMousePressCount;
    QTime m_StartTime;
    QTime m_SessionStartTime;    
    QTimer m_Ticker;
};

#endif // TYPEOMETERWIDGET_H
