#ifndef GAMECOMMON_H
#define GAMECOMMON_H

#include <QObject>

#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>
#include <QDebug>

#include "runtimeexception.h"

class GameCommon : public QObject
{
    Q_OBJECT
public:
    explicit GameCommon(QObject *parent = nullptr);

    // 获取进程句柄
    void getProcessHandle(LPCSTR className,LPCSTR windowName = NULL);

    // 获取模块基址
    int getModuleBaseAddress(const QString &moduleNameS);

    /**
     * @brief 读取内存值
     *
     * @param[in] address  地址
     * @param[in] sizeByte 读取字节数
     *
     * @return 纯内存的值或是内存地址
     */
    template<typename T>
    T readProcessMemoryValue(int address,size_t sizeByte = sizeof (T))
    {
        T value;

        ReadProcessMemory(m_processHandle,(LPCVOID &)address,&value,sizeByte,NULL) ?: throw RuntimeException("读取数据失败!");

        return value;
    }

    // 读取内存值转字符串
    char *readProcessMemoryValueToChar(int address,size_t sizeByte);

    // 写入内存值
    void writeProcessMemoryValue(int address,LPCVOID value,size_t sizeByte = 4);

private:
    // 进程句柄
    HANDLE m_processHandle;
};

#endif // GAMECOMMON_H
