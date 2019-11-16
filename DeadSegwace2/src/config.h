#ifndef CONFIG_H
#define CONFIG_H

#include "common.h"

class config
{
public:

	static string objPath;
	static string texPath;
	static string soundPath;


	static int windowwidth;
	static int windowheight;
	

	static bool freeCamera;
	static bool music;
	static bool bloom;
	static bool wireframe;
	static bool transparency;
	static bool shadow;
	static int mipmode;
	static bool texfilter;

	config(void);
	~config(void);

	static void readConfig();

};

#endif