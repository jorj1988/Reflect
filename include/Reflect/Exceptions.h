#pragma once
#include <stdexcept>

namespace Reflect
{

class CallException : public std::exception
  {
public:
  ~CallException() throw()
    {
    }
  };

class ArgCountException : public CallException
  {
public:
  ArgCountException(std::size_t expected, std::size_t actual)
      : m_expected(expected),
        m_actual(actual)
    {
    m_error = "Expected " + std::to_string(m_expected) + " argments,  got " + std::to_string(m_actual) + "";
    }

  ~ArgCountException() throw()
    {
    }

  const char* what() const throw()
    {
    return m_error.c_str();
    }

  bool operator==(const ArgCountException &e) const
    {
    return m_expected == e.m_expected && m_actual == e.m_actual;
    }

protected:
  std::string m_error;
  std::size_t m_expected;
  std::size_t m_actual;
  };

}
