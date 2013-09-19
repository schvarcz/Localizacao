#ifndef SIVIA_H
#define SIVIA_H
#include "interval.h"
#include "box.h"
#include <QVector>
#include "iboolean.h"
#include "landmark.h"
#include "imagem.h"

class Sivia
{
public:
    box big;
    Sivia();
    iboolean Inside (box& X, QVector<Landmark>  landmarks/*, box& V*/);
    QVector<box> execSivia(double boxIniMinX,double boxIniMaxX,double boxIniMinY,double boxIniMaxY,double boxIniMinZ,double boxIniMaxZ, QVector<Landmark> landmarks);

    void movimentaCaixa(double & xi,double & xs,double & yi,double & ys,double & zi,double & zs,double vx, double vy, double vz, double phi, double theta, double psi);
    //void movimentaCaixa2(double &xi, double &xs, double &yi, double &ys, double &zi, double &zs,double vx, double vy, double vz);

};

#endif // SIVIA_H
