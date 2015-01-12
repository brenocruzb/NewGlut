struct Ponto{
     //apenas um ponto com coordenadas 3d e uma cor RGB
     float x,y,z;
     float nx,ny,nz;
     Ponto(){
          Ponto(0, 0,0,1,1,1);
     }
     Ponto(float x, float y, float z, float nx, float ny, float nz){
          this->x = x;this->nx = nx;
          this->y = y;this->ny = ny;
          this->z = z;this->nz = nz;
     }
     void set(float x, float y, float z,float nx,float ny, float nz){
          this->x = x;this->nx = nx;
          this->y = y;this->ny = ny;
          this->z = z;this->nz = nz;
     }
};


struct EncPonto{
     EncPonto* proximo;
     Ponto* ponto;
     
     EncPonto(Ponto* ponto, EncPonto* prox = 0){
          this->ponto = ponto;
          this->proximo = prox;
     }
     
     EncPonto(){
          this->ponto = 0;
          this->proximo = 0;
     }
};

struct Face{
     Ponto** ponto;
     float* texX;
     float* texY;
     Face(Ponto* a, Ponto* b, Ponto* c){
          ponto = new Ponto* [3];
          texX = new float[3];
          texY = new float[3];
          ponto[0] = a;
          ponto[1] = b;
          ponto[2] = c;
          setupTextura(); 
     }
     Face(){
          ponto = new Ponto* [3];
          texX = new float[3];
          texY = new float[3];
          setupTextura();     
     }
     
     void setupTextura(){
          texX[0] = texX[1] = texX[2] = 0.0f;
          texY[0] = texY[1] = texY[2] = 0.0f;
     }
     
     ~Face(){
          delete [] ponto;
          delete [] texX;
          delete [] texY;        
     }     
};

struct EncFace{
     EncFace* proximo;
     Face* face;
     
     EncFace(Face* face, EncFace* prox = 0){
          this->face = face;
          this->proximo = prox; 
     }
     
     EncFace(){
          EncFace(0,0);          
     }
};

class Modelo{
     public:
     Modelo * proximo;    //proximo modelo: usado para encadear modelos
     Ponto** listaPontos;       //pontos do modelo: são muitos
     EncFace* listaFaces;
     int textura;         //se tiver uma fica bem bonito
     int numeroPontos;
     
     Modelo(){
          textura = 0;
          listaPontos = 0;
          numeroPontos = 0;
          listaFaces = 0;
          proximo = 0;       
     }
     
     ~Modelo(){
          if(proximo) delete proximo;
          EncFace* face;
          while(listaFaces){
               face = listaFaces;
               listaFaces = listaFaces->proximo;
               delete face;                  
          }
          
          int i;
          for(i = 0; i < numeroPontos; i++){
               delete listaPontos[i]; 
          }
          delete [] listaPontos;  
     }
     
     static Modelo* Plano(float tamx,float tamy,float textx,float texty){
          Modelo* modelo = new Modelo();
          
          modelo->numeroPontos = 4;
          modelo->listaPontos = new Ponto*[modelo->numeroPontos];
          
          Ponto* ponto00 = new Ponto(-tamx,0,-tamy,0,1,0);
          Ponto* ponto01 = new Ponto(-tamx,0,tamy,0,1,0);
          Ponto* ponto10 = new Ponto(tamx,0,-tamy,0,1,0);
          Ponto* ponto11 = new Ponto(tamx,0,tamy,0,1,0);
          
          modelo->listaPontos[0] = ponto00;
          modelo->listaPontos[1] = ponto01;
          modelo->listaPontos[2] = ponto10;
          modelo->listaPontos[3] = ponto11;
          
          EncFace* face = 0;
          face = new EncFace(new Face(ponto00,ponto01,ponto11),face);
          face->face->texX[0] = -textx;face->face->texY[0] = -texty;
          face->face->texX[1] = -textx;face->face->texY[1] = texty;
          face->face->texX[2] = textx;face->face->texY[2] = texty;
          face = new EncFace(new Face(ponto00,ponto10,ponto11),face);
          face->face->texX[0] = -textx;face->face->texY[0] = -texty;
          face->face->texX[1] = textx;face->face->texY[1] = -texty;
          face->face->texX[2] = textx;face->face->texY[2] = texty;
          modelo->listaFaces = face;
          
          return modelo;                          
     }
     
     static Modelo* Cubo(float x,float y,float z,float tamx,float tamy,float tamz, float r,float g, float b){
          Modelo* modelo = new Modelo();
          
          modelo->numeroPontos = 8;
          modelo->listaPontos = new Ponto*[modelo->numeroPontos];
          
          tamx = tamx/2;
          tamy = tamy/2;
          tamz = tamz/2;
          Ponto* ponto000 = new Ponto(x-tamx,y-tamy,z-tamz,r,g,b);
          Ponto* ponto001 = new Ponto(x-tamx,y-tamy,z+tamz,r,g,b);
          Ponto* ponto011 = new Ponto(x-tamx,y+tamy,z+tamz,r,g,b);
          Ponto* ponto010 = new Ponto(x-tamx,y+tamy,z-tamz,r,g,b);
          Ponto* ponto100 = new Ponto(x+tamx,y-tamy,z-tamz,r,g,b);
          Ponto* ponto101 = new Ponto(x+tamx,y-tamy,z+tamz,r,g,b);
          Ponto* ponto111 = new Ponto(x+tamx,y+tamy,z+tamz,r,g,b);
          Ponto* ponto110 = new Ponto(x+tamx,y+tamy,z-tamz,r,g,b);
          
          modelo->listaPontos[0] = ponto000;
          modelo->listaPontos[1] = ponto001;
          modelo->listaPontos[2] = ponto011;
          modelo->listaPontos[3] = ponto010;
          
          modelo->listaPontos[4] = ponto100;
          modelo->listaPontos[5] = ponto101;
          modelo->listaPontos[6] = ponto111;
          modelo->listaPontos[7] = ponto110;
          
          EncFace* face = 0;
          face = new EncFace(new Face(ponto000,ponto001,ponto011),face);
          face = new EncFace(new Face(ponto000,ponto010,ponto011),face);
          face = new EncFace(new Face(ponto100,ponto101,ponto111),face);
          face = new EncFace(new Face(ponto100,ponto110,ponto111),face);
          face = new EncFace(new Face(ponto010,ponto011,ponto111),face);
          face = new EncFace(new Face(ponto010,ponto110,ponto111),face);
          face = new EncFace(new Face(ponto000,ponto001,ponto101),face);
          face = new EncFace(new Face(ponto000,ponto100,ponto101),face);
          face = new EncFace(new Face(ponto000,ponto100,ponto110),face);
          face = new EncFace(new Face(ponto000,ponto010,ponto110),face);
          face = new EncFace(new Face(ponto001,ponto101,ponto111),face);
          face = new EncFace(new Face(ponto001,ponto011,ponto111),face);
          
          
          modelo->listaFaces = face;
          
          return modelo;       
     }
     
      void novaFace(Face* face,bool teste = true){
          EncFace* aux = listaFaces;
          while(aux && teste){
               if((aux->face->ponto[0] == face->ponto[0] || aux->face->ponto[0] == face->ponto[1] || aux->face->ponto[0] == face->ponto[2])&&
                  (aux->face->ponto[1] == face->ponto[1] || aux->face->ponto[1] == face->ponto[1] || aux->face->ponto[1] == face->ponto[2])&&
                  (aux->face->ponto[2] == face->ponto[2] || aux->face->ponto[2] == face->ponto[1] || aux->face->ponto[2] == face->ponto[2])
               ){cout<<"deny face\n";return;}
               aux = aux->proximo;           
          }
          //cout<<"criada face!\n";
          if(listaFaces){
              listaFaces = new EncFace(face,listaFaces);          
          }else{
              listaFaces = new EncFace(face);      
          }    
     }
     
     //carrega uma textura .bmp
     static GLuint * carregarTextura(char* nome, int transparente  = 0){
          Imagem* essa = new Imagem();
          essa->load(nome);
          if(transparente) essa->setpixels((single_pixel)transparente,(single_pixel)0x00000000);
          GLuint* textura = new GLuint;
          glGenTextures(1,textura);
          glBindTexture(GL_TEXTURE_2D,textura[0]); 
          glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);//GL_NEAREST_MIPMAP_NEAREST);
          glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,essa->width,essa->height,0,GL_RGBA,GL_UNSIGNED_BYTE,essa->pixels);
          delete essa;
          return textura;   
     }
     
     /*
     void salvar(char* nome){
          int i = 0;
          while(nome[i] != '.' && nome[i] != 0) i++;
          if(nome[i] == 0) return;
          i++;
          if((nome[i]|32) == 'm' && (nome[i+1]|32) == 'd' && (nome[i+2]) == '1') salvarModo1(nome);
          if((nome[i]|32) == 'o' && (nome[i+1]|32) == 'b' && (nome[i+2]|32) == 'j')  salvarObj(nome);
          return;
     }

     void salvarModo1(char* nome){
          ofstream arquivo;
          arquivo.open(nome);
          char* texto = new char[100];
          if(arquivo.is_open()){
               int numVer = 0;
               EncPonto* aux = listaPontos;
               while(aux){
                    aux = aux->proximo;
                    numVer++;           
               }
               arquivo << numVer;
               arquivo << "\n";
               SortElement* ponto = new SortElement[numVer];
               aux = listaPontos;
               numVer = 0;
               while(aux){
                    ponto[numVer].objeto = aux->ponto;
                    ponto[numVer].prioridade = (unsigned int) aux->ponto;
                    aux = aux->proximo;
                    numVer++;       
               }
               SortElement::merge_sort(ponto,numVer);
               int i;
               for(i = 0;i<numVer;i++){
                    arquivo << "v ";
                    arquivo << ((Ponto*)ponto[i].objeto)->x << " ";
                    arquivo << ((Ponto*)ponto[i].objeto)->y << " ";
                    arquivo << ((Ponto*)ponto[i].objeto)->z << " ";
                    arquivo << ((Ponto*)ponto[i].objeto)->nx << " ";
                    arquivo << ((Ponto*)ponto[i].objeto)->ny << " ";
                    arquivo << ((Ponto*)ponto[i].objeto)->nz << "\n";      
               }
               EncFace* auxf = listaFaces;
               while(auxf){
                    arquivo << "f ";
                    arquivo << SortElement::busca_binaria((unsigned int)auxf->face->ponto[0],ponto,numVer) <<" " << auxf->face->texX[0] << " " << auxf->face->texY[0] << " ";
                    arquivo << SortElement::busca_binaria((unsigned int)auxf->face->ponto[1],ponto,numVer) <<" "<< auxf->face->texX[1] << " " << auxf->face->texY[1] << " ";
                    arquivo << SortElement::busca_binaria((unsigned int)auxf->face->ponto[2],ponto,numVer) <<"\n"<< auxf->face->texX[2] << " " << auxf->face->texY[2] << " ";
                    auxf = auxf->proximo;           
               }
          }else{
               return;     
          }
          arquivo.close();
          return;
     }
     
     void salvarObj(char* nome){
          ofstream arquivo;
          arquivo.open(nome);
          
          if(arquivo.is_open()){
               int numVertices,numTex,numNorm,numFaces;
               EncFace* auxFaces = listaFaces;
               while(auxFaces){
                    int i;
                    for(i = 0;i<3;i++){
                         arquivo << "v ";
                         arquivo << auxFaces->face->ponto[i]->x;
                         arquivo << " ";
                         arquivo << auxFaces->face->ponto[i]->y;
                         arquivo << " ";
                         arquivo << auxFaces->face->ponto[i]->z;
                         arquivo << "\n"; 
                    }
                    auxFaces = auxFaces->proximo;            
               }
               
               auxFaces = listaFaces;
               while(auxFaces){
                    int i;
                    for(i = 0;i<3;i++){
                         arquivo << "vt ";
                         arquivo << auxFaces->face->ponto[i]->r;
                         arquivo << " ";
                         arquivo << auxFaces->face->ponto[i]->g;
                         arquivo << "\n"; 
                    }
                    auxFaces = auxFaces->proximo;            
               }
               
               auxFaces = listaFaces;
               while(auxFaces){
                    int i;
                    for(i = 0;i<3;i++){
                         arquivo << "vn ";
                         arquivo << 1.0f;
                         arquivo << " ";
                         arquivo << 1.0f;
                         arquivo << " ";
                         arquivo << 1.0f;
                         arquivo << "\n"; 
                    }
                    auxFaces = auxFaces->proximo;            
               }
               
               auxFaces = listaFaces;
               numFaces = 1;
               while(auxFaces){
                    arquivo << "f ";
                    arquivo << numFaces << '/' << numFaces << '/' << numFaces++ << " ";
                    arquivo << numFaces << '/' << numFaces << '/' << numFaces++ << " ";
                    arquivo << numFaces << '/' << numFaces << '/' << numFaces++ << "\n";
                    auxFaces = auxFaces->proximo;            
               }                     
          }     
     }*/
     
     bool carregar(char* nome){
          int i = 0;
          while(nome[i] != '.' && nome[i] != 0) i++;
          if(nome[i] == 0) return false;
          i++;
          if((nome[i]|32) == 'm' && (nome[i+1]|32) == 'd' && (nome[i+2]) == '1') return carregarModo1(nome);
          if((nome[i]|32) == 'o' && (nome[i+1]|32) == 'b' && (nome[i+2]|32) == 'j') return carregarObj(nome);
          return false;
     }
     
     bool carregarModo1(char* nome){
          ifstream arquivo;
          arquivo.open(nome);
          char* texto = new char[100];
          if(arquivo.is_open()){
               arquivo.seekg(0, arquivo.beg);
               int numVer;
               arquivo >> numVer;
               numeroPontos = numVer;
               listaPontos = new Ponto*[numVer];
               numVer = 0;
               while(true){
                     arquivo >> texto;
                     if(!texto[0]) break;
                     if(texto[0] == 'v') {
                          listaPontos[numVer] = new Ponto(0,0,0,0,0,0);
                          arquivo >> listaPontos[numVer]->x;
                          arquivo >> listaPontos[numVer]->y;
                          arquivo >> listaPontos[numVer]->z;
                          arquivo >> listaPontos[numVer]->nx;
                          arquivo >> listaPontos[numVer]->ny;
                          arquivo >> listaPontos[numVer]->nz;
                          numVer++;
                          texto[0] = 0;
                     }else if(texto[0] == 'f'){
                          unsigned int a,b,c;
                          float ax,ay,bx,by,cx,cy;
                          arquivo >> a >> ax >> ay;
                          arquivo >> b >> bx >> by;
                          arquivo >> c >> cx >> cy;
                          Face* face = new Face(listaPontos[a],listaPontos[b],listaPontos[c]);
                          face->texX[0] = ax;
                          face->texX[1] = bx;
                          face->texX[2] = cx;
                          face->texY[0] = ay;
                          face->texY[1] = by;
                          face->texY[2] = cy;
                          novaFace(face,false);
                          texto[0] = 0;      
                     }else{texto[0] = 0;}            
               }     
          }else{
               return false;     
          }
          arquivo.close();
          return true;
     }     
     
     //carrega objetos obj
     bool carregarObj(char* nome){
          ifstream arquivo;
          arquivo.open(nome);
          char* texto = new char[1000];
          if(arquivo.is_open()){
               arquivo.seekg(0, arquivo.beg);
               int numVert = 0;
               int texts = 0;
               int norms = 0;
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
                    }else{texto[0] = 0;}
               }
               arquivo.clear();
               arquivo.seekg(0, arquivo.beg);
               delete[] texto;
               texto = new char[1000];
               cout << numVert<<"\n";
               cout << texts<<"\n";
               cout << norms<<"\n";
               listaPontos = new Ponto*[numVert];
               Ponto* texturas = new Ponto[texts];
               Ponto* normais = new Ponto[norms];
               numVert = 0;
               texts = 0;
               norms = 0;
               int numFaces = 0;
               while(true){
                    if(arquivo.peek() == '#'){
                        while(arquivo.get() != 10){}              
                    }
                    arquivo >> texto;
                    if(texto[0] == 0)
                    {break;}
                    //if(texto[0] == 'o'){arquivo >> texto;}
                    //if(texto[0] == 'g'){arquivo >> texto;}
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
                              //break;
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
                              listaPontos[numVert++] = new Ponto(x,y,z,0, 0, 0);
                              texto[0] = 0;           
                         }
                                      
                    }else if(texto[0] == 'f'){
                          numFaces++;
                          unsigned int v1,t1 = 1,n1;
                          unsigned int v2,t2 = 1,n2;
                          unsigned int v3,t3 = 1,n3;
                          Ponto*p1,*p2,*p3;
                          
                          char barra;
                          arquivo >> v1;
                          arquivo >> barra;
                          if(arquivo.peek() != '/') arquivo >> t1;
                          arquivo >> barra;
                          arquivo >> n1;
                          v1 --;t1--;n1--;
                          
                          listaPontos[v1]->nx = normais[n1].x; 
                          listaPontos[v1]->ny = normais[n1].y;
                          listaPontos[v1]->nz = normais[n1].z;
                          
                          arquivo >> v2;
                          arquivo >> barra;
                          if(arquivo.peek() != '/') arquivo >> t2;
                          arquivo >> barra;
                          arquivo >> n2;
                           v2--;t2--;n2--;
                          
                          listaPontos[v2]->nx = normais[n2].x; 
                          listaPontos[v2]->ny = normais[n2].y;
                          listaPontos[v2]->nz = normais[n2].z;
                          
                          arquivo >> v3;
                          arquivo >> barra;
                          if(arquivo.peek() != '/') arquivo >> t3;
                          arquivo >> barra;
                          arquivo >> n3;
                          v3--;t3--;n3--;
                          
                          listaPontos[v3]->nx = normais[n3].x; 
                          listaPontos[v3]->ny = normais[n3].y;
                          listaPontos[v3]->nz = normais[n3].z;
                                        
                          Face* face = new Face(listaPontos[v1],listaPontos[v2],listaPontos[v3]);
                          face->texX[0] = texturas[t1].x;face->texY[0] = texturas[t1].y;
                          face->texX[1] = texturas[t2].x;face->texY[1] = texturas[t2].y;
                          face->texX[2] = texturas[t3].x;face->texY[2] = texturas[t3].y;
                          novaFace(face,false);
                          texto[0] = 0;
                    }else{texto[0] = 0;}
               }
               cout << numFaces<<"\n";
               delete [] texto;
               delete [] texturas;
               delete [] normais;        
          }else{
               return false;     
          }
          arquivo.close();
          return true;
     }    
};
