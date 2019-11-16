#include "texture.h"


texture::texture(){
	path = "textures/";//config::texPath;
}


void texture::generatecheckerboard()
{
	int i, j, c;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			c = ((((i&0x8)==0)^((j&0x8))==0))*255;
			image[i][j][0] = (GLubyte) c;
			image[i][j][1] = (GLubyte) c;
			image[i][j][2] = (GLubyte) c;
			image[i][j][3] = (GLubyte) 255;
		}
	}

	width = 256;
	height = 256;

	generated = true;
}

void texture::generateemptytexture()
{
	int i, j;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			image[i][j][0] = (GLubyte) 255;
			image[i][j][1] = (GLubyte) 255;
			image[i][j][2] = (GLubyte) 255;
			image[i][j][3] = (GLubyte) 255;
		}
	}
	width = 256;
	height = 256;
	generated = true;
}

GLuint texture::loadtexture()
{
	GLuint textureid;
	glGenTextures(1,&textureid);
	glBindTexture(GL_TEXTURE_2D,textureid);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	if(generated)
	{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, 
		height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
		image);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, destformat, width, 
		height, 0, destformat, desttype, 
		teximage);
	}
	glGenerateMipmapEXT(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,0);
	return textureid;
}

ILuint texture::loadimagefile(string filename)
{
	string source=path+filename;

	generated = false;
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    ILuint ImageNameID;
    ilGenImages(1, &ImageNameID);
    ilBindImage(ImageNameID);
	if (!ilLoadImage((wchar_t*)(source.c_str()))){
		return 0;
		cout << "could not open " << source << endl;
	}
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    teximage = ilGetData(); 
    destformat = ilGetInteger(IL_IMAGE_FORMAT);
    desttype = ilGetInteger(IL_IMAGE_TYPE);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
    
    return 1;
}
