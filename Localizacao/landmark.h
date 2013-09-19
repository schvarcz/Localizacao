#ifndef LANDMARK_H
#define LANDMARK_H
#include "configuracaoes.h"
#include "interval.h"

class Landmark
{
public:
    Landmark();
    xyz posicao;
    interval dist;
    int id;
};

#endif // LANDMARK_H
