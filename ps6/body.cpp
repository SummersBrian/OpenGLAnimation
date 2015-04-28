#include "body.h"

Body::Body()
{
    limbs = QVector<Limb>();
    limbVertices = QVector<float>();
    count = 0;
}

Body::~Body()
{

}

void Body::addLimb(Limb limb) {
    limbs.append(limb);
    limbVertices.append(limb.getV1().x());
    limbVertices.append(limb.getV1().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb.getV2().x());
    limbVertices.append(limb.getV2().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb.getV3().x());
    limbVertices.append(limb.getV3().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb.getV4().x());
    limbVertices.append(limb.getV4().y());
    limbVertices.append(0.0f);
    count += 12;
}

QVector<float> Body::getLimbVertices() {
    return limbVertices;
}

int Body::getCount() {
    return count;
}
