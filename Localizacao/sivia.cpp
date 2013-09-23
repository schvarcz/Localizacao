#include "sivia.h"

Sivia::Sivia()
{
}

iboolean Sivia::In(const Interval F, const Interval Y)
{
    //double r=0.0000000017;//1e-17;
    Interval Z= Y;//Y.inflate(r);
    if (F.is_disjoint(Z)) return false;
    if (F.is_subset(Z)) return true;
    return iboolean(iperhaps);
}

iboolean Sivia::Inside (IntervalVector X, QVector<Landmark>  landmarks, QVector<transponder> landmarksDist/*, box& V*/)
{
    int i;
    QVector<Interval> F;
    for(i=0;i<landmarks.size();i++){
        //cout<<landmarksDist[i].dist<<landmarks[i].posicao.x<<landmarks[i].posicao.y<<landmarks[i].posicao.z<<endl;
        Landmark aux=landmarks[i];
        Interval x=Interval(aux.posicao.x,aux.posicao.x);
        Interval y=Interval(aux.posicao.y,aux.posicao.y);
        Interval z=Interval(aux.posicao.z,aux.posicao.z);
        F.push_back(sqrt(sqr(X[0]-x)+sqr(X[1]-y)+sqr(X[2]-z)));//3d

    }
    iboolean result;
    double erroTransp=STD_DIST_TRANSP*SIGMA_FACTOR_TRA;
    if(F.size()>1){
        result=(In(F[0],Interval(landmarksDist[0].distNoise-erroTransp,landmarksDist[0].distNoise+erroTransp)) && In(F[1],Interval(landmarksDist[1].distNoise-erroTransp,landmarksDist[1].distNoise+erroTransp)));
        for(i=2;i<F.size();i++){
            result=(In(F[i],Interval(landmarksDist[i].distNoise-erroTransp,landmarksDist[i].distNoise+erroTransp)) && result);
        }
    }else{
        result=(In(F[0],Interval(landmarksDist[0].distNoise-erroTransp,landmarksDist[0].distNoise+erroTransp)));
    }




    //testa com caixa viajante, senão quiser que corte é só comentar essa linha
    //result=(In(X[1],V[1]) && In(X[2],V[2]) && result);

    return (result);

}


QVector <IntervalVector> Sivia::execSivia(IntervalVector box, QVector<Landmark> landmarks, QVector<transponder> landmarksDist){
    QVector <IntervalVector> caixas;

    double mxX=box[0].ub(),mxY=box[1].ub(),mxZ=box[2].ub(),mnX=box[0].lb(),mnY=box[1].lb(),mnZ=box[2].lb();


    list <IntervalVector> L;
    IntervalVector X=box;
    LargestFirst lf;

    caixas.clear();
    L.push_back (X);

    while ( !L.empty() )
    {
        IntervalVector Xaux(3);

        Xaux=L.front();   L.pop_front();

        iboolean test=Inside(Xaux,landmarks,landmarksDist);


        if (Xaux.max_diam()<EPSILON){
            caixas.push_back(Xaux);

            if(Xaux[0].lb()<mnX)mnX=Xaux[0].lb();   if(Xaux[1].lb()<mnY)mnY=Xaux[1].lb();   if(Xaux[2].lb()<mnZ)mnZ=Xaux[2].lb();
            if(Xaux[0].ub()<mxX)mxX=Xaux[0].ub();   if(Xaux[1].ub()<mxY)mxY=Xaux[1].ub();   if(Xaux[2].ub()<mxZ)mxZ=Xaux[2].ub();
        }
        else if ( test==itrue ){
            caixas.push_back(Xaux);

            if(Xaux[0].lb()<mnX)mnX=Xaux[0].lb();   if(Xaux[1].lb()<mnY)mnY=Xaux[1].lb();   if(Xaux[2].lb()<mnZ)mnZ=Xaux[2].lb();
            if(Xaux[0].ub()<mxX)mxX=Xaux[0].ub();   if(Xaux[1].ub()<mxY)mxY=Xaux[1].ub();   if(Xaux[2].ub()<mxZ)mxZ=Xaux[2].ub();
        }
        else if (test==ifalse){}
        else{
            pair <IntervalVector,IntervalVector> intervalos = lf.bisect(Xaux);//   Xaux.bisect(0.5);
            L.push_back(intervalos.first);  L.push_back(intervalos.second);
        }

   }

    return caixas;
}
