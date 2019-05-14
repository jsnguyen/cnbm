#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"

typedef struct{
    double m;
    double *pv;
} body;

void vecfgrav(double *vec, body *bsa, body *bsb){
    double G=6.674e-11;
    double dir[2];
    for(int i=0;i<2;i++){
        dir[i]=bsb->pv[i]-bsa->pv[i];
    }

    double mag = sqrt(dir[0]*dir[0]+dir[1]*dir[1]);
    double f = G*bsa->m*bsb->m/(mag*mag);
    for(int i=0;i<2;i++){
        vec[i] += dir[i]/mag*f;
    }
}

void velverl(body *bsi, int nb, double td){
    body *bsf=malloc(sizeof(body)*nb);
    memcpy(bsf,bsi,sizeof(body)*nb);

    for(int i=0;i<nb;i++){
      double fgrav_init[]={0,0};
      double fgrav_fina[]={0,0};

      for(int j=0;j<nb;j++){
        if(i!=j){
          vecfgrav(fgrav_init,&bsf[i],&bsf[j]);
        }
      }

      for(int j=0;j<2;j++){
        bsi[i].pv[j]=bsf[i].pv[j]+bsf[i].pv[2+j]*td+0.5*fgrav_init[j]/bsf[i].m*td*td;
      }

      for(int j=0;j<nb;j++){
        if(i!=j){
          vecfgrav(fgrav_fina,&bsf[i],&bsf[j]);
        }
      }

      for(int j=0;j<2;j++){
        bsi[i].pv[2+j]=bsf[i].pv[2+j]+0.5*(fgrav_init[j]/bsf[i].m+fgrav_fina[j]/bsf[i].m)*td;
      }
    }

    free(bsf);
}

int main(){

    int    niter    = 1e4;
    int    nb       = 3;
    double td       = 1e4;

    body bsi[nb];
    for(int i=0;i<nb;i++){
      bsi[i].pv = malloc(4*sizeof(double));
    }

    bsi[0].m = 2e30;
    bsi[1].m = 6e24;
    bsi[2].m = 2e30;

    bsi[1].pv[0] = 1.5e11;
    bsi[1].pv[3] = 3e4;

    bsi[2].pv[0] = 0.75e11;
    bsi[2].pv[3] = 1.5e4;

    for(int i=0;i<niter;i++){
        for(int j=0;j<nb;j++){
          printf("%.4e %.4e %.4e %.4e\n",bsi[j].pv[0],bsi[j].pv[1],bsi[j].pv[2],bsi[j].pv[3]);
        }
        velverl(bsi,nb,td);
    }
    
    for(int i=0;i<nb;i++){
        free(bsi[i].pv);
    }
    printf("%li\n",sizeof(body));

    return 0;
}
