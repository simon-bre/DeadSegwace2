#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "common.h"
#include "config.h"
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include "../external/coldet/src/coldet.h"



using std::ifstream;
using std::getline;


class geometry
{
public:
	geometry(string filename, bool _createHitbox, bool _randomColor);
	~geometry(void);
	
	GLfloat* getPositions();
	GLfloat* getNormals();
	GLfloat* getTexCoords();
	//GLfloat* getAmbient();
	GLfloat* getDiffuse();
	//GLfloat* getSpecular();
	//GLfloat* getSpecularCoeff();

	//string getTexturePath();

	int getVertexCount();
	int getTriangleCount();

	CollisionModel3D* colmod;
private:

	
	GLfloat *positions;			// Vertex-Positionen:	[x1,y1,z1,  x2,y2,z2, ...]
	GLfloat *normals;			// Normalvektoren:		-----------"-------------
	GLfloat *texCoords;			// Texturkoordinaten:	[u1,v1,  u2,v2,  u3,v3,..]
	//GLfloat *ambientColor;		// Ambient-Color:		[r1,g1,b1,  r2,g2,b2, ...]
	GLfloat *diffuseColor;		// Diffuse-Color:       -----------"--------------
	//GLfloat *specularColor;		// Specular-Color:		-----------"--------------
	//GLfloat *specularCoeff;		// Reflexionskoeffizienten: [n1, n2, n3,.........]
	
	
	/* vectorinhalt: 
	 * 0,1,2: ambient r,g,b
	 * 3,4,5: diffuse r,g,b
	 * 6,7,8: specular r,g,b
	 * 9: Reflexionskoeffizient
	 */
	map<string, vector<float>> materials;

	string path;
	//string texturePath;
	int realVertexCount; //die tatsaechliche Anzahl von Knoten des Objektes, z.b. 8 bei einem Wuerfel; _vertexCount waere hier hingegen, wenn der wuerfel aus 12 dreiecken bestuende: 12*3=36
	int vertexCount;
	int triangleCount;
	bool mtlExists;
	bool createColmod;	//fuer objekte, die nicht kollidieren muessen, sollte man nicht unnoetigen speicherplatz verschwenden
	bool randomColor;
	

	bool importObjFirstPass(const char* filename);
	bool importObjSecondPass(const char* filename);
	bool importMtl(const char* filename);


};


#endif