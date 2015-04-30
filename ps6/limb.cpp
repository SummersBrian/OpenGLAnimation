#include "limb.h"
#include <qmath.h>
#include <math.h>

Limb::Limb() {
    parent = NULL;
    //joint = NULL;
    v1 = QVector2D();
    v2 = QVector2D();
    v3 = QVector2D();
    v4 = QVector2D();
    curr_rot = 0.0f;
    rotation_direction = Rotation_Direction::CCW;
}

Limb::Limb(Limb *parent, QVector2D joint, QVector2D jointV1, QVector2D jointV2) {
    this->parent = parent;
    this->joint = joint;
    v1 = QVector2D();
    v1.setX(jointV1.x());
    v1.setY(jointV2.y());
    v2 = QVector2D();
    v2.setX(jointV1.x());
    v2.setY(jointV2.y());
    v3 = QVector2D();
    v4 = QVector2D();
    curr_rot = 0.0f;
    rotation_direction = Rotation_Direction::CCW;
}

Limb::~Limb(){

}

void Limb::setV1(float x, float y) {
    v1.setX(x);
    v1.setY(y);
}

void Limb::setV2(float x, float y) {
    v2.setX(x);
    v2.setY(y);
}

void Limb::setV3(float x, float y) {
    v3.setX(x);
    v3.setY(y);
}

void Limb::setV4(float x, float y) {
    v4.setX(x);
    v4.setY(y);
}

void Limb::setJoint(float x, float y) {
    this->joint = QVector2D(x,y);
}

void Limb::setParent(Limb *parent) {
    this->parent = parent;
}

void Limb::setV1(QVector2D v) {
    v1.setX(v.x());
    v1.setY(v.y());
}

void Limb::setV2(QVector2D v) {
    v2.setX(v.x());
    v2.setY(v.y());
}

void Limb::setV3(QVector2D v) {
    v3.setX(v.x());
    v3.setY(v.y());
}

void Limb::setV4(QVector2D v) {
    v4.setX(v.x());
    v4.setY(v.y());
}

QVector2D Limb::getV1() {
    return v1;
}

QVector2D Limb::getV2() {
    return v2;
}

QVector2D Limb::getV3() {
    return v3;
}

QVector2D Limb::getV4() {
    return v4;
}

Limb::Joint_Side Limb::jointSide(float x, float y) {
    if (v1.x() == x && v4.x() == x) {
        if (y == ((v4.y() - v1.y()) / 2.0f) + v1.y()) {
            return LEFT;
        }
    } else if (v2.x() == x && v3.x() == x) {
        if (y == ((v3.y() - v2.y()) / 2.0f) + v2.y()) {
            return RIGHT;
        }
    } else if (v1.y() == y && v2.y() == y) {
        if (x == ((v2.x() - v1.x()) / 2.0f) + v1.x()) {
            return BOTTOM;
        }
    } else if (v3.y() == y && v4.y() == y) {
        if (x == ((v3.x() - v4.x()) / 2.0f) + v4.x()) {
            return TOP;
        }
    }
    return NON_JOINT;
}

void Limb::rotateLimbAboutJoint(float radians) {
    if (curr_rot > max_rot) {
        rotation_direction = Rotation_Direction::CW;
    }
    if (curr_rot < min_rot) {
        rotation_direction = Rotation_Direction::CCW;
    }

    if (rotation_direction == Rotation_Direction::CW) {
        radians *= -1;
    }

    QVector2D vec1, vec2, vec3, vec4;
    float x_prime, y_prime;

    vec1 = v1 - joint;
    vec2 = v2 - joint;
    vec3 = v3 - joint;
    vec4 = v4 - joint;

    //x' = cos(theta) * x - sin(theta) * y; y' = sin(theta) * x + cos(theta) * y;
    x_prime = qCos(radians) * vec1.x() - qSin(radians) * vec1.y();
    y_prime = qSin(radians) * vec1.x() + qCos(radians) * vec1.y();
    vec1.setX(x_prime);
    vec1.setY(y_prime);

    x_prime = qCos(radians) * vec2.x() - qSin(radians) * vec2.y();
    y_prime = qSin(radians) * vec2.x() + qCos(radians) * vec2.y();
    vec2.setX(x_prime);
    vec2.setY(y_prime);

    x_prime = qCos(radians) * vec3.x() - qSin(radians) * vec3.y();
    y_prime = qSin(radians) * vec3.x() + qCos(radians) * vec3.y();
    vec3.setX(x_prime);
    vec3.setY(y_prime);

    x_prime = qCos(radians) * vec4.x() - qSin(radians) * vec4.y();
    y_prime = qSin(radians) * vec4.x() + qCos(radians) * vec4.y();
    vec4.setX(x_prime);
    vec4.setY(y_prime);

    v1.setX(vec1.x());
    v1.setY(vec1.y());
    v1 = v1 + joint;

    v2.setX(vec2.x());
    v2.setY(vec2.y());
    v2 = v2 + joint;

    v3.setX(vec3.x());
    v3.setY(vec3.y());
    v3 = v3 + joint;

    v4.setX(vec4.x());
    v4.setY(vec4.y());
    v4 = v4 + joint;

    curr_rot += radians;
}

void Limb::setCurrRot(float radians) {
    curr_rot = radians;
}

float Limb::getCurrRot() {
    return curr_rot;
}

float Limb::getMaxRot() {
    return max_rot;
}

float Limb::getMinRot() {
    return min_rot;
}

void Limb::setRotationDirection(Rotation_Direction rotation_direction) {
    this->rotation_direction = rotation_direction;
}

Limb::Rotation_Direction Limb::getRotationDirection() {
    return rotation_direction;
}
