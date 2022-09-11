#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>

namespace Event {
class Base {
  public:
    virtual void operator()() { delete this; }

    virtual ~Base() {}
};
} // namespace Event

#endif // EVENT_H