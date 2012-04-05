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
    connect(timer, SIGNAL(timeout()), this, SLOT(displayAPM()));
    timer->start(TICK_TIME);
    SystemActionHandler *handler = SystemActionHandler::instance();
    handler->setKeyboardConnected(true);
    handler->setMouseConnected(true);
    connect(handler, SIGNAL(keyPressed()), this, SLOT(registerKeypress()));
    connect(handler, SIGNAL(mousePressed()), this, SLOT(registerMouseClick()));
    connect(this, SIGNAL(APMChanged(int)), ui->lcdNumber, SLOT(display(int)));

    connect(&m_Ticker, SIGNAL(timeout()), this, SLOT(restartTime()));
    m_Ticker.start(RESTART_TIME);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(restartTime()));
    timer2->start(RESTART_TIME);
    m_StartTime = QTime::currentTime();
    m_SessionStartTime = m_StartTime;
    m_SessionKeyPressCount = 0;
    m_SessionMousePressCount = 0;
    m_TotalKeyPressCount = 0;
    m_TotalMousePressCount = 0;
    m_NextSessionKeyPressCount = 0;
    m_NextSessionMousePressCount = 0;

    //QPixmap pixmap(":/image/background");
    //setMask(QBitmap(pixmap));

}

TypeOMeterWidget::~TypeOMeterWidget()
{
    delete ui;
}

void TypeOMeterWidget::paintEvent(QPaintEvent *)
{
    static const QPoint minuteHand[3] = {
             QPoint(7, 8),
             QPoint(-7, 8),
             QPoint(0, -70)
         };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,400,400,QPixmap(":/image/background"));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(127, 0, 127));

    int difference = getElapsedTime();
    int apm = 0;
    if (difference > 0)
        apm = 60000 / difference * (m_SessionKeyPressCount + m_SessionMousePressCount);

    painter.save();
    painter.translate(100,100);
    painter.rotate(-130 + qMin(apm,260));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
}

void TypeOMeterWidget::restartTime()
{
    m_Ticker.start(SHIFT_TIME);
    m_SessionStartTime = m_SessionStartTime.addMSecs(SHIFT_TIME);
    //qDebug() << "restartin session @ " << m_SessionStartTime.minute() << ":" << m_SessionStartTime.second();
    m_SessionKeyPressCount = m_NextSessionKeyPressCount;
    m_SessionMousePressCount = m_NextSessionMousePressCount;
    m_NextSessionKeyPressCount = 0;
    m_NextSessionMousePressCount = 0;

}
