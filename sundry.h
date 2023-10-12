#ifndef SUNDRY_H
#define SUNDRY_H

#include <QMutex>
#include <QVector>

#include "gamecommon.h"

class Sundry : public QObject
{
    Q_OBJECT
public:
    // 获取实例
    static Sundry *getInstance();

    // 初始化操作
    void initialize();

    // 获取金币值
    int getGoldValue();

    // 设置金币
    void setGoldValue(int amount);

private:
    // 实例互存锁4
    static QMutex m_mutex;

    // 持有唯一实例，保证全局唯一性
    static Sundry *m_instance;

    // 游戏公共接口
    GameCommon *m_common;

    // 进程句柄
    HANDLE m_processHandel;

    // 金币地址
    QVector<int> m_goldAddress;

    // 构造函数声明为私有，确保只能在类内部创建对象
    Sundry(QObject *parent = nullptr);
};

#endif // SUNDRY_H
