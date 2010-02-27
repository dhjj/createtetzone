#ifndef Error_h__
#define Error_h__

#include <exception>
#include <string>

/**
 * Simple exception class that all other classes use for error handling (out of memory,
 * invalid zones, etc.).
 */
class Error : public std::exception
{
public:
    Error(const std::string& message);
    ~Error() throw();

    virtual const char* what() const throw();
private:
    std::string m_errorMessage;
};

#endif // Error_h__
