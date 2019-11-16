#include "geometry.h"


/*importiert .obj Dateien und zugehoerige .mtl-dateien.
 * liest als materialdaten nur ambient, diffuse und specular color + reflexionskoeffizient fuer blinnphong
 */

geometry::geometry(string filename, bool _createColmod, bool _randomColor):createColmod(_createColmod), randomColor(_randomColor) 
{
	//texturePath="undefined";
	vertexCount=0;
	triangleCount=0;
	realVertexCount=0;
	mtlExists=false;
	
	path = config::objPath;

	filename = path+filename;

	importObjFirstPass(filename.c_str());

	positions = new GLfloat[vertexCount*4];
	normals = new GLfloat[vertexCount*3];
	texCoords = new GLfloat[vertexCount*2];
	//ambientColor = new GLfloat[vertexCount*3];
	diffuseColor = new GLfloat[vertexCount*3];
	//specularColor = new GLfloat[vertexCount*3];
	//specularCoeff = new GLfloat[vertexCount];

	if(createColmod) colmod = newCollisionModel3D();
	else colmod = NULL;

	importObjSecondPass(filename.c_str());
	if(createColmod) colmod->finalize();
}


GLfloat *geometry::getPositions(){
	return positions;
}
GLfloat *geometry::getNormals(){
	return normals;
}
GLfloat *geometry::getTexCoords(){
	return texCoords;
}
/*GLfloat *geometry::getAmbient(){
	return ambientColor;
}*/
GLfloat *geometry::getDiffuse(){
	return diffuseColor;
}
/*GLfloat *geometry::getSpecular(){
	return specularColor;
}
GLfloat *geometry::getSpecularCoeff(){
	return specularCoeff;
}*/

int geometry::getVertexCount(){
	return vertexCount;
}
int geometry::getTriangleCount(){
	return triangleCount;
}

/*string geometry::getTexturePath(){
	return texturePath;
}*/

// zum ermittlen der Anzahlen von Knoten (fuer initialisierung der Arrays) und import des .mtl files
bool geometry::importObjFirstPass(const char* filename){
	
	//obj-File oeffnen
	FILE *objFile;
	char buffer[256];
	objFile = fopen(filename, "r");

	if(!objFile){
		cout << "Error opening file " << filename << endl;
		return false;
	}

	//quelle des assoziierten .mtl-files
	string mtlSource;


	//obj parsen
	while (fscanf(objFile, "%s", buffer) != EOF){
		
		switch (buffer[0]){

		
		case 'm':								//mtllib source
			
			fgets(buffer, sizeof(buffer), objFile);
			sscanf(buffer, "%s %s", buffer, buffer);
			mtlSource = buffer;
			mtlSource = path+mtlSource;
			mtlExists=true;
			importMtl(mtlSource.c_str());
			
			break;

		case 'f':								//dreiecke
			vertexCount+=3;
			triangleCount++;

			break;

		case 'v':
			if(buffer[1]=='\0')
			realVertexCount++;

			break;

		default:
            fgets(buffer, sizeof(buffer), objFile);
            break;
		}
    }
	return true;
}

//tatsaechliches einlesen der Geometriedaten
bool geometry::importObjSecondPass(const char* filename){
	
	//obj-File oeffnen
	FILE *objFile;
	
	char buffer[256];
	objFile = fopen(filename, "r");

	if(!objFile){
		cout << "Error opening file " << filename << endl;
		return false;
	}

	//diverse Zwischenspeicher:

	//da kommen die daten rein, so wie sie im .obj stehen (multi-indexed)
	vector<float> *tmp_positions = new vector<float>;	
	vector<float> *tmp_normals = new vector<float>;
	vector<float> *tmp_texCoords = new vector<float>;
	
	//zwischenspeicher fuer koordinaten und kndizes
	float x,y,z;
	int a,b,c,d,e,f,g,h,i;

	//zwischenspeicher fuer material-daten
	vector<float> *currentMaterial;

	//index, zum speichern der daten an die richtigen stellen im array
	int index=0;


	//freaky visuals
	vector<float> *randomColors = new vector<float>;
	float red;
	float green;
	float blue;

	//collision model
	float v1[] = {0,0,0};
	float v2[] = {0,0,0};
	float v3[] = {0,0,0};


	std::srand( (unsigned)time( NULL ) );			//macht irgedwas spaciges, damit man Zufallszahlen erzeugen kann, ka..
	
	if(randomColor){
		for(int i=0; i<realVertexCount; i++){	
			red = (float)rand()/RAND_MAX;			//in diesen 3 zeilen rumschrauben um die farben anzupassen.. sind aber momentan eh relativ geil
			green = (float)rand()/RAND_MAX-red;
			blue = (float)rand()/RAND_MAX-green-red;
			randomColors->push_back(red);
			randomColors->push_back(green);
			randomColors->push_back(blue);
		}
	}


	//obj parsen
	while (fscanf(objFile, "%s", buffer) != EOF){
		
		switch (buffer[0]){


		case 'u':								//usemtl
			fgets(buffer, sizeof(buffer), objFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			currentMaterial = &materials[buffer];
			break;
		

		case 'v':							// v, vn, or vt.
            switch (buffer[1])
            {
            case '\0':						// vertex position
				fscanf(objFile, "%f %f %f",&x, &y, &z);
				tmp_positions->push_back(x);
				tmp_positions->push_back(y);
				tmp_positions->push_back(z);

				break;

			case 'n':						//vertex normal
				fscanf(objFile, "%f %f %f",&x, &y, &z);
				tmp_normals->push_back(x);
				tmp_normals->push_back(y);
				tmp_normals->push_back(z);
				break;

			case 't':						//texture coordinate
				fscanf(objFile, "%f %f", &x, &y);
				tmp_texCoords->push_back(x);
				tmp_texCoords->push_back(y);
				break;
			}
			break;

		case 'f':								//dreiecke
			fscanf(objFile, "%u/%u/%u %u/%u/%u %u/%u/%u", &a,&b,&c,&d,&e,&f,&g,&h,&i);
			
			/* im folgenden wird die multi-indexed geometry in
			 * single-indexed geometry konvertiert.
			 * und es werden die Vertices und Triangles mitgezaehlt.. vielleicht is das ja mal fuer irgendwas nuetzlich.
			 */
			a=(a-1)*3;
			b=(b-1)*2;
			c=(c-1)*3;
			d=(d-1)*3;
			e=(e-1)*2;
			f=(f-1)*3;
			g=(g-1)*3;
			h=(h-1)*2;
			i=(i-1)*3;
			
			//vertex nummer 1
			positions[index*4]=tmp_positions->at(a);
			positions[index*4+1]=tmp_positions->at(a+1);
			positions[index*4+2]=tmp_positions->at(a+2);
			positions[index*4+3]=1;	//homogene Koordinate
			texCoords[index*2]=tmp_texCoords->at(b);
			texCoords[index*2+1]=tmp_texCoords->at(++b);
			normals[index*3]=tmp_normals->at(c);
			normals[index*3+1]=tmp_normals->at(++c);
			normals[index*3+2]=tmp_normals->at(++c);
			/*ambientColor[index*3]=currentMaterial->at(0);
			ambientColor[index*3+1]=currentMaterial->at(1);
			ambientColor[index*3+2]=currentMaterial->at(2);*/
			if(randomColor){
				diffuseColor[index*3]=randomColors->at(a);
				diffuseColor[index*3+1]=randomColors->at(a+1);
				diffuseColor[index*3+2]=randomColors->at(a+2);
			} else{
				diffuseColor[index*3]=currentMaterial->at(0);	//3
				diffuseColor[index*3+1]=currentMaterial->at(1);	//4
				diffuseColor[index*3+2]=currentMaterial->at(2);	//5
			}
			/*specularColor[index*3]=currentMaterial->at(6);
			specularColor[index*3+1]=currentMaterial->at(7);
			specularColor[index*3+2]=currentMaterial->at(8);
			specularCoeff[index]=currentMaterial->at(9);*/
			index++;

			//vertex nummer 2
			positions[index*4]=tmp_positions->at(d);
			positions[index*4+1]=tmp_positions->at(d+1);
			positions[index*4+2]=tmp_positions->at(d+2);
			positions[index*4+3]=1;	//homogene Koordinate
			texCoords[index*2]=tmp_texCoords->at(e);
			texCoords[index*2+1]=tmp_texCoords->at(++e);
			normals[index*3]=tmp_normals->at(f);
			normals[index*3+1]=tmp_normals->at(++f);
			normals[index*3+2]=tmp_normals->at(++f);
			/*ambientColor[index*3]=currentMaterial->at(0);
			ambientColor[index*3+1]=currentMaterial->at(1);
			ambientColor[index*3+2]=currentMaterial->at(2);*/
			if(randomColor){
				diffuseColor[index*3]=randomColors->at(d);
				diffuseColor[index*3+1]=randomColors->at(d+1);
				diffuseColor[index*3+2]=randomColors->at(d+2);
			} else{
				diffuseColor[index*3]=currentMaterial->at(0);	//3
				diffuseColor[index*3+1]=currentMaterial->at(1);	//4
				diffuseColor[index*3+2]=currentMaterial->at(2);	//5
			}
			/*specularColor[index*3]=currentMaterial->at(6);
			specularColor[index*3+1]=currentMaterial->at(7);
			specularColor[index*3+2]=currentMaterial->at(8);
			specularCoeff[index]=currentMaterial->at(9);*/
			index++;

			//vertex nummer 3
			positions[index*4]=tmp_positions->at(g);
			positions[index*4+1]=tmp_positions->at(g+1);
			positions[index*4+2]=tmp_positions->at(g+2);
			positions[index*4+3]=1;	//homogene Koordinate
			texCoords[index*2]=tmp_texCoords->at(h);
			texCoords[index*2+1]=tmp_texCoords->at(++h);
			normals[index*3]=tmp_normals->at(i);
			normals[index*3+1]=tmp_normals->at(++i);
			normals[index*3+2]=tmp_normals->at(++i);
			/*ambientColor[index*3]=currentMaterial->at(0);
			ambientColor[index*3+1]=currentMaterial->at(1);
			ambientColor[index*3+2]=currentMaterial->at(2);*/
			if(randomColor){
				diffuseColor[index*3]=randomColors->at(g);
				diffuseColor[index*3+1]=randomColors->at(g+1);
				diffuseColor[index*3+2]=randomColors->at(g+2);
			} else{
				diffuseColor[index*3]=currentMaterial->at(0);	//3
				diffuseColor[index*3+1]=currentMaterial->at(1);	//4
				diffuseColor[index*3+2]=currentMaterial->at(2);	//5
			}
			/*specularColor[index*3]=currentMaterial->at(6);
			specularColor[index*3+1]=currentMaterial->at(7);
			specularColor[index*3+2]=currentMaterial->at(8);
			specularCoeff[index]=currentMaterial->at(9);*/
			
			//collisionsbodell-setup:
			if(createColmod){
				v1[0] = tmp_positions->at(a);
				v1[1] = tmp_positions->at(a+1);
				v1[2] = tmp_positions->at(a+2);
				v2[0] = tmp_positions->at(d);
				v2[1] = tmp_positions->at(d+1);
				v2[2] = tmp_positions->at(d+2);
				v3[0] = tmp_positions->at(g);
				v3[1] = tmp_positions->at(g+1);
				v3[2] = tmp_positions->at(g+2);
				colmod->addTriangle(v1, v2, v3);
			}

			index++;
			
			//fertig ist ein dreieck

			break;

		default:
            fgets(buffer, sizeof(buffer), objFile);
            break;
		}
    }
	delete tmp_positions;
	delete tmp_normals;
	delete tmp_texCoords;
	delete randomColors;
	return true;
}

bool geometry::importMtl(const char* filename){	
	//mtl-File oeffnen
	FILE *mtlFile;
	char buffer[256];
	mtlFile = fopen(filename, "r");

	if(!mtlFile){
		cout << "Error opening file " << filename << endl;
		return false;
	}
	
	//zwischenspeicher:

	string materialName;
	vector<float> *tmp_data;
	float r,g,b,n;

	//mtl parsen
	while (fscanf(mtlFile, "%s", buffer) != EOF){
		
		switch (buffer[0]){

		/*case 'm':							//texture-picture
			fgets(buffer, sizeof(buffer), mtlFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			texturePath=buffer;
			break;*/
		
		
		case 'n':								//newmtl (new material)
			fgets(buffer, sizeof(buffer), mtlFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			materialName=buffer;
			tmp_data = new vector<float>(3,0);	//(10, 0)
			materials[materialName]=*tmp_data;
			break;

		case 'K':
			switch (buffer[1]){
			/*case 'a':							//ambient
				fscanf(mtlFile, "%f %f %f",&r, &g, &b);
				materials[materialName].at(0)=r;
				materials[materialName].at(1)=g;
				materials[materialName].at(2)=b;
				break;*/
			case 'd':							//diffuse
				fscanf(mtlFile, "%f %f %f",&r, &g, &b);
				materials[materialName].at(0)=r;	//at(3)
				materials[materialName].at(1)=g;	//at(4)
				materials[materialName].at(2)=b;	//at(5)
				break;
			/*case 's':							//specular
				fscanf(mtlFile, "%f %f %f",&r, &g, &b);
				materials[materialName].at(6)=r;
				materials[materialName].at(7)=g;
				materials[materialName].at(8)=b;
				break;*/
			}
		/*case 'N':							//specular-koeffizient
			if(buffer[1]=='s'){	
			fscanf(mtlFile, "%f" ,&n);
			materials[materialName].at(9)=n;
			}
			break;*/

		default:
            fgets(buffer, sizeof(buffer), mtlFile);
            break;
		}
		
	}
	
	delete tmp_data;
	return true;
}


geometry::~geometry(void)
{
	delete positions;
	delete normals;
	delete texCoords;
	//delete ambientColor;
	delete diffuseColor;
	//delete specularColor;
	//delete specularCoeff;
	delete colmod;
}
