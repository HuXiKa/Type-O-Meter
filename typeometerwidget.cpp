#include "typeometerwidget.h"
#include "ui_typeometerwidget.h"
#include "systemactionhandler.h"
#include <QPainter>
#include <QTimer>
#include <QBitmap>
#include <QTime>

TypeOMeterWidget::TypeOMeterWidget(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowSystemMenuHint /*| Qt::WindowStaysOnTopHint*/),
      ui(new Ui::TypeOMeterWidget),
    m_NextSessionKeyPressCount(0), m_NextSessionMousePressCount(0), m_SessionKeyPressCount(0), m_SessionMousePressCount(0),
      m_TotalKeyPressCount(0), m_TotalMousePressCount(0), m_StartTime(QTime::currentTime())
{
    ui->setupUi(this);
    this->move(100,100);
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(timer, SIGNAL(timeout()), this, SLOT(displayAPM()));
    timer->start(TICK_TIME);
    SystemActionHandler *handler = SystemActionHandler::instance();
    handler->setKeyboardConnected(true);
    handler->setMouseConnected(true);
    connect(handler, SIGNAL(keyPressed()), this, SLOT(registerKeypress()));
    connect(handler, SIGNAL(mousePressed()), this, SLOT(registerMouseClick()));
    connect(this, SIGNAL(APMChanged(int)), ui->currentAPMLcdNumber, SLOT(display(int)));
    connect(this, SIGNAL(totalAPMChanged(int)), ui->totalAPMLcdNumber, SLOT(display(int)));

    connect(&m_Ticker, SIGNAL(timeout()), this, SLOT(restartTime()));
    m_Ticker.start(RESTART_TIME);

    QTimer *timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(restartTime()));
    timer2->start(RESTART_TIME);    
    m_SessionStartTime = m_StartTime;


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
    painter.drawPixmap(0,0,QPixmap(":/image/background"));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 255));

    painter.save();
    painter.translate(200,200);
    painter.rotate(-130 + qMin(m_SessionAPM,260));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setBrush(QColor(0, 255, 255));
    painter.save();
    painter.translate(200,200);
    painter.rotate(-130 + m_AvarageAPM);
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

void TypeOMeterWidget::displayAPM()
{
    int difference = getElapsedTime();
    m_SessionAPM = 0;
    if (difference > 0)
        m_SessionAPM = 60000 / difference * (m_SessionKeyPressCount + m_SessionMousePressCount);

    //qDebug() << "KeyPress: " << m_SessionKeyPressCount << " mouse: " << m_SessionMousePressCount << " apm: " << apm << " difference: " << difference;
    emit APMChanged(m_SessionAPM);
    m_AvarageAPM = ((double)(m_TotalKeyPressCount + m_TotalMousePressCount) / m_StartTime.elapsed()) * 60000;
    emit totalAPMChanged(m_AvarageAPM);
}
