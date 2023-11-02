#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sundry.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_lineEditGold_editingFinished();

private:
    // 获取金币值
    void getGoldValue();

    // 获取练兵值
    void getTrainingValue();

    // 重新定时器事件
    void timerEvent(QTimerEvent *e);

    // 重新事件过滤器
    bool eventFilter(QObject *, QEvent *);

private:
    Sundry *m_sundry;

    int m_goldTimerId;

    int m_sundryTimerId;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
