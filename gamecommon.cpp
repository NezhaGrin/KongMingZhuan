#include "gamecommon.h"

GameCommon::GameCommon(QObject *parent) : QObject(parent)
{

}

/**
 * @brief 获取进程句柄
 *
 * @param[in] className  游戏类名
 * @param[in] windowName 游戏窗口名称
 *
 * @return 进程句柄
 */
HANDLE GameCommon::getProcessHandle(LPCSTR className,LPCSTR windowName)
{
    HWND windowHandle = FindWindowA(className,windowName);

    if (!windowHandle)
    {
        throw RuntimeException("获取窗口句柄失败,游戏是否已启动?");
    }

    DWORD processId = 0;
    GetWindowThreadProcessId(windowHandle,&processId);

    if (!processId)
    {
        throw RuntimeException("获取窗口进程标识失败!");
    }

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,processId);

    if (!processHandle)
    {
        throw RuntimeException("打开进程句柄失败!");
    }

    return processHandle;
}
