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

    //FiltroParticulas fp;
    //fp.executarFiltro(poseXYZ, yawPitchRoll, velXYZ, transponders, landmarksUsados,0);

    Sivia s;
    QVector <IntervalVector> resultSivia;
    double _x[3][2]={{mapaMinX,mapaMaxX},{mapaMinY, mapaMaxY},{mapaMinZ,mapaMaxZ}};
    IntervalVector searchSpace(3,_x);
    IntervalVector bb(3);

    for(int i=0;i<poseXYZ.size();i=i+LEITURAS_POR_TEMPO_LEITURAS){
        resultSivia=s.execSivia(searchSpace,landmarksUsados,transponders[i]);
        //searchSpace.clear();
        //searchSpace=bb;
        //caminhar caixa
        qDebug()<<resultSivia.size();
    }





Imagem im;
im.carregarImagem("../mapas/mapa1000x1000.bmp");
im.inicializarPaiter();
for(int i=0;i<resultSivia.size();i++) {
    im.desenhaCaixa2(resultSivia[i][0].lb(),resultSivia[i][0].ub(),resultSivia[i][1].lb(),resultSivia[i][1].ub(),Qt::red,Qt::NoBrush);
    //qDebug()<<resultSivia[i][0].lb()<<resultSivia[i][0].ub()<<resultSivia[i][1].lb()<<resultSivia[i][1].ub();
}
im.finalizarPainter();
im.salvarImagem("../mapas/mapaD.bmp");

    qDebug()<<"FIM";

    return a.exec();

}
