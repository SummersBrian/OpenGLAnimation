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

private:
    QVector<Limb> limbs;
};

#endif // BODY_H
