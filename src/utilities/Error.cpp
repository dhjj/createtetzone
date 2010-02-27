#include "Error.h"

Error::Error(const std::string& message)
    : m_errorMessage(message)
{
}

Error::~Error() throw()
{
}

const char* Error::what() const throw()
{
    return m_errorMessage.c_str();
}
