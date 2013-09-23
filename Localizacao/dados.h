#ifndef DADOS_H
#define DADOS_H
#include <QVector>
#include <QFile>
//#include "interval.h"
#include "configuracaoes.h"
#include "landmark.h"
class Dados
{
public:
    Dados();
    void carregarArquivo(QVector <xyz> & poseXYZ,
                         QVector <xyz> & yawPitchRoll,
                         QVector <xyz> & acelXYZ,
                         QVector <xyz> & velAng,
                         QVector <xyz> & velXYZ,
                         QVector < QVector <transponder> > & transponders);

    void localizacaoLandmarks(QVector <Landmark> & localizacao);


};

#endif // DADOS_H
