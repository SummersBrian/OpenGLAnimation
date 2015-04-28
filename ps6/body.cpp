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
    /*
        Limb models shaped ad quads
        v4 ------- v3
        |          |
        v1 ------- v2

        need to draw two triangles, so add v3 and v1 in vertex list twice to achieve
    */
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
    limbVertices.append(limb.getV3().x());
    limbVertices.append(limb.getV3().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb.getV4().x());
    limbVertices.append(limb.getV4().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb.getV1().x());
    limbVertices.append(limb.getV1().y());
    limbVertices.append(0.0f);
    count += 18;
}

QVector<float> Body::getLimbVertices() {
    return limbVertices;
}

int Body::getCount() {
    return count;
}
