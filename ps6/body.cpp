#include "body.h"

Body::Body()
{
    limbs = QVector<Limb*>();
    limb_count = 0;
    forward_leg = Leg_Forward::LEFT;
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

void Body::moveForward() {

    Limb* leaningFoot;

    if (forward_leg == Leg_Forward::LEFT &&
            limbs[1]->getJoint(0)->getCurrRot() >= limbs[1]->getJoint(0)->getMaxRot()) {
        limbs[1]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        limbs[2]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        forward_leg = Leg_Forward::RIGHT;
    } else if (forward_leg == Leg_Forward::RIGHT &&
               limbs[2]->getJoint(0)->getCurrRot() >= limbs[2]->getJoint(0)->getMaxRot()) {
        limbs[1]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        limbs[2]->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        forward_leg = Leg_Forward::LEFT;
    }

    if (forward_leg == Leg_Forward::LEFT) {
        leaningFoot = limbs[4];
        QVector2D midFoot = QVector2D((leaningFoot->getV1().x() + leaningFoot->getV2().x())/2.0f,
                                      (leaningFoot->getV1().y() + leaningFoot->getV2().y())/2.0f);
        if (midFoot.y() != -0.6f) {
            float deltaY = -0.6f - midFoot.y();
            for(int i = 0; i < limbs.count(); i++) {
                limbs[i]->translateLimbY(deltaY);
            }
        }

        midFoot = QVector2D((leaningFoot->getV1().x() + leaningFoot->getV2().x())/2.0f,
                            (leaningFoot->getV1().y() + leaningFoot->getV2().y())/2.0f);
        Limb* l;
        leaningFoot->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l = limbs[2];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->getJoint(0)->setCurrRot(l->getJoint(0)->getCurrRot() - M_PI/405.0f);
        l = limbs[0];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/810.0f, l->getJoint(1)->getPosition(), Joint::Rotation_Direction::CCW);
        l = limbs[1];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/405.0f,l->getJoint(0)->getPosition(), Joint::Rotation_Direction::CCW);
        l->getJoint(0)->setCurrRot(l->getJoint(0)->getCurrRot() + M_PI/405.0f);
        l = limbs[3];
        l->rotateLimbAboutJoint(M_PI/810.0f,midFoot,Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/405.0f,limbs[1]->getJoint(0)->getPosition(), Joint::Rotation_Direction::CCW);
        if (limbs[1]->getJoint(0)->getCurrRot() > 0) {
            l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        } else {
            l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        }
        l->rotateLimbAboutJoint(M_PI/270.0f, 0);
    } else {
        leaningFoot = limbs[3];
        QVector2D midFoot = QVector2D((leaningFoot->getV1().x() + leaningFoot->getV2().x())/2.0f,
                                      (leaningFoot->getV1().y() + leaningFoot->getV2().y())/2.0f);
        if (midFoot.y() != -0.6f) {
            float deltaY = -0.6f - midFoot.y();
            for(int i = 0; i < limbs.count(); i++) {
                limbs[i]->translateLimbY(deltaY);
            }
        }

        midFoot = QVector2D((leaningFoot->getV1().x() + leaningFoot->getV2().x())/2.0f,
                            (leaningFoot->getV1().y() + leaningFoot->getV2().y())/2.0f);
        Limb* l;
        leaningFoot->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l = limbs[1];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->getJoint(0)->setCurrRot(l->getJoint(0)->getCurrRot() - M_PI/405.0f);
        l = limbs[0];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/810.0f, l->getJoint(1)->getPosition(), Joint::Rotation_Direction::CCW);
        l = limbs[2];
        l->rotateLimbAboutJoint(M_PI/810.0f, midFoot, Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/405.0f,l->getJoint(0)->getPosition(), Joint::Rotation_Direction::CCW);
        l->getJoint(0)->setCurrRot(l->getJoint(0)->getCurrRot() + M_PI/405.0f);
        l = limbs[4];
        l->rotateLimbAboutJoint(M_PI/810.0f,midFoot,Joint::Rotation_Direction::CW);
        l->rotateLimbAboutJoint(M_PI/405.0f,limbs[2]->getJoint(0)->getPosition(), Joint::Rotation_Direction::CCW);
        if (limbs[2]->getJoint(0)->getCurrRot() > 0) {
            l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CCW);
        } else {
            l->getJoint(0)->setRotationDirection(Joint::Rotation_Direction::CW);
        }
        l->rotateLimbAboutJoint(M_PI/270.0f, 0);

    }



}
