#ifndef HIBRIDO_H
#define HIBRIDO_H
#include "sivia.h"
#include "configuracaoes.h"
#include <time.h>
#include <ibex.h>
#include "particula.h"
#include "filtroParticulas.h"
#include "paletacores.h"
#include <iostream>

using namespace  std;
using namespace ibex;

class Hibrido
{
public:
    Hibrido();
    void executarLocalizacaoHibridaContratores(IntervalVector searchSpace, QVector<xyz> poseXYZ, QVector<xyz> yawPitchRoll, QVector<xyz> velXYZ, QVector<QVector<transponder> > transponders, QVector<Landmark> landmarksUsados, int idExec);
    void executarLocalizacaoHibridaSivia1(IntervalVector searchSpace, QVector<xyz> poseXYZ, QVector<xyz> yawPitchRoll, QVector<xyz> velXYZ, QVector<QVector<transponder> > transponders, QVector<Landmark> landmarksUsados, int idExec);
    void executarLocalizacaoHibridaSivia2(IntervalVector searchSpace, QVector<xyz> poseXYZ, QVector<xyz> yawPitchRoll, QVector<xyz> velXYZ, QVector<QVector<transponder> > transponders, QVector<Landmark> landmarksUsados, int idExec);
};

#endif // HIBRIDO_H
