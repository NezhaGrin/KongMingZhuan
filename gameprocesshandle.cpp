#include "gameprocesshandle.h"

GameProcessHandle::GameProcessHandle(QObject *parent) : QObject(parent)
{

}


/**
 * @brief 获取进程句柄，通过类名方式
 *
 * @param[in] gameClassName 游戏类名
 *
 * @return NULL 表示失败
 */
HANDLE GameProcessHandle::getProcessHandleByClassName(LPCSTR gameClassName)
{
    HWND windowHandle = FindWindowA(gameClassName,NULL);

    if (!windowHandle)
    {
        qWarning() << "get window handle failed,game is open?";

        return NULL;
    }

    DWORD processId = 0;
    GetWindowThreadProcessId(windowHandle,&processId);

    if (!processId)
    {
        qWarning() << "get window process id fialed!";

        return NULL;
    }

    qDebug() << "process id = " << processId;

    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,processId);

    if (!processHandle)
    {
        qWarning() << "open process fialed!";

        return nullptr;
    }

    qDebug() << processHandle;

    return processHandle;
}
