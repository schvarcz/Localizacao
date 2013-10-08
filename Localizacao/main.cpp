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
#include "sivia.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;



std::string stringalizar(double x)
{
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

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
    dados.localizacaoLandmarks(landmarksUsados);
    dados.carregarArquivo(poseXYZ, yawPitchRoll, acelXYZ, velAng, velXYZ, transponders);



    srand(time(NULL));


    for(int i=0;i<1;i++){


        //qDebug()<<"Iniciar Filtro de Partículas"<<i;
        //FiltroParticulas fp;
        //fp.executarFiltro(poseXYZ, yawPitchRoll, velXYZ, transponders, landmarksUsados,i);

        //qDebug()<<"Iniciar SIVIA1"<<i;
        Sivia s;
        double _x[3][2]={{mapaMinX,mapaMaxX},{mapaMinY, mapaMaxY},{mapaMinZ,mapaMaxZ}};
        IntervalVector searchSpace(3,_x);
        //s.executarLocalizacaoSivia1(searchSpace,poseXYZ,yawPitchRoll,velXYZ,transponders,landmarksUsados,i);



//        s.executarLocalizacaoContratores(searchSpace,poseXYZ,yawPitchRoll,velXYZ,transponders,landmarksUsados,i);


        s.executarLocalizacaoSivia2(searchSpace,poseXYZ,yawPitchRoll,velXYZ,transponders,landmarksUsados,i);


    }

    qDebug()<<"FIM";

    return a.exec();

}
