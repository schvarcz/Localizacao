#include "dados.h"
#include <QDebug>
#include "configuracaoes.h"
#include <QStringList>




Dados::Dados()
{

}

void Dados::localizacaoLandmarks(QVector<Landmark> & localizacao){

    QString dataset=DATASET_TRANSPONDERS;
    QFile file(dataset);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"Problema: não abriu o "<<dataset;


    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        line=line.trimmed();
        QStringList list =line.split(";");

        if(!QString(list[0]).contains("#")){//se não forem linhas comentada
            int count=0;
            Landmark l;
            l.posicao.x=list[count].toDouble();count++;
            l.posicao.y=list[count].toDouble();count++;
            l.posicao.z=list[count].toDouble();
            localizacao.push_back(l);

    }
    }


    //INFORMAR A LOCALIZAÇÃO DOS LANDMARKS

    //2D-4Transponders-Circle.res1607
    //    Landmark l1;
    //    l1.posicao.x=18;
    //    l1.posicao.y=-30;
    //    l1.posicao.z=3;
    //    l1.id=1;
    //    localizacao.push_back(l1);

    //    Landmark l2;
    //    l2.posicao.x=-7;
    //    l2.posicao.y=12;
    //    l2.posicao.z=17;
    //    l2.id=2;
    //    localizacao.push_back(l2);

    //    Landmark l3;
    //    l3.posicao.x=0;
    //    l3.posicao.y=20;
    //    l3.posicao.z=5;
    //    l3.id=3;
    //    localizacao.push_back(l3);

    //    Landmark l4;
    //    l4.posicao.x=-27;
    //    l4.posicao.y=-20;
    //    l4.posicao.z=10;
    //    l1.id=4;
    //    localizacao.push_back(l4);


    //inst1
    /*Landmark l1;
    l1.posicao.x=-54.845;
    l1.posicao.y= 52.115;
    l1.posicao.z=-26.181;
    l1.id=1;
    localizacao.push_back(l1);
    Landmark l2;
    l2.posicao.x=-36.739;
    l2.posicao.y= -0.684;
    l2.posicao.z=-27.752;
    l2.id=2;
    localizacao.push_back(l2);
    Landmark l3;
    l3.posicao.x= 22.738;
    l3.posicao.y= 43.308;
    l3.posicao.z=-29.530;
    l3.id=3;
    localizacao.push_back(l3);
    Landmark l4;
    l4.posicao.x=-70.747;
    l4.posicao.y= 50.364;
    l4.posicao.z=-27.836;
    l1.id=4;
    localizacao.push_back(l4);
*/

    //        //inst2
    //        Landmark l1;
    //        l1.posicao.x= 0;
    //        l1.posicao.y= 0;
    //        l1.posicao.z=-10;
    //        l1.id=1;
    //        localizacao.push_back(l1);
    //        Landmark l2;
    //        l2.posicao.x= 60;
    //        l2.posicao.y= 0;
    //        l2.posicao.z=-30;
    //        l2.id=2;
    //        localizacao.push_back(l2);
    //        Landmark l3;
    //        l3.posicao.x= 30;
    //        l3.posicao.y= 30;
    //        l3.posicao.z=-50;
    //        l3.id=3;
    //        localizacao.push_back(l3);
    //        Landmark l4;
    //        l4.posicao.x= 0;
    //        l4.posicao.y= 60;
    //        l4.posicao.z=-30;
    //        l4.id=4;
    //        localizacao.push_back(l4);
    //        Landmark l5;
    //        l5.posicao.x= 60;
    //        l5.posicao.y= 60;
    //        l5.posicao.z=-10;
    //        l5.id=5;
    //        localizacao.push_back(l5);
    //        Landmark l6;
    //        l6.posicao.x= 30;
    //        l6.posicao.y= 90;
    //        l6.posicao.z=-50;
    //        l6.id=6;
    //        localizacao.push_back(l6);
    //        Landmark l7;
    //        l7.posicao.x= 0;
    //        l7.posicao.y= 120;
    //        l7.posicao.z=-10;
    //        l7.id=7;
    //        localizacao.push_back(l7);
    //        Landmark l8;
    //        l8.posicao.x= 60;
    //        l8.posicao.y= 120;
    //        l8.posicao.z=-30;
    //        l8.id=8;
    //        localizacao.push_back(l8);
}

void Dados::carregarArquivo(QVector <xyz> & poseXYZ,
                            QVector <xyz> & yawPitchRoll,
                            QVector <xyz> & acelXYZ,
                            QVector <xyz> & velAng,
                            QVector <xyz> & velXYZ,
                            QVector < QVector <transponder> > & transponders){

    QString dataset=DATASET;
    QFile file(dataset);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug()<<"Problema: não abriu o "<<dataset;


    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        line=line.trimmed();
        QStringList list =line.split(";");

        if(!QString(list[0]).contains("#")){//se não forem linhas comentada
            int count=0;
            xyz pose,euler,vAng,vLin,acel;

            pose.x=list[count].toDouble();count++;
            pose.y=list[count].toDouble();count++;
            pose.z=list[count].toDouble();count++;

            euler.x=list[count].toDouble();count++;
            euler.y=list[count].toDouble();count++;
            euler.z=list[count].toDouble();count++;

            vAng.x=list[count].toDouble();count++;
            vAng.y=list[count].toDouble();count++;
            vAng.z=list[count].toDouble();count++;

            vLin.x=list[count].toDouble();count++;
            vLin.y=list[count].toDouble();count++;
            vLin.z=list[count].toDouble();count++;

            acel.x=list[count].toDouble();count++;
            acel.y=list[count].toDouble();count++;
            acel.z=list[count].toDouble();count++;

            QVector <transponder> trans;
            for(int i=0;i<NRO_TRANSPONDERS;i++){
                transponder t;
                t.dist=list[count].toDouble();count++;
                trans.push_back(t);
            }



            pose.xNoise=list[count].toDouble();count++;
            pose.yNoise=list[count].toDouble();count++;
            pose.zNoise=list[count].toDouble();count++;

            euler.xNoise=list[count].toDouble();count++;
            euler.yNoise=list[count].toDouble();count++;
            euler.zNoise=list[count].toDouble();count++;

            vAng.xNoise=list[count].toDouble();count++;
            vAng.yNoise=list[count].toDouble();count++;
            vAng.zNoise=list[count].toDouble();count++;

            vLin.xNoise=list[count].toDouble();count++;
            vLin.yNoise=list[count].toDouble();count++;
            vLin.zNoise=list[count].toDouble();count++;

            acel.xNoise=list[count].toDouble();count++;
            acel.yNoise=list[count].toDouble();count++;
            acel.zNoise=list[count].toDouble();count++;


            for(int i=0;i<trans.size();i++){
                trans[i].distNoise=list[count].toDouble();count++;
            }

            poseXYZ.push_back(pose);
            yawPitchRoll.push_back(euler);
            velAng.push_back(vAng);
            velXYZ.push_back(vLin);
            acelXYZ.push_back(acel);
            transponders.push_back(trans);


        }
    }
}

