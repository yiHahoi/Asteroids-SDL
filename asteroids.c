#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

const int ANCHO = 800;
const int ALTO = 600;
const int BPP = 32;  
const char* TITULO = "UmRiAgPluTiX";  
const int tamanioFuente = 25;           
const int MAXBALAS = 5000;
const float VELBALA = 2;

typedef struct{             
    SDL_Surface* frame;     
    float pos[2];          
    float vel[2];         
    float accel[2];        
    float masa;             
    float atraccion;        
    SDL_Rect* indexIMG;     
    SDL_Rect posIMG;
    int anchoIMG;           
    int anchoSubIMG;        
    int qIMG;               
    float radio;
}objeto;

objeto cargarObjeto(char*);
SDL_Surface* cargarIMG(char*);
int cirCollision( float*, float*, float, float);

int main(int argc, char* argv[]){

    if(TTF_Init()!=0)   
        return 1;       

    if(SDL_Init(SDL_INIT_EVERYTHING)!=0)    
        return 1;                          

    SDL_Surface* ventana = NULL;      
    SDL_Rect img;
    SDL_Surface* FPS = NULL;
    SDL_Surface* RELOJ = NULL;
    SDL_Color colorTexto = {255,0,0}; 
    TTF_Font* font = NULL;
    SDL_Event evento;                 
    char sFPS[10];
    char sTIEMPO[13];
    int QUIT = 0;                     
    int i=0;

    ventana = SDL_SetVideoMode(ANCHO, ALTO, BPP, SDL_SWSURFACE);        
    if(ventana==NULL)                                                   
        return 1;

    SDL_WM_SetCaption(TITULO, NULL);    

    objeto nave = cargarObjeto("nave.txt");             
    objeto asteroide = cargarObjeto("asteroide.txt");   
    objeto bala = cargarObjeto("bala.txt");            

    objeto nBalas[MAXBALAS];
    int totalBalas = 0;

    nave.posIMG.x = nave.pos[0]-nave.anchoSubIMG/2;
    nave.posIMG.y = nave.pos[1]-nave.anchoSubIMG/2;
    img = nave.indexIMG[0];

    font = TTF_OpenFont( "All Caps.ttf", tamanioFuente );                     
    if(font==NULL)                                                  
        return 1;

    FPS = TTF_RenderText_Solid(font, sFPS, colorTexto);    
    RELOJ = TTF_RenderText_Solid(font, sTIEMPO, colorTexto);         
    SDL_Rect posFPS, posRELOJ;                                         
    posRELOJ.x = ANCHO - 175;
    posRELOJ.y = 35;
    posFPS.x = posRELOJ.x;
    posFPS.y = posRELOJ.y + tamanioFuente + 10;

    Uint8* estadoTeclas = SDL_GetKeyState(NULL);    
    Uint32 contadorFPS = 0, contadorTIEMPO = 0, frames=1;

    SDL_FillRect( ventana, &ventana->clip_rect, SDL_MapRGB( ventana->format, 0xFF, 0xFF, 0xFF ) );
    SDL_BlitSurface(nave.frame, &img, ventana, &nave.posIMG);
    SDL_BlitSurface(RELOJ, NULL, ventana, &posRELOJ);
    SDL_BlitSurface(FPS, NULL, ventana, &posFPS);
    SDL_Flip(ventana);

    float Fuerza = 0.1;
    float Roce = 0.1;
    float IndiceFR = 0.1; 

    while(QUIT!=1){

        while(SDL_PollEvent(&evento)){
            if(evento.type == SDL_QUIT)
                QUIT = 1;
                                     }

    if(nave.pos[0]<=12.5){
        nave.vel[0]*=-1;
        nave.accel[0]*=-1;}

    if(nave.pos[0]>=ANCHO-12.5){
        nave.vel[0]*=-1;
        nave.accel[0]*=-1;}

    if(nave.pos[1]<=12.5){
        nave.vel[1]*=-1;
        nave.accel[1]*=-1;}

    if(nave.pos[1]>=ALTO-12.5){
        nave.vel[1]*=-1;
        nave.accel[1]*=-1;}

    i=0;
    while(i<=(totalBalas-1)){
    //colision balas.
    if(nBalas[i].pos[0]<=2){
        nBalas[i].vel[0]*=-1;}
    if(nBalas[i].pos[0]>=ANCHO-2){
        nBalas[i].vel[0]*=-1;}
    if(nBalas[i].pos[1]<=2){
        nBalas[i].vel[1]*=-1;}
    if(nBalas[i].pos[1]>=ALTO-2){
        nBalas[i].vel[1]*=-1;}
        i++;
                            }

    if(estadoTeclas[SDLK_UP]){
        nave.accel[1]=-Fuerza;
        img = nave.indexIMG[0];}
    if(estadoTeclas[SDLK_DOWN]){
        nave.accel[1]=Fuerza;
        img = nave.indexIMG[1];}
    if(estadoTeclas[SDLK_RIGHT]){
        nave.accel[0]=Fuerza;
        img = nave.indexIMG[2];}
    if(estadoTeclas[SDLK_LEFT]){
        nave.accel[0]=-Fuerza;
        img = nave.indexIMG[3];}

    if(estadoTeclas[SDLK_UP]&&estadoTeclas[SDLK_LEFT])
        img = nave.indexIMG[4];
    if(estadoTeclas[SDLK_UP]&&estadoTeclas[SDLK_RIGHT])
        img = nave.indexIMG[5];
    if(estadoTeclas[SDLK_DOWN]&&estadoTeclas[SDLK_LEFT])
        img = nave.indexIMG[6];
    if(estadoTeclas[SDLK_DOWN]&&estadoTeclas[SDLK_RIGHT])
        img = nave.indexIMG[7];

    if( estadoTeclas[SDLK_d] && totalBalas<MAXBALAS ){
        nBalas[totalBalas] = bala;
        if(img.x==nave.indexIMG[0].x && img.y==nave.indexIMG[0].y){
            nBalas[totalBalas].vel[0] = 0;
            nBalas[totalBalas].vel[1] = -VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0];
            nBalas[totalBalas].pos[1] = nave.pos[1]-14;
                                                                  }
        if(img.x==nave.indexIMG[1].x && img.y==nave.indexIMG[1].y){
            nBalas[totalBalas].vel[0] = 0;
            nBalas[totalBalas].vel[1] = VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0];
            nBalas[totalBalas].pos[1] = nave.pos[1]+14;
                                                                  }
        if(img.x==nave.indexIMG[2].x && img.y==nave.indexIMG[2].y){
            nBalas[totalBalas].vel[0] = VELBALA;
            nBalas[totalBalas].vel[1] = 0;
            nBalas[totalBalas].pos[0] = nave.pos[0]+14;
            nBalas[totalBalas].pos[1] = nave.pos[1];
                                                                  }
        if(img.x==nave.indexIMG[3].x && img.y==nave.indexIMG[3].y){
            nBalas[totalBalas].vel[0] = -VELBALA;
            nBalas[totalBalas].vel[1] = 0;
            nBalas[totalBalas].pos[0] = nave.pos[0]-14;
            nBalas[totalBalas].pos[1] = nave.pos[1];
                                                                  }
        if(img.x==nave.indexIMG[4].x && img.y==nave.indexIMG[4].y){
            nBalas[totalBalas].vel[0] = -VELBALA;
            nBalas[totalBalas].vel[1] = -VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0]-14;
            nBalas[totalBalas].pos[1] = nave.pos[1]-14;
                                                                  }
        if(img.x==nave.indexIMG[5].x && img.y==nave.indexIMG[5].y){
            nBalas[totalBalas].vel[0] = VELBALA;
            nBalas[totalBalas].vel[1] = -VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0]+14;
            nBalas[totalBalas].pos[1] = nave.pos[1]-14;
                                                                  }
        if(img.x==nave.indexIMG[6].x && img.y==nave.indexIMG[6].y){
            nBalas[totalBalas].vel[0] = -VELBALA;
            nBalas[totalBalas].vel[1] = VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0]-14;
            nBalas[totalBalas].pos[1] = nave.pos[1]+14;
                                                                  }
        if(img.x==nave.indexIMG[7].x && img.y==nave.indexIMG[7].y){
            nBalas[totalBalas].vel[0] = VELBALA;
            nBalas[totalBalas].vel[1] = VELBALA;
            nBalas[totalBalas].pos[0] = nave.pos[0]+14;
            nBalas[totalBalas].pos[1] = nave.pos[1]+14;
                                                                  }
        totalBalas += 1;
                                                     }
                                                     
    nave.vel[0] += nave.accel[0];
    nave.vel[1] += nave.accel[1];
    nave.pos[0] += nave.vel[0]+nave.accel[0];
    nave.pos[1] += nave.vel[1]+nave.accel[1];

    i=0;
    while(i<=(totalBalas-1)){
        nBalas[i].pos[0] += nBalas[i].vel[0];
        nBalas[i].pos[1] += nBalas[i].vel[1];
        i++;
                            }

    frames += 1;
    if(SDL_GetTicks()-contadorTIEMPO>=1000){
        contadorTIEMPO += 1000;
        contadorFPS = frames;
        frames = 1;
                                           }
    snprintf( sFPS, sizeof(sFPS), "FPS: %u", contadorFPS);
    snprintf( sTIEMPO, sizeof(sTIEMPO), "TIEMPO: %u", contadorTIEMPO/1000);
    FPS = TTF_RenderText_Solid(font, sFPS, colorTexto);
    RELOJ = TTF_RenderText_Solid(font, sTIEMPO, colorTexto);

    SDL_FillRect( ventana, &ventana->clip_rect, SDL_MapRGB( ventana->format, 0xFF, 0xFF, 0xFF ) );
    i=0;
    while(i<=totalBalas-1){
        nBalas[i].posIMG.x = nBalas[i].pos[0]-bala.anchoSubIMG;
        nBalas[i].posIMG.y = nBalas[i].pos[1]-bala.anchoSubIMG;
        SDL_BlitSurface(nBalas[i].frame, NULL, ventana, &(nBalas[i].posIMG));
        i++;
                        }
    SDL_BlitSurface(nave.frame, &img, ventana, &nave.posIMG);
    SDL_BlitSurface(RELOJ, NULL, ventana, &posRELOJ);
    SDL_BlitSurface(FPS, NULL, ventana, &posFPS);

    SDL_FreeSurface(RELOJ);
    SDL_FreeSurface(FPS);

    nave.posIMG.x = nave.pos[0]-nave.anchoSubIMG/2;
    nave.posIMG.y = nave.pos[1]-nave.anchoSubIMG/2;

    SDL_Flip(ventana);
    SDL_Delay(12);

    //aplicacion de fuerza roce a la nave.
    if(!estadoTeclas[SDLK_UP] && !estadoTeclas[SDLK_DOWN]){
        if(nave.vel[0]>0){
            nave.accel[0]=-Roce;
            if(nave.vel[0]<IndiceFR){
                nave.accel[0]=0;
                nave.vel[0]=0;
                          }
                    }
        if(nave.vel[0]<0){
            nave.accel[0]=Roce;
            if(nave.vel[0]>-IndiceFR){
                nave.accel[0]=0;
                nave.vel[0]=0;
                           }
                    }
        if(nave.vel[1]>0){
            nave.accel[1]=-Roce;
            if(nave.vel[1]<IndiceFR){
                nave.accel[1]=0;
                nave.vel[1]=0;
                          }
                    }
        if(nave.vel[1]<0){
            nave.accel[1]=Roce;
            if(nave.vel[1]>-IndiceFR){
                nave.accel[1]=0;
                nave.vel[1]=0;
                           }
                    }
                                                          }
                  }

    free(nave.indexIMG);
    free(asteroide.indexIMG);
    free(bala.indexIMG);
    SDL_FreeSurface(nave.frame);
    SDL_FreeSurface(asteroide.frame);
    SDL_FreeSurface(bala.frame);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_Quit();
    return 0;
}


objeto cargarObjeto(char* archivo){
    objeto obj;
    FILE* datos = NULL;
    char *imagen;
    char linea[32];
    datos = fopen(archivo, "r");
    if(datos==NULL)
        printf("ERROR al leer %s", archivo);

    fgets(linea, 33, datos);
    sscanf(linea,"%s", imagen);
    fgets(linea, 33, datos);
    sscanf(linea,"%d", &(obj.anchoIMG));
    fgets(linea, 33, datos);
    sscanf(linea,"%d", &(obj.anchoSubIMG));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.pos[0]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.pos[1]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.vel[0]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.vel[1]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.accel[0]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.accel[1]));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.masa));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.atraccion));
    fgets(linea, 33, datos);
    sscanf(linea,"%f", &(obj.radio));

    fclose(datos);

    int i = 0;
    int cantidadIMG = obj.anchoIMG/obj.anchoSubIMG;
    cantidadIMG *= cantidadIMG;   
    SDL_Rect* index;
    index = malloc(sizeof(SDL_Rect)*cantidadIMG); 
    obj.frame = cargarIMG(imagen);
    if(obj.frame==NULL)
        printf("ERROR al leer %s", imagen);

    int pixX = 0;
    int pixY = 0;

    while(i<cantidadIMG){
        index[i].x = pixX;
        index[i].y = pixY;
        index[i].w = obj.anchoSubIMG;
        index[i].h = obj.anchoSubIMG;
        pixX+=obj.anchoSubIMG;
        if((index[i].x+index[i].w)==obj.anchoIMG){
            pixX = 0;
            pixY += obj.anchoSubIMG;
                                             }
        i++;
                        }

    obj.indexIMG = index;
    obj.qIMG = cantidadIMG;

    return obj;
}

SDL_Surface* cargarIMG(char* img){
    SDL_Surface* tmp1 = IMG_Load(img);
    SDL_Surface* tmp2 = NULL;
    if(tmp1==NULL)
        return NULL;
    tmp2 = SDL_DisplayFormatAlpha(tmp1);
    SDL_FreeSurface(tmp1);
    return tmp2;
}
