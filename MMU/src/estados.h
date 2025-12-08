#pragma once
#include <raylib.h>


enum estadodejuego
{
    MENU,
    INCIAJUEGO,
    JUGANDO,
    PELOTAPARA,
    GANAR,
    PREGUNTA,
    PIERDES
};
estadodejuego estado_j = MENU;