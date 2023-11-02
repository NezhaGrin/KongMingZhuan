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
  , m_trainingAddress({0x2AC044, 0xC, 0x10, 0x14, 0x14, 0xC, 0x8, 0x0, 0x4, 0xC, 0x2C})
  , m_test({0x26A080,0x4,0x0,0x28,0x4,0x4,0x8,0x348,0xC,0x9C,0x30})
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
    int baseAddress = m_common->getModuleBaseAddress("rgss300.dll");

    // 向第一个元素插入基址
    m_goldAddress.prepend(baseAddress);
    m_trainingAddress.prepend(baseAddress);
    m_test.prepend(baseAddress);
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
    amount = (amount * 2) + 1;

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

    m_common->writeProcessMemoryValue(value,&amount);
}

/**
 * @brief 获取练兵值
 *
 * @return 练兵值
 */
int Sundry::getTrainingValue()
{
    QMutexLocker lock(&m_mutex);

    // 获取基址
    int value = m_common->readProcessMemoryValue<int>(m_trainingAddress[0] + m_trainingAddress[1]);

    // 基址 + 偏移
    for (int i = 2; i < m_trainingAddress.size(); i++)
    {
        value = m_common->readProcessMemoryValue<int>(value + m_trainingAddress[i]);
    }

    return value / 2;
}

void Sundry::test()
{
    QMutexLocker lock(&m_mutex);

    // 获取基址
    int value = m_common->readProcessMemoryValue<int>(m_test[0] + m_test[1]);

    // 基址 + 偏移
    for (int i = 2; i < m_test.size(); i++)
    {
        if (i == m_test.size() - 1)
        {
            value += m_test[i];
        }
        else
        {
            value = m_common->readProcessMemoryValue<int>(value + m_test[i]);
        }
    }

    // 不同场景下无法读取
    char *ansiString = m_common->readProcessMemoryValueToChar(value,10);

    qInfo() << ansiString;

    delete [] ansiString;
}
