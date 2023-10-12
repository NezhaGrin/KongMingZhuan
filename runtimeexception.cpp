#include "runtimeexception.h"

RuntimeException::RuntimeException() : m_message("Error.")
{

}


RuntimeException::RuntimeException(const QString &message) : m_message(message)
{

}
