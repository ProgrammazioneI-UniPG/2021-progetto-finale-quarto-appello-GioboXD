#include "gamelib.h"
#include <string.h>
#define MAX_LEN 128

static void print_image(FILE *fptr){
	system("clear");
    char read_string[MAX_LEN];

    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);
}

int main(){
  int scelta = 0, success = 0;
  char *filename = "start.txt";
  FILE *fptr = NULL;
	char f;

  while(true){
		if((fptr = fopen(filename,"r")) == NULL){
	      fprintf(stderr,"error opening %s\n",filename);
	      return 1;
	  }
	    print_image(fptr);
				 fclose(fptr);
				 scelta = 0;
    printf("\n\t===========================================\n\tScegli una delle seguenti opzioni =)\n\t- 1 Impostare il gioco\n\t- 2 Avviare la partita\n\t- 3 Terminare il programma\n\t===========================================\n");
    scanf("%d", &scelta);

    switch(scelta){
      case 1:
				if(success == 1){
					libera_memoria();
					cancella();
				}
        system("clear");
        success = imposta_gioco();
        break;

      case 2:
        if(success == 1){
          system("clear");
          gioca();
					success = 0;
          system("clear");
        }else{
					system("clear");
          printf("\n\t================================\n\tPRIMA DI GIOCARE DEVI IMPOSTARE IL GIOCO\n\t================================\n");
					printf("\n###\tPremi invio per continuare....\n");
					while(getchar() != '\n' && getchar() != EOF);
					getchar();
        }
        break;

      case 3:
        system("clear");
        printf("\n\t====================================\n\tGrazie per aver giocato =)\n\tAlla prossima!\n\t====================================\n");
				libera_memoria();
        exit(0);
        break;

      default:
			while((f=getchar())!= '\n' && f != EOF);
        printf("\n\t====================================\n\tINSERISCI UN INPUT VALIDO\n\t====================================\n");
        //system("clear");
        break;
    }
  }
}
