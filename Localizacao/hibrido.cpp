#include "hibrido.h"

Hibrido::Hibrido()
{
}

void Hibrido::executarLocalizacaoHibridaContratores(IntervalVector searchSpace, QVector<xyz> poseXYZ, QVector<xyz> yawPitchRoll, QVector<xyz> velXYZ, QVector<QVector<transponder> > transponders, QVector<Landmark> landmarksUsados, int idExec){

    clock_t inicio=clock();
    std::stringstream sstr;
    sstr << idExec;
    string nome = PATH_RESULTS + sstr.str() + string("hibridoContratores.csv");
    ofstream logCaixas(nome.c_str());
    nome = PATH_RESULTS + sstr.str() + string("hibridoContratoresFP.csv");
    ofstream logParticulas(nome.c_str());


    Dados d;
    Sivia s;
    QVector<Particula> populacao;
    int movAnterior=0;
    double normalizador=0;
    FiltroParticulas fp;
    int reinicializaFP=0;


    logCaixas<<"#minx;maxx;miny;maxy;minz;maxz\n";
    logParticulas<<"#melhor.pose.x;melhor.pose.y;melhor.pose.z;melhor.ypr.x;melhor.ypr.y;melhor.ypr.z;pior.pose.x;pior.pose.y;pior.pose.z;pior.ypr.x;pior.ypr.y;pior.ypr.z;media.pose.x;media.pose.y;media.pose.z;media.ypr.x;media.ypr.y;media.ypr.z;\n";


    for(int j=0;j<poseXYZ.size();j=j+LEITURAS_POR_TEMPO_LEITURAS){

//        cout<<j<<endl;
//        Imagem im;
//        im.carregarImagem("../mapas/mapa1000x1000.bmp");
//        im.inicializarPaiter();
//        im.desenhaCaixa2(searchSpace[0].lb(),searchSpace[0].ub(),searchSpace[1].lb(),searchSpace[1].ub(),Qt::blue,Qt::NoBrush);

        //fp*************************************************************************
        normalizador=0;

        //ai*************************************************************************
        const int N=NRO_TRANSPONDERS;
        Interval bX[N];
        Interval bY[N];
        Interval bZ[N];
        Interval bD[N];
        for (int i=0; i<N; i++) {
            bX[i]=Interval(landmarksUsados[i].posicao.x);
            bY[i]=Interval(landmarksUsados[i].posicao.y);
            bZ[i]=Interval(landmarksUsados[i].posicao.z);
            bD[i]=Interval(transponders[j][i].distNoise-(STD_DIST_TRANSP*SIGMA_FACTOR_TRA),transponders[j][i].distNoise+(STD_DIST_TRANSP*SIGMA_FACTOR_TRA));
        }
        Array<Ctc> array(N);
        Variable x(3);
        for(int c=0;c<N;c++){
            Function *fun2 =new Function(x,((sqrt(sqr(x[0]-bX[c])+sqr(x[1]-bY[c])+sqr(x[2]-bZ[c]))-bD[c])));
            CtcFwdBwd *ar=new CtcFwdBwd(*fun2);
            array.set_ref(c,*ar);
        }
        CtcCompo comp(array);
        CtcFixPoint fix(comp);// O FIX POINT FICA CONTRAINDO ATÉ NÃO MUDAR MAIS SERIA COMO FAZER UM MONTE DE "comp.contract(array);"
        fix.contract(searchSpace);

        double _x[3][2]={{poseXYZ[j].x,poseXYZ[j].x},{poseXYZ[j].y, poseXYZ[j].y},{poseXYZ[j].z,poseXYZ[j].z}};
        IntervalVector p(3,_x);
        if(!p.is_subset(searchSpace)) {qDebug()<<"Robo fora da caixa!!!!!!!!";}

        logCaixas<<d.stringalizar(searchSpace[0].lb())+";"+d.stringalizar(searchSpace[0].ub())+";"+d.stringalizar(searchSpace[1].lb())+";"+d.stringalizar(searchSpace[1].ub())+";"+d.stringalizar(searchSpace[2].lb())+";"+d.stringalizar(searchSpace[2].ub())+";\n";


//        im.desenhaCaixa2(searchSpace[0].lb(),searchSpace[0].ub(),searchSpace[1].lb(),searchSpace[1].ub(),Qt::red,Qt::NoBrush);

        //fp*************************************************************************
        //se for 1a iteração criar partículas dentro do searchSpace
        if(j==0){
            for(int k=0;k<TAMANHO_POPULACAO;k++){
                Particula p;
                p.pose.x=p.fRand(searchSpace[0].lb(),searchSpace[0].ub());
                p.pose.y=p.fRand(searchSpace[1].lb(),searchSpace[1].ub());
                p.pose.z=p.fRand(searchSpace[2].lb(),searchSpace[2].ub());
                p.yawPitchRoll.x=p.fRand(yawPitchRoll[j].x-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].x+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.yawPitchRoll.y=p.fRand(yawPitchRoll[j].y-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].y+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.yawPitchRoll.z=p.fRand(yawPitchRoll[j].z-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].z+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                p.id=k;
                populacao.push_back(p);
                normalizador += populacao[populacao.size()-1].calcularPeso(transponders[j],landmarksUsados/*,limXmin,limXmax,limYmin,limYmax,limZmin,limZmax*/);
            }
        }

        //se não é a primeira iteração, então move as particulas da populacao existente
        if(j!=0){
            QVector <Particula> populacaoAux;
            std::swap(populacao,populacaoAux);
            populacao.clear();
            for(int k=0;k<populacaoAux.size();k++){
                populacaoAux[k].modeloMovimento2(velXYZ[j].xNoise,velXYZ[j].yNoise,velXYZ[j].zNoise,yawPitchRoll[j].xNoise,yawPitchRoll[j].yNoise,yawPitchRoll[j].zNoise, (yawPitchRoll[j].xNoise-yawPitchRoll[movAnterior].xNoise) ,(yawPitchRoll[j].yNoise-yawPitchRoll[movAnterior].yNoise),(yawPitchRoll[j].zNoise-yawPitchRoll[movAnterior].zNoise));

                //testa se as partículas da população estão dentro do searchSpace, eliminar as que estão fora e repoe com uma aleatoria
                if(searchSpace[0].contains(populacaoAux[k].pose.x) && searchSpace[1].contains(populacaoAux[k].pose.y) && searchSpace[2].contains(populacaoAux[k].pose.z)){
                    populacao.push_back(populacaoAux[k]);
                }
                else{
                    Particula p;
                    p.pose.x=p.fRand(searchSpace[0].lb(),searchSpace[0].ub());
                    p.pose.y=p.fRand(searchSpace[1].lb(),searchSpace[1].ub());
                    p.pose.z=p.fRand(searchSpace[2].lb(),searchSpace[2].ub());
                    p.yawPitchRoll.x=p.fRand(yawPitchRoll[j].x-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].x+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.yawPitchRoll.y=p.fRand(yawPitchRoll[j].y-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].y+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.yawPitchRoll.z=p.fRand(yawPitchRoll[j].z-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].z+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.id=k;
                    populacao.push_back(p);
                }
                normalizador += populacao[populacao.size()-1].calcularPeso(transponders[j],landmarksUsados/*,limXmin,limXmax,limYmin,limYmax,limZmin,limZmax*/);
            }
            movAnterior=j;
            populacaoAux.clear();

            if(populacao.size()<1){
                qDebug()<<"Reinicializar população";
                normalizador=0;
                for(int k=0;k<TAMANHO_POPULACAO;k++){
                    Particula p;
                    p.pose.x=p.fRand(searchSpace[0].lb(),searchSpace[0].ub());
                    p.pose.y=p.fRand(searchSpace[1].lb(),searchSpace[1].ub());
                    p.pose.z=p.fRand(searchSpace[2].lb(),searchSpace[2].ub());
                    p.yawPitchRoll.x=p.fRand(yawPitchRoll[j].x-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].x+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.yawPitchRoll.y=p.fRand(yawPitchRoll[j].y-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].y+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.yawPitchRoll.z=p.fRand(yawPitchRoll[j].z-SIGMA_FACTOR_ORI*STD_ORIENTATION,yawPitchRoll[j].z+SIGMA_FACTOR_ORI*STD_ORIENTATION);
                    p.id=k;
                    populacao.push_back(p);
                    normalizador += populacao[populacao.size()-1].calcularPeso(transponders[j],landmarksUsados/*,limXmin,limXmax,limYmin,limYmax,limZmin,limZmax*/);
                }
                reinicializaFP++;
            }
        }

        //executa roleta na população
        for(int k=0;k<populacao.size();k++){
            populacao[k].peso=populacao[k].peso/normalizador;
        }
        fp.roleta(populacao);


        //ai*************************************************************************
        //move searchSpace
        Interval vx((velXYZ[j].xNoise*TEMPO_LEITURAS)-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),(velXYZ[j].xNoise*TEMPO_LEITURAS)+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));
        Interval vy((velXYZ[j].yNoise*TEMPO_LEITURAS)-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),(velXYZ[j].yNoise*TEMPO_LEITURAS)+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));
        Interval vz((velXYZ[j].zNoise*TEMPO_LEITURAS)-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),(velXYZ[j].zNoise*TEMPO_LEITURAS)+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));
        Interval phi  (yawPitchRoll[j].xNoise-(SIGMA_FACTOR_ORI*STD_ORIENTATION),yawPitchRoll[j].xNoise+(SIGMA_FACTOR_ORI*STD_ORIENTATION));
        Interval theta(yawPitchRoll[j].yNoise-(SIGMA_FACTOR_ORI*STD_ORIENTATION),yawPitchRoll[j].yNoise+(SIGMA_FACTOR_ORI*STD_ORIENTATION));
        Interval psi  (yawPitchRoll[j].zNoise-(SIGMA_FACTOR_ORI*STD_ORIENTATION),yawPitchRoll[j].zNoise+(SIGMA_FACTOR_ORI*STD_ORIENTATION));
        s.moveCaixa( searchSpace,  vx,  vy,  vz,  phi,  theta,  psi);

        //fp*************************************************************************
        Particula piorParticula, melhorParticula, mp;
        piorParticula.peso=-1;melhorParticula.peso=-1;
        double mpx=0, mpy=0, mpz=0, a1=0, a2=0, a3=0, angx=0, angy=0, angz=0, a4=0, a5=0, a6=0;
        for(int k=0;k<populacao.size();k++){
            if(populacao[k].peso<piorParticula.peso || piorParticula.peso==-1){
                piorParticula=populacao[k];
            }
            if(populacao[k].peso>melhorParticula.peso || melhorParticula.peso==-1){
                melhorParticula=populacao[k];
            }
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
        mp.pose.x=mpx/a1;
        mp.pose.y=mpy/a2;
        mp.pose.z=mpz/a3;
        mp.yawPitchRoll.x=angx/a4;
        mp.yawPitchRoll.y=angy/a5;
        mp.yawPitchRoll.z=angz/a6;

        logParticulas<<d.stringalizar(melhorParticula.pose.x)+";"+d.stringalizar(melhorParticula.pose.y)+";"+d.stringalizar(melhorParticula.pose.z)+";"+d.stringalizar(melhorParticula.yawPitchRoll.x)+";"+d.stringalizar(melhorParticula.yawPitchRoll.y)+";"+d.stringalizar(melhorParticula.yawPitchRoll.z)+";"+d.stringalizar(piorParticula.pose.x)+";"+d.stringalizar(piorParticula.pose.y)+";"+d.stringalizar(piorParticula.pose.z)+";"+d.stringalizar(piorParticula.yawPitchRoll.x)+";"+d.stringalizar(piorParticula.yawPitchRoll.y)+";"+d.stringalizar(piorParticula.yawPitchRoll.z)+";"+d.stringalizar(mp.pose.x)+";"+d.stringalizar(mp.pose.y)+";"+d.stringalizar(mp.pose.z)+";"+d.stringalizar(mp.yawPitchRoll.x)+";"+d.stringalizar(mp.yawPitchRoll.y)+";"+d.stringalizar(mp.yawPitchRoll.z)+";"+"\n";


//        PaletaCores paletaCor;
//        double v1 = piorParticula.peso;
//        double v3 = melhorParticula.peso;
//        if(v1==v3){
//            v1=v1-1;
//            v3=v3-1;
//        }
//        for(int k=0;k<populacao.size();k++){
//            double v2 = populacao[k].peso;
//            paletaCor.updatePorcentagem((v2-v1)/(v3-v1));
//            im.desenhaElipse2(populacao[k].pose.x,populacao[k].pose.y,0.2,0.2,QColor(paletaCor.cor->red(),paletaCor.cor->green(),paletaCor.cor->blue()),Qt::NoBrush);
//            //cout<<searchSpace<<" "<<populacao[k].pose.x<<" "<<populacao[k].pose.y<<" "<<populacao[k].pose.z<<endl;
//        }
//        im.desenhaElipse2(poseXYZ[j].x,poseXYZ[j].y,0.5,0.5,Qt::black,Qt::black);
//        im.finalizarPainter();
//        im.salvarImagem("../mapasResult/"+QString::number(j)+".bmp");

//cout<<endl;
    }
    logParticulas<<"#Tempo: "+d.stringalizar(d.tempo(inicio))+"\n";
    logParticulas<<"#Inicializacoes do filtro "+d.stringalizar(reinicializaFP);
    logParticulas<<"\n#Fim";
    logCaixas<<"#Tempo: "+d.stringalizar(d.tempo(inicio));
    logCaixas<<"\n#Fim";
    logParticulas.close();
    logCaixas.close();

}