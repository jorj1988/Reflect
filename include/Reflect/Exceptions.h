#pragma once
#include <stdexcept>
#include <string>
#include <stdint.h>
#include "TupleEach.h"

namespace Reflect
{

class CallException : public std::exception
  {
public:
  ~CallException() throw()
    {
    }
  };

class OverloadException : public CallException
  {
public:
  template <typename InvHelper, typename Tuple> class OverloadHelper
    {
  public:
    template <std::size_t Idx> bool visit()
      {
      typedef typename std::tuple_element<Idx, Tuple>::type ElementType;

      options += InvHelper::template describeFunction<ElementType>() + "\n";
      return false;
      }

    std::string options;
    };

  template <typename InvHelper, typename Overloads>
      static OverloadException build(typename InvHelper::CallData data)
    {
    OverloadException excep;

    OverloadHelper<InvHelper, typename Overloads::Selection> helper;
    tupleEach<typename Overloads::Selection>(helper);

    excep.m_error = "Unable to find overload matching passed arguments '" + InvHelper::describeArguments(data) + "'\nPossibilities are: " + helper.options;
    return excep;
    }

  ~OverloadException() throw()
    {
    }

  const char* what() const throw()
    {
    return m_error.c_str();
    }

  bool operator==(const OverloadException &e) const
    {
    return m_error == e.m_error;
    }

protected:
  std::string m_error;
  };

class OverloadArgCountException : public CallException
  {
public:
  template <typename InvHelper, typename Tuple> class OverloadHelper
    {
  public:
    template <std::size_t Idx> bool visit()
      {
      typedef typename std::tuple_element<Idx, Tuple>::type ElementType;
      typedef typename ElementType::Count Count;

      if (Idx == std::tuple_size<Tuple>::value-1)
        {
        options += " or ";
        }
      else if (Idx != 0)
        {
        options += ", ";
        }

      options += std::to_string(Count::value);
      return false;
      }

    std::string options;
    };

  template <typename InvHelper, typename Overloads>
      static OverloadArgCountException build(typename InvHelper::CallData data)
    {
    OverloadArgCountException excep;

    OverloadHelper<InvHelper, typename Overloads::Selection> helper;
    tupleEach<typename Overloads::Selection>(helper);

    excep.m_error = "Unable to find overload matching passed arguments '" + InvHelper::describeArguments(data) + "'\nPossibilities are functions taking " + helper.options + " arguments";
    return excep;
    }

  ~OverloadArgCountException() throw()
    {
    }

  const char* what() const throw()
    {
    return m_error.c_str();
    }

  bool operator==(const OverloadArgCountException &e) const
    {
    return m_error == e.m_error;
    }

protected:
  std::string m_error;
  };

class ArgCountException : public CallException
  {
public:
  ArgCountException(std::size_t expected, std::size_t actual)
      : m_expected(expected),
        m_actual(actual)
    {
    m_error = "Expected " + std::to_string((uint64_t)m_expected) + " argments,  got " + std::to_string((uint64_t)m_actual) + "";
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
