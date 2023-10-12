#ifndef GAMEPROCESSHANDLE_H
#define GAMEPROCESSHANDLE_H

#include <QObject>
#include <QDebug>

#include <windows.h>

class GameProcessHandle : public QObject
{
    Q_OBJECT
public:
    explicit GameProcessHandle(QObject *parent = nullptr);

    HANDLE getProcessHandleByClassName(LPCSTR gameClassName);
};

#endif // GAMEPROCESSHANDLE_H
