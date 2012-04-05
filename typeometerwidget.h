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

public slots:    
    void registerMouseClick();
    void forgetMouseClick();

    void registerKeypress();
    void forgetKeypress();

    void displayAPM();

signals:
    void APMChanged(int APM);
    void totalAPMChanged(int APM);
private:
    Ui::TypeOMeterWidget *ui;
    static const int MINUTE = 60000;
    static const int SESSION_TIME = MINUTE / 60 * 2;
    static const int TICK_TIME = MINUTE / 60 / 30;   // 30 FPS

    int m_SessionKeyPressCount;
    int m_SessionMousePressCount;
    int m_TotalKeyPressCount;
    int m_TotalMousePressCount;
    QTime m_StartTime;    
    QTimer m_Ticker;
    int m_AvarageAPM;
    int m_SessionAPM;
};

#endif // TYPEOMETERWIDGET_H
