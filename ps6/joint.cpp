#include "joint.h"
#include <qmath.h>

Joint::Joint(float x, float y)
{
    curr_rot = 0.0f;
    max_rot = M_PI/12.0f;
    min_rot = -M_PI/12.0f;
    position = QVector2D(x,y);
    rotation_direction = Rotation_Direction::CCW;
    joint_side = Joint_Side::NON_JOINT;
}

Joint::~Joint()
{

}

void Joint::setCurrRot(float rotation) {
    curr_rot = rotation;
}

void Joint::setMaxRot(float max_rot) {
    this->max_rot = max_rot;
}

void Joint::setMinRot(float min_rot) {
    this->min_rot = min_rot;
}

void Joint::setPosition(float x, float y) {
    position = QVector2D(x,y);
}

void Joint::setRotationDirection(Rotation_Direction rotation_direction) {
    this->rotation_direction = rotation_direction;
}

void Joint::setJointSide(Joint_Side joint_side) {
    this->joint_side = joint_side;
}

float Joint::getCurrRot() {
    return curr_rot;
}

float Joint::getMaxRot() {
    return max_rot;
}

float Joint::getMinRot() {
    return min_rot;
}

QVector2D &Joint::getPosition() {
    return position;
}

Joint::Rotation_Direction Joint::getRotationDirection() {
    return rotation_direction;
}

Joint::Joint_Side Joint::getJointSide() {
    return joint_side;
}

