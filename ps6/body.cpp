#include "body.h"

Body::Body()
{
    limbs = QVector<Limb*>();
    limb_count = 0;
}

Body::~Body()
{

}

void Body::addLimb(Limb* limb) {
    limbs.append(limb);
    limb_count++;
    vertex_count += 18;
}

void Body::appendLimb(Limb* limb, QVector<float> &limbVertices) {
    /*
        Limb models shaped as quads
        v4 -- v3
        |    / |
        |   /  |
        |  /   |
        v1 -- v2

        need to draw two triangles, so add v3 and v1 in vertex list twice to achieve
    */
    limbVertices.append(limb->getV1().x());
    limbVertices.append(limb->getV1().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb->getV2().x());
    limbVertices.append(limb->getV2().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb->getV3().x());
    limbVertices.append(limb->getV3().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb->getV3().x());
    limbVertices.append(limb->getV3().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb->getV4().x());
    limbVertices.append(limb->getV4().y());
    limbVertices.append(0.0f);
    limbVertices.append(limb->getV1().x());
    limbVertices.append(limb->getV1().y());
    limbVertices.append(0.0f);
}

void Body::prependLimb(Limb* limb, QVector<float> &limbVertices) {
    /*
        Limb models shaped as quads
        v4 -- v3
        |    / |
        |   /  |
        |  /   |
        v1 -- v2

        need to draw two triangles, so add v3 and v1 in vertex list twice to achieve
    */
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV1().y());
    limbVertices.prepend(limb->getV1().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV4().y());
    limbVertices.prepend(limb->getV4().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV3().y());
    limbVertices.prepend(limb->getV3().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV3().y());
    limbVertices.prepend(limb->getV3().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV2().y());
    limbVertices.prepend(limb->getV2().x());
    limbVertices.prepend(0.0f);
    limbVertices.prepend(limb->getV1().y());
    limbVertices.prepend(limb->getV1().x());
}

QVector<float> Body::getLimbVertices() {
    QVector<float> tempVertices;

    for (int i = 0; i < limb_count; i++) {
        prependLimb(limbs[i], tempVertices);
    }

    return tempVertices;
}

int Body::getVertexCount() {
    return vertex_count;
}

int Body::getLimbCount() {
    return limb_count;
}

Limb* Body::getLimbAt(int index) {
    return limbs[index];
}
