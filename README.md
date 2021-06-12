BOMBGAME	
===========

Comenzi
=======
1. Pentru rularea programului, comanda "make" sau "make run"
2. Pentru oprirea programului, comanda "make kill"
3. Pentru recompilare, comanda "make clean"



Reguli
=======
nota: toate distantele se refera la distanta manhattan.

Fiecare echipa are 5 jucatori. Scopul este de a lua steagul adversarilor si a-l aduce in baza. 
Fiecare gameTick este impartit in 4 actiuni, iar fiecare player poate alege ce sa faca doar pentru primele actiuni, in functie de viteza acestuia.
Spre exemplu, daca un player are viteza 2 iar alt player are viteza 3, primul player va putea alege ce actiunea doar pentru primele doua actiuni, iar pentru celelalte va fi fortat sa stea pe loc, in timp ce playerul cu viteza 3 va alege primele 3 actiuni si va sta pe loc ca ultima actiune.
Actiunile sunt de 6 tipuri:

- const int MOVE_STAY = 0;
- const int MOVE_UP = 1;
- const int MOVE_LEFT = 2;
- const int MOVE_DOWN = 3;
- const int MOVE_RIGHT = 4;
- const int MOVE_ABILITY = 5;

primele 5 reprezentand miscarea playerului, iar ultima actiune reprezentand utilizarea abilitatii.
Pentru un player cu viteza diferita de 4, ultimele actiuni ramase vor fi automat egale cu MOVE_STAY;
Abilitatiile au cooldown, in functie de clasa, daca un player incearca sa isi foloseasca abilitatea cand aceasta este pe cooldown, actiunea respectiva va fi inlocuita de MOVE_STAY.
Inainte de fiecare tick, playerii vor trimite ce actiuni vor sa faca in tick-ul respectiv, iar acestea se vor efectua in acelasi timp incepand cu prima actiune, apoi a doua, etc. In timpul unei actiuni, intai se fac mutarile, apoi abilitatile.

Un pozitia unui player este cunoscuta de echipa inamica daca si numai daca cel putin una din urmatoarele conditii este valida:
1. playerul are steagul inamic.
2. playerul se afla la distanta cel mult 13 de baza steagului inamic.
3. playerul nu este invizibil si se afla la distanta cel mult 10 de un player inamic.

In functie de clasa aleasa, playerii au diferite viteze si abilitati.

Clase
=======
**1. Bomber**
- Viteza: 3
- Viteza cu steagul: 2
- HP: 1

Abilitate: explodeaza pe o raza egala cu 6, si scade 1 din viata tuturor playerilor din aceasta raza (inclusiv el si alti aliati)
Cooldown: 9 tick-uri

Nota: Deoarece aceasta abilitate omoara playerul respectiv, aceasta nu are nevoie de cooldown, acesta e just in case.

**2. Ninja**
- viteza: 4
- viteza cu steagul: 3
- HP: 1

Abilitate: pe o raza egala cu 4, face toti aliatii (el inclusiv) invizibili pentru urmatoarele 5 tick-uri.
Cooldown: 15 tick-uri

**3. Tank**
- Viteza: 2
- viteza cu steagul: 2
- HP: 2

Abilitate: pe o raza egala cu 8, scade 1 din viteza tuturor inamicilor (aceasta scadere nu se stackeaza) pentru urmatoarele 8 tick-uri
Cooldown: 10 tick-uri


In cazul in care cineva are alte idei de clase (sau gaseste bug-uri), ne puteti scrie pe discord:
lucaperju#7280
jido#9711

Protocol (kinda)
=======

**Structura Coordinates:**
```
struct Coordinates{
	int row, col;
};
```
In plus, are si operator de == si !=

**Structura Playerstate:**

```
struct PlayerState{
	Coordinates position; // pozitia playerului
	int speed; // viteza playerului
	int respawnTime; // este in viata daca respawnTime = 0, altfel, se va respawna peste respawnTime tick-uri
	int slowTime; // nu este incetinit daca slowTime = 0, altfel, incetinirea va trece peste slowTime tick-uri
	int invisibleTime; // este vizibil daca invisibleTime = 0, altfel, invizibilitatea va trece peste invisibleTime tick-uri
    int cooldown; // numarul de tick-uri ramase pana cand playerul isi poate folosi abilitatea
	   
    int hp; // HP-ul ramas al playerului
    int team; // echipa playerului
    bool hasFlag; // este true daca playerul are steagul inamic
    
    int classType; // clasa aleasa de player
};
```

**Structura FlagState:**

```
struct FlagState{
	Coordinates position; // pozitia steagului
    
	int isAtPlayer; // numarul playerului care are steagul, in cazul in care steagul nu este la niciun player, aceasta are valorile de mai jos:
};
```
- const int AT\_HOME = -1;_
- const int ON\_GROUND = -2;_

**Structura GameTable:**
```
struct GameTable{
	PlayerState players[NUMBER_OF_PLAYERS]; // playerii 0-4 sunt in prima echipa (RED\_TEAM) iar playerii 5-9 sunt in a doua echipa (BLUE_TEAM)
	FlagState flags[NUMBER_OF_TEAMS];
	int gameState;
};
```
**Structura ServerMessage:**
```
struct ServerMessage {
	GameTable table;
	int currentPlayer; // playerul la mutare
};
```
**Structura PlayerMessage:**
```
struct PlayerMessage {
	int actions[MAX_SPEED];
};
```
**Structura PlayerInitMessage:**
(trimisa de playeri la inceput pentru a isi alege clasa)
```
struct PlayerInitMessage{
	int type; // reprezinta clasa
};
```
---
La inceput, fiecare player trebuie sa citeasca un ServerMessage, in care va vedea clasele playerilor care au ales deja, iar apoi trebuie sa isi aleaga clasa.
Initial, playerii 0 si 5 vor trimite ce clasa isi aleg, fara a sti clasele celorlalti playeri, 
Apoi playerii 1 si 6 vor trimite ce clasa aleg, stiind clasele alese de playerii 0 si 5, 
etc..

*TODO: functiile din protocol*

