#include "body.h"

Body::Body()
{
    limbs = QVector<Limb>();
}

Body::~Body()
{

}

void Body::addLimb(Limb limb) {
    limbs.append(limb);
}
