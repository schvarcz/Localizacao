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

