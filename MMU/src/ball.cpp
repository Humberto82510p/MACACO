#include "ball.h"
#include <raylib.h>
#include <cmath>

Ball::Ball()
    : x(50), y(50), speedX(1200), speedY(1200), radius(35)
{
}

void Ball ::PARARSE()
{
    speedX = 0;
    speedY = 0;
    separo = 1;
}

void Ball ::MUEVETE()
{
    speedX = 1200;
    speedY = 1200;
    radius = 35;
}

void Ball::Update(float dt, Rectangle playArea)
{
    // Aquí va el código del rebote dentro del rectángulo
    x += speedX * dt;
    y += speedY * dt;

    float left = playArea.x;
    float right = playArea.x + playArea.width;
    float top = playArea.y;
    float bottom = playArea.y + playArea.height;

    if (x - radius <= left)
    {
        x = left + radius;
        speedX *= -1;
    }

    if (x + radius >= right)
    {
        x = right - radius;
        speedX *= -1;
    }

    if (y - radius <= top)
    {
        y = top + radius;
        speedY *= -1;
    }

    if (y + radius >= bottom)
    {
        y = bottom - radius;
        speedY *= -1;
    }
}

void Ball::velocidadpuntos(float mul)
{
    speedX *= mul;
    speedY *= mul;

    const float VEL_MAX = 5000.0f; // límite superior

    // Solo limitar por arriba, nunca por abajo
    if (fabs(speedX) > VEL_MAX) 
        speedX = (speedX > 0 ? VEL_MAX : -VEL_MAX);
    if (fabs(speedY) > VEL_MAX)
        speedY = (speedY > 0 ? VEL_MAX : -VEL_MAX);

}

    void Ball::Draw() const
    {
        DrawCircle(x, y, radius, WHITE);
    }
