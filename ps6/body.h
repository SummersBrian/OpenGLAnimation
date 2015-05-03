#ifndef BODY_H
#define BODY_H

#include "limb.h"
#include <QVector>

class Body
{
public:
    Body();
    ~Body();
    enum Leg_Forward {LEFT,RIGHT};
    void addLimb(Limb* limb);
    QVector<float> getLimbVertices();
    int getVertexCount();
    int getLimbCount();
    Limb* getLimbAt(int index);
    //void keepFootOnGround(int leaningFootIndex);
    void moveForward();

private:
    QVector<Limb*> limbs;
    int limb_count;
    int vertex_count;
    Leg_Forward forward_leg;

    void appendLimb(Limb* limb, QVector<float> &limbVertices);
    void prependLimb(Limb* limb, QVector<float> &limbVertices);
};

#endif // BODY_H
