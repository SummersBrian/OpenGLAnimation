#ifndef BODY_H
#define BODY_H

#include "limb.h"
#include <QVector>

class Body
{
public:
    Body();
    ~Body();
    void addLimb(Limb* limb);
    QVector<float> getLimbVertices();
    int getVertexCount();
    int getLimbCount();
    Limb* getLimbAt(int index);

private:
    QVector<Limb*> limbs;
    int limb_count;
    int vertex_count;

    void appendLimb(Limb* limb, QVector<float> &limbVertices);
    void prependLimb(Limb* limb, QVector<float> &limbVertices);
};

#endif // BODY_H
