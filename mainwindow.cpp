#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("吞食孔明传金手指 支持游戏版本：v4.3");

    setFixedSize(810,620);

    setWindowIcon(QIcon(":/images/1.jpg"));

    try {
        m_sundry = Sundry::getInstance();

        getGoldValue();
    }  catch (RuntimeException &runtime) {
        QMessageBox::critical(this,"Error",runtime.getMessage());
    }

    m_goldTimerId = startTimer(1000);

    ui->lineEditGold->installEventFilter(this);
}

void MainWindow::getGoldValue()
{
    ui->lineEditGold->setText(QString::number(m_sundry->getGoldValue()));
    ui->lineEditGold->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    try {
        if (e->timerId() == m_goldTimerId)
        {
            getGoldValue();
        }
    }  catch (RuntimeException &runtime) {
        qDebug() << runtime.getMessage();
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->lineEditGold && e->type() == QEvent::MouseButtonPress && m_goldTimerId)
    {
        killTimer(m_goldTimerId);

        m_goldTimerId = 0;

        return true;
    }

    if (obj == ui->lineEditGold && e->type() == QEvent::Enter && !m_goldTimerId)
    {
        m_goldTimerId = startTimer(1000);

        return true;
    }

    return QWidget::eventFilter(obj,e);
}

void MainWindow::on_lineEditGold_editingFinished()
{
    if (!m_goldTimerId)
    {
        m_goldTimerId = startTimer(1000);
    }

    try {
        m_sundry->setGoldValue(ui->lineEditGold->text().toInt());
    } catch (RuntimeException &runtime) {
        qDebug() << runtime.getMessage();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

