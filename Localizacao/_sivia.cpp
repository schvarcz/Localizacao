#include "sivia.h"

Sivia::Sivia()
{
}

iboolean Sivia::Inside (box& X, QVector<Landmark>  landmarks/*, box& V*/)
{
    int i;
    vector<interval> F;
    for(i=0;i<landmarks.size();i++){
        Landmark aux=landmarks[i];
        interval x=interval(aux.posicao.x,aux.posicao.x);
        interval y=interval(aux.posicao.y,aux.posicao.y);
        interval z=interval(aux.posicao.z,aux.posicao.z);
//        if(DIMENSOES==2){
//            F.push_back(Sqrt(Sqr(X[1]-x)+Sqr(X[2]-y)));
//        }else if(DIMENSOES==3){
            F.push_back(Sqrt(Sqr(X[1]-x)+Sqr(X[2]-y)+Sqr(X[3]-z)));//3d
//        }
    }
    iboolean result;
    if(F.size()>1){
        result=(In(F[0],landmarks[0].dist) && In(F[1],landmarks[1].dist));
        for(i=2;i<F.size();i++){
            result=(In(F[i],landmarks[i].dist) && result);
        }
    }else{
        result=(In(F[0],landmarks[0].dist));
    }

    //testa com caixa viajante, senão quiser que corte é só comentar essa linha
    //result=(In(X[1],V[1]) && In(X[2],V[2]) && result);

    return (result);
};


QVector <box> Sivia::execSivia(double boxIniMinX,double boxIniMaxX,double boxIniMinY,double boxIniMaxY,double boxIniMinZ,double boxIniMaxZ, QVector<Landmark> landmarks){
    QVector <box> caixas;
    vector<int>  aux;
    //double bbxi=-1,bbxs=-1,bbyi=-1,bbys=-1,bbzi=-1,bbzs=-1;


    list<box> L;
    box X(interval(boxIniMinX,boxIniMaxX),interval(boxIniMinY,boxIniMaxY),interval(boxIniMinZ,boxIniMaxZ));//3d
    caixas.clear();
    L.push_back (X);
    while ( !L.empty() )
    {
        box Xaux;
        Xaux.s=-1;
        Xaux=L.front();   L.pop_front();
        iboolean test=Inside(Xaux,landmarks);

        if (Xaux.Width()<EPSILON){
            Xaux.s=0;
            caixas.push_back(Xaux);
            aux.push_back(0);
        }
        else if ( test==itrue ){
            Xaux.s=1;
            caixas.push_back(Xaux);
            aux.push_back(1);
        }
        else if (test==ifalse)     {}
        else{
            box X1(2);  box X2(2);
            Bisect (Xaux,X1,X2);
            L.push_back(X1);  L.push_back(X2);
        }
    }

    for(int i=0;i<caixas.size();i++){
        caixas[i].s=aux[i];
    }

    return caixas;
}

void Sivia::movimentaCaixa(double &xi, double &xs, double &yi, double &ys, double &zi, double &zs,double vx, double vy, double vz, double phi, double theta, double psi){

    //double eOri=SIGMA_FACTOR_ORI, eVel=SIGMA_FACTOR_VEL;

    vx=vx*TEMPO_LEITURAS;
    vy=vy*TEMPO_LEITURAS;
    vz=vz*TEMPO_LEITURAS;

//    interval Iphi  (phi-(eOri*phi),phi+(eOri*phi));
//    interval Itheta(theta-(eOri*theta),theta+(eOri*theta));
//    interval Ipsi  (psi-(eOri*psi),psi+(eOri*psi));
    interval Iphi  (phi-(SIGMA_FACTOR_ORI*STD_ORIENTATION),phi+(SIGMA_FACTOR_ORI*STD_ORIENTATION));
    interval Itheta(theta-(SIGMA_FACTOR_ORI*STD_ORIENTATION),theta+(SIGMA_FACTOR_ORI*STD_ORIENTATION));
    interval Ipsi  (psi-(SIGMA_FACTOR_ORI*STD_ORIENTATION),psi+(SIGMA_FACTOR_ORI*STD_ORIENTATION));

    interval J[6][6];
    J[0][0]= Cos(Ipsi)*Cos(Itheta);
    J[0][1]=-Sin(Ipsi)*Cos(Iphi)+Cos(Ipsi)*Sin(Itheta)*Sin(Iphi);
    J[0][2]= Sin(Ipsi)*Sin(Iphi)+Cos(Ipsi)*Sin(Itheta)*Cos(Iphi);
    J[1][0]= Sin(Ipsi)*Cos(Itheta);
    J[1][1]= Cos(Ipsi)*Cos(Itheta)+Sin(Iphi)*Sin(Itheta)*Sin(Ipsi);
    J[1][2]=-Cos(Ipsi)*Sin(Iphi)+Sin(Itheta)*Sin(Ipsi)*Cos(Iphi);
    J[2][0]=-Sin(Itheta);
    J[2][1]= Cos(Itheta)*Sin(Iphi);
    J[2][2]= Cos(Itheta)*Cos(Iphi);

    interval t[3];
    t[0]=interval(vx-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),vx+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));
    t[1]=interval(vy-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),vy+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));
    t[2]=interval(vz-(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER),vz+(SIGMA_FACTOR_VEL*STD_VEL_DOPPLER));

    interval x(xi,xs);
    interval y(yi,ys);
    interval z(zi,zs);
    x=x+t[0]*J[0][0]+t[1]*J[0][1]+t[2]*J[0][2];
    y=y+t[0]*J[1][0]+t[1]*J[1][1]+t[2]*J[1][2];
    z=z+t[0]*J[2][0]+t[1]*J[2][1]+t[2]*J[2][2];

    xi=x.inf;yi=y.inf;zi=z.inf;
    xs=x.sup;ys=y.sup;zs=z.sup;

}

/*
void Sivia::movimentaCaixa2(double &xi, double &xs, double &yi, double &ys, double &zi, double &zs,double vx, double vy, double vz){
    double eOri=SIGMA_FACTOR, eVel=SIGMA_FACTOR;
    vx=vx*TEMPO_LEITURAS;
    vy=vy*TEMPO_LEITURAS;
    vz=vz*TEMPO_LEITURAS;
    interval Iphi(-M_PI,M_PI);
    interval Itheta(-M_PI,M_PI);
    interval Ipsi(-M_PI,M_PI);
//    interval Iphi  (phi-(eOri*phi),phi+(eOri*phi));
//    interval Itheta(theta-(eOri*theta),theta+(eOri*theta));
//    interval Ipsi  (psi-(eOri*psi),psi+(eOri*psi));

    interval J[6][6];
    J[0][0]= Cos(Ipsi)*Cos(Itheta);
    J[0][1]=-Sin(Ipsi)*Cos(Iphi)+Cos(Ipsi)*Sin(Itheta)*Sin(Iphi);
    J[0][2]= Sin(Ipsi)*Sin(Iphi)+Cos(Ipsi)*Sin(Itheta)*Cos(Iphi);
    J[1][0]= Sin(Ipsi)*Cos(Itheta);
    J[1][1]= Cos(Ipsi)*Cos(Itheta)+Sin(Iphi)*Sin(Itheta)*Sin(Ipsi);
    J[1][2]=-Cos(Ipsi)*Sin(Iphi)+Sin(Itheta)*Sin(Ipsi)*Cos(Iphi);
    J[2][0]=-Sin(Itheta);
    J[2][1]= Cos(Itheta)*Sin(Iphi);
    J[2][2]= Cos(Itheta)*Cos(Iphi);
    interval t[3];
    t[0]=interval(-vx+(eVel*vx),vx+(eVel*vx));
    t[1]=interval(-vy+(eVel*vy),vy+(eVel*vy));
    t[2]=interval(-vz+(eVel*vz),vz+(eVel*vz));
    interval x(xi,xs);
    interval y(yi,ys);
    interval z(zi,zs);
    x=x+t[0]*J[0][0]+t[1]*J[0][1]+t[2]*J[0][2];
    y=y+t[0]*J[1][0]+t[1]*J[1][1]+t[2]*J[1][2];
    z=z+t[0]*J[2][0]+t[1]*J[2][1]+t[2]*J[2][2];
    xi=x.inf;yi=y.inf;zi=z.inf;
    xs=x.sup;ys=y.sup;zs=z.sup;
}
*/
