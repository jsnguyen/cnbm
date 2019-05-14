#include "stdlib.h"//    -= cnbm =-
#include "stdio.h" //   by: jsnguyen
#include "math.h"  // gcc cnbm.c -o cnbm
#include "string.h"//    -lm -Wall 
typedef struct{double m , *p; } b;void a
( double *v , b *f , b *g ) { double G =
6.674e-11, d[2]; int i;for(i=0;i<2;i++){
d[i] = g->p[i] - f->p[i]; }double m=sqrt
(d[0]*d[0]+d[1] * d[1]);double h=G*f->m*
g->m/(m*m);for( i=0;i<2;i++)v[i]+=d[i]/m
*h;}int main(){int n=1e4,m= 3,t=1e4,i,j,
k;b z[m];for(i=0;i<m;i++)z[i].p= malloc(
32);z[0].m=2e30;z[1].m=6e24;z[2].m=1e10;
z[1].p[0]=1.5e11;z[1].p[3]=3e4;z[2].p[0]
=3e11;z[2].p[3]=8e3;for(i=0;i<n;i++){for
(j=0;j<m;j++) // ./cnbm > out.txt && ./p
printf("%.4e %.4e %.4e %.4e\n",z[j].p[0]
 , z[j].p[1],z[j].p[2],z[j].p[3]);b *c =
malloc( 16*m);memcpy(c,&z,16*m);for(k=0;
k<m;k++){double v[]={0,0},w[]={0,0};for(
j= 0;j<m;j++){if(k!=j)a(v,&c[k],&c[j]);}
for(j = 0;j<2;j++)z[k].p[j] = c[k].p[j]+
c[k].p[2+j]*t+ 0.5*v[j]/c[k].m*t*t; for(
j=0;j<m;j++){if(k!=j)a(w,&c[k],&c[j]); }
for(j=0;j<2;j++)z[k].p[2+j]=c[k].p[2+j]+
0.5*(v[j]/c[k].m+w[j]/c[k].m)*t;}free(c)
;}for(i=0;i<m;i++)free(z[i].p);}
