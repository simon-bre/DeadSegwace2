#include "config.h"



config::config(void)
{
}

config::~config(void)
{
}
//statische variablen muessen hier initialisiert werden, aus irgendeinem grund
string config::objPath="undefined";
string config::texPath="undefined";
string config::soundPath="undefined";

int config::windowwidth=800;
int config::windowheight=600;

bool config::freeCamera=false;
bool config::music=true;
bool config::bloom=true;
bool config::wireframe=false;
bool config::transparency=true;
bool config::shadow=true;
int config::mipmode=MIPMODE_NONE;
bool config::texfilter=false;

void config::readConfig(){

	FILE *configFile;
	char buffer[64];

	char *config = "config.cfg";
	configFile = fopen(config, "r");

	if(!configFile){
		cout << "Error opening file " << config << endl;
		return;
	}


	while (fscanf(configFile, "%s", buffer) != EOF){
		
		if(buffer[0] == 'm' && buffer[1] == 'p'){
			fgets(buffer, sizeof(buffer), configFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			config::objPath = buffer;
		}

		if(buffer[0] == 't' && buffer[1] == 'p'){
			fgets(buffer, sizeof(buffer), configFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			config::texPath = buffer;
		}

		if(buffer[0] == 's' && buffer[1] == 'p'){
			fgets(buffer, sizeof(buffer), configFile);
            sscanf(buffer, "%s %s", buffer, buffer);
			config::soundPath = buffer;
		}

		
		


		if(buffer[0] == 'w' && buffer[1] == 'i' && buffer[2] == 'd' && buffer[3] == 't' && buffer[4] == 'h'){
			int tmp;
			fscanf(configFile, "%d", &tmp);
			config::windowwidth = tmp;
		}

		if(buffer[0] == 'h' && buffer[1] == 'e' && buffer[2] == 'i' && buffer[3] == 'g' && buffer[4] == 'h' && buffer[5] == 't'){
			int tmp;
			fscanf(configFile, "%d", &tmp);
			config::windowheight = tmp;
		}




		
		
		if(buffer[0] == 'f' && buffer[1] == 'c'){
			int tmp;
			fscanf(configFile, "%f", &tmp);
			if(tmp==1) freeCamera=true;
			else freeCamera=false;
		}

		if(buffer[0] == 'm' && buffer[1] == 'u' && buffer[2] == 's' && buffer[3] == 'i' && buffer[4] == 'c'){
			int tmp;
			fscanf(configFile, "%f", &tmp);
			if(tmp==0) music=false;
			else music=true;
		}

		if(buffer[0] == 'b' && buffer[1] == 'l' && buffer[2] == 'o' && buffer[3] == 'o' && buffer[4] == 'm'){
			int tmp;
			fscanf(configFile, "%f", &tmp);
			if(tmp==0)bloom=false;
			else bloom=true;
		}

		if(buffer[0] == 'w' && buffer[1] == 'f'){
			int tmp;
			fscanf(configFile, "%f", &tmp);
			if(tmp==1)wireframe=true;
			else wireframe=false;
		}

		if(buffer[0] == 't' && buffer[1] == 'r' && buffer[2] == 'n' && buffer[3] == 's' && buffer[4] == 'p'){
			int tmp;
			fscanf(configFile, "%f", &tmp);
			if(tmp==0) transparency=false;
			else transparency=true;
		}

	}
 }
