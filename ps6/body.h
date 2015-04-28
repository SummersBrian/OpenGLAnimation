#ifndef BODY_H
#define BODY_H

#include "limb.h"
#include <QVector>

class Body
{
public:
    Body();
    ~Body();
    void addLimb(Limb limb);
    QVector<float> getLimbVertices();
    int getCount();

private:
    QVector<Limb> limbs;
    QVector<float> limbVertices;
    int count;
};

#endif // BODY_H
