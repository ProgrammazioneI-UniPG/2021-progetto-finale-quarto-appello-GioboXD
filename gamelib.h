
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

enum Stato_giocatore {astronauta, impostore, assassinato, defenestrato};
enum Nome_giocatore {rosso, blu, rosa, giallo, arancione, verde, ciano, viola, bianco, nero};
enum Tipo_stanza {vuota, quest_semplice, quest_complicata, botola};

struct Giocatore{
	struct Stanza* posizioneA;
	enum Stato_giocatore stato;
	enum Nome_giocatore nome;
};

struct Stanza{
	struct Stanza* avanti;
	struct Stanza* destra;
	struct Stanza* sinistra;
	struct Stanza* pNext;
	struct Stanza* stanza_precedente;

	enum Tipo_stanza tipo;

	bool emergenza_chiamata;
};


int imposta_gioco();
void gioca();
void libera_memoria();
void cancella();
