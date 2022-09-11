#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <MatrixMath.h>
#include <Motor.h>

namespace Movement {
class Base {
  public:
    virtual ~Base();

    //@param speed_module: the speed module
    //@param angle: angle of the speed.
    //@return true if everything worked well and false if everything went wrong.
    virtual bool applySpeed(uint32_t speed_module, uint16_t angle);

    //@param deegres: rotate the robot in the deegres specified.
    //@return true if everything worked well and false if everything went wrong.
    virtual bool rotate(uint8_t deegres);

    // stop the robot
    //@return true if everything worked well and false if everything went wrong.
    virtual bool stop();
};

class Onmidirectional_3Wheels : public Movement::Base {
  public:
    Onmidirectional_3Wheels(Motor::Base *MotorA, Motor::Base *MotorB, Motor::Base *MotorC);

    bool applySpeed(uint32_t speed_module, uint16_t angle);
    bool rotate(uint8_t deegres);
    bool stop();

  private:
    Motor::Base *motors[3];
    mtx_type movement_matrix[3][3];
};

} // namespace Movement

#endif // MOVEMENT_H
