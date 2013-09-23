#ifndef SIVIA_H
#define SIVIA_H
#include "iboolean.h"
#include <ibex.h>
#include <QVector>
#include "landmark.h"
#include <math.h>
#include <list>
#include <QDebug>
#include "configuracaoes.h"

using namespace std;
using namespace ibex;

class Sivia
{
public:
    Sivia();

    QVector <IntervalVector> execSivia(IntervalVector box, QVector<Landmark> landmarks, QVector<transponder> landmarksDist);
    iboolean In(const Interval F, const Interval Y);
    iboolean Inside (IntervalVector X, QVector<Landmark>  landmarks, QVector<transponder> landmarksDist/*, box& V*/);
};

#endif // SIVIA_H
