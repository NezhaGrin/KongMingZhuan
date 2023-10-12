#ifndef GAMECOMMON_H
#define GAMECOMMON_H

#include <QObject>
#include <windows.h>

#include "runtimeexception.h"
#include <QDebug>

class GameCommon : public QObject
{
    Q_OBJECT
public:
    explicit GameCommon(QObject *parent = nullptr);

    // 获取进程句柄
    HANDLE getProcessHandle(LPCSTR className,LPCSTR windowName = NULL);

    /**
     * @brief 读取内存值
     *
     * @param[in] processHandle 进程句柄
     * @param[in] address       内存地址
     * @param[in] size          读取类型大小
     *
     * @return 读取的值
     */
    template<typename T>
    int readProcessMemoryValue(HANDLE processHandle,T address)
    {
        int value = 0;

        int res = ReadProcessMemory(processHandle,(LPCVOID &)address,&value,sizeof (T),NULL);

        if (!res)
        {
            throw RuntimeException("读取数据失败!");
        }

        return value;
    }

    /**
     * @brief writeProcessMemoryValue
     * @param processHandle
     * @param address
     * @param value
     * @param size
     */
    template<typename T>
    void writeProcessMemoryValue(HANDLE processHandle,int address,LPCVOID value,SIZE_T size = sizeof (T))
    {
        bool res = WriteProcessMemory(processHandle,(LPVOID&)address,value,size,NULL);

        if (!res)
        {
            throw RuntimeException("写入数据失败!");
        }
    }
};

#endif // GAMECOMMON_H
