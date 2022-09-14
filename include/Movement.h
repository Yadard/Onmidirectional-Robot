#ifndef MOVEMENT_H
#define MOVEMENT_H

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

    // stop the robot
    //@return true if everything worked well and false if everything went wrong.
    virtual bool stop();
};

/*
 * Used this video as base
 * video: https://youtu.be/wwQQnSWqB7A
 */
class Onmidirectional_3Wheels : public Movement::Base {
  public:
    Onmidirectional_3Wheels(Motor::Base *MotorA, Motor::Base *MotorB, Motor::Base *MotorC);
    ~Onmidirectional_3Wheels() = default;

    bool applySpeed(double sx, double sy);
    bool stop();

  private:
    Motor::Base *motors[3];
    mtx_type movement_matrix[3][3];
};

} // namespace Movement

#endif // MOVEMENT_H
