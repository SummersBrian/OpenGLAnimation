#ifndef JOINT_H
#define JOINT_H
#include <qvector2d.h>
class Joint
{
public:
    Joint(float x, float y);
    ~Joint();

    enum Rotation_Direction {CW, CCW};
    enum Joint_Side{LEFT, RIGHT, TOP, BOTTOM, NON_JOINT};

    void setCurrRot(float rotation);
    void setMaxRot(float max_rot);
    void setMinRot(float min_rot);
    void setPosition(float x, float y);
    void setRotationDirection(Rotation_Direction rotation_direction);
    void setJointSide(Joint_Side joint_side);

    float getCurrRot();
    float getMaxRot();
    float getMinRot();
    QVector2D &getPosition();
    Rotation_Direction getRotationDirection();
    Joint_Side getJointSide();

private:
    float curr_rot;
    float max_rot;
    float min_rot;
    QVector2D position;
    Rotation_Direction rotation_direction;
    Joint_Side joint_side;
};

#endif // JOINT_H
