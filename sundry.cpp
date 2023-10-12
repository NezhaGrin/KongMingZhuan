#include "sundry.h"
#include "runtimeexception.h"
#include <QDebug>
/**
 * @brief 互斥锁
 */
QMutex Sundry::m_mutex;

/**
 * @brief 唯一实例
 */
Sundry *Sundry::m_instance = nullptr;

/**
 * @brief 构造函数
 */
Sundry::Sundry(QObject *parent) : QObject(parent)
  , m_common(nullptr)
  , m_processHandel(nullptr)
  , m_goldAddress({0x0EAEC044, 0x0C, 0x0, 0x10, 0x10, 0x08, 0x0, 0x04, 0x0C, 0x14})
{
    m_common = new GameCommon(this);
}

/**
 * @brief 初始化
 */
void Sundry::initialize()
{
    m_processHandel = m_processHandel ?: m_common->getProcessHandle("TWKPLAYER");
}

/**
 * @brief 单列模式
 *
 * @return
 */
Sundry *Sundry::getInstance()
{
    if (!m_instance)
    {
        // 加锁
        QMutexLocker lock(&m_mutex);

        // 第二次检查
        if (!m_instance)
        {
            m_instance = new Sundry;
            m_instance->initialize();
        }
    }

    return m_instance;
}

/**
 * @brief 获取金币的值
 *
 * @return 金币的值
 */
int Sundry::getGoldValue()
{
    // 第一个是基址
    int value = m_common->readProcessMemoryValue<int>(m_processHandel,m_goldAddress.first());

    // 基址 + 偏移
    for (int i = 1; i < m_goldAddress.size(); i++)
    {
        value += m_goldAddress[i];
        value = m_common->readProcessMemoryValue<int>(m_processHandel,value);
    }

    return value / 2;
}

/**
 * @brief 设置金币
 *
 * @param[in] amount 数量
 */
void Sundry::setGoldValue(int amount)
{
//    amount *= 2;

//    // 第一个是基址
//    int value = m_common->readProcessMemoryValue<int>(m_processHandel,m_goldAddress.first());

//    // 基址 + 偏移
//    for (int i = 1; i < m_goldAddress.size(); i++)
//    {
//        if (i == m_goldAddress.size() - 1)
//        {
//            value += m_goldAddress[i];
//        }
//        else
//        {
//            value = m_common->readProcessMemoryValue<int>(m_processHandel,value + m_goldAddress[i]);
//        }
//    }

//    m_common->writeProcessMemoryValue<int>(m_processHandel,value,&amount);
}
