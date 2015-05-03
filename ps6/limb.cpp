#include "limb.h"
#include <qmath.h>
#include <math.h>

Limb::Limb() {
    parent = NULL;
    children = QVector<Limb*>();
    //joint = NULL;
    v1 = QVector2D();
    v2 = QVector2D();
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

bool Limb::addNullParentJoint() {
    if (joints.count() != 0) {
        return false;
    } else {
        joints.append(NULL);
        return true;
    }
}

bool Limb::addParentJoint(float x, float y) {
    if (joints.count() != 0) {
        return false;
    } else {
        joints.append(new Joint(x,y));
        return true;
    }
}

bool Limb::addChildJoint(float x, float y) {
    if (joints.count() > 0) {
        joints.append(new Joint(x,y));
        return true;
    } else {
        return false;

    }
}

QVector<Joint*> &Limb::getJoints() {
    return joints;
}

void Limb::setParent(Limb *parent) {
    this->parent = parent;
}

Limb* &Limb::getParent() {
    return parent;
}

void Limb::addChild(Limb *child) {
    children.append(child);
}

QVector<Limb *> &Limb::getChildren() {
    return children;
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

Joint* &Limb::getJoint(int index) {
    return joints[index];
}

void Limb::setJoint(Joint* joint, int index) {
    joints.replace(index,joint);
}

Joint::Joint_Side Limb::getJointSide(float x, float y) {
    if (v1.x() == x && v4.x() == x) {
        if (y == ((v4.y() - v1.y()) / 2.0f) + v1.y()) {
            return Joint::Joint_Side::LEFT;
        }
    } else if (v2.x() == x && v3.x() == x) {
        if (y == ((v3.y() - v2.y()) / 2.0f) + v2.y()) {
            return Joint::Joint_Side::RIGHT;
        }
    } else if (v1.y() == y && v2.y() == y) {
        if (x == ((v2.x() - v1.x()) / 2.0f) + v1.x()) {
            return Joint::Joint_Side::BOTTOM;
        }
    } else if (v3.y() == y && v4.y() == y) {
        if (x == ((v3.x() - v4.x()) / 2.0f) + v4.x()) {
            return Joint::Joint_Side::TOP;
        }
    }
    return Joint::Joint_Side::NON_JOINT;
}

void Limb::rotateLimbAboutJoint(float radians, int index) {
    Joint* joint = getJoint(index);
    if (joint->getRotationDirection() ==  Joint::Rotation_Direction::CCW) {
        if ( (joint->getCurrRot() + radians) > joint->getMaxRot()) {
            radians = joint->getMaxRot() - joint->getCurrRot();
        }
    }
    if (joint->getRotationDirection() == Joint::Rotation_Direction::CW) {
        radians *= -1;
    }

    QVector2D vec1, vec2, vec3, vec4;
    float x_prime, y_prime;

    vec1 = v1 - joint->getPosition();
    vec2 = v2 - joint->getPosition();
    vec3 = v3 - joint->getPosition();
    vec4 = v4 - joint->getPosition();

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
    v1 = v1 + joint->getPosition();

    v2.setX(vec2.x());
    v2.setY(vec2.y());
    v2 = v2 + joint->getPosition();

    v3.setX(vec3.x());
    v3.setY(vec3.y());
    v3 = v3 + joint->getPosition();

    v4.setX(vec4.x());
    v4.setY(vec4.y());
    v4 = v4 + joint->getPosition();

    /*
    if (!children.empty()) {
        for(int i = 0; i < children.count() ; i++) {
            Limb* child = children[i];
            child->rotateLimbAboutJoint(radians, joint->getPosition(), 0);
        }
    }
*/

    joint->setCurrRot(joint->getCurrRot() + radians);
}

void Limb::rotateLimbAboutJoint(float radians, QVector2D oJoint, int index) {
    Joint* joint = getJoint(index);
    QVector2D vec1, vec2, vec3, vec4, vec5;
    float x_prime, y_prime;

    vec1 = v1 - oJoint;
    vec2 = v2 - oJoint;
    vec3 = v3 - oJoint;
    vec4 = v4 - oJoint;
    vec5 = joint->getPosition() - oJoint;

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

    x_prime = qCos(radians) * vec5.x() - qSin(radians) * vec5.y();
    y_prime = qSin(radians) * vec5.x() + qCos(radians) * vec5.y();
    vec5.setX(x_prime);
    vec5.setY(y_prime);

    v1.setX(vec1.x());
    v1.setY(vec1.y());
    v1 = v1 + oJoint;

    v2.setX(vec2.x());
    v2.setY(vec2.y());
    v2 = v2 + oJoint;

    v3.setX(vec3.x());
    v3.setY(vec3.y());
    v3 = v3 + oJoint;

    v4.setX(vec4.x());
    v4.setY(vec4.y());
    v4 = v4 + oJoint;

    joint->getPosition().setX(vec5.x() + oJoint.x());
    joint->getPosition().setY(vec5.y() + oJoint.y());

    /*
    if (!children.empty()) {
        for(int i = 0; i < children.count() ; i++) {
            Limb* child = children[i];
            child->rotateLimbAboutJoint(radians, oJoint, 0);
        }
    }
    */
}

void Limb::rotateLimbAboutJoint(float radians, QVector2D oJoint, Joint::Rotation_Direction rotation_dir) {
    QVector2D vec1, vec2, vec3, vec4, vec5;
    float x_prime, y_prime;

    if (rotation_dir == Joint::Rotation_Direction::CW) {
        radians *= -1;
    }

    vec1 = v1 - oJoint;
    vec2 = v2 - oJoint;
    vec3 = v3 - oJoint;
    vec4 = v4 - oJoint;

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
    v1 = v1 + oJoint;

    v2.setX(vec2.x());
    v2.setY(vec2.y());
    v2 = v2 + oJoint;

    v3.setX(vec3.x());
    v3.setY(vec3.y());
    v3 = v3 + oJoint;

    v4.setX(vec4.x());
    v4.setY(vec4.y());
    v4 = v4 + oJoint;


    for (int i = 0; i < joints.count(); i++) {
        if (joints[i] != NULL) {
            vec5 = joints[i]->getPosition() - oJoint;

            x_prime = qCos(radians) * vec5.x() - qSin(radians) * vec5.y();
            y_prime = qSin(radians) * vec5.x() + qCos(radians) * vec5.y();
            vec5.setX(x_prime);
            vec5.setY(y_prime);

            joints[i]->setPosition(vec5.x() + oJoint.x(), vec5.y() + oJoint.y());
        }
    }


}

void Limb::translateLimbX(float radians, int index) {
    Joint* joint = getJoint(index);
    // v1.setX(v1.x() + qAbs(v1.y()) * qTan(radians));
    // v2.setX(v2.x() + qAbs(v2.y()) * qTan(radians));
    //v3.setX(v3.x() + qAbs(v3.y()) * qTan(radians));
    // v4.setX(v4.x() + qAbs(v4.y()) * qTan(radians));
    joint->getPosition().setX(joint->getPosition().x() + qAbs(joint->getPosition().y()) * qTan(radians));

}

void Limb::translateLimbX(float xVal) {
    //float xVal = v1.y() * qTan(radians);
    v1.setX(v1.x() + xVal);
    v2.setX(v2.x() + xVal);
    v3.setX(v3.x() + xVal);
    v4.setX(v4.x() + xVal);
    if (joints[0] != NULL) {
        for (int i = 0; i < joints.count(); i++) {
            Joint* joint = joints[i];
            joint->setPosition(joint->getPosition().x() + xVal, joint->getPosition().y());
        }
    } else {
        joints[1]->setPosition(joints[1]->getPosition().x() + xVal, joints[1]->getPosition().y());
    }
}

void Limb::translateLimbY(float yVal) {
    v1.setY(v1.y() + yVal);
    v2.setY(v2.y() + yVal);
    v3.setY(v3.y() + yVal);
    v4.setY(v4.y() + yVal);
    if (joints[0] != NULL) {
        for (int i = 0; i < joints.count(); i++) {
            Joint* joint = joints[i];
            joint->setPosition(joint->getPosition().x(), joint->getPosition().y() + yVal);
        }
    } else {
        joints[1]->setPosition(joints[1]->getPosition().x(), joints[1]->getPosition().y() + yVal);
    }
}

void Limb::foot_grounded(QVector2D midfoot) {

}
