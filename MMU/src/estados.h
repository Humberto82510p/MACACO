#pragma once
#include <raylib.h>


enum estadodejuego
{
    MENU,
    INCIAJUEGO,
    JUGANDO,
    PELOTAPARA,
    GANAR,
    PIERDES
};
estadodejuego estado_j = MENU;