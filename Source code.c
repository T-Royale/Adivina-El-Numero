#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>
#include<windows.h>
#include<time.h>
#include<stdbool.h>
#include<cdecode.h>
#include<cencode.h>
//Mensajes largos//
#define TituloJuego "<<<Adivina un numero>>>\n1-Jugar\n2-Opciones\n"
#define Niveles "===Elige el Nivel===\n1-Facil: Del 1 al 15\n2-Normal: Del 1 al 50\n3-Tryhard: Del 1 al 100\n4-Partida personalizada\n5-Salir\n"
#define OpcionesPosibles "===OPCIONES===\n1-Ver record\n2-Resetear record\n3-Elegir Nivel\n"
//Variables de rangos de elección//
//hola//
int MinimoRango = 1;
int MaximoRango = 15;
//===Variables de juego===//
int intentos;           //Numero de intentos
int NumeroElegido = 0;  //Numero elegido por el usuario
int NivelElegido;       //Nivel elegido por el usuario (1-3)
int Puntos;             //Puntos conseguidos en esa partida
int MayorAcierto;       //Numero elegido mas cercano al correcto, se usa para dar los puntos al final en caso de no acertarlo
int NumeroRandom;       //Numero elegido aleatoriamente por el programa
bool JugarOtra = TRUE;  //Esta variable cambiará a false cuando no se desee jugar otra partida
char Buffer;            //Guardará cada elemento codificado o decodificado antes de imprimirlo en el archivo
//Array que guarda la puntuación máxima en cada nivel//
int Records[3] = {0};
#define RecordFacil Records[0]
#define RecordNormal Records[1]
#define RecordTryhard Records[2]
//Funciones de records//
void WriteRecord(){     //Escribirlo en el archivo de guardado//
    FILE *SaveRecord = fopen("Save.txt", "w");
    base64_encodestate state;
    base64_init_encodestate(&state);
    for (int Y = 0; Y <= 2; Y++) {
        base64_encode_block(Records[Y], 5, Buffer, &state);
        fprintf(SaveRecord, "%s\n", Buffer);
    }
    fclose(SaveRecord);
}
void ReadRecord(){      //Leer los datos del archivo de guardado y asignarlos a sus variables//
    FILE *SaveRecord = fopen("Save.txt", "r");
    base64_decodestate state;
    base64_init_decodestate(&state);
    for (int i = 0; i < 3; i++) {
        fscanf(SaveRecord, "%d", &Buffer);
        base64_decode_block(&Buffer, 5, Records[i], &state);
    }
    fclose(SaveRecord);
}
void MostrarRecord(){   //Muestra los records en la terminal//
    system("cls");
    printf("###RECORDS###\nNivel: Facil = %d puntos\nNivel: Normal = %d puntos\nNivel: Tryhard = %d puntos\n", RecordFacil, RecordNormal, RecordTryhard);
    system("pause");
}
void ResetRecord(){     //Asigna un valor de 0 a los records de todos los niveles//
    system("cls");
    FILE *SaveRecord = fopen ("Save.txt","w");
    for (int i = 0; i == 2; i++){
        fprintf(SaveRecord, "%s", "0");
    }
    fclose(SaveRecord);
}
void CerrarJuego() {
    char a;
    Sleep(100);
    printf("Salir del juego\n[S/N]\n");
    scanf("%c", &a);
    a = tolower(a);
    if (a == 's') {
        JugarOtra = FALSE;
    }
}
void Opciones(){        //Muestra las opciones posibles y hace su respectiva acción//
    while(1){
        system("cls");
        printf(OpcionesPosibles);
        switch (getchar()) {
        case '1':
        MostrarRecord();
            break;
        case '2':
            ResetRecord();
            Sleep(100);
            printf("Records reseteados con exito\n");
            Sleep(100);
            system("pause");
            break;
        case '3':
            return;
        default:
            break;
        }
    }
}
//Pregunta al usuario si quiere jugar otra o cerrar el programa//
//Da la opcion de jugar o mostrar opciones adicionales//
void Bienvenido(){
    system("cls");
    system("color a");
    intentos = 5;
    printf(TituloJuego);
    if (getchar() == '2') {
        Opciones();
    }
}
//Selecciona el nivel elegido//
void EligeNivel(){
    system("cls");
    MinimoRango = 1;
    printf(Niveles);
    scanf("%d", &NivelElegido);
    while(getchar() != '\n');  // Limpiar el bufer de entrada
    switch (NivelElegido) {
    case 1:                                         //Facil
        MaximoRango = 15;
        break;
    case 2:                                         //Normal
        MaximoRango = 50;
        break;
    case 3:                                         //Tryhard
        MaximoRango = 100;
        break;
    case 4:                                         //Custom
        printf("Define el numero maximo\n");
        scanf("%d", &MaximoRango);
        while (getchar() != '\n');
        printf("Buena eleccion\n");
        Sleep(100);
        printf("Elige el Numero de intentos\n");
        scanf("%d", &intentos);
        while (getchar() != '\n');
        printf("De acuerdo\nEntonces empezemos\n");
        break;
    case 5:
        system("taskkill /F /IM cmd.exe");
        system("pause");
    default:
        break;
    }
    Sleep(100);
    printf("Nivel Definido Con Exito\n");
}
//Define un numero aleatorio en el rango definido//
void InicioJuego() {
    system("cls");
    printf("Elige un numero del 1 al %d\n", MaximoRango);
    NumeroRandom = rand() % (MaximoRango - MinimoRango + 1) + MinimoRango;
}
//Funcion principal del codigo//
int main() {
    ReadRecord();
    srand(time(NULL));
    while(JugarOtra) {
        NumeroElegido = 0;
        Bienvenido();
        EligeNivel();
        intentos = 5;
        InicioJuego();
        while(NumeroRandom != NumeroElegido && intentos > 0) {
            system("cls");
            printf("Elige un numero:\n");
            scanf("%d", &NumeroElegido);
            while (getchar() != '\n');
            while(NumeroElegido > MaximoRango || NumeroElegido < MinimoRango){
                system("cls");
                printf("Ese numero no esta dentro del rango\nTiene que ser del %d al %d\nElige otro numero\n", MinimoRango, MaximoRango);
                scanf("%d", &NumeroElegido);
                while (getchar() != '\n');
            }
            //Calcula el numero elegido anteriormente mas cercano al generado aleatoriamente//
            if (abs(MayorAcierto-NumeroRandom) > abs(NumeroElegido-NumeroRandom)) { 
                MayorAcierto = NumeroElegido;
            }
            //Otorga una puntuación en función a lo cerca que esta el numero adivinado del correcto//
            if (NumeroElegido < NumeroRandom) {
                printf("Te has quedado corto\n");
                Puntos = Puntos + 2*(NumeroRandom - NumeroElegido);
            }           
            else if (NumeroElegido > NumeroRandom) {
                printf("Te has pasado\n");
                Puntos = Puntos + 2*(NumeroElegido - NumeroRandom);
            }
            intentos--;
            Sleep(100);
            printf("Te quedan %d intentos\n", intentos);
            Sleep(100);
            system("pause");
        }
        if (NumeroElegido==NumeroRandom) {
            printf("LO HAS CLAVADO\n");
            system("color 21");
            Puntos = Puntos + 100 + (intentos * 25);
            printf("Has conseguido %d puntos\n", Puntos);
            if (Puntos > Records[NivelElegido]){
                Records[NivelElegido-1] = Puntos;
                   WriteRecord();
            }
            CerrarJuego();
        }
        else if (intentos == 0) {
            printf("Te has quedado sin intentos\n :(\n");
            system("color 60");
            printf("El numero era el %d\n", NumeroRandom);
            Sleep(150);
            printf("Has estado cerca con el %d\n", MayorAcierto);
            Puntos = Puntos+(10-abs(MayorAcierto-NumeroRandom));
            printf("Has conseguido %d puntos\n", Puntos);
            CerrarJuego();
        }
    }
    system("pause");
    return 0;
}