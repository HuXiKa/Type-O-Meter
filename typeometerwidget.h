#ifndef TYPEOMETERWIDGET_H
#define TYPEOMETERWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QTime>
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
  3 / 2 + 1 = 60 / x
  60000 / (delayed) * (2+1) = x
  */
public:
    explicit TypeOMeterWidget(QWidget *parent = 0);
    ~TypeOMeterWidget();
    void paintEvent(QPaintEvent *);
    int getElapsedTime() { return -QTime::currentTime().msecsTo(m_SessionStartTime); }

public slots:
    void registerMouseClick() { m_SessionMousePressCount++; }
    void registerKeypress() { m_SessionKeyPressCount++; }
    void restartTime();
signals:
    void APMChanged(int APM);
private:
    Ui::TypeOMeterWidget *ui;
    static const int TICK_TIME = 20;
    static const int RESTART_TIME = 10000;
    static const int SHIFT_TIME = RESTART_TIME / 2;
    int m_SessionKeyPressCount;
    int m_SessionMousePressCount;
    int m_TotalKeyPressCount;
    int m_TotalMousePressCount;
    QTime m_StartTime;
    QTime m_SessionStartTime;
};

#endif // TYPEOMETERWIDGET_H
