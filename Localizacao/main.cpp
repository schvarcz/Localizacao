#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>
#include "dados.h"
#include "configuracaoes.h"
#include "particula.h"
//#include "box.h"
#include "imagem.h"
#include <time.h>
#include <stdlib.h>
#include "paletacores.h"
//#include "interval.h"
#include "filtroParticulas.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    qDebug()<<"Carregar dados ...";
    //Dados do arquivo
    QVector <xyz> poseXYZ;
    QVector <xyz> yawPitchRoll;
    QVector <xyz> acelXYZ;
    QVector <xyz> velAng;
    QVector <xyz> velXYZ;
    QVector <QVector<transponder> > transponders;
    QVector <Landmark> landmarksUsados;
    Dados dados;
    dados.carregarArquivo(poseXYZ, yawPitchRoll, acelXYZ, velAng, velXYZ, transponders);
    dados.localizacaoLandmarks(landmarksUsados);


    srand(time(NULL));

    FiltroParticulas fp;
    fp.executarFiltro(poseXYZ, yawPitchRoll, velXYZ, transponders, landmarksUsados,0);


    qDebug()<<"FIM";

    return a.exec();

}
