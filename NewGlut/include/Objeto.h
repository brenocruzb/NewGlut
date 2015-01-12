#ifndef OBJETO_H
#define OBJETO_H
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

#include "Objeto.h"
#include <Vetor.h>
#include <Face.h>

class Objeto
{
    public:

        int numVert;// = 0;
        int texts;// = 0;
        int norms;// = 0;
        int numFace;// = 0;

        Vetor* listaPontos;// = new Vetor*[numVert];
        Vetor* texturas;// = new Vetor[texts];
        Vetor* normais;// = new Vetor[norms];

        Face* listaFace;

        void desenharOBJ(){

            glBegin(GL_TRIANGLES);

            for (int i = 0; i < numFace; i++){
                glTexCoord2f(texturas[listaFace[i].t1].x, texturas[listaFace[i].t1].y);
                glNormal3f(normais[listaFace[i].n1].x, normais[listaFace[i].n1].y, normais[listaFace[i].n1].z);
                glVertex3f(listaPontos[listaFace[i].p1].x, listaPontos[listaFace[i].p1].y, listaPontos[listaFace[i].p1].z);

                glTexCoord2f(texturas[listaFace[i].t2].x, texturas[listaFace[i].t2].y );
                glNormal3f(normais[listaFace[i].n2].x,normais[listaFace[i].n2].y,normais[listaFace[i].n2].z);
                glVertex3f(listaPontos[listaFace[i].p2].x, listaPontos[listaFace[i].p2].y, listaPontos[listaFace[i].p2].z);

                glTexCoord2f(texturas[listaFace[i].t3].x, texturas[listaFace[i].t3].y);
                glNormal3f(normais[listaFace[i].n3].x,normais[listaFace[i].n3].y,normais[listaFace[i].n3].z);
                glVertex3f(listaPontos[listaFace[i].p3].x, listaPontos[listaFace[i].p3].y, listaPontos[listaFace[i].p3].z);
            }
            glEnd();
        }

        //Claudio
        bool carregarObj(char* nome){
                using namespace std;
                  ifstream arquivo;

                  arquivo.open(nome);
                  char* texto = new char[1000];
                  if(arquivo.is_open()){
                       arquivo.seekg(0, arquivo.beg);
                       numVert = 0;
                       texts = 0;
                       norms = 0;
                       numFace = 0;
                       while(true){
                            arquivo >> texto;
                            if(texto[0] == 0)
                            {break;}
                            if(texto[0] == '#'){
                                while(arquivo.get() != 10){}
                            }
                            else if(texto[0] == 'v'){
                                 if(texto[1] == 't'){
                                      float x,y,z;
                                      arquivo >> x;
                                      arquivo >> y;
                                      texto[0] = 0;
                                      texts ++;
                                 }else if(texto[1] == 'n'){
                                      float x,y,z;
                                      arquivo >> x;
                                      arquivo >> y;
                                      arquivo >> z;
                                      texto[0] = 0;
                                      norms++;
                                 }else{
                                      float x,y,z;
                                      arquivo >> x;
                                      arquivo >> y;
                                      arquivo >> z;
                                      texto[0] = 0;
                                      numVert++;
                                 }
                            }else if(texto[0] == 'f'){
                                      texto[0] = 0;
                                      numFace++;
                            }
                            else{texto[0] = 0;}
                       }
                       arquivo.clear();
                       arquivo.seekg(0, arquivo.beg);
                       delete[] texto;
                       texto = new char[1000];

                       listaPontos = new Vetor[numVert];
                       texturas = new Vetor[texts];
                       normais = new Vetor[norms];
                       listaFace = new Face[numFace];

                       numVert = 0;
                       texts = 0;
                       norms = 0;
                       numFace = 0;
                       while(true){
                            if(arquivo.peek() == '#'){
                                while(arquivo.get() != 10){}
                            }
                            arquivo >> texto;
                            if(texto[0] == 0)
                            {break;}
                            if(texto[0] == '#'){
                                while(arquivo.get() != 10){}
                            }
                            else if(texto[0] == 'v'){
                                 if(texto[1] == 't'){
                                      float x,y;
                                      arquivo >> x;
                                      arquivo >> y;
                                      texturas[texts].x = x;
                                      texturas[texts++].y = y;
                                      texto[0] = 0;
                                 }else if(texto[1] == 'n'){
                                      float x,y,z;
                                      arquivo >> x;
                                      arquivo >> y;
                                      arquivo >> z;
                                      normais[norms].x = x;
                                      normais[norms].y = y;
                                      normais[norms++].z = z;
                                      texto[0] = 0;
                                 }else{
                                      float x,y,z;
                                      arquivo >> x;
                                      arquivo >> y;
                                      arquivo >> z;
                                      listaPontos[numVert].x = x;
                                      listaPontos[numVert].y = y;
                                      listaPontos[numVert++].z = z;
                                      texto[0] = 0;
                                 }

                            }else if(texto[0] == 'f'){

                                  unsigned int v1,t1 = 1,n1;
                                  unsigned int v2,t2 = 1,n2;
                                  unsigned int v3,t3 = 1,n3;
                                  Vetor*p1,*p2,*p3;

                                  char barra;
                                  arquivo >> v1;
                                  arquivo >> barra;
                                  if(arquivo.peek() != '/') arquivo >> t1;
                                  arquivo >> barra;
                                  arquivo >> n1;
                                  v1 --;t1--;n1--;


                                  listaFace[numFace].p1 = v1;
                                  listaFace[numFace].t1 = t1;
                                  listaFace[numFace].n1 = n1;

                                  arquivo >> v2;
                                  arquivo >> barra;
                                  if(arquivo.peek() != '/') arquivo >> t2;
                                  arquivo >> barra;
                                  arquivo >> n2;
                                   v2--;t2--;n2--;


                                  listaFace[numFace].p2 = v2;
                                  listaFace[numFace].t2 = t2;
                                  listaFace[numFace].n2 = n2;


                                  arquivo >> v3;
                                  arquivo >> barra;
                                  if(arquivo.peek() != '/') arquivo >> t3;
                                  arquivo >> barra;
                                  arquivo >> n3;
                                  v3--;t3--;n3--;


                                  listaFace[numFace].p3 = v3;
                                  listaFace[numFace].t3 = t3;
                                  listaFace[numFace].n3 = n3;

                                  texto[0] = 0;
                                  numFace++;
                            }else{texto[0] = 0;}
                       }

                       delete [] texto;
                  }else{
                       return false;
                  }
                  arquivo.close();
                  return true;
             }

};

#endif // OBJETO_H
