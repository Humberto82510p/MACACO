#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include "ball.h"
#include "orificios.h"
#include "estados.h"
#include <string.h>

constexpr int filas = 5;    // filas del tablero
constexpr int columnas = 5; // columnas del tablero

void DrawHole(Texture2D texture, Rectangle hitbox, float scale);

// hola

int main()
{
    InitAudioDevice();
    /*************************TAMAÑO DE LA PANTALLA (NO MOVER)************************************ */
    constexpr int screenWidth = 1500;
    constexpr int screenHeight = 853;

    /************************************************************************* */

    /************************BORDES DE LOS RECTANGULOS************************** */
    float redondeo = 0.2; // entre 0.0 y 1.0
    int segnmentos = 10;  // qué tan suave es la curva
    /************************************************************************* */

    Ball ball;

    InitWindow(screenWidth, screenHeight, "MACACO MARBLE ULTIMATE"); // titulo

    Image icon = LoadImage("src/imagen/orificios/icono.png"); // carga icono

    SetWindowIcon(icon); // muestra icono

    Texture2D tablero = LoadTexture("src/imagen/fondo.jpeg"); // fondo de jungla

    Image fondotablero = LoadImage("src/imagen/fondotab.png"); // fondo de madera

    Font miFuente = LoadFont("src/fuentestxt/LuckiestGuy-Regular.ttf"); // fuente de texto

    Texture2D texturatab = LoadTextureFromImage(fondotablero); // carga textura de madera

    /*******************************  TIPO DE PUNTAJE  **************************************** */
    Texture2D orvacio1 = LoadTexture("src/imagen/orificios/orificio1.png");
    Texture2D orvacio2 = LoadTexture("src/imagen/orificios/orificio2.png");
    Texture2D orvacio3 = LoadTexture("src/imagen/orificios/orificio3.png");
    Texture2D orvacio4 = LoadTexture("src/imagen/orificios/orificio4.png");
    Texture2D bomba = LoadTexture("src/imagen/orificios/bomb.png");

    Texture2D bombaexplo = LoadTexture("src/imagen/orificios/explosion.png");
    Texture2D orlleno1 = LoadTexture("src/imagen/orificios/orilleno1.png");
    Texture2D orlleno2 = LoadTexture("src/imagen/orificios/orilleno2.png");
    Texture2D orlleno3 = LoadTexture("src/imagen/orificios/orilleno3.png");
    Texture2D orlleno4 = LoadTexture("src/imagen/orificios/orilleno4.png");
    /******************************************************************** */
    Music musica = LoadMusicStream("src/musica/Cumbia.ogg");
    Music intro = LoadMusicStream("src/musica/inicio.ogg");

    PlayMusicStream(intro);
    SetMusicVolume(intro, 1);

    PlayMusicStream(musica);
    SetMusicVolume(musica, 1);

    SetTargetFPS(60); // FPS

    Rectangle puntaje = {30, 100, 370, 700}; // el que muestra el puntaje

    Rectangle caja = {500, 100, 900, 700}; // tamaño del tablero

    Cell cells[filas][columnas]; // matriz de celdas

    /******************DECLARACIONES****************** */
    int casillascol;   // casillas de la col
    int casillasfil;   // casillas por fila
    /* float chico; */ // a la que se le asigna el menor valor
    /* float radioori; */
    int puntajefinal = 0;
    int puntajeobjetivo;
    int promedio;
    int oportunidades;
    int nivel = 2;
    int opcs[20];
    int nopcs;
    int suma = 0;
    /****************************************************************************** */

    /*********************************HITBOX****************************************** */
    casillascol = (int)(caja.width / columnas); // ancho entero por columna
    casillasfil = (int)(caja.height / filas);   // alto entero por filafff

    /****************************************POSICION DE LA TEXTURA DE MADERA********************************************************** */
    Rectangle sourceRec = {0, 0, float(texturatab.width), float(texturatab.height)}; // donde se va aponer
    Rectangle destRec = caja;                                                        // donde se dibuja
    Vector2 origin = {0, 0};     
    
    // --- AGREGADO: Rectángulo del botón JUGAR ---
    Rectangle btnJugar = { screenWidth/2.0f - 150, screenHeight/2.0f + 50, 300, 80 };                                                    // para rotar
    /*********************************************************************************************** */

    /***************************PARTE LOGICA DEL JUEGO************************************** */

    oportunidades = 5;

    nopcs = 0;

    switch (nivel)
    {
    case 1:
        // Nivel 1: SOLO valores positivos
        opcs[0] = 1;
        opcs[1] = 10;
        opcs[2] = 100;
        opcs[3] = 1000;
        opcs[4] = 100;
        nopcs = 5; // cantidad REAL de valores
        break;

    case 2:
        // Nivel 2: Positivos + negativos
        opcs[0] = -1;
        opcs[1] = -10;
        opcs[2] = -100;
        opcs[3] = -1000;
        opcs[4] = 1;
        opcs[5] = 10;
        opcs[6] = 100;
        opcs[7] = 1000;
        opcs[8] = 1;
        opcs[9] = 10;
        opcs[10] = 100;
        opcs[11] = 1;
        opcs[12] = 10;
        opcs[13] = 100;
        opcs[14] = 1000;

        nopcs = 15;
        break;

    default:
        opcs[0] = 1;
        nopcs = 1;
        break;
    }

    /********************************************************************************** */

    for (int i = 0; i < nopcs; i++)
    {
        suma += opcs[i];
    }

    promedio = suma / nopcs;

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cells[i][j].valor = opcs[rand() % nopcs]; // asigna un valor aleatorio a la celda
            cells[i][j].ocupado = 0;
            cells[i][j].hitbox.x = caja.x + j * casillascol;
            cells[i][j].hitbox.y = caja.y + i * casillasfil;
            cells[i][j].hitbox.width = casillascol;
            cells[i][j].hitbox.height = casillasfil;
        }
    }

    int rangomin = promedio * 6.3;

    puntajeobjetivo = 1 + rand() % rangomin + (rangomin * 1.1);

    /*************************************************************** */

    /*************************************************************************************** */
    while (!WindowShouldClose())
    {
        UpdateMusicStream(intro);
         UpdateMusicStream(musica);

         // --- AGREGADO: BLOQUE DEL MENÚ ---
        // Si estamos en MENU, dibujamos el menú y usamos 'continue' para saltar el resto
        if (estado_j == MENU)
        {
            Vector2 mousePos = GetMousePosition();
            
            // Lógica del botón
            if (CheckCollisionPointRec(mousePos, btnJugar)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    estado_j = INCIAJUEGO; // Cambiamos de estado
                }
            }

            BeginDrawing();
            ClearBackground(BLACK); // Importante: Limpiar fondo
            DrawTexture(tablero, 0, 0, WHITE); // Dibujar fondo jungla

            // Títulos
            DrawTextEx(miFuente, "MACACO MARBLE", {screenWidth/2.0f - 350, 150}, 90, 2, YELLOW);
            DrawTextEx(miFuente, "ULTIMATE", {screenWidth/2.0f - 180, 250}, 70, 2, ORANGE);

            // Dibujar botón con efecto hover
            Color colorBtn = CheckCollisionPointRec(mousePos, btnJugar) ? ORANGE : BROWN;
            DrawRectangleRec(btnJugar, colorBtn);
            DrawRectangleLinesEx(btnJugar, 4, BLACK);
            DrawTextEx(miFuente, "JUGAR", {btnJugar.x + 85, btnJugar.y + 20}, 40, 2, WHITE);
            
            EndDrawing();
            continue; // <--- ESTO ES CLAVE: Vuelve al inicio del while y no ejecuta el juego abajo
        }


        float dt = GetFrameTime();
        BeginDrawing();
        /*************************F O N D O***************************** */
        DrawTexture(tablero, 0, 0, WHITE); // DIBUJA EL fondo
        /*************************************************** */

        /****************************P U N T A J E*******************************************/
        DrawRectangleRounded(puntaje, redondeo, segnmentos, BLACK);            // RECTANGULO DEL PUINTAJE
        DrawRectangleRoundedLinesEx(puntaje, redondeo, segnmentos, 10, WHITE); // ORILLAS DEL RECTANGULO
                                                                               /*********************************************************************** */

        /*******************************T E X T O  D E L  T A B L E R  O******************************************** */
        DrawTextEx(miFuente, "PUNTAJE:", {puntaje.x * 5, puntaje.y + 10}, 30, 2, YELLOW);

        /******************************************************************************* */

        /**********************************T A B L E R O************************************* */
        DrawTexturePro(texturatab, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawRectangleRoundedLinesEx(caja, redondeo, segnmentos, 10.0f, BLACK); // ORILLAS DEL AREA DE JUEGO
        DrawTexturePro(texturatab, sourceRec, destRec, origin, 0.0f, WHITE);

        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                Texture2D tex; // la textura que se va a dibujar

                // Selecciona la textura según el valor
                switch (cells[i][j].valor)
                {

                case -1000:
                {
                    if (cells[i][j].ocupado == 0)

                    {
                        tex = bomba;
                    }
                    else
                    {
                        tex = bombaexplo;
                    }
                }

                break;

                case -10:
                {
                    if (cells[i][j].ocupado == 0)

                    {
                        tex = bomba;
                    }
                    else
                    {
                        tex = bombaexplo;
                    }
                }

                break;

                case -1:
                {
                   if (cells[i][j].ocupado == 0)

                    {
                        tex = bomba;
                    }
                    else
                    {
                        tex = bombaexplo;
                    }
                }

                break;

                case 1:
                    if (cells[i][j].ocupado == 0)
                    {
                        tex = orvacio1;
                    }
                    else
                    {
                        tex = orlleno1;
                    }
                    break;

                case 10:
                    if (cells[i][j].ocupado == 0)
                    {
                        tex = orvacio2;
                    }
                    else
                    {
                        tex = orlleno2;
                    }
                    break;
                case 100:
                     if (cells[i][j].ocupado == 0)
                    {
                        tex = orvacio3;
                    }
                    else
                    {
                        tex = orlleno3;
                    }

                    break;
                case 1000:
                    if (cells[i][j].ocupado == 0)
                    {
                        tex = orvacio4;
                    }
                    else
                    {
                        tex = orlleno4;
                    }
                    break;

                default:

                    break; // fallback
                }
                DrawHole(tex, cells[i][j].hitbox, 0.85); // ← escala ajustable
            }
        }
        /************************************************************************* */

        /******************************zona reflejada**************** */

        /**for (int i = 0; i < filas; i++)
         {
             for (int j = 0; j < columnas; j++)
             {
                 DrawRectangleLines(
                     cells[i][j].hitbox.x,
                     cells[i][j].hitbox.y,
                     cells[i][j].hitbox.width,
                     cells[i][j].hitbox.height,
                     LIGHTGRAY);
             }
         }*/
        /******************************************************************************* */

        if (estado_j == INCIAJUEGO)
        {
            PauseMusicStream(intro);
            UpdateMusicStream(musica);
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f)); // oscurece un poco
            DrawText("LISTO?", screenWidth / 2 - 60, screenHeight / 2 - 50, 40, YELLOW);
            DrawText("PRESIONA ESPACIO PARA INICIAR", screenWidth / 2 - 200, screenHeight / 2 + 10, 20, WHITE);

            // Dibuja la pelota en posición inicial "de espera"
            ball.x = caja.x + caja.width / 2;
            ball.y = caja.y + caja.height - 50;
            ball.Draw();

            if (IsKeyPressed(KEY_SPACE))
            {
                estado_j = JUGANDO; // empieza el juego
            }
        }

        /**************************LO QUE HACE LA PELOTA**************************** */

        else
        {
            if (estado_j == JUGANDO)
            {
                /* code */
            }
            
            ball.Update(dt, caja);
            ball.Draw();

            if (oportunidades <= 0)
            {
                if (puntajefinal >= puntajeobjetivo)
                {
                    DrawText("GANASTE WEI", screenWidth / 2 - 100, screenHeight / 2 - 50, 80, WHITE);
                    estado_j = GANAR;
                }

                else
                {
                    DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 50, 80, WHITE);
                    estado_j = PIERDES;

                }
            }
            else
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    ball.PARARSE();
                    ball.frenodemano = 0;
                }

                if (ball.separo == 1)
                {
                    bool encontrada = false;

                    for (int i = 0; i < filas && !encontrada; i++)
                    {
                        for (int j = 0; j < columnas && !encontrada; j++)
                        {
                            if (!cells[i][j].ocupado)
                            {
                                if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, cells[i][j].hitbox))
                                {
                                    cells[i][j].ocupado = 1;
                                    puntajefinal += cells[i][j].valor;
                                    encontrada = true;
                                    oportunidades--;
                                }
                            }
                        }
                    }

                    ball.MUEVETE();
                    ball.separo = 0;
                }
            }
        }

        DrawText(TextFormat("nPts obt: %d", puntajefinal), puntaje.x + 30, puntaje.y + 50, 40, GREEN);
        DrawText(TextFormat("Vidas: %d", oportunidades), puntaje.x + 60, puntaje.y + 100, 40, GREEN);
        DrawText(TextFormat("PTS pa GANAR: %d", puntajeobjetivo), puntaje.x + 30, puntaje.y + 600, 20, GREEN);
        DrawText(TextFormat("promedio: %d", promedio), puntaje.x + 30, puntaje.y + 300, 20, GREEN);

        if (estado_j == GANAR)
        {
            //AWQUI VA DONDE SE LE PREGUNTA CUANTO ES LA SUMA FINAL
        }

        if (estado_j == PIERDES)
        {
            //QUIERES INTERNTARLO DE NUEVO
            //estado de juego = iniciakjuegp
        }
        
        
        EndDrawing();
    }

    UnloadMusicStream(intro);
    UnloadMusicStream(musica);
    
    CloseAudioDevice();
    UnloadTexture(tablero);
    UnloadImage(icon);
    UnloadImage(fondotablero);

    CloseWindow();
}

/************************************************************* */
void DrawHole(Texture2D texture, Rectangle hitbox, float scale)
{
    // Tamaño reducido
    float destW = (hitbox.width * 0.85) * scale;
    float destH = hitbox.height * scale;

    // Centramos la textura dentro del hitbox
    Rectangle dest = {
        hitbox.x + ((hitbox.width * 0.85f) - destW) / 2.0f,
        hitbox.y + (hitbox.height - destH) / 2,
        destW,
        destH};

    DrawTexturePro(
        texture,
        Rectangle{0, 0, float(texture.width), float(texture.height)}, // textura completa
        dest,
        Vector2{0, 0},
        0.0f,
        WHITE);
}
