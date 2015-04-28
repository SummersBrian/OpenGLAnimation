#include "limb.h"

Limb::Limb() {
    parent = NULL;
    joint = QVector2D();
    v1 = QVector2D();
    v2 = QVector2D();
    v3 = QVector2D();
    v4 = QVector2D();
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
    if (v1.x() == x && v2.x() == x) {
        if (y == ((v2.y() - v1.y()) / 2.0f)) {
            return LEFT;
        }
    } else if (v3.x() == x && v4.x() == x) {
        if (y == ((v2.y() - v1.y()) / 2.0f)) {
            return RIGHT;
        }
    } else if (v1.y() == y && v3.y() == y) {
        if (x == ((v4.x() - v2.y()) / 2.0f)) {
            return BOTTOM;
        }
    } else if (v2.y() == y && v4.y() == y) {
        if (x == ((v4.x() - v2.y()) / 2.0f)) {
            return TOP;
        }
    }
    return NON_JOINT;
}
