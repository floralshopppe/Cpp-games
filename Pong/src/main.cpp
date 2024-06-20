#include <raylib.h>
int playerScore = 0;
int cpuScore = 0;
int direction[2] = {-1, 1};
Color green = {0, 153, 76, 255};

class ball
{
public:
    int radius;
    float posX;
    float posY;
    int speed;
    int directionX;
    int directionY;

    void draw()
    {
        DrawCircle(posX, posY, radius, YELLOW);
    }

    void update()
    {
        posX += speed * directionX;                                   // incrementarea pozitiei bilei
        if (posY <= 0 + radius || posY >= GetScreenHeight() - radius) // verif coliziune cu marginile de sus si de jos
            directionY *= -1;                                         // schimbarea directiei
        posY += speed * directionY;
        // verif coliziune cu marginile din stanga si dreapta (goluri)
        if (posX <= 0 + radius)
        {
            cpuScore++;
            reset();
        }
        if (posX >= GetScreenWidth() - radius)
        {
            playerScore++;
            reset();
        }
    }

    void reset()
    {
        posX = GetScreenWidth() / 2; // pozitionarea bilei in mijlocul ecranului
        posY = GetScreenHeight() / 2;
        directionX = direction[GetRandomValue(0, 1)]; // randomizare directie bila dupa fiecare gol
        directionY = direction[GetRandomValue(0, 1)];
    }

    ball() // valorile initiale
    {
        radius = 20;
        posX = GetScreenWidth() / 2;
        posY = GetScreenHeight() / 2;
        speed = 7;
        directionX = 1;
        directionY = 1;
    }
};

class paddle
{
protected:
    void outOfBounds()
    {
        if (posY < 0)                           // verif coliziune cu marginea de sus
            posY = 0;                           // resetare pozitie
        if (posY >= GetScreenHeight() - height) // verif coliziune cu marginea de jos
            posY = GetScreenHeight() - height;  // resetare pozitie
    }

public:
    float width;
    float height;
    float posX;
    float posY;
    int speed;

    paddle() // valorile initiale
    {
        width = 25;
        height = 130;
        posX = 10;                                 // offset de la margine stanga de 10px
        posY = GetScreenHeight() / 2 - height / 2; // -height/2 deoarece coordonata dreptunghiului este coltul din stanga sus, nu mijlocul
        speed = 6;
    }

    void draw()
    {
        DrawRectangle(posX, posY, width, height, WHITE);
        // DrawRectangleRounded({posX, posY, width, height}, 0.8, 0, WHITE);
    }

    void update()
    {
        if (IsKeyDown(KEY_UP))
            posY -= speed;
        if (IsKeyDown(KEY_DOWN))
            posY += speed;
        outOfBounds();
    }
};

class cpuPaddle : public paddle
{
public:
    cpuPaddle() // valorile initiale
    {
        width = 25;
        height = 130;
        posX = GetScreenWidth() - width - 10;      // offset de la margine dreapta de 10px
        posY = GetScreenHeight() / 2 - height / 2; // -height/2 deoarece coordonata dreptunghiului este coltul din stanga sus, nu mijlocul
        speed = 6;
    }

    void update(int ballY)
    {
        // AI bazat pe pozitia bilei, daca bila este in partea superioara a ecranului cpu urca, else coboara
        if (ballY + height < GetScreenWidth() / 2) // verif daca bila este in partea superioara
            posY -= speed;
        if (ballY + height > GetScreenWidth() / 2) // verif daca bila este in partea inferioara
            posY += speed;
        outOfBounds();
    }
};

int main()
{
    const int screenWidth = 1280;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Pong");
    SetTargetFPS(60);
    ball ball;
    paddle player;
    cpuPaddle cpu;
    // Game loop
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        // Updating
        ball.update();
        player.update();
        cpu.update(ball.posY);
        if (CheckCollisionCircleRec(Vector2{ball.posX, ball.posY}, ball.radius, Rectangle{player.posX, player.posY, player.width, player.height}) || CheckCollisionCircleRec(Vector2{ball.posX, ball.posY}, ball.radius, Rectangle{cpu.posX, cpu.posY, cpu.width, cpu.height}))
        {
            ball.directionX *= -1; // schimbare directie bila cand atinge paleta
        }

        // Drawing
        ball.draw();
        player.draw();
        cpu.draw();
        DrawText(TextFormat("%i", playerScore), screenWidth / 4 - 20, 20, 80, WHITE); // afisare scor
        DrawText(TextFormat("%i", cpuScore), screenWidth / 4 * 3 - 20, 20, 80, WHITE);
        DrawLine(GetScreenWidth() / 2, 0, GetScreenWidth() / 2, GetScreenHeight(), WHITE);
        ClearBackground(green);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}