#include "stdlib.h"
#include "stdio.h"

static inline double get_rand_between(double a, double b) {
    assert(a<b);
    return a+((double)rand() / (double)RAND_MAX)*(b-a);
}

static inline void random_bodies(body *bs, int nb, double mass_low, double mass_up, double vel_low, double vel_up, double cubelen){
    for(int i=0;i<nb;i++){
        body_set_id(&bs[i],i);
        body_set_mass(&bs[i],get_rand_between(mass_low,mass_up));
        double pos[3];
        double vel[]={0,0,0};
        for(int j=0;j<2;j++){
            pos[j]=get_rand_between(-cubelen/2.0,cubelen/2.0);
            vel[j]=get_rand_between(vel_low,vel_up);
        }
        pos[2]=0;
        vel[2]=0;
        body_set_pos(&bs[i],pos);
        body_set_vel(&bs[i],vel);
    }
}

static inline void write_params(const char* fn, const int niter, const int nb, const double td, const double mass_low, const double mass_up, const double vel_low, const double vel_up, const double cubelen){
    FILE *f;
    f = fopen(fn,"w");
    fprintf(f,"%i\n",niter);
    fprintf(f,"%i\n",nb);
    fprintf(f,"%f\n",td);
    fprintf(f,"%f\n",mass_low);
    fprintf(f,"%f\n",mass_up);
    fprintf(f,"%f\n",vel_low);
    fprintf(f,"%f\n",vel_up);
    fprintf(f,"%f\n",cubelen);
    fclose(f);
}

int main(int argc, char* argv[]){
    if(argc!=2){
        printf("Usage: ./sim.exe <config filename>\n");
        return(1);
    }

    srand((unsigned)time(NULL));
    rand();

    config *conf=NULL;
    conf = config_create();
    config_read(conf,argv[1]);
    config_print(conf);

    const char*  fn       = conf->fn; 
    const int    n_thread = conf->n_thread;
    const int    niter    = conf->niter;
    const int    nb       = conf->nb;
    const double td       = conf->td;
    const double mass_low = conf->mass_low*SOL_MASS;
    const double mass_up  = conf->mass_up*SOL_MASS;
    const double vel_low  = conf->vel_low;
    const double vel_up   = conf->vel_up;
    const double cubelen  = conf->cubelen*PARSEC;

    body bs[nb];
    for(int i=0;i<nb;i++){
        body_init(&bs[i]);
    }
    random_bodies(bs,nb,mass_low,mass_up,vel_low,vel_up,cubelen);
    write_params(fn,niter,nb,td,mass_low,mass_up,vel_low,vel_up,cubelen);
    posvel nprop[nb];

    for(int i=0;i<niter;i++){
        begin_loop_t = clock();

        for(int j=0;j<nb;j++){
            body_write(bs[j],fn);
        }

        for(int j=0;j<nb;j++){
            nprop[j] = apply_velverl(&bs[j], bs, nb, td);
            body_set_pos(&bs[j],nprop[j].pos);
            body_set_vel(&bs[j],nprop[j].vel);
            for(int k=0;k<3;k++){
                double curr_pos=body_get_pos(bs[j])[k];
                if(curr_pos < -cubelen/2.0){
                    switch (k){
                        case 0:
                            body_set_pos_x(&bs[j],curr_pos+cubelen);
                            break;
                        case 1:
                            body_set_pos_y(&bs[j],curr_pos+cubelen);
                            break;
                        case 2:
                            body_set_pos_z(&bs[j],curr_pos+cubelen);
                            break;
                    }
                }
                else if(curr_pos > cubelen/2.0){
                    switch (k){
                        case 0:
                            body_set_pos_x(&bs[j],curr_pos-cubelen);
                            break;
                        case 1:
                            body_set_pos_y(&bs[j],curr_pos-cubelen);
                            break;
                        case 2:
                            body_set_pos_z(&bs[j],curr_pos-cubelen);
                            break;
                    }
                }
            }
        }
    }
    printf("\n");
    
    for(int i=0;i<nb;i++){
        body_delete(&bs[i]);
    }

    config_destroy(conf);
    return 0;
}
