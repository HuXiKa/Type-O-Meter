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
      m_SessionKeyPressCount(0), m_SessionMousePressCount(0), m_SessionAPM(0), m_AvarageAPM(0),
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

    QPixmap pixmap(":/image/bg");
    setMask(QBitmap(pixmap));

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
        QPoint(0, -150)
    };

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(0,0,QPixmap(":/image/bg"));
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 255));

    painter.save();
    painter.translate(200,270);
    painter.rotate(-110 + qMin(m_SessionAPM * 0.7,220.));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();

    painter.setBrush(QColor(0, 255, 255));
    painter.save();
    painter.translate(200,270);
    painter.rotate(-110 + qMin(m_AvarageAPM * 0.7,220.));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
}

void TypeOMeterWidget::displayAPM()
{

    int currentAPM = MINUTE / SESSION_TIME * (m_SessionKeyPressCount + m_SessionMousePressCount);
    if(currentAPM > m_SessionAPM){
        for(int i = m_SessionAPM; i < currentAPM; ++i){
            ui->currentAPMLcdNumber->display(i);
            m_SessionAPM = currentAPM;
        }
    }
    else if(currentAPM < m_SessionAPM){
        for(int i = m_SessionAPM; i > currentAPM; --i){
            ui->currentAPMLcdNumber->display(i);
            m_SessionAPM = currentAPM;
        }
    }




    //qDebug() << "KeyPress: " << m_SessionKeyPressCount << " mouse: " << m_SessionMousePressCount << " apm: " << apm << " difference: " << difference;

    emit APMChanged(m_SessionAPM);
    m_AvarageAPM = ((double)(m_TotalKeyPressCount + m_TotalMousePressCount) / m_StartTime.elapsed()) * MINUTE;
    emit totalAPMChanged(m_AvarageAPM);
}

void TypeOMeterWidget::registerMouseClick()
{
    m_TotalMousePressCount++;
    m_SessionMousePressCount++;
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(forgetMouseClick()));
    timer->setSingleShot(true);
    timer->start(SESSION_TIME);
}

void TypeOMeterWidget::forgetMouseClick()
{
    m_SessionMousePressCount--;
}

void TypeOMeterWidget::registerKeypress()
{
    m_TotalKeyPressCount++;
    m_SessionKeyPressCount++;
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), this, SLOT(forgetKeypress()));
    timer->setSingleShot(true);
    timer->start(SESSION_TIME);
}

void TypeOMeterWidget::forgetKeypress()
{
    m_SessionKeyPressCount--;
}
