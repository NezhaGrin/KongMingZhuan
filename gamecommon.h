#ifndef GAMECOMMON_H
#define GAMECOMMON_H

#include <QObject>

#include <windows.h>
#include <Tlhelp32.h>
#include <Psapi.h>

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
     * @param[in] address 内存地址
     *
     * @return 读取的值
     */
    template<typename T>
    int readProcessMemoryValue(T address)
    {
        int value = 0;

        ReadProcessMemory(m_processHandle,(LPCVOID &)address,&value,sizeof (T),NULL) ?: throw RuntimeException("读取数据失败!");

        return value;
    }


    template<typename T>
    void writeProcessMemoryValue(T address,LPCVOID value)
    {
        WriteProcessMemory(m_processHandle,(LPVOID&)address,value,sizeof (T),NULL) ?: throw RuntimeException("写入数据失败!");
    }

private:
    // 进程句柄
    HANDLE m_processHandle;
};

#endif // GAMECOMMON_H
