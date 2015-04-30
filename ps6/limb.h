#ifndef LIMB
#define LIMB

#include <QVector2D>
#include <qmath.h>
class Limb
{

public:
    Limb();
    Limb(Limb *parent, QVector2D joint, QVector2D jointV1, QVector2D jointV2);
    ~Limb();
    enum Rotation_Direction {CW, CCW};
    void setV1(float x, float y);
    void setV2(float x, float y);
    void setV3(float x, float y);
    void setV4(float x, float y);
    void setV1(QVector2D v);
    void setV2(QVector2D v);
    void setV3(QVector2D v);
    void setV4(QVector2D v);
    void setJoint(float x, float y);
    void setParent(Limb* parent);
    void setCurrRot(float radians);
    void setRotationDirection(Rotation_Direction rotation_direction);

    QVector2D getV1();
    QVector2D getV2();
    QVector2D getV3();
    QVector2D getV4();
    float getCurrRot();
    float getMaxRot();
    float getMinRot();
    Limb::Rotation_Direction getRotationDirection();


    enum Joint_Side{LEFT, RIGHT, TOP, BOTTOM, NON_JOINT};
    Joint_Side jointSide(float x, float y);

    void rotateLimbAboutJoint(float radians);

private:
    QVector2D v1;
    QVector2D v2;
    QVector2D v3;
    QVector2D v4;
    QVector2D joint;
    Limb* parent;
    float max_rot = M_PI/8.0f;
    float min_rot = -M_PI/8.0f;
    float curr_rot;
    Rotation_Direction rotation_direction;

};

#endif // Limb

