#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "imageloader.cpp"
#include <time.h>

#include<Objeto.h>

// Posição da câmera
float posCameraX,posCameraY,posCameraZ;

float fracao = 0.5f;

Objeto* obj1;
Objeto* obj2;
Objeto* obj3;

/**
    Variáveis para o incremento do centro da camera,
    Se a camera  vai pra esquerda,direita, frente e tras
    Então o foco dela deve estar na mesma direção do movimento
*/
float incX = 0, incY= 0.1f, incZ = -1.0f;

//Velocidades de movimentação nos eixos x e y.
float speedX = 0.5f,speedY = 0.7f;

// foco da câmera no eixo Y.
float lookY = 0.1;

//Dimensões do plano
float largura = 1000.0f, profundidade = 1000.0f;

//Angulo de visão.
float ang = 45.0f;

//Posições das paredes
float _pos = 0.08;

GLuint texturaChao,texturaParede,texturaAzulejo, gato, raptor, porco, cabra;
const float FLOOR_TEXTURE_SIZE = 15.0f; // Tamanho dos slices do chão.

char c[0];
int carac = 0;
int numCaixas = 0;
int media[1][15];
int desvio[1][15];
int quantidadeObj1[15];
int quantidadeObj2[15];

float random1, random2, random3, random4;

GLuint skybox[6];//Faces do Mundo


//prof-------------------------------
GLfloat luz_pontual[] = {0.3*10, 0.5*10, 0.5*10, 0.0 };
void iluminar(){
   //LUZ
   // no mínimo 8 fontes podem ser utilizadas
   //(iniciadas com cor preta)
   // número de fontes de luz afeta performance

   //LUZ 0

   //define características para a fonte de luz 0
   //fonte de luz direcional (por que w==0?)
   //deletada

   //LUZ 1
   //fonte de luz posicional
   //define características para a fonte de luz 1
   GLfloat light1_diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
   GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat light1_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

   //atribui as características para a fonte de luz 1
   //(experimentem remover alguns dos componentes abaixo)
   glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
   glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
   glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

   //"acende" cada uma das luzes configuradas
   //glEnable(GL_LIGHT0);
   glEnable(GL_LIGHT1);
}

void desenhar_luz(){

  glPushAttrib (GL_LIGHTING_BIT);
//v = new Vetor();

   GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
  // GLfloat mat_emission[] = { 1.0, 1.0, 0.0, 1.0 };

   //atribui características ao material
   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   //glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

   glPushMatrix();
   glTranslatef(luz_pontual[0],luz_pontual[1],luz_pontual[2]);

   glEnable(GL_LIGHTING);
   glColor3f (1.0, 0.0, 0.0);
   glutSolidSphere(1.5,50,50);
   glDisable(GL_LIGHTING);

   glPopAttrib();
   glPopMatrix();
   }

void desenhar_eixos(){

    //não há efeitos de iluminação nos eixos
	glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f (1.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(10.0, 0.0, 0.0);

        glColor3f (0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 10.0, 0.0);

        glColor3f (0.0, 0.0, 1.0);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 0.0, 10.0);
    glEnd();
	}

//prof-------------------------------


/**
    Desenha as paredes.
*/
void desenharCaixas(){
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 30.0 };

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

   glEnable(GL_LIGHTING);

   glEnable(GL_TEXTURE_2D);
   glColor3f(1.0f, 1.0f, 1.0f);
   glTranslatef(0.0f ,0.95f, 0.0f);
   glBindTexture(GL_TEXTURE_2D,texturaParede);

    float x = 20.0f;
    float y = 20.0f;
    float z = 20.0f;

    float yChao = -0.94f;

   glBegin ( GL_QUADS );
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y,  z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y,  z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y,  z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y,  z);

		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, -z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, -z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, -z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, -z);

		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( x, -y, -z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( x,  y, -z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y,  z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y,  z);

		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-x, -y, -z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y,  z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y,  z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-x,  y, -z);

    glEnd();

    //Desenha o Chão
    glBindTexture(GL_TEXTURE_2D,texturaAzulejo);
	glBegin ( GL_QUADS );
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-x, yChao, -z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( x, yChao, -z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( x, yChao,  z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, yChao,  z);

	glEnd();

	glDisable(GL_LIGHTING);
}


//Retorna um identificador pra textura.
GLuint loadMipmappedTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
					  GL_RGB,
					  image->width, image->height,
					  GL_RGB,
					  GL_UNSIGNED_BYTE,
					  image->pixels);
	return textureId;
}


void carregarTexturas(){
 	Image* image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\chão.bmp");
	texturaChao = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, texturaChao);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\parede.bmp");
	texturaParede = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, texturaParede);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


	image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\azulejo.bmp");
	texturaAzulejo = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, texturaAzulejo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


    image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\cat.bmp");
	gato = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, gato);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


	image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\raptor.bmp");
	raptor = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, raptor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


    image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\pig_s.bmp");
	porco = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, porco);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


    image = loadBMP("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\goat_d.bmp");
	cabra = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, cabra);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);


	delete image;
}

/*Inicializa o skyBox*/
void initCeu()
{

	//frente
	Image* image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_ft.bmp");
	skybox[0] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	//direita
	image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_rt.bmp");
	skybox[1] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	//esquerda
	image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_lf.bmp");
	skybox[2] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	//trás
	image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_bk.bmp");
	skybox[3] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	//cima
	image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_up.bmp");
	skybox[4] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	// baixo
	image = loadBMP("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Mundo/nevada_dn.bmp");
	skybox[5] = loadMipmappedTexture(image);
	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);

	delete image;
}

void init(void){
   glClearColor (0.7, 0.7, 1.0, 0.0); //Cor de fundo
   posCameraX = 0.0;
   posCameraY = 1.0;
   posCameraZ = 15.0;

   iluminar();//professora

   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_SMOOTH);

   carregarTexturas();
   initCeu();


}


void desenhaMundo(){
    glBindTexture(GL_TEXTURE_2D,texturaParede);
	glBegin ( GL_QUADS );

        float x = 100.0f;
        float y = 100.0f;
        float z = 100.0f;

        //tras
        glColor3f(1.0f, 0.5f, 0.5f);
        glVertex3f(x, y, z);
        glVertex3f(-x, y, z);
        glVertex3f(-x, -y, z);
        glVertex3f(x, -y, z);

        //frente
        glColor3f(0.5f, 1.0f, 0.5f);
        glVertex3f(x, y, -z);
        glVertex3f(-x, y, -z);
        glVertex3f(-x, -y, -z);
        glVertex3f(x, -y, -z);

        //esquerda
        glColor3f(0.5f, 0.5f, 1.0f);
        glVertex3f(-x, y, z);
        glVertex3f(-x, y, -z);
        glVertex3f(-x, -y, -z);
        glVertex3f(-x, -y, z);

        //direita
        glColor3f(1.0f, 1.0f, 0.5f);
        glVertex3f(x, y, z);
        glVertex3f(x, y, -z);
        glVertex3f(x, -y, -z);
        glVertex3f(x, -y, z);

        //cima (tampa)
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex3f(x, y, z);
        glVertex3f(-x, y, z);
        glVertex3f(-x, y, -z);
        glVertex3f(x, y, -z);

	glEnd();

}


void desenharCeu(float posX, float posY, float posZ, float largura, float altura, float prof)
{

	// Centraliza a camera em torno da largura e altura.
	posX = posX - largura  / 2;
	posY = posY - altura / 2;
	posZ = posZ - prof  / 2;

    glEnable(GL_TEXTURE_2D) ;
	// Desenha a frente
	glBindTexture(GL_TEXTURE_2D, skybox[0]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX,		   posY,		posZ+prof);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX,		   posY+altura, posZ+prof);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX+largura, posY+altura, posZ+prof);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX+largura, posY,		posZ+prof);
	glEnd();

	// desenha o fundo
	glBindTexture(GL_TEXTURE_2D, skybox[3]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX+largura, posY,		posZ);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX+largura, posY+altura, posZ);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX,		   posY+altura,	posZ);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX,		   posY,		posZ);
	glEnd();

	// desenha o lado esquerdo
	glBindTexture(GL_TEXTURE_2D, skybox[2]);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX,		  posY+altura,	posZ);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX,		  posY+altura,	posZ+prof);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX,		  posY,		    posZ+prof);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX,		  posY,		    posZ);
	glEnd();

	// desenha o lado direito
	glBindTexture(GL_TEXTURE_2D, skybox[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX+largura, posY,		posZ);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX+largura, posY,		posZ+prof);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX+largura, posY+altura,	posZ+prof);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX+largura, posY+altura,	posZ);
	glEnd();

	// desenha a parte de cima
	glBindTexture(GL_TEXTURE_2D, skybox[4]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX+largura, posY+altura, posZ);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX+largura, posY+altura, posZ+prof);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX,		   posY+altura,	posZ+prof);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX,		   posY+altura,	posZ);
	glEnd();

	// desenha a parte de baixo
	glBindTexture(GL_TEXTURE_2D, skybox[5]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(posX,		   posY,		posZ);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(posX,		   posY,		posZ+prof);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(posX+largura, posY,		posZ+prof);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(posX+largura, posY,		posZ);
	glEnd();

}

/**
    Desenha um plano em torno dos eixos X e Z
*/
void desenharPlano()
{
    glEnable(GL_TEXTURE_2D);
    glColor3f(0.7f, 1.0f, 0.7f);
   	glBindTexture(GL_TEXTURE_2D, texturaChao);
    glBegin(GL_QUADS);
	    glTexCoord2f(2*largura / FLOOR_TEXTURE_SIZE, _pos / FLOOR_TEXTURE_SIZE);
        glVertex3f(-largura, 0.0f, -profundidade);

	   	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
        glVertex3f(-largura, 0.0f, profundidade);

     	glTexCoord2f(0.0f, (2000 + _pos) / FLOOR_TEXTURE_SIZE);
        glVertex3f(largura, 0.0f, profundidade);

		glTexCoord2f(0.0f, _pos / FLOOR_TEXTURE_SIZE);
        glVertex3f(largura, 0.0f, -profundidade);
    glEnd();
}


/**
    Eventos de teclado.
*/
void asciiKeys(unsigned char key, int x, int y){

   switch (key)
   {
       case 32: // move pra cima [Space]
            posCameraY += speedY * fracao; // aqui poderia limitar até onde pode subir, mas não precisa.
            break;

       case 8: // move pra baixo [BackSpace]
           if(posCameraY <= 1.0f) posCameraY += 0; // não passa do plano
           else posCameraY -= speedY * fracao;
           break;

       case 100: // move pra direita [D] -- corrigir
           if (posCameraX >= largura) posCameraX += 0;
           else posCameraX +=speedX;
           break;

       case 97: // move pra esquerda [A] -- corrigir
            if (posCameraX <= -largura) posCameraX += 0;
            else posCameraX -=speedX;
            break;

       case 119: // move para frente [W]
           if(posCameraX >= largura) posCameraX+=0;
           else posCameraX += incX * fracao;

           if(posCameraZ >= profundidade) posCameraZ+=0;
           else posCameraZ += incZ * fracao;
           break;

       case 115: //move para trás [S]
           if(posCameraX <= -largura) posCameraX+=0;
           else posCameraX -= incX * fracao;

           if(posCameraZ <= -profundidade) posCameraZ+=0;
           else posCameraZ -= incZ * fracao;
           break;
       case 122: //[z]
            printf("Velocidade: %f\n",fracao);
            fracao++;
            break;

       case 120:
           printf("Velocidade: %f\n",fracao);
           fracao--;
           if(fracao < 0) fracao = 0;
           break;
   }
   glutPostRedisplay();
}


void specialKeys (int key, int x, int y){
    float angulo = 2*M_PI/180;
    switch (key){

       case GLUT_KEY_LEFT : //rotaciona em torno de Y pra frente
            incX =  incX*cos(angulo) + incZ*sin(angulo);
            incZ = -incX*sin(angulo) + incZ*cos(angulo);
            break;

       case GLUT_KEY_RIGHT : //rotaciona em torno de Y pra trás
            incX =  incX*cos(-angulo) + incZ*sin(-angulo);
            incZ = -incX*sin(-angulo) + incZ*cos(-angulo);
            break;

       case GLUT_KEY_UP: //rotaciona em torno de X pra frente
            lookY = lookY*cos(angulo) - (-1.0f*sin(angulo));
            break;

       case GLUT_KEY_DOWN: //rotaciona em torno de X pra trás
           lookY = lookY*cos(-angulo) - (-1.0f*sin(-angulo));
           break;

    }
    glutPostRedisplay();
}


void display(void){
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Ativa o buffer de profundidade
   glLoadIdentity();

   gluLookAt (posCameraX, posCameraY, posCameraZ,
              posCameraX+incX, posCameraY+lookY,posCameraZ+incZ,
              0.0, 1.0, 0.0);


   glLightfv(GL_LIGHT1, GL_POSITION, luz_pontual);

 //  desenhar_luz();
   desenhar_eixos();

   desenharCeu(posCameraX,posCameraY,posCameraZ,2000,2000,2000);

   desenharPlano();
   if(random3 == 0) random3 = -1;
   if(random4 == 0) random4 = -1;

    //Desenha as Caixas
        for(int i = 0; i < numCaixas; i++){
                glPushMatrix();
                glTranslatef((random1)*i*(random3) ,0 , (random2)*i*(random4));
                desenharCaixas();
                glPopMatrix();
        }

        for(int i = 0; i < numCaixas; i++){
            glPushMatrix();
            glTranslatef((random1)*i*(random3) ,0 , (random2)*i*(random4));

            glTranslatef(-10,0,-13);
            for(int j = 0; j < quantidadeObj1[i]/2; j++){
                glBindTexture(GL_TEXTURE_2D, raptor);
                glTranslatef(2,0,0);
                glPushMatrix();
                glScalef(0.04f,0.04f,0.04f);
                glRotatef(90,0,1,0);
                obj1->desenharOBJ();
                glPopMatrix();
            }

            glTranslated(-quantidadeObj1[i],0,0);
            glTranslatef(0,0,10);
            for(int j = quantidadeObj1[i]/2 + 1; j < quantidadeObj1[i]; j++){
                glBindTexture(GL_TEXTURE_2D, raptor);
                glTranslatef(2,0,0);
                glPushMatrix();
                glScalef(0.04f,0.04f,0.04f);
                glRotatef(90,0,1,0);
                obj1->desenharOBJ();
                glPopMatrix();
            }


            glTranslated(-quantidadeObj1[i],0,0);
            glTranslatef(0,0,10);
            for(int j = 0; j < quantidadeObj2[i]/2; j++){
                glTranslatef(2.0f,0,0);
                glBindTexture(GL_TEXTURE_2D, porco);
                obj2->desenharOBJ();
            }

            glTranslated(-quantidadeObj1[i],0,0);
            glTranslatef(0,0,5);
            for(int j = quantidadeObj2[i]/2 +1; j < quantidadeObj2[i]; j++){
                glTranslatef(2.0f,0,0);
                glBindTexture(GL_TEXTURE_2D, porco);
         //       obj2->desenharOBJ();
            }

            // glTranslatef(2.0f,0,0);
            // glBindTexture(GL_TEXTURE_2D, gato);
            // obj3->desenharOBJ();

            glTranslated(-quantidadeObj1[i],0,0);
            glPopMatrix();
 }

   glDisable(GL_LIGHTING);

   //troca de buffers.
   glutSwapBuffers();

    glDisable(GL_TEXTURE_2D);

}

/**
    Função chamada quando à tela é redimensionada.
*/
void reshape(int w, int h){
    if (h == 0) h = 1;
    float Ratio = w * 1.0 / h;

    // Usa a matriz de projeção
    glMatrixMode(GL_PROJECTION);

    // Reseta a matriz
    glLoadIdentity();

    // Configura a janela para que toda a janela seja usada como área de desenho.
    glViewport(0, 0, w, h);

    // Configura a perspectiva.
    gluPerspective(ang, Ratio, 0.1f, 2000.0f);

    // obtém a matriz de visualização.
    glMatrixMode(GL_MODELVIEW);
}


void funcaoMouse(int button,int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON)
        if(state == GLUT_DOWN)
        {
                if(ang >= 10) ang-=5;
        }
    if(button == GLUT_RIGHT_BUTTON)
        if(state == GLUT_DOWN)
        {
                if(ang <= 130) ang+=5;
        }

     glutPostRedisplay();
}

void arquivos(){
    FILE *arq;

    arq = fopen("C:/Users/Breno Cruz/Documents/OpenGL Projects/NewGlut/Entrada.txt","r");

    carac = fgetc(arq);
    c[0] = carac;
    numCaixas = atoi(c);
    fgetc(arq);

    printf("%d\n" , numCaixas);

    int i = 0;
    int j = 0;

    for (i = 0; i < numCaixas; i++){
        for(j = 0; j <= 5; j++){//Objeto1
            fgetc(arq);
        }

        carac = fgetc(arq);
        c[0] = carac;
        media[0][i] = atoi(c);
        printf("\n%d", media[0][i]);

        fgetc(arq);

        carac = fgetc(arq);
        c[0] = carac;
        desvio[0][i] = atoi(c);
        printf("%d\n", desvio[0][i]);

        for(j = 0; j < 4; j++){//Objeto2
            fgetc(arq);
        }

        carac = fgetc(arq);
        c[0] = carac;
        media[1][i] = atoi(c);
        printf("%d", media[1][i]);

        fgetc(arq);

        carac = fgetc(arq);
        c[0] = carac;
        desvio[1][i] = atoi(c);
        printf("%d", desvio[1][i]);

        fgetc(arq);


    }

     for(int i = 0; i < numCaixas; i++){
        quantidadeObj1[i] = rand()%(media[0][i]-desvio[0][i]+1)+(media[0][i]+desvio[0][i]);
        quantidadeObj2[i] = rand()%(media[0][i]-desvio[1][i]+1)+(media[1][i]+desvio[0][i]);

        printf("\n%d-----1--%d\n", i,quantidadeObj1[i]/2);
        printf("---------2--%d\n", quantidadeObj2[i]);
    }

    random1 = rand()%10+50;//((50*(i-1));//+(50*i);
    random2 = rand()%10+50;
    random3 = rand()%2;
    random4 = rand()%2;
    printf("Random1: %f\n",random1);
    printf("Random2: %f\n",random2);
    printf("Random3: %f\n",random3);
    printf("Random4: %f\n",random4);

    fclose(arq);

}



int main(int argc, char** argv){
   srand ( time(NULL) );//semente randomica

   arquivos();
   obj1 = new Objeto();
   obj2 = new Objeto();
   obj3 = new Objeto();

   obj1->carregarObj("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\raptor.obj");
   obj2->carregarObj("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\pig.obj");
   obj3->carregarObj("C:\\Users\\Breno Cruz\\Documents\\OpenGL Projects\\NewGlut\\Objetos\\cat.obj");

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);

   init ();

   glutDisplayFunc(display);

   glutSpecialFunc(specialKeys);
   glutKeyboardFunc(asciiKeys);
   glutReshapeFunc(reshape);
   glEnable(GL_DEPTH_TEST);
   glutMainLoop();
   return 0;
}
