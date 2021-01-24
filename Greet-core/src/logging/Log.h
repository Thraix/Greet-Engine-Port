#pragma once

#include <logging/Logger.h>
#include <logging/LogLevel.h>
#include <string.h>

namespace Greet {
  /*
     Singleton class for logging
     */
  class Log
  {
    private:
      // Logger for the logging system
      Logger m_logger;

      // Singleton Log
      static Log m_log;
    public:

    private:
      Log();
      virtual ~Log();
    public:

      /*
       * Add a log stream to the logging system.
       */
      static void AddLogStream(LogStream* stream);

      /*
       * Remove a log stream from the logging system.
       */
      static LogStream* RemoveLogStream(const std::string& name);

      /*
       * Remove a log stream from the logging system.
       */
      static LogStream* RemoveLogStream(LogStream* stream);

      // Default logging capabilities

      /*
       * Logging at information log level.
       */
      template <typename... Args>
      static void Info(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Information(), "[Greet][INF] ", args...);
      }

      /*
       * Logging at warning log level.
       */
      template <typename... Args>
      static void Warning(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Warning(), "\e[0;33m[Greet][WRN] ", args..., "\e[m");
      }

      /*
       * Logging at error log level.
       */
      template <typename... Args>
      static void Error(const Args&... args)
      {
        m_log.m_logger.Log(LogLevel::Error(),"\e[0;31m[Greet][ERR] ",args..., "\e[m");
      }

      /*
       * Logging at user defined log level.
       */
      template <typename... Args>
      static void LogAt(const LogLevel& level, const Args&... args)
      {
        m_log.m_logger.Log(level, "[Greet]", args...);
      }
  };
}

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#if _DEBUG
#define ASSERT(x,...) \
if(!(x)) \
{ \
  Greet::Log::LogAt(Greet::LogLevel::Error(),"[ASSERT]","[",__FILENAME__, "::", __func__,":", __LINE__, "] " __VA_ARGS__);\
  abort();\
}
#else
#define ASSERT(x,...)
#endif
