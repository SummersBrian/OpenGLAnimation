#ifndef LIMB
#define LIMB

#include "joint.h"
#include <QVector2D>
#include <QVector>
#include <qmath.h>
class Limb
{

public:
    Limb();
    ~Limb();

    void setV1(float x, float y);
    void setV2(float x, float y);
    void setV3(float x, float y);
    void setV4(float x, float y);

    void setV1(QVector2D v);
    void setV2(QVector2D v);
    void setV3(QVector2D v);
    void setV4(QVector2D v);

    bool addNullParentJoint();
    bool addParentJoint(float x, float y);
    bool addChildJoint(float x, float y);
    void setParent(Limb* parent);
    void addChild(Limb* child);
    void setJoint(Joint* joint, int index);

    QVector2D getV1();
    QVector2D getV2();
    QVector2D getV3();
    QVector2D getV4();
    QVector<Limb*> &getChildren();
    QVector<Joint*> &getJoints();
    Joint* &getJoint(int index);
    Limb* &getParent();
    Joint::Joint_Side getJointSide(float x, float y);

    void rotateLimbAboutJoint(float radians, int index);
    void rotateLimbAboutJoint(float radians, QVector2D joint, int index);
    void translateLimbX(float radians, int index);
    void translateLimbY(QVector2D ground_point, float hypo_length, float startY, int index);

private:
    QVector2D v1;
    QVector2D v2;
    QVector2D v3;
    QVector2D v4;
    QVector<Joint*> joints;
    Limb* parent;
    QVector<Limb*> children;
};

#endif // Limb

