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

void Body::appendLimb(Limb limb) {
    /*
        Limb models shaped as quads
        v4 -- v3
        |    / |
        |   /  |
        |  /   |
        v1 -- v2

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

void Body::prependLimb(Limb limb) {
    /*
        Limb models shaped as quads
        v4 -- v3
        |    / |
        |   /  |
        |  /   |
        v1 -- v2

        need to draw two triangles, so add v3 and v1 in vertex list twice to achieve
    */
    limbs.prepend(limb);
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV1().y());
    limbVertices.prepend(limb.getV1().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV4().y());
    limbVertices.prepend(limb.getV4().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV3().y());
    limbVertices.prepend(limb.getV3().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV3().y());
    limbVertices.prepend(limb.getV3().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV2().y());
    limbVertices.prepend(limb.getV2().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb.getV1().y());
    limbVertices.prepend(limb.getV1().x());
    count += 18;

}

QVector<float> Body::getLimbVertices() {
    QVector<float> tempVertices;

    return tempVertices;
}

int Body::getCount() {
    return count;
}
