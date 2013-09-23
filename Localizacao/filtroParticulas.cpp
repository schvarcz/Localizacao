#include "filtroParticulas.h"

using namespace std;

FiltroParticulas::FiltroParticulas()
{
}

double FiltroParticulas::tempo(clock_t inicio){
    clock_t fim;
    fim=clock();
    double t=(double)(fim - inicio)/CLOCKS_PER_SEC;
    return t;
}

std::string stringalizar(double x)
{
    std::ostringstream oss;
    oss << x;
    return oss.str();
}

void FiltroParticulas::executarFiltro(QVector <xyz> poseXYZ, QVector <xyz> yawPitchRoll,QVector <xyz> velXYZ,QVector <QVector<transponder> > transponders,QVector <Landmark> landmarksUsados, int idExec){
    double conti=0;
    int reinicializarFP=0;
    int nroInicializa=0;


    std::stringstream sstr;
    sstr << idExec;
    string nome = PATH_RESULTS + sstr.str() + string("fp.csv");
    ofstream logParticulas(nome.c_str());
    nome = PATH_RESULTS + sstr.str() + string("fpDist.csv");
    ofstream logParticulasDist(nome.c_str());

    logParticulas<<"#melhor.pose.x;melhor.pose.y;melhor.pose.z;melhor.ypr.x;melhor.ypr.y;melhor.ypr.z;pior.pose.x;pior.pose.y;pior.pose.z;pior.ypr.x;pior.ypr.y;pior.ypr.z;media.pose.x;media.pose.y;media.pose.z;media.ypr.x;media.ypr.y;media.ypr.z;\n";
    logParticulasDist<<"#distMelhor;distMedia;\n";


    int iteracoes=poseXYZ.size();

    clock_t inicio=clock();

    double limXmin=mapaMinX,limXmax=mapaMaxX,limYmin=mapaMinY,limYmax=mapaMaxY,limZmin=mapaMinZ,limZmax=mapaMaxZ;

    QVector  <Particula> populacao;

    int movAnterior=0;

    // FAZER USAR OS DADOS DO DATASET
    for(int i=0;i<iteracoes;i=i+LEITURAS_POR_TEMPO_LEITURAS){
        conti++;

        //logLocReal<<stringalizar(poseXYZ[i].x)+";"+stringalizar(poseXYZ[i].y)+";"+stringalizar(poseXYZ[i].z)+";"+stringalizar(yawPitchRoll[i].x)+";"+stringalizar(yawPitchRoll[i].y)+";"+stringalizar(yawPitchRoll[i].z)+"\n";



        Particula piorParticula, melhorParticula, mp;

        piorParticula.peso=-1;
        melhorParticula.peso=-1;

        double normalizador=0;

        if(i==0 || reinicializarFP==1){//cria população
            nroInicializa++;
            populacao.clear();

            for(int j=0;j<TAMANHO_POPULACAO;j++){
                Particula p;
                p.pose.x=p.fRand(mapaMinX,mapaMaxX);
                p.pose.y=p.fRand(mapaMinY,mapaMaxY);
                p.pose.z=p.fRand(mapaMinZ,mapaMaxZ);
                p.yawPitchRoll.x=p.fRand(yawPitchRoll[i].x-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[i].x+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.yawPitchRoll.y=p.fRand(yawPitchRoll[i].y-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[i].y+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.yawPitchRoll.z=p.fRand(yawPitchRoll[i].z-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[i].z+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.id=j;
                populacao.push_back(p);
            }

            if(reinicializarFP==1){
                reinicializarFP=0;
            }

            //encontra normalizador
            for(int k=0;k<populacao.size();k++){
                normalizador += populacao[k].calcularPeso(transponders[i],landmarksUsados,limXmin,limXmax,limYmin,limYmax,limZmin,limZmax);
            }

        }
        else{//mover a população
            //movimenta e encontra normalizador
            for(int k=0;k<populacao.size();k++){
                populacao[k].modeloMovimento2(velXYZ[i].xNoise,velXYZ[i].yNoise,velXYZ[i].zNoise,yawPitchRoll[i].xNoise,yawPitchRoll[i].yNoise,yawPitchRoll[i].zNoise, (yawPitchRoll[i].xNoise-yawPitchRoll[movAnterior].xNoise) ,(yawPitchRoll[i].yNoise-yawPitchRoll[movAnterior].yNoise),(yawPitchRoll[i].zNoise-yawPitchRoll[movAnterior].zNoise));
                //SE ELIMINAR, elimina aki para o normalizador ficar certo
                normalizador += populacao[k].calcularPeso(transponders[i],landmarksUsados,limXmin,limXmax,limYmin,limYmax,limZmin,limZmax);
            }
            movAnterior=i;
        }

        //normaliza e acha melhor e pior e media
        double mpx=0, mpy=0, mpz=0, a1=0, a2=0, a3=0, angx=0, angy=0, angz=0, a4=0, a5=0, a6=0;
        for(int k=0;k<populacao.size();k++){
            if(normalizador<=0){qDebug()<< "Erro: Normalizador="<<normalizador;}
            populacao[k].peso=(populacao[k].peso/normalizador);
            if(populacao[k].peso<piorParticula.peso || piorParticula.peso==-1){
                piorParticula=populacao[k];
            }
            if(populacao[k].peso>melhorParticula.peso || melhorParticula.peso==-1){
                melhorParticula=populacao[k];
            }

            //calcula particula media
            mpx=mpx+populacao[k].pose.x*populacao[k].peso;
            a1=a1+populacao[k].peso;
            mpy=mpy+populacao[k].pose.y*populacao[k].peso;
            a2=a2+populacao[k].peso;
            mpz=mpz+populacao[k].pose.z*populacao[k].peso;
            a3=a3+populacao[k].peso;
            angx=angx+populacao[k].yawPitchRoll.x*populacao[k].peso;
            a4=a4+populacao[k].peso;
            angy=angy+populacao[k].yawPitchRoll.y*populacao[k].peso;
            a5=a5+populacao[k].peso;
            angz=angz+populacao[k].yawPitchRoll.z*populacao[k].peso;
            a6=a6+populacao[k].peso;
        }

        //define particula media
        mp.pose.x=mpx/a1;
        mp.pose.y=mpy/a2;
        mp.pose.z=mpz/a3;
        mp.yawPitchRoll.x=angx/a4;
        mp.yawPitchRoll.y=angy/a5;
        mp.yawPitchRoll.z=angz/a6;


        //avisa erro, particulas iguais
        if(piorParticula.peso==melhorParticula.peso){
            qDebug()<<" Partículas com mesmo peso";
        }

        //ROLETA
        QVector <Particula> populacaoAux;
        std::swap(populacao,populacaoAux);
        for(int k=0;k<TAMANHO_POPULACAO;k++){
            int l;
            double r= (double)rand() / RAND_MAX;
            l=-1;
            do{
                l++;
                if(l>populacaoAux.size()-1)l=populacaoAux.size()-1;
                r=r-populacaoAux[l].peso;
            } while(r>0);
            populacao.push_back(populacaoAux[l]);
        }


        double dist=sqrt(pow(poseXYZ[i].x-mp.pose.x,2)+pow(poseXYZ[i].y-mp.pose.y,2)+pow(poseXYZ[i].z-mp.pose.z,2));
        double dist2=sqrt(pow(poseXYZ[i].x-melhorParticula.pose.x,2)+pow(poseXYZ[i].y-melhorParticula.pose.y,2)+pow(poseXYZ[i].z-melhorParticula.pose.z,2));

        logParticulasDist<<stringalizar(dist2)+";"+stringalizar(dist)+";\n";
        logParticulas<<stringalizar(melhorParticula.pose.x)+";"+stringalizar(melhorParticula.pose.y)+";"+stringalizar(melhorParticula.pose.z)+";"+stringalizar(melhorParticula.yawPitchRoll.x)+";"+stringalizar(melhorParticula.yawPitchRoll.y)+";"+stringalizar(melhorParticula.yawPitchRoll.z)+";"+stringalizar(piorParticula.pose.x)+";"+stringalizar(piorParticula.pose.y)+";"+stringalizar(piorParticula.pose.z)+";"+stringalizar(piorParticula.yawPitchRoll.x)+";"+stringalizar(piorParticula.yawPitchRoll.y)+";"+stringalizar(piorParticula.yawPitchRoll.z)+";"+stringalizar(mp.pose.x)+";"+stringalizar(mp.pose.y)+";"+stringalizar(mp.pose.z)+";"+stringalizar(mp.yawPitchRoll.x)+";"+stringalizar(mp.yawPitchRoll.y)+";"+stringalizar(mp.yawPitchRoll.z)+";"+"\n";

    }//iterações

    logParticulas<<"#Tempo: "+stringalizar(tempo(inicio))+"\n";
    logParticulas<<"#Inicializacoes do filtro "+stringalizar(nroInicializa);
    logParticulas<<"\n#Fim";

    logParticulas.close();
}
