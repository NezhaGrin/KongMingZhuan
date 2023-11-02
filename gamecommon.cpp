#include "gamecommon.h"

GameCommon::GameCommon(QObject *parent) : QObject(parent)
  , m_processHandle(nullptr)
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
void GameCommon::getProcessHandle(LPCSTR className,LPCSTR windowName)
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

    m_processHandle = OpenProcess(PROCESS_ALL_ACCESS,false,processId);

    if (!m_processHandle)
    {
        throw RuntimeException("打开进程句柄失败!");
    }
}

/**
 * @brief 获取模块基址
 *
 * @param[in] moduleNameS 模块名称
 *
 * @return 模块基址
 */
int GameCommon::getModuleBaseAddress(const QString &moduleNameS)
{
    // 模块信息的字节数
    DWORD cbNeeded;

    // 加载的模块句柄的数组
    HMODULE hModules[1024];

    // 检索指定进程中满足指定筛选条件的每个模块的句柄，只检索 32 位的
    EnumProcessModulesEx(m_processHandle, hModules, sizeof(hModules), &cbNeeded, LIST_MODULES_32BIT) ?: throw RuntimeException("检索进程模块句柄失败!");

    // 模块基址
    int moduleBaseAddress = 0;

    // 模块数量
    int moduleCount = cbNeeded / sizeof(HMODULE);

    // 遍历每个加载的模块
    for (int i = 0; i < moduleCount; i++)
    {
        TCHAR moduleName[MAX_PATH];

        // 获取模块的文件名
        if (GetModuleBaseName(m_processHandle, hModules[i], moduleName, sizeof(moduleName) / sizeof(TCHAR)))
        {
            // 转换为宽字符串
            QString moduleNameQt = QString::fromWCharArray(moduleName);

            // 检查模块名是否匹配，不区分大小写
            if (moduleNameQt.compare(moduleNameS, Qt::CaseInsensitive) == 0) {
                moduleBaseAddress = (DWORD_PTR)hModules[i];
                break;
            }
        }
    }

    return moduleBaseAddress;
}

/**
 * @brief 读取内存值转字符串
 *
 * @param[in] address  地址
 * @param[in] sizeByte 读取字节数
 *
 * @return 字符串
 */
char *GameCommon::readProcessMemoryValueToChar(int address,size_t sizeByte)
{
    char *value = new char[++sizeByte];

    if (!ReadProcessMemory(m_processHandle,(LPCVOID &)address,value,sizeByte,NULL))
    {
        delete [] value;
        throw RuntimeException("读取数据失败!");
    }

    return value;
}

/**
 * @brief 写入内存值
 *
 * @param[in] address  地址
 * @param[in] value    写入的值
 * @param[in] sizeByte 写入的字节数
 */
void GameCommon::writeProcessMemoryValue(int address, LPCVOID value, size_t sizeByte)
{
    WriteProcessMemory(m_processHandle,(LPVOID&)address,value,sizeByte,NULL) ?: throw RuntimeException("写入数据失败!");
}
