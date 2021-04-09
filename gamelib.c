#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "gamelib.h"
#include <string.h>
#define MAX_LEN 128

int inserimento = 0;
int pelandroni = 0;
int quests = 0;
struct Giocatore* giocatori;
struct Stanza* stanza_inizio;
struct Stanza* lista_stanze;
struct Stanza* pFirst;
struct Stanza* stanza_attuale;

const char* NomeGiocatore[] = {"\e[48;5;0m\033[38;2;255;0;0;1m Rosso \033[0m\e[0m","\e[48;5;0m\033[38;2;0;0;255;1m Blu \033[0m\e[0m", "\e[48;5;0m\033[38;2;255;153;255;1m Rosa \033[0m\e[0m", "\e[48;5;0m\033[38;2;255;255;0;1m Giallo \033[0m\e[0m", "\e[48;5;0m\033[38;2;255;128;0;1m Arancione \033[0m\e[0m", "\e[48;5;0m\033[38;2;0;255;0;1m Verde \033[0m\e[0m", "\e[48;5;0m\033[38;2;153;255;255;1m Ciano \033[0m\e[0m", "\e[48;5;0m\033[38;2;153;51;255;1m Viola \033[0m\e[0m", "\e[48;5;0m\033[38;2;255;255;255;1m Bianco \033[0m\e[0m", "\e[48;5;0m\033[38;2;0;0;0;1m Nero \033[0m\e[0m"};
const char* TipoStanza[] = {"Vuota", "Quest Semplice", "Quest Complessa", "Botola"};
const char* StatoGiocatore[] = {"Astronauta", "Impostore", "Assassinato", "Defenestrato"};

unsigned short quest_da_finire;
int giocatoriInsieme = 0;

static void lista(int *nTurni, int turnoAtt){
	struct Stanza* lista_stanze = (struct Stanza*) malloc(sizeof(struct Stanza));
	lista_stanze = giocatori[nTurni[turnoAtt]].posizioneA;
	lista_stanze -> pNext = NULL;
	if(pFirst == NULL){
		pFirst = lista_stanze;
	}else{
		lista_stanze -> pNext = pFirst;
		pFirst = lista_stanze;
	}
}//Funzione che permette di aggiungere la stanza appena creata alla lista delle stanze

static void print_image(FILE *fptr){
	system("clear");
    char read_string[MAX_LEN];

    while(fgets(read_string,sizeof(read_string),fptr) != NULL)
        printf("%s",read_string);
} //Funzione richiamata da tutte le funzioni che stampano un "immagine" la quale va a leggere i file.txt

static void stampa_kill(){
	system("clear");
			char *filename = "Kill.txt";
			FILE *fptr = NULL;

			if((fptr = fopen(filename,"r")) == NULL){
					fprintf(stderr,"error opening %s\n",filename);
			}

			print_image(fptr);

			fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();

} //Stampa l'immagine della kill

static void vault(){
	system("clear");
			char *filename = "vault.txt";
			FILE *fptr = NULL;

			if((fptr = fopen(filename,"r")) == NULL){
					fprintf(stderr,"error opening %s\n",filename);
			}

			print_image(fptr);

			fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();

} //Stampa l'immagine di una porta quando si cambia stanza

static void pelandroniEG(){
	printf("\n\t====================================\n\tVi porto anche delle brioches?\n\t====================================\n");
	system("clear");
	    char *filename = "pelandroni.txt";
	    FILE *fptr = NULL;

	    if((fptr = fopen(filename,"r")) == NULL){
	        fprintf(stderr,"error opening %s\n",filename);
	    }

	    print_image(fptr);

	    fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();
} //Piccolo EG che appare se i giocatori rimangono per più di 10 turni fermi

static void stampa_def(){
	system("clear");
	    char *filename = "def.txt";
	    FILE *fptr = NULL;

	    if((fptr = fopen(filename,"r")) == NULL){
	        fprintf(stderr,"error opening %s\n",filename);
	    }

	    print_image(fptr);

	    fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();

} //Stampa l'immagine di quando si è defenestrati

static void stampa_sab(){
	system("clear");
	    char *filename = "sab.txt";
	    FILE *fptr = NULL;

	    if((fptr = fopen(filename,"r")) == NULL){
	        fprintf(stderr,"error opening %s\n",filename);
	    }

	    print_image(fptr);

	    fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();

} //Stampa l'immagine di quando si sabota

static void wAstronauti(){
	system("clear");
	    char *filename = "wAstronauti.txt";
	    FILE *fptr = NULL;

	    if((fptr = fopen(filename,"r")) == NULL){
	        fprintf(stderr,"error opening %s\n",filename);
	    }

	    print_image(fptr);

	    fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();

} //Stampa la scritta in caso di vincita degli astronauti

static void wImpostori(){
	system("clear");
	    char *filename = "wImpostori.txt";
	    FILE *fptr = NULL;

	    if((fptr = fopen(filename,"r")) == NULL)
	    {
	        fprintf(stderr,"error opening %s\n",filename);
	    }

	    print_image(fptr);

	    fclose(fptr);
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();
} //Stampa la scritta in caso di vincita degli impostori

static void nuova_stanza(int *nTurni, int turnoAtt){
	int prob = 0;
	time_t t;
	srand((unsigned)time(&t));

	giocatori[nTurni[turnoAtt]].posizioneA -> avanti = NULL;
	giocatori[nTurni[turnoAtt]].posizioneA -> destra = NULL;
	giocatori[nTurni[turnoAtt]].posizioneA -> sinistra = NULL;
	giocatori[nTurni[turnoAtt]].posizioneA -> emergenza_chiamata = false;

	//Calcola la probabilità che andrà a definire il tipo della stanza
	prob = rand() %100;
	if(prob <= 25)
		giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 3;
	if(prob >= 26 && prob <= 40)
		giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 2;
	if(prob >= 41 && prob <= 70)
		giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 1;
	if(prob >= 71 && prob <= 99)
		giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 0;

	lista(nTurni, turnoAtt); //chiama la funzione per aggiungere la stanza alla lista

} //Funzione che crea una nuova stanza e la passa alla funzione lista()

static void avanza(int *nTurni, int turnoAtt){
	system("clear");
	bool ok = false;
	int scelta = 0;
	while(ok == false){
		printf("\n\t==================================================\n\t Scegli dove vuoi avanzare:\n\t- 1 Avanti\n\t- 2 Destra\n\t- 3 sinistra\n\t- 4 Rimani in questa stanza\n\t==================================================\n");
		scanf("%d", &scelta);
		switch(scelta){ //Se la stanza non esiste il programma provvede a crearne una nuova
			case 1:
				if(giocatori[nTurni[turnoAtt]].posizioneA -> avanti != NULL){
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> avanti;
					giocatori[nTurni[turnoAtt]].posizioneA -> stanza_precedente = stanza_attuale;
				}else{
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA -> avanti = (struct Stanza*)malloc(sizeof(struct Stanza));
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> avanti;
					nuova_stanza(nTurni, turnoAtt);
				}
				vault();
				ok = true;
				break;

			case 2:
				if(giocatori[nTurni[turnoAtt]].posizioneA -> destra != NULL){
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> destra;
					giocatori[nTurni[turnoAtt]].posizioneA -> stanza_precedente = stanza_attuale;
				}else{
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA -> destra = (struct Stanza*)malloc(sizeof(struct Stanza));
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> destra;
					nuova_stanza(nTurni, turnoAtt);
				}
				vault();
				ok = true;
				break;

			case 3:
				if(giocatori[nTurni[turnoAtt]].posizioneA -> sinistra != NULL){
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> sinistra;
					giocatori[nTurni[turnoAtt]].posizioneA -> stanza_precedente = stanza_attuale;
				}else{
					stanza_attuale = giocatori[nTurni[turnoAtt]].posizioneA;
					giocatori[nTurni[turnoAtt]].posizioneA -> sinistra = (struct Stanza*)malloc(sizeof(struct Stanza));
					giocatori[nTurni[turnoAtt]].posizioneA = giocatori[nTurni[turnoAtt]].posizioneA -> sinistra;
					nuova_stanza(nTurni, turnoAtt);
				}
				vault();
				ok = true;
				break;

			case 4:
				printf("\n\t=================================\n\tSembri trovarti bene qui =)\n\tGoditi il tuo caffè\n\t=================================\n");
				pelandroni++;
				if(pelandroni > 5){
						pelandroniEG();
				}
				ok = true;
				break;

			default:
				printf("\n\t====================================\n\tINSERISCI UN INPUT VALIDO\n\t====================================\n");
				system("clear");
				break;
		}
	}
} //Funzione che permette di cambiare stanza, con la possibilità di andare avanti, destra, sinistra oppure rimanere fermi nella stanza

static void info(){
	system("clear");
	int nAstronauti = 0, nImpostori = 0, kill = 0, def = 0;
	for(int i = 0; i < inserimento; i++){
		if(giocatori[i].stato == 0){
			nAstronauti++;
		}
		if(giocatori[i].stato == 1){
			nImpostori++;
		}
		if(giocatori[i].stato == 2){
			kill++;
		}
		if(giocatori[i].stato == 3){
			def++;
		}
	}

	printf("\n\t###########################################\n\tINFORMAZIONI PARTITA\n\t- Astronauti vivi: %d\n\t- Impostori vivi: %d\n\t- numero omicidi: %d\n\t- numero defenestrati: %d\n\t###########################################",nAstronauti,nImpostori,kill,def);
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
} //Funzione che mostra le info della partita solamente dopo una chiamata di emergenza

static void kill(int *nTurni, int turnoAtt){
	time_t t;
	srand((unsigned)time(&t));
	int nAstronautiQui = 0, nAstronautiPre = 0, r = 0, def = 0, kill = 0, scelta = 0;
	for(int i = 0; i < inserimento; i++){
		if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 0){
			nAstronautiQui++;
		}
		if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA -> stanza_precedente && giocatori[i].stato == 0){
			nAstronautiPre++;
		}
	}
	if(nAstronautiQui == 1){
		for(int i = 0; i < inserimento; i++){
			if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 0){
				giocatori[i].stato = 2;
				stampa_kill();
				printf("\n=======================================\n\tHai ucciso il giocatore: %s\n=======================================\n", NomeGiocatore[giocatori[i].nome]);
				if(nAstronautiPre > 0){
					r = rand()%101;
					def = nAstronautiPre*20;
					if(r < def){ //Calcola la probabilità di essere scoperti ed in caso si venga scoperti viene chiamata la funzione che stampa l'immagine del defenestraggio
						printf("\nMa sei stato scoperto!");
						printf("\n###\tPremi invio per continuare....\n");
						while(getchar() != '\n' && getchar() != EOF);
						getchar();
						giocatori[nTurni[turnoAtt]].stato = 3;
						stampa_def();
					}else{
						printf("\n###\tPremi invio per continuare....\n");
						while(getchar() != '\n' && getchar() != EOF);
						getchar();
					}
				}
		}
	}
	}

	if(nAstronautiQui > 1){
		int i = 0;
		while(kill == 0 && i < inserimento){
				if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 0){
					printf("\n\t================================\n\tDecidi cosa fare\n\t- 1 Per uccidere %s\n\t- 2 Per cambiare bersaglio\n\t================================\n", NomeGiocatore[giocatori[i].nome]);
					scanf("%d", &scelta);
					int test = 0;
					while(test == 0){
						switch(scelta){
							case 1:
								giocatori[i].stato = 2;
								stampa_kill();
								printf("\n=======================================\n\tHai ucciso il giocatore: %s\n=======================================\n", NomeGiocatore[giocatori[i].nome]);
								nAstronautiQui--;
								if(nAstronautiPre > 0 || nAstronautiQui > 0){
									r = rand()%101;
									def = (nAstronautiPre*20) + (nAstronautiQui*50);
									if(r < def){
										printf("\nMa sei stato scoperto!");
										printf("\n###\tPremi invio per continuare....\n");
										while(getchar() != '\n' && getchar() != EOF);
										getchar();
										giocatori[nTurni[turnoAtt]].stato = 3;
										stampa_def();
									}else{
										printf("\n###\tPremi invio per continuare....\n");
										while(getchar() != '\n' && getchar() != EOF);
										getchar();
									}
								}
								test = 1;
								kill = 1;
							break;

							case 2:
								test = 1;
								break;

							default:
								test = 1;
							break;

						}
					}
				}
				i++;
			}
		}
	} //Funzione che permette di uccidere un astronauta, restituisce degli output diversi in base alla situazione (Es. 1 sola persona nella stanza)

static void sabota(int *nTurni, int turnoAtt){
	giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 0;
	printf("\n\t===============================\n\t(rumore di cacciaviti...\n...Dei bulloni volano per la stanza)\nSTANZA SABOTATA CON SUCCESSO\n\t===============================\n");
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
	stampa_sab();
} //Funzione che permette di sabotare una quest trasformando la stanza in una vuota

static void giocatoriVicini(int *nTurni, int turnoAtt){
	giocatoriInsieme = 0;
	for(int i = 0; i < inserimento; i++){
		if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].nome != giocatori[nTurni[turnoAtt]].nome && (giocatori[i].stato == 0 || giocatori[i].stato == 1)){
			giocatoriInsieme++;
		}
	}
		if(giocatoriInsieme == 0){
			printf ("\n\t------------------------\n\tSei solo nella stanza\n\t(Suono di Grilli)\n\t------------------------\n");
		}else{
			printf("\nTi trovi in compagnia di: \n");
			for(int i = 0; i < inserimento; i++){
				if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].nome != giocatori[nTurni[turnoAtt]].nome && (giocatori[i].stato == 0 || giocatori[i].stato == 1)){
					printf("- %s\n", NomeGiocatore[giocatori[i].nome]);
				}
			}
		}
} //Funzione che permette di controllare se il giocatore ha altri giocatori nella sua stanza

static void botolafn(int *nTurni, int turnoAtt){
	int nBotole = 0, test = 0;

	time_t t;
	srand((unsigned)time(&t));

	if(pFirst == NULL){
		printf("\n\tERRORE LISTA DELLE STANZE VUOTA\n\t");
	}else{
		struct Stanza* pScan = pFirst;
		while(pScan != NULL){
			if(pScan -> tipo == 3){
				nBotole++;
			}
			pScan = pScan -> pNext;
		}

		if(nBotole == 1){
			printf("\n\t=================================\n\tQuesta botola sembra non portare a nulla!\n\tSarà meglio continuare a piedi.......\n\t=================================\n");
			printf("\n###\tPremi invio per continuare....\n");
			while(getchar() != '\n' && getchar() != EOF);
			getchar();
			avanza(nTurni, turnoAtt);
		}

		if(nBotole == 2){
			struct Stanza* pScan = pFirst;
			test = 0;
			while(pScan != NULL && test == 0){
				if(pScan -> tipo == 3 && giocatori[nTurni[turnoAtt]].posizioneA != pScan){
					giocatori[nTurni[turnoAtt]].posizioneA = pScan;
					test = 1;
					giocatoriVicini(nTurni, turnoAtt);
				}
				pScan = pScan -> pNext;
			}
		}

		if(nBotole > 2){
			test = 0;
			while(test == 0){
				if(pScan == NULL){
					pScan = pFirst;
				}
				if(pScan -> tipo == 3 && pScan != giocatori[nTurni[turnoAtt]].posizioneA){
					giocatori[nTurni[turnoAtt]].posizioneA = pScan;
					test = 1;
					giocatoriVicini(nTurni, turnoAtt);
				}

				pScan = pScan -> pNext;
			}
		}
	}
} //Funzione che permette di spostarsi con le botole

static void chiamata_emergenza(int *nTurni, int turnoAtt){
	giocatori[nTurni[turnoAtt]].posizioneA -> emergenza_chiamata = true;
	int nAstronauti = 0, nImpostori = 0, percentualeA = 0, percentualeI = 0, n = 0, exit = 0, i = 0;
	time_t t;
	srand((unsigned)time(&t));
	bool def = false;
	for(int c = 0; c < inserimento; c++){
		if(giocatori[c].stato == 0 && giocatori[c].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA){
			nAstronauti++;
		}
		if(giocatori[c].stato == 1 && giocatori[c].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA){
			nImpostori++;
		}
	}
	percentualeA = 30 + (20*nImpostori) - (30*nAstronauti);
	percentualeI = 30 + (20*nAstronauti) - (30*nImpostori);
	while(exit == 0 && i < inserimento){
		n = rand()%101;
		if(giocatori[nTurni[turnoAtt]].posizioneA == giocatori[i].posizioneA && giocatori[i].stato == 0){
			if(n <= percentualeA){
				giocatori[i].stato = 3;
				stampa_def();
				printf("\n\t=============================\n\tVedi il giocatore %s volare nello spazio profondo..... \n\tEra un ASTRONAUTA\n\t=============================\t", NomeGiocatore[giocatori[i].nome]);
				def = true;
				exit = 1;
			}
		}else{
		if(giocatori[i].stato == 1 && giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA){
			if(n <= percentualeI){
				giocatori[i].stato = 3;
				stampa_def();
				printf("\n\t=============================\n\tVedi il giocatore %s volare nello spazio profondo..... \n\tEra un IMPOSTORE\n\t=============================\t", NomeGiocatore[giocatori[i].nome]);
				def = true;
				exit = 1;
			}
		}
	}
		i++;
	}
	if(def == false){
		printf("\n\t=============================\n\tTi fermi un secondo ad ammirare lo spazio profondo.....\n\t=============================\t");
	}
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
	info();
} //Questa funzione permette di effetture una chiamata di emergenza, solamente 1 volta per stanza

static void esegui_quest_semplice(int *nTurni, int turnoAtt){
	system("clear");
	quests --;
	if(quests <= 0){
		quest_da_finire = 0;
	}else{
		quest_da_finire = quests;
	}
	giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 0;
	printf("\n\t=====================================\n\tQuest completata con successo!\n\t=====================================");
	printf("\n\t=============================\n\t Complimenti restano: %d quest\n\t=============================\n", quest_da_finire);
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
} //Questa funzione andrà a decrementare "quest_da_finire" di 1

static void esegui_quest_complessa(int *nTurni, int turnoAtt){
	system("clear");
	quests -= 2;
	if(quests <= 0){
		quest_da_finire = 0;
	}else{
		quest_da_finire = quests;
	}
	giocatori[nTurni[turnoAtt]].posizioneA -> tipo = 0;
	printf("\n\t=====================================\n\tQuest completata con successo!\n\t=====================================");
	printf("\n\t=============================\n\t Complimenti restano: %d quest\n\t=============================\n", quest_da_finire);
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
} //Questa funzione andrà a decrementare "quest_da_finire" di 2

void libera_memoria(){
	//system("clear");
	if(lista_stanze != NULL){
		while(lista_stanze != NULL){
			lista_stanze = lista_stanze -> pNext;
			free(lista_stanze);
		}
		free(giocatori);
		free(lista_stanze);
		inserimento = 0;
	}
	printf("\n@@@@@@@@@@@@@@@@@@@\n@\tMEMORIA CANCELLATA\n@@@@@@@@@@@@@@@@@@@\n");
	printf("\n###\tPremi invio per continuare....\n");
	while(getchar() != '\n' && getchar() != EOF);
	getchar();
} //Dealloca la memoria utilizzata

static int win(){
	system("clear");
	int vittoria=0;
	int vivi = 0;
	int iVivi = 0;
	for(int i = 0; i < inserimento; i++){
		if(giocatori[i].stato == 0)
		{
			vivi++;
		}
		if(giocatori[i].stato == 1){
			iVivi++;
		}
	}
	if(quest_da_finire <= 0 || iVivi == 0)
	{
		printf("\n======================================\n");
		wAstronauti();
		printf("\n======================================\n");
		vittoria = 1;
	}
	if(vivi == 0){
		printf("\n======================================\n");
		wImpostori();
		printf("\n======================================\n");
		vittoria = 1;
	}
	return vittoria;
} //Funzione che calcola le varie possibilità di vittoria

static bool Cadaveri(int *nTurni, int turnoAtt){
	system("clear");
	bool test = false;
	int nCadaveri = 0;
	for(int i = 0; i < inserimento; i++){
		if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 2 && giocatori[nTurni[turnoAtt]].posizioneA -> emergenza_chiamata == false){
			nCadaveri++;
		}
	}
	if(nCadaveri == 0){
		printf("\n\t---------------------------------\n\t(Senti il tipico odore dello spazio)\n\t---------------------------------\n");
		printf("\n###\tPremi invio per continuare....\n");
		while(getchar() != '\n' && getchar() != EOF);
		getchar();
	}
	if(nCadaveri == 1){
		printf("\n\t---------------------------------\n\tEntri nella stnza\n\tSenti il ronzio di una mosca e trovi il cadavere di: \n");
		for(int i = 0; i < inserimento; i++){
			if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 2 && giocatori[nTurni[turnoAtt]].posizioneA -> emergenza_chiamata == false){
				printf("\t- %s\n", NomeGiocatore[giocatori[i].nome]);
			}
		}
		printf("\t---------------------------------\n");
		printf("\n###\tPremi invio per continuare....\n");
		while(getchar() != '\n' && getchar() != EOF);
		getchar();
		test = true;
	}
	if(nCadaveri > 1){
		printf("\n\t---------------------------------\n\tEntri nella stanza\n\t Un tanfo penetra le tue narici e trovi i cadaveri di: \n");
		for(int i = 0; i < inserimento; i++){
			if(giocatori[i].posizioneA == giocatori[nTurni[turnoAtt]].posizioneA && giocatori[i].stato == 2 && giocatori[nTurni[turnoAtt]].posizioneA -> emergenza_chiamata == false){
				printf("\t- %s\n", NomeGiocatore[giocatori[i].nome]);
			}
		}
		printf("\t---------------------------------\n");
		printf("\n###\tPremi invio per continuare....\n");
		while(getchar() != '\n' && getchar() != EOF);
		getchar();
		test = true;
	}
	return test;
} //Controlla la presenza di cadaveri nella stanza

static void inizioTurno(int *nTurni, int turnoAtt){
	system("clear");
	giocatoriInsieme = 0;
	system("clear");
	printf("\n\t *************************************************\n");
	printf("\n\t Turno del giocatore: %s\n", NomeGiocatore[giocatori[nTurni[turnoAtt]].nome]);
	printf("\n\t *************************************************\n");
	printf("Ti trovi in una stanza: %s\n", TipoStanza[giocatori[nTurni[turnoAtt]].posizioneA -> tipo]);
	giocatoriVicini(nTurni, turnoAtt);
} //Parte iniziale di ogni turno

void cancella(){
	quest_da_finire = 0;
	pelandroni = 0;
} //Funzione che mette a 0 le varie variabili globali

static void stampa_giocatori(){
	system("clear");
	printf("////////// \t ELENCO DEI GIOCATORI \t //////////");
	for(int i = 0; i != inserimento; i++){
		printf("\n- %s: ", NomeGiocatore[giocatori[i].nome]);
		printf("- %s\n ", StatoGiocatore[giocatori[i].stato]);
	}
	printf("La stanza iniziale è di tipo: %s\n",TipoStanza[giocatori[0].posizioneA -> tipo] );
} //Stampa una lista completa dei giocatori

int imposta_gioco(){
	system("clear");
	int success = 0;

		printf("\n######################\n");
		printf("\n#\n");
		printf("\n# IMPOSTAZIONI DI GIOCO\n");
		printf("\n#\n");
		printf("\n######################\n");

		time_t t;
		srand((unsigned) time(&t));
		int ni=0, probabilita=0, flag=0;
		while(flag==0){
			printf("\n Quanti giocatori siete?\n");
			scanf("%d", &inserimento);
			if(inserimento < 4){
				printf("\n ERROR Troppi pochi giocatori ERROR\n");
				continue;
			}else if(inserimento > 10){
				printf("\n ERROR Troppi giocatori ERROR\n");
				continue;
			}else{
				flag=1;
			}
		}

		giocatori = (struct Giocatore*) calloc(inserimento, sizeof(struct Giocatore));

		for(int i = 0; i < inserimento; i++){
			giocatori[i].nome = rand() % 10;
			for(int i1 = 0; i1 < i; i1++){
				if(giocatori[i].nome == giocatori[i1].nome){
					i--;
				}
			}
		}
						   //Inizializzo tutti gli stati dei giocatori a 0
		for(int i = 0; i < inserimento; i++){
			giocatori[i].stato = 0;
		}
						   //Se ci sono solamente 4 giocatori ci sarà solamente 1 impostore
		if(inserimento == 4){
			ni = 1;
			int c = rand() %inserimento;
			giocatori[c].stato = 1;
		}
						   //Se ci sono da 5 a 7 giocatori ci saranno 1 o 2 impostori
		if(inserimento >= 5 && inserimento <= 7){
			ni = (rand() %2)+1;
			int i = 0;
			while(i < ni){
				int c = rand() %inserimento;
				if(giocatori[c].stato != 1){
					giocatori[c].stato = 1;
					i++;
				}
			}
		}
						   //Se ci sono da 8 a 10 giocatori ci saranno 2 o 3 impostori
		if(inserimento >= 8){
			ni = (rand() %2)+2;
			int i = 0;
			while(i < ni){
				int c = rand() %inserimento;
				if(giocatori[c].stato != 1){
					giocatori[c].stato = 1;
					i++;
				}
			}
		}

		stanza_inizio=(struct Stanza*) malloc(sizeof(struct Stanza));
		stanza_inizio -> avanti = NULL;
		stanza_inizio -> destra = NULL;
		stanza_inizio -> sinistra = NULL;
		stanza_inizio -> stanza_precedente = NULL;
		stanza_inizio -> emergenza_chiamata = false;

		probabilita = rand() %100;
		if(probabilita <= 25)
			stanza_inizio -> tipo = 3;
		if(probabilita >= 26 && probabilita <= 40)
			stanza_inizio -> tipo = 2;
		if(probabilita >= 41 && probabilita <= 70)
			stanza_inizio -> tipo = 1;
		if(probabilita >= 71 && probabilita <= 99)
			stanza_inizio -> tipo = 0;

		struct Stanza* lista_stanze = (struct Stanza*) malloc(sizeof(struct Stanza));
		lista_stanze = stanza_inizio;
		lista_stanze -> pNext = NULL;
		pFirst = lista_stanze;

											  //Tutti i giocatori vengono portati alla stanza iniziale

		for(int i = 0; i < inserimento; i++){
			giocatori[i].posizioneA = stanza_inizio;
		}

//Inserimento quest ho deciso di non inserire il valore direttamente su quest_da_finire dato che con una variabile unsigned non potrei controllare se vengono immessi numeri negativi

		while(quest_da_finire < inserimento){
			printf("\n Inserisci il numero di quest da completare:\n");
			scanf("%d", &quests);
			if(quests < 0){
				printf("\nERROR Il Numero di quest non può essere negativo! ERROR\n");
			}else{
				if (quest_da_finire < inserimento){
					printf("\nERROR Numero quest insufficente! ERROR\n");
				}
				quest_da_finire = quests;
			}
		}

											  //Stampa tutti i giocatori impostati
		stampa_giocatori();
		printf("\n CONFIGURAZIONE COMPLETATA CON SUCCESSO\n ###\tPremi invio per continuare....\n");
		while(getchar() != '\n' && getchar() != EOF);
		getchar();
		success = 1;
	return success;
} //Funzione che permette di fare l'impostazione iniziale del gioco

void gioca(){
	int gameOver = 0;
	bool ok;
	while(gameOver == 0){
		int nTurni[inserimento];
		time_t t;
		srand((unsigned)time(&t));
		for(int i = 0; i < inserimento; i++){
			nTurni[i]=rand()%inserimento;
			for(int c = 0; c < i; c++){
				if(nTurni[i] == nTurni[c]){
					i--;
					break;
				}
			}
		}
		for(int turnoAtt = 0; (turnoAtt < inserimento) && (gameOver == 0); turnoAtt++){
			bool cadaveri = false;
			int test = 0;
			ok = false;

			switch (giocatori[nTurni[turnoAtt]].stato){ //Switch per la differenzazione in base al tipo di giocatore

				case 0:
				cadaveri = Cadaveri(nTurni, turnoAtt);
					inizioTurno(nTurni, turnoAtt);
					switch(giocatori[nTurni[turnoAtt]].posizioneA -> tipo){ //Switch per la differenzazione in base al tipo di stanza
						case 0:
							while(ok == false){
								if(cadaveri == true){
									printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n=============================\n");
									scanf("%d", &test);
									switch (test){
										case 1:
											avanza(nTurni, turnoAtt);
											ok = true;
											break;

										case 2:
											chiamata_emergenza(nTurni, turnoAtt);
											gameOver = win();
											ok = true;
											break;

										default:
											break;
									}
								}else{
									printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n=============================\n");
									scanf("%d", &test);
									switch (test){
										case 1:
											avanza(nTurni, turnoAtt);
											ok = true;
											break;

										default:
											break;
									}
								}
							}
						break;

						case 1:
							while(ok == false){
								if(cadaveri == true){
									printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per eseguire la quest\n=============================\n");
									scanf("%d", &test);
									switch (test){
										case 1:
											avanza(nTurni, turnoAtt);
											ok = true;
											break;

										case 2:
											chiamata_emergenza(nTurni, turnoAtt);
											gameOver = win();
											ok = true;
											break;

										case 3:
											esegui_quest_semplice(nTurni, turnoAtt);
											gameOver = win();
											ok = true;
											break;

										default:
											break;
									}
								}else{
									printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per eseguire la quest\n=============================\n");
									scanf("%d", &test);
									switch (test){
										case 1:
											avanza(nTurni, turnoAtt);
											ok = true;
											break;

										case 2:
											esegui_quest_semplice(nTurni, turnoAtt);
											gameOver = win();
											ok = true;
											break;

										default:
											break;
									}
								}
							}
						break;

						case 2:
						while(ok == false){
							if(cadaveri == true){
								printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per eseguire la quest\n=============================\n");
								scanf("%d", &test);
								switch (test){
									case 1:
										avanza(nTurni, turnoAtt);
										ok = true;
										break;

									case 2:
										chiamata_emergenza(nTurni, turnoAtt);
										gameOver = win();
										ok = true;
										break;

									case 3:
										esegui_quest_complessa(nTurni, turnoAtt);
										gameOver = win();
										ok = true;
										break;

									default:
										break;
								}
							}else{
								printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per eseguire la quest\n=============================\n");
								scanf("%d", &test);
								switch (test){
									case 1:
										avanza(nTurni, turnoAtt);
										ok = true;
										break;

									case 2:
										esegui_quest_complessa(nTurni, turnoAtt);
										gameOver = win();
										ok = true;
										break;

									default:
										break;
								}
							}
						}
						break;

						case 3:
						while(ok == false){
							if(cadaveri == true){
								printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n=============================\n");
								scanf("%d", &test);
								switch (test){
									case 1:
										avanza(nTurni, turnoAtt);
										ok = true;
										break;

									case 2:
										chiamata_emergenza(nTurni, turnoAtt);
										gameOver = win();
										ok = true;
										break;

									default:
										break;
								}
							}else{
								printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n=============================\n");
								scanf("%d", &test);
								switch (test){
									case 1:
										avanza(nTurni, turnoAtt);
										ok = true;
										break;

									default:
										break;
								}
							}
						}
						break;

						default:
						break;
					}
				break;

				case 1:
				cadaveri = Cadaveri(nTurni, turnoAtt);
							inizioTurno(nTurni, turnoAtt);
							switch(giocatori[nTurni[turnoAtt]].posizioneA -> tipo){
								case 0:
									while(ok == false){
										if(cadaveri == true){
											if(giocatoriInsieme != 0){
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per assassinare un astronauta\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														chiamata_emergenza(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													case 3:
														kill(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													default:
														break;
												}
											}else{
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														chiamata_emergenza(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													default:
														break;
												}
											}
										}else{
											if(giocatoriInsieme != 0){
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per assassinare un astronauta\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														kill(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													default:
														break;
												}
											}else{
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													default:
														break;
												}
											}
										}
									}
								break;

							case 1:
									while(ok == false){
										if(cadaveri == true){
											if(giocatoriInsieme != 0){
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per sabotare la quest\n- 4 Per assassinare un astronauta\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														chiamata_emergenza(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													case 3:
														sabota(nTurni, turnoAtt);
														ok = true;
														break;

													case 4:
														kill(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													default:
														break;
												}
											}else{
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per sabotare la quest\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														chiamata_emergenza(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													case 3:
														sabota(nTurni, turnoAtt);
														ok = true;
														break;

													default:
														break;
												}
											}
										}else{
											if(giocatoriInsieme != 0){
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per sabotare la quest\n- 3 Per assassinare un astronauta\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														sabota(nTurni, turnoAtt);
														ok = true;
														break;

													case 3:
														kill(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

													default:
														break;
												}
											}else{
												printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per sabotare la quest\n=============================\n");
												scanf("%d", &test);
												switch (test){
													case 1:
														avanza(nTurni, turnoAtt);
														ok = true;
														break;

													case 2:
														sabota(nTurni, turnoAtt);
														ok = true;
														break;

													default:
														break;
												}
											}
										}
									}
							break;

							case 2:
								while(ok == false){
									if(cadaveri == true){
										if(giocatoriInsieme != 0){
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per sabotare la quest\n- 4 Per assassinare un astronauta\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													chiamata_emergenza(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												case 3:
													sabota(nTurni, turnoAtt);
													ok = true;
													break;

												case 4:
													kill(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												default:
													break;
											}
										}else{
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n- 3 Per sabotare la quest\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

													case 2:
														chiamata_emergenza(nTurni, turnoAtt);
														gameOver = win();
														ok = true;
														break;

												case 3:
													sabota(nTurni, turnoAtt);
													ok = true;
													break;

												default:
													break;
											}
										}
									}else{
										if(giocatoriInsieme != 0){
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per sabotare la quest\n- 3 Per assassinare un astronauta\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													sabota(nTurni, turnoAtt);
													ok = true;
													break;

												case 3:
													kill(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												default:
													break;
											}
										}else{
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per sabotare la quest\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													sabota(nTurni, turnoAtt);
													ok = true;
													break;

												default:
													break;
											}
										}
									}
								}
							break;

							case 3:
								while(ok == false){
									if(cadaveri == true){
										if(giocatoriInsieme != 0){
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n - 3 Per usare la botola\n- 4 Per assassinare un astronauta\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													chiamata_emergenza(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												case 3:
													botolafn(nTurni, turnoAtt);
													ok = true;
													break;

												case 4:
													kill(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												default:
													break;
											}
										}else{
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per effettuare una chiamata di emergenza\n - 3 Per usare la botola\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													chiamata_emergenza(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												case 3:
													botolafn(nTurni, turnoAtt);
													ok = true;
													break;

												default:
													break;
											}
										}
									}else{
										if(giocatoriInsieme != 0){
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per usare la botola\n- 3 Per assassinare un astronauta\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													botolafn(nTurni, turnoAtt);
													ok = true;
													break;

												case 3:
													kill(nTurni, turnoAtt);
													gameOver = win();
													ok = true;
													break;

												default:
													break;
											}
										}else{
											printf("\n=============================\nScegli:\n- 1 Per cambiare stanza\n- 2 Per usare la botola\n=============================\n");
											scanf("%d", &test);
											switch (test){
												case 1:
													avanza(nTurni, turnoAtt);
													ok = true;
													break;

												case 2:
													botolafn(nTurni, turnoAtt);
													ok = true;
													break;

												default:
													break;
											}
										}
									break;
									}
								}
							break;

							default:
								break;
							}
				break;

				case 2:
				break;

				case 3:
				break;
			}
		}

	}
	libera_memoria();

} //Funzione che gestisce i vari turni, e li differenzia in base al tipo di giocatore e di stanza (tramite 2 switch)
