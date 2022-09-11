#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

class Logger {
  public:
    enum class LogLevel { NONE, RELEASE, DEBUG };

    void setLogLevel(LogLevel log_level) { this->log_level = log_level; }

    void log(LogLevel loglevel) {
        if (static_cast<int>(loglevel) <= static_cast<int>(log_level)) {
            Serial.println();
        }
    }

    template <typename Arg, typename... Args>
    void log(LogLevel loglevel, Arg arg, Args... args) {
        if (static_cast<int>(loglevel) <= static_cast<int>(log_level)) {
            Serial.print(arg);
            log(loglevel, args...);
        }
    }

  private:
    LogLevel log_level = LogLevel::NONE;
};

#endif // LOGGER_H