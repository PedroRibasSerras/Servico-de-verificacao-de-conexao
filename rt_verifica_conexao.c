#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define measuringInterval 10 // seconds
#define logSaveInterval 1 // minuts, approximately

int testConnection(FILE *terminal,char* buffer, char* time, char* wordConnectionTest) //0 - Disconnected; 1- Connected; -1 - Terminal file opening error.
{
	terminal = popen("date; ping -n -c 3 google.com","r");

	if(terminal == NULL){
		fputs("POPEN: Failed to execute command. \n", stderr);
		return -1;
	}
	else{
		int count = 0;
		while(fgets(buffer, 1023, terminal) != NULL){
			if(count == 0){
				buffer[strlen(buffer)-1] =' ';
				sprintf(time, "----- %s -----\n", buffer);
			}
			else{
				if(strstr(buffer, wordConnectionTest) != NULL)
					return 1;
			}
			count++;
		}
		
	}

	pclose(terminal);
	return 0;
}

int main(){
 
	FILE *terminal, *log;
	char buffer[1024];
	char time[1024];
	char logPath[30] = "/home/rt-log.txt";

	int isConnected;
	int measurementsPerSave = logSaveInterval*60/measuringInterval;

	//Palavra usada para verificar se a requisição usando o comando "ping" foi bem sucedida.
	//Caso essa palavra seja encontrada no retorno é porque o computador esta conectado a internet.
	//Caso contrario, o computador nao esta conectado a internet. 
	char* wordConnectionTest = "tempo=";
	
	while(1){

		log = fopen(logPath,"a");

		if(log == NULL){
			printf("Nao foi possivel criar o arquivo de log!");
			return 0;
		}

		for(int i = 0; i<measurementsPerSave; i++){
			
			isConnected = testConnection(terminal, buffer, time, wordConnectionTest);
			if(isConnected == 1){
				fputs(time,log);
				fputs("CONNECTED\n", log);
			}else if(isConnected == 0){
				fputs(time,log);
				fputs("DISCONNECTED\n", log);
			}else{
				fclose(log);
				return 0;
			}

			sleep(measuringInterval);
		}
		fclose(log);
	}
	return 0;
}
