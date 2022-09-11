#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <Event.h>
#include <MatrixMath.h>
#include <Motor.h>

namespace Movement {
class Base {
  public:
    virtual ~Base(){};

    //@param sx: component x of the speed vector.
    //@param sy: component y of the speed vector.
    //@return true if everything worked well and false if everything went wrong.
    virtual bool applySpeed(double sx, double sy);

    //@param deegres: rotate the robot in the deegres specified.
    //@return true if everything worked well and false if everything went wrong.
    virtual bool rotate(uint8_t deegres);

    // stop the robot
    //@return true if everything worked well and false if everything went wrong.
    virtual bool stop();
};

/*
 * Used this video as base to make this piece o junk move
 * video: https://youtu.be/wwQQnSWqB7A
 */
class Onmidirectional_3Wheels : public Movement::Base {
  public:
    Onmidirectional_3Wheels(Motor::Base *MotorA, Motor::Base *MotorB, Motor::Base *MotorC);
    ~Onmidirectional_3Wheels() = default;

    bool applySpeed(double sx, double sy);
    bool rotate(uint8_t deegres);
    bool stop();

  private:
    Motor::Base *motors[3];
    mtx_type movement_matrix[3][3];
};

} // namespace Movement

namespace Event {
class Movement : public Event::Base {
  public:
    Movement(::Movement::Base *mov, double fx, double fy);
    void operator()() override;
    ~Movement() override;

  private:
    ::Movement::Base *mov;
    double fx, fy;
};
} // namespace Event

#endif // MOVEMENT_H
