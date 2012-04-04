#include "typeometerwidget.h"
#include "ui_typeometerwidget.h"
#include "systemactionhandler.h"
#include <QPainter>
#include <QTimer>
#include <QBitmap>
#include <QTime>

TypeOMeterWidget::TypeOMeterWidget(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint),
      ui(new Ui::TypeOMeterWidget)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(TICK_TIME);
    SystemActionHandler *handler = SystemActionHandler::instance();
    handler->setKeyboardConnected(true);
    handler->setMouseConnected(true);
    connect(handler, SIGNAL(keyPressed()), this, SLOT(registerKeypress()));
    connect(handler, SIGNAL(mousePressed()), this, SLOT(registerMouseClick()));
    connect(this, SIGNAL(APMChanged(int)), ui->lcdNumber, SLOT(display(int)));
    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(restartTime()));
    timer2->start(RESTART_TIME);
    m_StartTime = QTime::currentTime();
    m_SessionStartTime = m_StartTime;
    m_SessionKeyPressCount = 0;
    m_SessionMousePressCount = 0;
    m_TotalKeyPressCount = 0;
    m_TotalMousePressCount = 0;
    //QPixmap pixmap(":/image/background");
    //setMask(QBitmap(pixmap));

}

TypeOMeterWidget::~TypeOMeterWidget()
{
    delete ui;
}

void TypeOMeterWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,400,400,QPixmap(":/image/background"));
    int difference = getElapsedTime();
    int apm = 0;
    if (difference > 0)
        apm = RESTART_TIME / difference * (m_SessionKeyPressCount + m_SessionMousePressCount);

    //qDebug() << "KeyPress: " << m_KeyPressCount << " mouse: " << m_MousePressCount << " apm: " << apm;
    emit APMChanged(apm);
}

void TypeOMeterWidget::restartTime()
{
     m_SessionStartTime.addMSecs(SHIFT_TIME);
     m_TotalKeyPressCount = m_SessionKeyPressCount;
     m_TotalMousePressCount = m_SessionMousePressCount;
     m_SessionKeyPressCount = 0;
     m_SessionMousePressCount = 0;
}
