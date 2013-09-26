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
#include "dados.h"

using namespace std;
using namespace ibex;

class Sivia
{
public:
    Sivia();

    double tempo(clock_t inicio);

    QVector <IntervalVector> execSivia(IntervalVector& box, QVector<Landmark> landmarks, QVector<transponder> landmarksDist);
    iboolean In(const Interval F, const Interval Y);
    iboolean Inside (IntervalVector X, QVector<Landmark>  landmarks, QVector<transponder> landmarksDist/*, box& V*/);
    void moveCaixa(IntervalVector& box, Interval vx, Interval vy, Interval vz, Interval phi, Interval theta, Interval psi);
    void executarLocalizacaoSivia1(IntervalVector searchSpace, QVector <xyz> poseXYZ, QVector <xyz> yawPitchRoll,QVector <xyz> velXYZ, QVector <QVector<transponder> > transponders,QVector <Landmark> landmarksUsados, int idExec);
};

#endif // SIVIA_H
