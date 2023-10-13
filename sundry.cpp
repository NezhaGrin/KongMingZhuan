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
  , m_goldAddress({0X2AC044, 0x0C, 0x0, 0x10, 0x10, 0x08, 0x0, 0x04, 0x0C, 0x14})
{
    m_common = new GameCommon(this);
}

/**
 * @brief 初始化
 */
void Sundry::initialize()
{
    m_common->getProcessHandle("TWKPLAYER");

    // rgss300.dll 模块地址
    m_goldAddress.prepend(m_common->getModuleBaseAddress("rgss300.dll"));
}

/**
 * @brief 单列模式
 *
 * @return 实例化
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
    QMutexLocker lock(&m_mutex);

    // 获取基址
    int value = m_common->readProcessMemoryValue<int>(m_goldAddress[0] + m_goldAddress[1]);

    // 基址 + 偏移
    for (int i = 2; i < m_goldAddress.size(); i++)
    {
        value = m_common->readProcessMemoryValue<int>(value + m_goldAddress[i]);
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
    amount *= 2;

    qInfo() << amount;

    // 获取基址
    int value = m_common->readProcessMemoryValue<int>(m_goldAddress[0] + m_goldAddress[1]);

    // 基址 + 偏移
    for (int i = 2; i < m_goldAddress.size(); i++)
    {
        if (i == m_goldAddress.size() - 1)
        {
            value += m_goldAddress[i];
        }
        else
        {
            value = m_common->readProcessMemoryValue<int>(value + m_goldAddress[i]);
        }
    }

    m_common->writeProcessMemoryValue<int>(value,&amount);
}
