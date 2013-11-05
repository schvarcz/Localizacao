//#ifndef CONFIGURACAOES_H
#define CONFIGURACAOES_H
#pragma once


#define DATASET "../trajetoriasDissertacao/ambiente1traj1.log";
#define DATASET_TRANSPONDERS "../transpondersAmbiente1.log"
#define MAPA "../mapas/mapa1000x1000.bmp";
#define PATH_RESULTS "../resultados/"


struct xyz{
    double x;
    double y;
    double z;
    double xNoise;
    double yNoise;
    double zNoise;
};

struct transponder{
    double dist;
    double distNoise;
};
//#define DIMENSOES 2

//inst1
#define mapaMinX -200 //-100
#define mapaMaxX 200
#define mapaMinY -200
#define mapaMaxY 200
#define mapaMinZ -80 //-40
#define mapaMaxZ 1
#define NRO_TRANSPONDERS 1

//inst2
//#define mapaMinX -70 //-50
//#define mapaMaxX 200
//#define mapaMinY -70
//#define mapaMaxY 220 //200
//#define mapaMinZ -80
//#define mapaMaxZ 10
//#define NRO_TRANSPONDERS 8

#define EPSILON 0.5

#define TEMPO_LEITURAS 1 //*1=uma leitura a cada 1s /0.1 =uma leitura a cada 0.1s
#define LEITURAS_POR_TEMPO_LEITURAS 10 //10 = uma leitura por segundo

//#define SIGMA_FACTOR 0.5
#define SIGMA_FACTOR_VEL 4      //influencia junto com STD_VEL_DOPPLER para achar o erro na velocidade
#define SIGMA_FACTOR_ORI 4      //influencia junto com STD_ORIENTATION para achar o erro na orientacao
#define SIGMA_FACTOR_TRA 4      //influencia junto com STD_DIST_TRANSP para achar o erro na dist dos transponders
#define STD_ORIENTATION 0.02    //desvio padr�o dado na simulacao
#define STD_VEL_DOPPLER 0.04    //desvio padr�o dado na simulacao
#define STD_DIST_TRANSP 0.3    //desvio padr�o dado na simulacao

#define TAMANHO_POPULACAO 1000
#define DESVIO_PADRAO_PESO_PARTICULAS 25 //ou um desvio alto, ou usa soma nos pesos, ou um desvio baixo mas com uma popula��o tamanho monstro
