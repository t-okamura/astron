/*ds9のboxのregionをpolygonに変換する*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define LINE_LENGTH_MAX 3000 // １行の長さの上限
#define DAT_NUM_MAX 500    // データ数の上限
#define PI 3.141592653589793

int main(int argc,char *argv[]){
    FILE *fp;
    FILE *gp;
    char *filename = argv[1];
    char *filename_out = argv[2];
    gp = fopen(filename,"r");
    hp = fopen(filename_out,"w");
    char line[LINE_LENGTH_MAX];
    char *w = "box";
    double boxin[5] = {0};
    double x,y; //boxの横、縦の長さ
    double cen_x,cen_y; //boxの中心座標
    double alpha,beta,gamma,epu;
    int i;

    printf("# Region file format: DS9 version 4.1\nglobal color=blue dashlist=8 3 width=1 font=\"helvetica 10 normal roman\" select=1 highlite=1 dash=0 fixed=0 edit=1 move=1 delete=1 include=1 source=1\nfk5\n");
    while(fgets(line, LINE_LENGTH_MAX, fp)){
        char *ch = line; //line[0]の先頭のポインタをchに格納
        if(strncmp(ch,w,3) == 0){
            ch += 4;
            for(i=0;i<5;i++){
                boxin[i] = atof(ch);
                ch = strchr(ch,',');
                ch++;
            }
            cen_x = boxin[0];
            cen_y = boxin[1];
            x = boxin[2] / 3600.;
            y = boxin[3] / 3600.;
            alpha = (boxin[4])* PI /180.;
            beta = atan(y/x);
            gamma = beta + alpha ;
            epu = alpha - beta;
            double hosei = 1./cos(cen_y * PI/180.); //wcsのx座標補正
            double squa = pow(x*x+y*y,0.5)/2;
            double Ax = cen_x + squa * cos(gamma) * hosei;
            double Bx = cen_x - squa * cos(epu) * hosei;
            double Cx = cen_x - squa * cos(gamma) * hosei;
            double Dx = cen_x + squa * cos(epu) * hosei;
            double Ay = cen_y - squa * sin(gamma);
            double By = cen_y + squa * sin(epu);
            double Cy = cen_y + squa * sin(gamma);
            double Dy = cen_y - squa * sin(epu);
            printf("polygon(%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf)\n",Ax,Ay,Bx,By,Cx,Cy,Dx,Dy);
        }
    }

    fclose(fp);
    fclose(gp);
    return 0;
}



