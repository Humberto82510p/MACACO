#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include "ball.h"
#include "orificios.h"
#include "estados.h"
#include <string.h> // Necesario para inputs
#include <stdio.h>  // Necesario para conversiones

constexpr int filas = 5;    // filas del tablero
constexpr int columnas = 5; // columnas del tablero

void DrawHole(Texture2D texture, Rectangle hitbox, float scale);

int main()
{
    InitAudioDevice();
    /*************************TAMAÑO DE LA PANTALLA ************************************ */
    constexpr int screenWidth = 1500;
    constexpr int screenHeight = 853;

    /************************************************************************* */
    float redondeo = 0.2; // entre 0.0 y 1.0
    int segnmentos = 10;  // qué tan suave es la curva
    /************************************************************************* */

    Ball ball;

    InitWindow(screenWidth, screenHeight, "MACACO MARBLE ULTIMATE");

    // CARGA ARCHIVOS
    Image icon = LoadImage("src/imagen/orificios/icono.png");
    SetWindowIcon(icon);
    Image fondotablero = LoadImage("src/imagen/fondotab.png");
    Texture2D tablero = LoadTexture("src/imagen/fondo.jpeg");
    Texture2D texturatab = LoadTextureFromImage(fondotablero);
    Texture2D portada = LoadTexture("src/imagen/portada.png");

    Font miFuente = LoadFont("src/fuentestxt/LuckiestGuy-Regular.ttf");
    Font fuentemine = LoadFont("src/fuentestxt/MinecraftRegular-Bmg3.ttf");
    Font titulos = LoadFont("src/fuentestxt/DIMITRI_.TTF");
    Font titulosbuenos = LoadFont("src/fuentestxt/DIMIS___.TTF");

    // IMAGENES DE PUNTAJES
    Texture2D orvacio1 = LoadTexture("src/imagen/orificios/orificio1.png");
    Texture2D orvacio2 = LoadTexture("src/imagen/orificios/orificio2.png");
    Texture2D orvacio3 = LoadTexture("src/imagen/orificios/orificio3.png");
    Texture2D orvacio4 = LoadTexture("src/imagen/orificios/orificio4.png");
    Texture2D bomba1 = LoadTexture("src/imagen/orificios/bomba1.png");
    Texture2D bomba2 = LoadTexture("src/imagen/orificios/bomba2.png");
    Texture2D bomba3 = LoadTexture("src/imagen/orificios/bomba3.png");
    Texture2D bomba4 = LoadTexture("src/imagen/orificios/bomba4.png");

    Texture2D bombaexplo1 = LoadTexture("src/imagen/orificios/bombaexplo1.png");
    Texture2D bombaexplo2 = LoadTexture("src/imagen/orificios/bombaexplo2.png");
    Texture2D bombaexplo3 = LoadTexture("src/imagen/orificios/bombaexplo3.png");
    Texture2D bombaexplo4 = LoadTexture("src/imagen/orificios/bombaexplo4.png");

    Texture2D orlleno1 = LoadTexture("src/imagen/orificios/orilleno1.png");
    Texture2D orlleno2 = LoadTexture("src/imagen/orificios/orilleno2.png");
    Texture2D orlleno3 = LoadTexture("src/imagen/orificios/orilleno3.png");
    Texture2D orlleno4 = LoadTexture("src/imagen/orificios/orilleno4.png");

    // CARGA MUSICA
    Music musica = LoadMusicStream("src/musica/Cumbia.ogg");

    Music introdos = LoadMusicStream("src/musica/1000.ogg");
    Sound chango = LoadSound("src/musica/1000.wav");

    Sound sonidoEspecial = LoadSound("src/musica/1000.mp3");
    Music intro = LoadMusicStream("src/musica/inicio.ogg");
    SetSoundVolume(sonidoEspecial, 1);
    SetMusicVolume(intro, 1);
    SetMusicVolume(musica, 1);

    PlayMusicStream(intro);
    PlayMusicStream(musica);
    PlayMusicStream(introdos);

    SetTargetFPS(60);

    Cell cells[filas][columnas]; // matriz de celdas

    // DECLARACIONES
    int casillascol;
    int casillasfil;
    int puntajefinal = 0;
    int puntajeobjetivo;
    int promedio;
    int oportunidades;
    int nivel = 1;
    int opcs[20] = {0};
    int nopcs;
    int suma = 0;

    // --- VARIABLES IMPLEMENTADAS ---
    char inputTexto[10] = "";
    int contadorTeclas = 0;
    bool calculoRealizado = false;
    int bolasCapturadas[8] = {0, 0, 0, 0};
    int intentos = 3;

    // DIMESIONES BASICAS DEL JUEGO
    Rectangle puntaje = {30, 100, 370, 700};
    Rectangle caja = {500, 100, 900, 700};
    Rectangle btnComprobar = {screenWidth / 2.0f - 125, 680, 250, 60}; // Movido un poco abajo

    // HITBOX Y CALCULOS INICIALES
    casillascol = (int)(caja.width / columnas);
    casillasfil = (int)(caja.height / filas);

    // INICIALIZACION DE SEGURIDAD (ANTI-CRASH)
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cells[i][j].valor = 1;
            cells[i][j].ocupado = 0;
            cells[i][j].hitbox.x = caja.x + j * casillascol;
            cells[i][j].hitbox.y = caja.y + i * casillasfil;
            cells[i][j].hitbox.width = casillascol;
            cells[i][j].hitbox.height = casillasfil;
        }
    }

    Rectangle sourceRec = {0, 0, float(texturatab.width), float(texturatab.height)};
    Rectangle destRec = caja;
    Vector2 origin = {0, 0};

    Rectangle btnJugar = {screenWidth / 2.0f - 150, screenHeight / 2.0f + 50, 300, 80};

    /*************************** EMPIEZA A CORRER ********************************** */
    while (!WindowShouldClose())
    {
        UpdateMusicStream(intro);
        UpdateMusicStream(musica);

        // ================== BLOQUE DE MENÚ ==================
        if (estado_j == MENU)
        {
            Vector2 mousePos = GetMousePosition();

            if (CheckCollisionPointRec(mousePos, btnJugar))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    estado_j = INCIAJUEGO;
                }
            }

            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(tablero, 0, 0, WHITE);

            DrawTexture(portada, (screenWidth / 2) - (portada.width / 2), 100, WHITE);

            Color colorBtn = CheckCollisionPointRec(mousePos, btnJugar) ? ORANGE : GRAY;
            DrawRectangleRec(btnJugar, colorBtn);
            DrawRectangleLinesEx(btnJugar, 4, BLACK);
            DrawTextEx(fuentemine, "JUGAR", {btnJugar.x + 68, btnJugar.y + 10}, 60, 2, WHITE);

            EndDrawing();
            continue;
        }

        // ================== BLOQUE DE PREGUNTA (MEJORADO) ==================
        if (estado_j == PREGUNTA)
        {
            if (!calculoRealizado)
            {
                // Resetear contadores de bolas
                for (int k = 0; k < 4; k++)
                {
                    bolasCapturadas[k] = 0;
                }

                // Contar bolas obtenidas
                for (int i = 0; i < filas; i++)
                {
                    for (int j = 0; j < columnas; j++)
                    {
                        if (cells[i][j].ocupado)
                        {
                            if (cells[i][j].valor == -1 || cells[i][j].valor == -4)
                            {
                                bolasCapturadas[4]++;
                            }
                            if (cells[i][j].valor == -10 || cells[i][j].valor == -13)
                            {
                                bolasCapturadas[5]++;
                            }

                            if (cells[i][j].valor == -100 || cells[i][j].valor == -132)
                            {
                                bolasCapturadas[6]++;
                            }
                            if (cells[i][j].valor == -1000 || cells[i][j].valor == -950)
                            {
                                bolasCapturadas[7]++;
                            }

                            if (cells[i][j].valor == 1 || cells[i][j].valor == 4)
                            {
                                bolasCapturadas[0]++;
                            }
                            else
                            {
                                if (cells[i][j].valor == 10 || cells[i][j].valor == 13)
                                {
                                    bolasCapturadas[1]++;
                                }
                                else
                                {
                                    if (cells[i][j].valor == 100 || cells[i][j].valor == 132)
                                    {
                                        bolasCapturadas[2]++;
                                    }
                                    else
                                    {
                                        if (cells[i][j].valor == 1000 || cells[i][j].valor == 950)
                                        {
                                            bolasCapturadas[3]++;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                calculoRealizado = true;
                contadorTeclas = 0;
                inputTexto[0] = '\0';
                intentos = 3;
            }

            // Input de teclado numérico
            int key = GetCharPressed();
            while (key > 0)
            {
                // Permitir números (48-57) O el signo menos (45) solo al inicio
                if (((key >= 48 && key <= 57) || (key == 45 && contadorTeclas == 0)) && (contadorTeclas < 6))
                {
                    inputTexto[contadorTeclas] = (char)key;
                    inputTexto[contadorTeclas + 1] = '\0';
                    contadorTeclas++;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && contadorTeclas > 0)
            {
                contadorTeclas--;
                inputTexto[contadorTeclas] = '\0';
            }

            Vector2 mousePos = GetMousePosition();
            bool click = CheckCollisionPointRec(mousePos, btnComprobar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

            // Validar respuesta
            if (click || IsKeyPressed(KEY_ENTER))
            {
                int respuesta = atoi(inputTexto);

                if (respuesta == puntajefinal)
                {
                    if (puntajefinal >= puntajeobjetivo)
                    {
                        // GANASTE -> PASA A PREGUNTA
                        // CORRECTO: SUBIR NIVEL
                        nivel++;
                        estado_j = INCIAJUEGO;
                    }
                    else
                    {
                        estado_j = PIERDES;
                    }

                    // Reiniciar lógica para siguiente nivel
                    suma = 0;
                    nopcs = 0;
                    switch (nivel)
                    {
                    case 1:
                        opcs[0] = 1;
                        opcs[1] = 10;
                        opcs[2] = 100;
                        opcs[3] = 1000;
                        opcs[4] = 100;
                        nopcs = 5;
                        break;
                    case 2:
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
                        opcs[0] = -1;
                        opcs[1] = -10;
                        opcs[2] = -100;
                        opcs[3] = -1000;
                        opcs[4] = 1;
                        opcs[5] = 10;
                        opcs[6] = 100;
                        opcs[8] = 1;
                        opcs[9] = 10;
                        opcs[10] = 100;
                        opcs[11] = 1;
                        opcs[12] = 10;
                        opcs[13] = 100;
                        opcs[14] = 1000;
                        nopcs = 15;
                        break;
                    }

                    for (int i = 0; i < nopcs; i++)
                    {
                        suma += opcs[i];
                        promedio = (nopcs > 0) ? suma / nopcs : 1;
                    }

                    for (int i = 0; i < filas; i++)
                    {
                        for (int j = 0; j < columnas; j++)
                        {
                            cells[i][j].valor = opcs[rand() % nopcs];
                            cells[i][j].ocupado = 0;
                            cells[i][j].hitbox.x = caja.x + j * casillascol;
                            cells[i][j].hitbox.y = caja.y + i * casillasfil;
                            cells[i][j].hitbox.width = casillascol;
                            cells[i][j].hitbox.height = casillasfil;
                        }
                    }
                    int rangomin = promedio * 5.3;
                    if (rangomin <= 0)
                    {
                        rangomin = 100;
                    }

                    puntajeobjetivo = 1 + rand() % rangomin + (rangomin * 1.1);

                    puntajefinal = 0;
                    estado_j = INCIAJUEGO;
                    calculoRealizado = false;
                }
                else
                {
                    intentos--;
                    contadorTeclas = 0;
                    inputTexto[0] = '\0';
                    if (intentos <= 0)
                    {
                        estado_j = PIERDES;
                    }
                }
            }

            // --- DIBUJO DE PREGUNTA MEJORADO ---
            BeginDrawing();
            ClearBackground(BLACK);
            DrawTexture(tablero, 0, 0, WHITE);

            // 1. Fondo oscurecido
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));

            // 2. Panel Principal (Estilo madera oscura o UI)
            Rectangle panelRect = {screenWidth / 2.0f - 350, 50, 700, 750};
            DrawRectangleRounded(panelRect, 0.05f, 10, BLACK);
            DrawRectangleRoundedLinesEx(panelRect, 0.05f, 10, 5, WHITE);

            // 3. Título
            DrawTextEx(titulos, "CUANTOS PUNTOS??", {screenWidth / 2.0f - 200, 50}, 40, 2, GOLD);
            DrawText("Suma los puntos de las canicas obtenidas:", screenWidth / 2 - 220, 0, 25, LIGHTGRAY);
            

            // 4. Tabla de Recuento (DIVIDIDA EN COLUMNAS)
            float startY = 200;                  // Subimos un poco el inicio
            float leftX = screenWidth / 2 - 300; // Columna IZQUIERDA (Canicas)
            float rightX = screenWidth / 2 + 150;// Columna DERECHA (Bombas)
            int rowHeight = 70;                  // Espacio entre filas

            // ================== COLUMNA IZQUIERDA (POSITIVOS) ==================

            // Fila 1: Valor 1
            DrawTexturePro(orlleno1, {0, 0, (float)orlleno1.width, (float)orlleno1.height}, {leftX, startY, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[0]), leftX + 80, startY + 15, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[0]), leftX + 200, startY + 25, 20, GREEN);

            // Fila 2: Valor 10
            DrawTexturePro(orlleno2, {0, 0, (float)orlleno2.width, (float)orlleno2.height}, {leftX, startY + rowHeight, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[1]), leftX + 80, startY + rowHeight + 15, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[1]), leftX + 200, startY + rowHeight + 25, 20, GREEN);

            // Fila 3: Valor 100
            DrawTexturePro(orlleno3, {0, 0, (float)orlleno3.width, (float)orlleno3.height}, {leftX, startY + rowHeight * 2, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[2]), leftX + 80, startY + rowHeight * 2 + 15, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[2]), leftX + 200, startY + rowHeight * 2 + 25, 20, GREEN);

            // Fila 4: Valor 1000
            DrawTexturePro(orlleno4, {0, 0, (float)orlleno4.width, (float)orlleno4.height}, {leftX, startY + rowHeight * 3, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[3]), leftX + 80, startY + rowHeight * 3 + 15, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[3]), leftX + 200, startY + rowHeight * 3 + 25, 20, GREEN);

            // ================== COLUMNA DERECHA (BOMBAS) ==================
            // Corrección: Usamos 'rightX' y reiniciamos la altura para que empiecen desde arriba

            // Bomba 1
            DrawTexturePro(bomba1, {0, 0, (float)bomba1.width, (float)bomba1.height}, {rightX, startY, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[4]), rightX + 80, startY + 25, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[4]), rightX + 200, startY + 25, 20, RED); // Puse texto rojo para distinguir

            // Bomba 2
            DrawTexturePro(bomba2, {0, 0, (float)bomba2.width, (float)bomba2.height}, {rightX, startY + rowHeight, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[5]), rightX + 80, startY + rowHeight + 25, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[5]), rightX + 200, startY + rowHeight + 25, 20, RED);

            // Bomba 3
            DrawTexturePro(bomba3, {0, 0, (float)bomba3.width, (float)bomba3.height}, {rightX, startY + rowHeight * 2, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[6]), rightX + 80, startY + rowHeight * 2 + 25, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[6]), rightX + 200, startY + rowHeight * 2 + 25, 20, RED);

            // Bomba 4 (Usaba bomba3 repetida en tu código, aquí ajusté la altura a *3)
            DrawTexturePro(bomba4, {0, 0, (float)bomba3.width, (float)bomba3.height}, {rightX, startY + rowHeight * 3, 60, 60}, {0, 0}, 0, WHITE);
            DrawText(TextFormat("x %d", bolasCapturadas[7]), rightX + 80, startY + rowHeight * 3 + 25, 40, WHITE);
            DrawText(TextFormat("(Valor: %d)", opcs[7]), rightX + 200, startY + rowHeight * 3 + 25, 20, RED);

            // ================== ZONA DE INPUT (ABAJO CENTRADO) ==================
            // Bajamos esto a 560 para que quede debajo de las dos columnas
            float yInput = 560;

            // 5. Área de Respuesta
            DrawText("INGRESE TOTAL:", screenWidth / 2 - 100, yInput, 25, WHITE);

            // Caja de Input
            Rectangle inputRect = {screenWidth / 2 - 150, yInput + 40, 300, 60};
            DrawRectangleRec(inputRect, RAYWHITE);
            DrawRectangleLinesEx(inputRect, 3, ORANGE);

            // Texto del input (centrado)
            int textWidth = MeasureText(inputTexto, 50);
            DrawText(inputTexto, (int)(inputRect.x + inputRect.width / 2 - textWidth / 2), (int)(inputRect.y + 5), 50, BLACK);

            // Cursor parpadeante
            if (((int)(GetTime() * 2) % 2) == 0 && contadorTeclas < 6)
            {
                DrawText("_", (int)(inputRect.x + inputRect.width / 2 + textWidth / 2 + 5), (int)(inputRect.y + 5), 50, BLACK);
            }

            // 6. Botón Comprobar
            // Actualizamos coordenadas del botón para que el click coincida con el dibujo
            btnComprobar.x = screenWidth / 2.0f - 125;
            btnComprobar.y = yInput + 120;

            Color colBtn = CheckCollisionPointRec(mousePos, btnComprobar) ? GOLD : LIGHTGRAY;
            DrawRectangleRounded(btnComprobar, 0.3f, 5, colBtn);
            DrawTextEx(miFuente, "COMPROBAR", {btnComprobar.x + 40, btnComprobar.y + 15}, 30, 2, BLACK);

            // 7. Intentos restantes
            Color colInt = (intentos == 1) ? RED : GREEN;
            DrawText(TextFormat("VIDAS DE RESPUESTA: %d", intentos), screenWidth / 2 - 120, yInput + 200, 20, colInt);

            EndDrawing();
            continue;
        }

        if (estado_j == GANAR)
        {
        }

        /************************ DIBUJADO DEL JUEGO NORMAL *************************/
        float dt = GetFrameTime();
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(tablero, 0, 0, WHITE);

        // PUNTAJE
        DrawRectangleRounded(puntaje, redondeo, segnmentos, BLACK);
        DrawRectangleRoundedLinesEx(puntaje, redondeo, segnmentos, 10, WHITE);
        DrawTextEx(titulos, "PUNTAJE:", {puntaje.x * 3, puntaje.y + 15}, 50, 10, WHITE);

        // TABLERO
        DrawTexturePro(texturatab, sourceRec, destRec, origin, 0.0f, WHITE);
        DrawRectangleRoundedLinesEx(caja, redondeo, segnmentos, 10.0f, BLACK);
        DrawTexturePro(texturatab, sourceRec, destRec, origin, 0.0f, WHITE);

        // --- BUCLE DE DIBUJO SEGURO (CON FIX) ---
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < columnas; j++)
            {
                Texture2D tex = orvacio1; // Fallback

                switch (cells[i][j].valor)
                {
                case -1000:
                case -950:
                    tex = (cells[i][j].ocupado == 0) ? bomba4 : bombaexplo4;
                    break;
                case -100:
                case -132:
                    tex = (cells[i][j].ocupado == 0) ? bomba3 : bombaexplo3;
                    break;
                case -10:
                case -13:
                    tex = (cells[i][j].ocupado == 0) ? bomba2 : bombaexplo2;
                    break;
                case -1:
                case -4:
                    tex = (cells[i][j].ocupado == 0) ? bomba1 : bombaexplo1;
                    break;
                case 1:
                case 4:
                    tex = (cells[i][j].ocupado == 0) ? orvacio1 : orlleno1;
                    break;
                case 10:
                case 13:
                    tex = (cells[i][j].ocupado == 0) ? orvacio2 : orlleno2;
                    break;
                case 100:
                case 132:
                    tex = (cells[i][j].ocupado == 0) ? orvacio3 : orlleno3;
                    break;
                case 950:
                case 1000:
                    tex = (cells[i][j].ocupado == 0) ? orvacio4 : orlleno4;
                    break;
                default:
                    tex = orvacio1;
                    break;
                }
                DrawHole(tex, cells[i][j].hitbox, 0.85);
            }
        }

        /************************ LOGICA DE INICIO (NIVELES) *************************/
        if (estado_j == INCIAJUEGO)
        {
            oportunidades = 5;
            nopcs = 0;
            suma = 0;

            switch (nivel)
            {
            case 1:
                opcs[0] = 1;
                opcs[1] = 10;
                opcs[2] = 100;
                opcs[3] = 1000;
                opcs[4] = 100;
                nopcs = 5;
                break;
            case 2:
            {
                int tmp[] = {1, 10, 100, 1000, -1, -10, -100, -1000, 1, 10, 100, 1, 10, 100, 1000};
                for (int k = 0; k < 15; k++)
                    opcs[k] = tmp[k];
                nopcs = 15;
            }
            break;
            case 3:
            {
                int tmp[] = {
                    4,
                    13,
                    132,
                    950,
                    13,
                    132,
                    4,
                    13,
                    132,
                    4,
                    -4,
                    -13,
                    -132,
                    -950,
                };
                for (int k = 0; k < 14; k++)
                    opcs[k] = tmp[k];
                nopcs = 14;
            }
            break;
            default:
                // Fallback nivel default
                {
                    int tmp[] = {-1, -10, -100, -1000, 1, 10, 100, 1, 10, 100, 1, 10, 100, 1000};
                    for (int k = 0; k < 14; k++)
                        opcs[k] = tmp[k];
                    nopcs = 14;
                }
                break;
            }

            for (int i = 0; i < nopcs; i++)
                suma += opcs[i];

            promedio = (nopcs > 0) ? suma / nopcs : 1;

            for (int i = 0; i < filas; i++)
            {
                for (int j = 0; j < columnas; j++)
                {
                    if (nopcs > 0)
                        cells[i][j].valor = opcs[rand() % nopcs];

                    cells[i][j].ocupado = 0;
                    cells[i][j].hitbox.x = caja.x + j * casillascol;
                    cells[i][j].hitbox.y = caja.y + i * casillasfil;
                    cells[i][j].hitbox.width = casillascol;
                    cells[i][j].hitbox.height = casillasfil;
                }
            }

            int rangomin = promedio * 6.3;
            if (rangomin <= 0)
                rangomin = 100;

            puntajeobjetivo = 1 + rand() % rangomin + (rangomin * 1.1);

            PauseMusicStream(intro);
            UpdateMusicStream(introdos);

            UpdateMusicStream(musica);
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.5f));
            DrawTextEx(titulos, "LIST@?", {((screenWidth / 2) - 200), ((screenHeight / 2) - 60)}, 100, 1, WHITE);
            DrawTextEx(fuentemine, "PRESIONA ESPACIO PARA INICIAR", {((screenWidth / 2) - 280), ((screenHeight / 2) + 50)}, 35, 1, WHITE);

            ball.x = caja.x + caja.width / 2;
            ball.y = caja.y + caja.height - 50;
            ball.Draw();

            if (IsKeyPressed(KEY_SPACE))
            {
                estado_j = JUGANDO;
            }
        }
        else
        {
            if (estado_j == JUGANDO)
            {
                PauseMusicStream(introdos);
                ball.Update(dt, caja);
                ball.Draw();

                if (oportunidades <= 0)
                {
                    estado_j = PREGUNTA;
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
                                        if (cells[i][j].valor == 1000 || cells[i][j].valor == 950)
                                            PlaySound(chango);

                                        puntajefinal += cells[i][j].valor;
                                        encontrada = true;
                                        oportunidades--;
                                    }
                                }
                            }
                        }
                        ball.separo = 0;
                        ball.MUEVETE();
                    }
                }
            }
        }
        // TEXTOS GLOBALES

        DrawText(TextFormat("Canicas: %d", oportunidades), puntaje.x + 25, puntaje.y + 100, 20, WHITE);
        DrawText(TextFormat("PTS a GANAR: %d", puntajeobjetivo), puntaje.x + 25, puntaje.y + 150, 20, WHITE);
        DrawText(TextFormat("ACTUAL: %d", puntajefinal), puntaje.x + 25, puntaje.y + 180, 20, GREEN);
        DrawText(TextFormat("NIVEL: %d", nivel), puntaje.x + 120, puntaje.y + 200, 30, ORANGE);
        DrawText(TextFormat("ORIFICIOS:", nivel), puntaje.x + 80, puntaje.y + 300, 40, WHITE);
        DrawText(TextFormat("NARANJA: %d", opcs[0]), puntaje.x + 30, puntaje.y + 400, 20, ORANGE);
        DrawText(TextFormat("AZUL: %d", opcs[1]), puntaje.x + 30, puntaje.y + 450, 20, BLUE);
        DrawText(TextFormat("VERDE: %d", opcs[2]), puntaje.x + 30, puntaje.y + 500, 20, GREEN);
        DrawText(TextFormat("MORADO: %d", opcs[3]), puntaje.x + 30, puntaje.y + 550, 20, PURPLE);

        // --- PANTALLA PIERDES CON REINICIO (NUEVO) ---
        if (estado_j == PIERDES)
        {
            DrawText("GAME OVER", screenWidth / 2 - 200, screenHeight / 2 - 50, 80, WHITE);
            DrawText("Presiona Q para Reiniciar Nivel", screenWidth / 2 - 250, screenHeight / 2 + 50, 30, YELLOW);

            if (IsKeyPressed(KEY_Q))
            {
                // Reiniciar variables para el mismo nivel
                puntajefinal = 0;
                oportunidades = 5;
                intentos = 3;
                calculoRealizado = false;
                inputTexto[0] = '\0';
                contadorTeclas = 0;

                // REGENERAR EL MISMO NIVEL
                suma = 0;
                nopcs = 0;
                switch (nivel)
                {
                case 1:
                    opcs[0] = 1;
                    opcs[1] = 10;
                    opcs[2] = 100;
                    opcs[3] = 1000;
                    opcs[4] = 100;
                    nopcs = 5;
                    break;
                case 2:
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
                    opcs[0] = -1;
                    opcs[1] = -10;
                    opcs[2] = -100;
                    opcs[3] = -1000;
                    opcs[4] = 1;
                    opcs[5] = 10;
                    opcs[6] = 100;
                    opcs[8] = 1;
                    opcs[9] = 10;
                    opcs[10] = 100;
                    opcs[11] = 1;
                    opcs[12] = 10;
                    opcs[13] = 100;
                    opcs[14] = 1000;
                    nopcs = 15;
                    break;
                }

                for (int i = 0; i < nopcs; i++)
                    suma += opcs[i];
                promedio = (nopcs > 0) ? suma / nopcs : 1;

                for (int i = 0; i < filas; i++)
                {
                    for (int j = 0; j < columnas; j++)
                    {
                        cells[i][j].valor = opcs[rand() % nopcs];
                        cells[i][j].ocupado = 0;
                        // Recalcular hitbox
                        cells[i][j].hitbox.x = caja.x + j * casillascol;
                        cells[i][j].hitbox.y = caja.y + i * casillasfil;
                        cells[i][j].hitbox.width = casillascol;
                        cells[i][j].hitbox.height = casillasfil;
                    }
                }
                int rangomin = promedio * 6.3;
                if (rangomin <= 0)
                    rangomin = 100;
                puntajeobjetivo = 1 + rand() % rangomin + (rangomin * 1.1);

                estado_j = INCIAJUEGO;
            }
        }

        EndDrawing();
    }

    UnloadMusicStream(intro);
    UnloadMusicStream(introdos);
    UnloadSound(chango);
    UnloadMusicStream(musica);
    UnloadSound(sonidoEspecial);
    CloseAudioDevice();

    UnloadTexture(tablero);
    UnloadImage(icon);
    UnloadImage(fondotablero);

    CloseWindow();
}

void DrawHole(Texture2D texture, Rectangle hitbox, float scale)
{
    float destW = (hitbox.width * 0.85) * scale;
    float destH = hitbox.height * scale;

    Rectangle dest = {
        hitbox.x + ((hitbox.width * 0.85f) - destW) / 2.0f,
        hitbox.y + (hitbox.height - destH) / 2,
        destW,
        destH};

    DrawTexturePro(
        texture,
        Rectangle{0, 0, float(texture.width), float(texture.height)},
        dest,
        Vector2{0, 0},
        0.0f,
        WHITE);
}