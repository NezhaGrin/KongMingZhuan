#ifndef RUNTIMEEXCEPTION_H
#define RUNTIMEEXCEPTION_H

#include <QException>

class RuntimeException : public QException
{
public:
    RuntimeException();

    RuntimeException(const QString &message);

    QString getMessage()
    {
        return m_message;
    }
private:
    QString m_message;
};

#endif // RUNTIMEEXCEPTION_H
