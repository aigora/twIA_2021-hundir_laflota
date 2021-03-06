#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <locale.h>


// variables globales para utilizar las flechas del teclado
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77

// varialoes globales auxiliares
#define TAM 9
#define NBARCOS 5
#define LTEXTOS 30
#define FIN_JUEGO 5


// estructura con la posicion
typedef struct
{
	int fila, columna;
}POSICION;


// estructura con los datos de cada barco
typedef struct
{
	POSICION posicion;
	char orientacion;
	char simbolo;
	int longitud;
	char nombre[LTEXTOS];
}BARCO;

typedef struct
{
	int tocados;            // Guarda el numero de tocados 
	int tocados_seguidos; // Guarda el numero de tocados seguidos
	int ndisparos;        // Guarda el numero total de disparos 
	char nombre[LTEXTOS];
	float precision;

	POSICION disparo_anterior;  // Almacena la posicion de disparo aterior
	POSICION posicion[15];    // Almacena la posicion de cada casilla tocada

} MEMORIA;

// Funciones 

// Menus para elegir el modo de juego
int menu();
int menu1();
int menu2();


void jugar();
void instrucciones();


// funciones para colocar los barcos
char leer_orientacion(char[]);
void barcos_usuario(char[TAM][TAM], BARCO[],int, int); 
void barcos_maquina(char[TAM][TAM], BARCO[]);


// funciones para comprobar si es correcta la posicion de los barcos del usuario
int hueco_libre(int, int, char, int, char[][TAM]);
int hueco_suficiente(int, int, char, int);


// funciones para los disparos
POSICION posicion_de_disparo_usuario(int, int);
int disparos_usuario(char[][TAM], char[][TAM], char[][TAM], POSICION,MEMORIA*,MEMORIA ,int, int);
void disparos_maquina(char[][TAM], char[][TAM], POSICION, MEMORIA,MEMORIA*, int,int *);
int comprobar_disparo_repetido(MEMORIA, POSICION);


// funcion para impresion de datos en fichero
void fichero1(MEMORIA,MEMORIA,char[][TAM],char[][TAM]);


// funciones auxiliares
void gotoxy(int, int);
void wait_intro();
void borrar_pantalla();
void vaciar_tablero(char[][TAM]);
void imprimir_tablero(char[][TAM], int, int);
void fin_juego(int, int);
void mostrar_marcador(MEMORIA,MEMORIA);
void barco_adorno();




int main()
{
	int opcion;
	setlocale(LC_ALL, "spanish"); // Para configurar alfabeto en castellano
	srand(time(NULL));
	do
	{
		borrar_pantalla();
		barco_adorno();
		opcion = menu();

	} while (opcion != 2 && opcion != 1 && opcion != 3);
	switch (opcion)
	{
	case 1:
		jugar();
		break;
	case 2:
		instrucciones();
		break;
	case 3:
		break;
	default:
		printf("\n---OPCION INCORRECTA---");
		printf("Pulse intro\n");
		getchar();
	}
}




int menu()
{
	int opc;

	gotoxy(40, 10);
	printf("1.JUGAR\n");
	gotoxy(40, 11);
	printf("2.INSTRUCCIONES\n");
	gotoxy(40, 12);
	printf("3.SALIR\n");
	printf("Seleccione una opcion: ");
	scanf_s("%d", &opc);
	getchar();
	return opc;
}

int menu1()
{
	int modo;

	borrar_pantalla();
	barco_adorno();

	gotoxy(40, 10);
	printf("1.DOS JUGADORES\n");
	gotoxy(40, 11);
	printf("2.UN JUGADOR\n\n");

	printf("Seleccione un modo de juego: ");
	scanf_s("%d", &modo);
	getchar();
	return modo;
}

int menu2()
{
	int modo;

	borrar_pantalla();
	barco_adorno();

	gotoxy(40, 10);
	printf("1.FACIL\n");
	gotoxy(40, 11);
	printf("2.DIFICIL\n\n");

	printf("Seleccione la dificultad: ");
	scanf_s("%d", &modo);
	getchar();
	return modo;
}



void jugar()
{
	char tablero1[TAM][TAM]; // El tablero comienza en (0,0) y se extiende hasta (TAM,TAM)
	char tablero2[TAM][TAM];// El tablero comienza en (0,0) y se extiende hasta (TAM,TAM)
	char tablero3[TAM][TAM];
	char tablero4[TAM][TAM];
	int fila, columna, tocados_usuario = 0, tocados_maquina = 0, error, modo, hundidos[] = { 0,0,0,0,0 }, impresos[] = { 0,0,0,0,0 }, c1 = 15, c2 = 75, tocados_maquina_seguidos;
	BARCO barcos1[NBARCOS]; // Espacio para almacenar los datos de cada barco
	BARCO barcos2[NBARCOS]; // Espacio para almacenar los datos de cada barco
	POSICION disparo;
	MEMORIA usuario1,*pusuario1;
	MEMORIA usuario2,*pusuario2;

	pusuario1 = &usuario1;
	pusuario2 = &usuario2;

	pusuario1->tocados = 0;
	pusuario2->tocados = 0;
	pusuario2->tocados_seguidos = 0;
	pusuario1->ndisparos = 0;
	pusuario2->ndisparos = 0;



	borrar_pantalla();
	barco_adorno();

	do
	{
		modo = menu1();

	} while (modo != 1 && modo != 2);

	borrar_pantalla();
	barco_adorno();
	vaciar_tablero(tablero1);
	vaciar_tablero(tablero2);
	vaciar_tablero(tablero3);
	vaciar_tablero(tablero4);

	switch (modo)
	{
	case 1:
	{

		// El usuario introduce su nombre
		printf("\nIntroduce el nombre de usuario del jugador1: ");
		gets_s(usuario1.nombre);
		printf("\nIntroduce el nombre de usuario del jugador2: ");
		gets_s(usuario2.nombre);

		barcos_usuario(tablero1, barcos1, 12, 15);
		gotoxy(50, 9);
		printf("TURNO DEL OPONENTE");
		gotoxy(50, 10);
		printf("Pulse una tecla para continuar");
		wait_intro();
		barcos_usuario(tablero2, barcos2, 12, 75);
		gotoxy(50, 9);
		printf("TURNO DEL OPONENTE");
		gotoxy(50, 10);
		printf("Pulse una tecla para continuar");
		wait_intro();

		mostrar_marcador( usuario1, *pusuario2);

		do
		{
			do
			{
				pusuario1->ndisparos++;
				error = 0;
				borrar_pantalla();
				barco_adorno();
				mostrar_marcador(usuario1,usuario2);
				imprimir_tablero(tablero3, 12, 75);
				imprimir_tablero(tablero1, 12, 15);
				disparo = posicion_de_disparo_usuario(12, 75);

				error = disparos_usuario(tablero1, tablero2, tablero3, disparo,pusuario1,usuario2 ,c1, c2);

			} while (error == 0 && tocados_usuario != FIN_JUEGO);

			gotoxy(50, 9);
			printf("TURNO DEL OPONENTE");
			gotoxy(50, 10);
			printf("Pulse una tecla para continuar");
			wait_intro();
			borrar_pantalla();
			barco_adorno();
			mostrar_marcador(usuario1, usuario2);
			imprimir_tablero(tablero2, 12, 75);
			imprimir_tablero(tablero4, 12, 15);


			if (pusuario1->tocados != FIN_JUEGO)
			{
				do
				{
					error = 0;
					pusuario1->ndisparos++;

					disparo = posicion_de_disparo_usuario(12, 15);

					error = disparos_usuario(tablero2, tablero1, tablero4, disparo, pusuario2,usuario1,c2, c1);

				} while (error == 0 && pusuario2->tocados != FIN_JUEGO);

			}
			gotoxy(50, 9);
			printf("TURNO DEL OPONENTE");
			gotoxy(50, 10);
			printf("Pulse una tecla para continuar");
			wait_intro(); 


		} while (pusuario1->tocados != FIN_JUEGO && pusuario2->tocados != FIN_JUEGO);

		fin_juego(tocados_usuario, tocados_maquina);

		break;
	}

	case 2:
	{
		do
		{
			modo = menu2();

		} while (modo != 1 && modo != 2);

		printf("\nIntroduce el nombre de usuario: ");
		gets_s(usuario1.nombre);

		// -> Hay que dar nombre al usuario2 <-
		strcpy_s(usuario2.nombre, LTEXTOS, "M?quina");

		barcos_usuario(tablero1, barcos1, 12, 15);
		barcos_maquina(tablero2, barcos2);

		imprimir_tablero(tablero3, 12, 75);
		mostrar_marcador(usuario1, usuario2);

		do
		{
			do
			{
				error = 0;
				pusuario1->ndisparos++;
				disparo = posicion_de_disparo_usuario(12, 75);
				error = disparos_usuario(tablero1, tablero2, tablero3, disparo, pusuario1,usuario2 ,c1, c2);

			} while (error == 0 && pusuario1->tocados != FIN_JUEGO);

			borrar_pantalla();
			mostrar_marcador( usuario1, usuario2);
			barco_adorno();

			gotoxy(50, 9);
			printf("TURNO DEL OPONENTE");
			gotoxy(50, 10);
			printf("Pulse una tecla para continuar");

		 	wait_intro();

			if (pusuario1->tocados != FIN_JUEGO)
			{

				pusuario2->tocados_seguidos = 0;
				error = 0;
				pusuario2->ndisparos++;

				do
				{
						disparos_maquina(tablero1,tablero3, disparo,usuario1,pusuario2,modo,&error);
						wait_intro();

				} while (error == 0 && pusuario2->tocados != FIN_JUEGO);

				gotoxy(50, 9);
				printf("ES SU TURNO");
				gotoxy(50, 10);
				printf("Pulse una tecla para continuar");
				wait_intro();
				borrar_pantalla();
				barco_adorno();
				mostrar_marcador(usuario1,usuario2);
				imprimir_tablero(tablero3, 12, 75);
				imprimir_tablero(tablero1, 12, 15);
			}

		} while (pusuario1->tocados != FIN_JUEGO && pusuario2->tocados != FIN_JUEGO);

		fin_juego(pusuario1->tocados, pusuario2->tocados);
		fichero1(usuario1, usuario2,tablero1,tablero2);

		break;
	}

	}
}

void instrucciones()
{
	char texto[] = "@ 1.El juego consiste en hundir la flota del contrincante. Para ello, debe colocar su propia \nflota de forma estrategica y encontrar y hundir con los disparos la flota contraria. \n@2. Cada uno de los jugadores dispone de dos tableros que ocultara al otro jugador: en \nuna debe colocar sus barcos y en la otra ira anotando los resultados de los disparos que \nrealiza en cada turno. \n@3. Cada jugador debe colocar en el tablero primero los siguientes barcos en posicion horizontal o vertical: \n1. Barco que ocupa 5 puestos o portaaviones.\n2. Barco de 4 puestos o crucero. \n3. Barco de 3 puestos o buque. \n4. Barco de 2 puestos o submarino.\n5. Barco de 1 solo puesto o lancha.\n@4. Cada jugador dispone de un turno de disparo que se ira alternando. Para hacerlo se desplazara sobre el tablero\ncon las flechas y con el espacio ejecutara el disparo en las coordenadas que haya indicado.\nAl disparar, el otro jugador comprobara el resultado en su tablero: \nSi la casilla esta marcada con una X, respondera 'agua'. \nSi en la casilla se encuentra parte de un barco respondera 'tocado'.\nEn ese caso el jugador tiene derecho a un nuevo disparo en el mismo turno.Si en la casilla\n se encuentra la ultima parte de un barco que haya tocado,\n respondera 'hundido' y tambien tiene derecho a un nuevo disparo.\n@5. El jugador que dispara anota en su tablero de disparos los resultados. Si los tiros son \n'agua', marcara con una 'X' la cuadricula; si los disparos son 'tocado' o 'hundido', los \npuede marca con un '*'. De esta forma el jugador puede saber las cuadriculas que \nquedan en blanco y en las que ya ha disparado.\n@6.Finalmente, gana el jugador que antes consigue hundir la flota del otro.";
	int i = 0, opcion;

	borrar_pantalla();
	barco_adorno();

	/* gotoxy(25, 25);  // Como hacer que pare cuando pulse una tecla
	printf("Pulsa una tecla para salir");*/


	while (texto[i] != '\0')
	{

		if (texto[i] == '@')
		{
			Sleep(250);
			borrar_pantalla();
			barco_adorno();
			printf("\n\n\n\n\n\n");
		}
		else
			printf("%c", texto[i]);
		i++;
		Sleep(70);
	}

	borrar_pantalla();
	barco_adorno();

	do
	{
		gotoxy(40, 10);
		printf("Desea empezar a jugar?\n");
		gotoxy(40, 12);
		printf("1.SI\n");
		gotoxy(40, 13);
		printf("2.NO\n");
		printf("Seleccione una opcion: ");
		scanf_s("%d", &opcion);
		getchar();
	} while (opcion != 1 && opcion != 2);

	if (opcion == 1)
	{
		jugar();
	}
}


char leer_orientacion(char nombre[])
{
	char orientacion;
	char cadena[LTEXTOS];
	do
	{
		printf("\nIntroduzca orientacion del %s (H=Horizontal, V=Vertical):", nombre);   // Pedimos y guardamos la orientacion de cada barco
		
		// -> Optamos por leer cadenas para que la entrada sea m?s robusta <-
		gets_s(cadena, LTEXTOS);
		orientacion = cadena[0];

		/*scanf_s("%c", &orientacion, 1);
		getchar();    // A?adimos un getchar para evitar que guarde valores libres y se superpongan los printf
		*/
		
		if (orientacion == 'h')    // Transformaci?n a may?sculas si son h o v (min?sculas)
			orientacion = 'H';
		else
			if (orientacion == 'v')
				orientacion = 'V';

		if (orientacion != 'H' && orientacion != 'V')
			printf("La orientacion debe ser H o V\n");
	} while (orientacion != 'H' && orientacion != 'V');  // Repetimos el bucle hasta que el caracter introducido sea igual a h, H, v ? V
	return orientacion;
}

void barcos_usuario(char tablero[TAM][TAM], BARCO barcos[], int f, int c)
{
	const char* nombres[] = { "Portaaviones","Crucero","Buque","Submarino","Lancha" };
	int longitudes[] = { 5,4,3,2,1 };
	char simbolos[] = { 'P','C','B','S','L' };
	int i, j, fila, columna, longitud, hay_hueco, esta_libre;    // Variables auxiliares

	borrar_pantalla();
	barco_adorno();

	gotoxy(45, 10);
	// Recorremos el bucle para crear cada barco
	for (i = 0; i < NBARCOS; i++)
	{
		strcpy_s(barcos[i].nombre, nombres[i]);   // Copiamos el nombre de cada barco dentro de la variable nombre de la estructuctura con los datos de los barcos
		barcos[i].longitud = longitudes[i];      // Guardamos la longitud de cada barco dentro de la variable longitud de la estructuctura con los datos de los barcos
		barcos[i].simbolo = simbolos[i];        // Guardamos el simbolo de cada barco dentro de la variable simbolo de la estructuctura con los datos de los barcos


		barcos[i].orientacion = leer_orientacion(barcos[i].nombre);  // Guardamos la orientacion  de cada barco dentro de la variable orientacion de la estructuctura con los datos de los barcos


		// bucle para guardar la posici?n de cada barco
		do
		{
			printf("Introduzca posicion del %s (fila columna):", barcos[i].nombre);     // Pedimos y guardamos la posici?n de cada barco
			scanf_s("%d %d", &fila, &columna);                      // IMPORTANTE QUE SE PONGA UN ESPACION ENTRE AMBOS SI NO EL BUCLE NO AVANZA
			getchar();        // A?adimos un getchar para evitar que guarde valores libres y se superpongan los printf

			hay_hueco = hueco_suficiente(fila, columna, barcos[i].orientacion, barcos[i].longitud);
			if (hay_hueco == 0)
				printf("En esa posicion el %s no cabe en el tablero\n", barcos[i].nombre);
			else
			{
				esta_libre = hueco_libre(fila, columna, barcos[i].orientacion, barcos[i].longitud, tablero);
				if (esta_libre == 0)
					printf("En esa posicion el %s solapa con otro barco\n", barcos[i].nombre);
			}
		} while (hay_hueco == 0 || esta_libre == 0);

		barcos[i].posicion.fila = fila;             // Guardamos la posicion de cada barco dentro de las variables fila y columna de la estructuctura con los datos de los barcos
		barcos[i].posicion.columna = columna;

		//Guardamos el barco en el tablero
		for (j = 0; j < barcos[i].longitud; j++)
		{
			if (barcos[i].orientacion == 'H')         // Diferenciamos si la orientaci?n elegida fue Horizontal o Vertical para introducir un vector fila o un vector columna
				tablero[fila][columna + j] = barcos[i].simbolo;
			else
				tablero[fila + j][columna] = barcos[i].simbolo;
		}
		borrar_pantalla();
		barco_adorno();
		imprimir_tablero(tablero, f, c);
	}
}

void barcos_maquina(char tablero[TAM][TAM], BARCO barcos[])
{
	const char* nombres[] = { "Portaaviones","Crucero","Buque","Submarino","Lancha" };
	int longitudes[] = { 5,4,3,2,1 };
	char simbolos[] = { 'P','C','B','S','L' };

	int i, j, fila, columna, longitud, orientacion, hay_hueco, esta_libre;    // Variables auxiliares
	char usuario[LTEXTOS];

	for (i = 0; i < NBARCOS; i++)
	{
		strcpy_s(barcos[i].nombre, nombres[i]);   // Copiamos el nombre de cada barco dentro de la variable nombre de la estructuctura con los datos de los barcos de la maquina
		barcos[i].longitud = longitudes[i];      // Guardamos la longitud de cada barco dentro de la variable longitud de la estructuctura con los datos de los barcos de la maquina
		barcos[i].simbolo = simbolos[i];        // Guardamos el simbolo de cada barco dentro de la variable simbolo de la estructuctura con los datos de los barcos de la maquina

		// bucle para generar de manera aleatoria la orientacion de cada barco

		orientacion = rand() % 2;  // funcion rand genera un valor aleatorio en un intervalo

		if (orientacion == 0)
			barcos[i].orientacion = 'H';
		else if (orientacion == 1)
			barcos[i].orientacion = 'V';

		// bucle para generar de manera aleatoria la posici?n de cada barco
		do
		{
			columna = rand() % 9;
			fila = rand() % 9;

			hay_hueco = hueco_suficiente(fila, columna, barcos[i].orientacion, barcos[i].longitud);
			esta_libre = hueco_libre(fila, columna, barcos[i].orientacion, barcos[i].longitud, tablero);

		} while (hay_hueco == 0 || esta_libre == 0);

		barcos[i].posicion.fila = fila;             // Guardamos la posicion de cada barco dentro de las variables fila y columna de la estructuctura con los datos de los barcos
		barcos[i].posicion.columna = columna;


		//Guardamos el barco en el tablero de la maquina
		for (j = 0; j < barcos[i].longitud; j++)
		{
			if (barcos[i].orientacion == 'H')         // Diferenciamos si la orientaci?n fue Horizontal o Vertical para introducir un vector fila o un vector columna
				tablero[fila][columna + j] = barcos[i].simbolo;
			else
				tablero[fila + j][columna] = barcos[i].simbolo;
		}

	}

}



int disparos_usuario(char tablero1[][TAM], char tablero2[][TAM], char tablero3[][TAM], POSICION disparo, MEMORIA* pmemoria, MEMORIA memoria2, int c1, int c2)
{
	int error = 0;


	borrar_pantalla();
	barco_adorno();
	mostrar_marcador(*pmemoria, memoria2);
	imprimir_tablero(tablero1, 12, c1);
	imprimir_tablero(tablero3, 12, c2);


	error = 0;

	if (tablero2[disparo.fila][disparo.columna] == ' ')
	{
		gotoxy(50, 25);
		printf("Has disparado a (%d %d)\n", disparo.fila, disparo.columna);
		gotoxy(53, 26);
		printf("------AGUA-----");
		tablero3[disparo.fila][disparo.columna] = 'X';
		tablero2[disparo.fila][disparo.columna] = 'X';
		imprimir_tablero(tablero3, 12, c2);
		error = 1;
	}
	else if (tablero2[disparo.fila][disparo.columna] == 'X' || tablero2[disparo.fila][disparo.columna] == '*')
	{
		gotoxy(50, 25);
		printf("Ya has disparado a (%d %d)\n", disparo.fila, disparo.columna);
		error = 0;
	}
	else
	{
		gotoxy(50, 25);
		printf("Has disparado a (%d %d)\n", disparo.fila, disparo.columna);
		gotoxy(53, 26);
		printf("----TOCADO----");
		tablero2[disparo.fila][disparo.columna] = '*';
		tablero3[disparo.fila][disparo.columna] = '*';
		pmemoria->tocados++;
		imprimir_tablero(tablero3, 12, c2);
		error = 0;
	}

	mostrar_marcador(*pmemoria,memoria2);

	return error;
}

POSICION posicion_de_disparo_usuario(int f, int c)
{
	POSICION p;
	int hay_disparo = 0;
	int limite_dcho = c + (TAM - 1) * 4;
	int limite_inf = f + 1 + (TAM - 1);
	int fila, columna;

	fila = f + 1; // En la primera fila est?n las coordendas
	columna = c + 3; // En la primera columna est?n las coordenadas, luego un espacio en blanco y luego el [

	gotoxy(40, 25);
	printf("Selecciona con las flechas la posicion del disparo\n");
	gotoxy(40, 26);
	printf("Pulsa el espacio cuando lo hayas decidido");

	while (hay_disparo == 0)
	{
		gotoxy(columna, fila);
		if (_kbhit())
		{
			char tecla = _getch();
			gotoxy(columna, fila);

			if (tecla == IZQUIERDA && columna > c + 3)
				columna -= 4;
			if (tecla == DERECHA && columna < limite_dcho)
				columna += 4;
			if (tecla == ARRIBA && fila > f + 1)
				fila--;
			if (tecla == ABAJO && fila < limite_inf)
				fila++;
			if (tecla == ' ')
			{
				hay_disparo = 1;
				p.fila = fila - 13;
				p.columna = ((columna - c) / 4);
			}
			gotoxy(columna, fila);
		}
		Sleep(30);
	}
	return p;
}


void disparos_maquina(char tablero1[][TAM], char tablero3[][TAM], POSICION disparo, MEMORIA memoria, MEMORIA* pmemoria2, int modo,int *error)
{
	int aleatorio;
	int j = 0,repetidos=0;

	borrar_pantalla();
	barco_adorno();
	mostrar_marcador(memoria,*pmemoria2);
	imprimir_tablero(tablero1, 12, 15);
	imprimir_tablero(tablero3, 12, 75);

	switch (modo)
	{
	case 1:
	{

		disparo.fila = rand() % 9;
		disparo.columna = rand() % 9;

		break;
	}
	case 2:
	{
		do
		{

			if(pmemoria2->tocados==0)  // dispara en el cuadrado del centro del tablero ya que la posibilidad de que haya ahi un barco es mayor
			{
				disparo.fila = 3+rand() % (6-3);
				disparo.columna =3+ rand() % (6-3) ;

				*error = 1;

			}
			else if (pmemoria2->tocados_seguidos == 1)   // si acierta dispara a una de las 4 casillas de alrededor 
			{
					aleatorio = rand() % 4;

					if (aleatorio == 0)
						pmemoria2->disparo_anterior.fila++;
					else if (aleatorio == 1)
						pmemoria2->disparo_anterior.fila--;
					if (aleatorio == 2)
						pmemoria2->disparo_anterior.columna++;
					else if (aleatorio == 3)
						pmemoria2->disparo_anterior.columna--;

					disparo = pmemoria2->disparo_anterior;

				*error = 1;

			}
			else if (pmemoria2->tocados_seguidos > 1) 
			{
				if ((pmemoria2->posicion[pmemoria2->tocados-1].fila - pmemoria2->posicion[pmemoria2->tocados -2].fila) == 1)
					pmemoria2->disparo_anterior.columna--;
				else if ((pmemoria2->posicion[pmemoria2->tocados-1].fila - pmemoria2->posicion[pmemoria2->tocados - 2].fila) == -1)
					pmemoria2->disparo_anterior.columna++;
				if ((pmemoria2->posicion[pmemoria2->tocados-1].columna - pmemoria2->posicion[pmemoria2->tocados - 2].columna) == 1)
					pmemoria2->disparo_anterior.fila--;
				else if ((pmemoria2->posicion[pmemoria2->tocados-1].columna - pmemoria2->posicion[pmemoria2->tocados - 2].columna) == -1)
					pmemoria2->disparo_anterior.fila++;

				*error = 1;
			}
			else if (repetidos > 3)  // si se atasca en algun lado disparo totalmente aleatorio
			{
				disparo.fila = rand() % 9;
				disparo.columna = rand() % 9;

				pmemoria2->disparo_anterior = disparo;
				*error = 1;

			}
			repetidos++;

			if (tablero1[disparo.fila][disparo.columna] == 'X')
			*error = 2;

		} while (*error != 1);

		break;
	}
	}


	if (tablero1[disparo.fila][disparo.columna] == ' ')
	{
		gotoxy(50, 25);
		printf("La maquina ha disparado a (%d %d)\n", disparo.fila, disparo.columna);
		gotoxy(53, 26);
		printf("----AGUA----");
		tablero1[disparo.fila][disparo.columna] = 'X';
		imprimir_tablero(tablero1, 12, 15);
		*error = 1;
	}
	else if (tablero1[disparo.fila][disparo.columna] == 'X' || tablero1[disparo.fila][disparo.columna] == '*')
	{
		imprimir_tablero(tablero1, 12, 15);
		*error = 0;
	}
	else
	{
		gotoxy(50, 25);
		printf("La maquina ha disparado a (%d %d)", disparo.fila, disparo.columna);
		tablero1[disparo.fila][disparo.columna] = '*';
		gotoxy(53, 26);
		printf("----TOCADO----");
		pmemoria2->posicion[pmemoria2->tocados].fila = disparo.fila;
		pmemoria2->posicion[pmemoria2->tocados].columna = disparo.columna;
		pmemoria2->disparo_anterior = pmemoria2->posicion[pmemoria2->tocados];
		pmemoria2->tocados++;
		pmemoria2->tocados_seguidos++;
		imprimir_tablero(tablero1, 12, 15);
		*error = 0;

	}
	pmemoria2->disparo_anterior = disparo;
}


int hueco_suficiente(int fila, int columna, char orientacion, int longitud)
{
	int resultado = 1;
	if (orientacion == 'H' && columna + longitud > TAM)     // Verificamos que las posiciones introducidas son correctas y el barco no se sale del tablero
		resultado = 0;
	else
		if (orientacion == 'V' && fila + longitud > TAM)
			resultado = 0;

	return resultado;
}

int hueco_libre(int fila, int columna, char orientacion, int longitud, char tablero[][TAM])
{
	int resultado = 1;
	int j;

	for (j = 0; j < longitud && resultado == 1; j++)
		if (orientacion == 'H' && tablero[fila][columna + j] != ' ')
			resultado = 0;
		else
			if (orientacion == 'V' && tablero[fila + j][columna] != ' ')
				resultado = 0;
	return resultado;
}



void gotoxy(int x, int y)
{
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

void barco_adorno()    // Decoramos el tablero con un barco fijo arriba
{
	gotoxy(25, 1);
	printf("                              o o o o o\n");
	gotoxy(25, 2);
	printf("                                    _\\_\\_ \n");
	gotoxy(25, 3);
	printf("                         ___________|_____|_________\n");
	gotoxy(25, 4);
	printf("                        \\                         /\n");
	gotoxy(25, 5);
	printf("                         \\  O  O  O  O  O  O  O  /\n");
	gotoxy(25, 6);
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^\\_____________________/^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");
	gotoxy(25, 7);
	printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\n");

}

void imprimir_tablero(char t[][TAM], int margen_fila, int margen_columna)
{
	int fila, columna;

	gotoxy(margen_columna, margen_fila);
	printf("  ");

	for (columna = 0;columna < TAM;columna++)
		printf(" %d  ", columna);
	printf("\n");
	gotoxy(margen_columna, ++margen_fila);
	for (fila = 0; fila < TAM; fila++)
	{
		printf("%d ", fila);
		for (columna = 0; columna < TAM; columna++)
			printf("[%c] ", t[fila][columna]);
		printf("\n");
		gotoxy(margen_columna, ++margen_fila);
	}
}

void vaciar_tablero(char t[][TAM])
{
	int fila, columna;

	for (fila = 0; fila < TAM; fila++)
		for (columna = 0; columna < TAM; columna++)
			t[fila][columna] = ' ';
}

void borrar_pantalla()
{
	system("cls");
}

void fin_juego(int tocados_usuario, int tocados_maquina)
{
	borrar_pantalla();

	if (tocados_maquina == FIN_JUEGO)
	{
		gotoxy(20, 5);
		printf("   _________    __  _________   ____ _    ____________\n");
		gotoxy(20, 6);
		printf("  / ____/   |  /  |/  / ____/  / __ \\ |  / / ____/ __ \\ \n");
		gotoxy(20, 7);
		printf(" / / __/ /| | / /|_/ / __/    / / / / | / / __/ / /_/ /\n");
		gotoxy(20, 8);
		printf("/ /_/ / ___ |/ /  / / /___   / /_/ /| |/ / /___/ __  /\n");
		gotoxy(20, 9);
		printf("\\____/_/  |_/_/  /_/_____/   \\____/ |___/_____/_/ |_|\n");

	}
	else if (tocados_usuario == FIN_JUEGO)
	{
		gotoxy(20, 5);
		printf("__     ______  _    _  __          _______ _   _ _ \n");
		gotoxy(20, 6);
		printf("\\ \\   / / __ \\| |  | | \\ \\        / /_   _| \\ | | |\n");
		gotoxy(20, 7);
		printf(" \\ \\_/ / |  | | |  | |  \\ \\  /\\  / /  | | |  \\| | |\n");
		gotoxy(20, 8);
		printf("  \\   /| |  | | |  | |   \\ \\/  \\/ /   | | | . ` | |\n");
		gotoxy(20, 9);
		printf("   | | | |__| | |__| |    \\  /\\  /   _| |_| |\\  |_|\n");
		gotoxy(20, 10);
		printf("   |_|  \\____/ \\____/      \\/  \\/   |_____|_| \\_(_)\n");
	}
	printf("\n\n\n");
	system("PAUSE");
}

void mostrar_marcador(MEMORIA memoria,MEMORIA memoria2)
{
	gotoxy(80, 1);
	printf("%s -> %d ", memoria.nombre, memoria.tocados);
	gotoxy(80, 2);
	printf("--------------");
	gotoxy(80, 3);
	printf("%s-> %d", memoria2.nombre, memoria2.tocados);

}

void wait_intro(void)
{
	char tecla;
	while (!_kbhit());
	tecla = _getch();
}


void fichero1(MEMORIA memoria1, MEMORIA memoria2,char tablero1[][TAM],char tablero2[][TAM])
{
	FILE* pfichero;
	float precision;
	int margen_columna, margen_fila;
	int fila, columna;
	float tocados,disparos;


	if (memoria2.nombre == "Maquina");
	strcpy_s(memoria2.nombre, LTEXTOS, "La M?quina");

	errno_t err;

	err = fopen_s(&pfichero, "Datos_Partida.txt", "w");

	if (err == 0)
	{

		fprintf_s(pfichero, "----------DATOS PARTIDA----------\n\n");
		fprintf_s(pfichero, "%s ha realizado un total de %d disparos, acertando %d\n", memoria1.nombre, memoria1.ndisparos, memoria1.tocados);
		disparos = memoria1.ndisparos * 1.00;
		tocados = memoria1.tocados * 1.00;
		precision =  (tocados/disparos)* 100;
		fprintf_s(pfichero, "La precision de los disparos ha sido de %.2f\n\n", precision);

		fprintf_s(pfichero, "%s ha realizado un total de %d disparos, acertando %d\n", memoria2.nombre, memoria2.ndisparos, memoria2.tocados);
		disparos = memoria2.ndisparos * 1.00;
		tocados = memoria2.tocados * 1.00;
		precision = (tocados / disparos) * 100;
		fprintf_s(pfichero, "La precision de los disparos ha sido de %.2f\n\n\n", precision);
		fprintf_s(pfichero, "Los tableros finales son: \n\n\n");

		
		fprintf_s(pfichero,"  ");

		for (columna = 0; columna < TAM; columna++)
			fprintf_s(pfichero," %d  ", columna);
		fprintf_s(pfichero,"\n");
		for (fila = 0; fila < TAM; fila++)
		{
			fprintf_s(pfichero,"%d ", fila);
			for (columna = 0; columna < TAM; columna++)
			fprintf_s(pfichero,"[%c] ", tablero1[fila][columna]);
			fprintf_s(pfichero,"\n");
		}


		fprintf_s(pfichero, "\n\n\n");
		fprintf_s(pfichero, "  ");

		for (columna = 0; columna < TAM; columna++)
			fprintf_s(pfichero, " %d  ", columna);
		fprintf_s(pfichero, "\n");
		for (fila = 0; fila < TAM; fila++)
		{
			fprintf_s(pfichero, "%d ", fila);
			for (columna = 0; columna < TAM; columna++)
				fprintf_s(pfichero, "[%c] ", tablero2[fila][columna]);
			fprintf_s(pfichero, "\n");
		}


		fclose(pfichero);
	}
}

