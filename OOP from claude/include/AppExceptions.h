#ifndef APP_EXCEPTIONS_H
#define APP_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class ProjectException : public std::runtime_error {
public:
    explicit ProjectException(const std::string& message)
        : std::runtime_error(message) {}
};

class DataAccessException : public ProjectException {
public:
    explicit DataAccessException(const std::string& message)
        : ProjectException(message) {}
};

class ValidationException : public ProjectException {
public:
    explicit ValidationException(const std::string& message)
        : ProjectException(message) {}
};

#endif
