#include "SimpleFramework.h"
#include <cmath>

Demo::Demo(const int screenWidth, const int screenHeight, const std::string title):	screenWidth(screenWidth),
															screenHeight(screenHeight), TRANSPARENT_WHITE({ 255, 255, 255, 200 })
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, title.c_str());
    pause = false;
    framesCounter = 0;

    SetTargetFPS(60);

}

Demo::~Demo()
{
	// De-Initialization
	//---------------------------------------------------------
	CloseWindow();        // Close window and OpenGL context
	//---------------------------------------------------------
}

void Demo::run() {

    init();
      
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //-----------------------------------------------------
        if (IsKeyPressed(KEY_SPACE)) pause = !pause;

        if (!pause)
        { 
          update();
        }
        else framesCounter++;
        //-----------------------------------------------------

        // Draw
        //-----------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);
         
        draw();

        // On pause, we draw a blinking message
        if (pause && ((framesCounter / 30) % 2))
        {
            int x = screenWidth / 2 - 60;
            int y = screenHeight / 2 - 20;

            //draw a backdrop so that the text is visible
            DrawRectangle(x, y, 120, 25, TRANSPARENT_WHITE);
            DrawText("PAUSED", x, y, 30, GRAY);
        }
        DrawFPS(10, 10);

        EndDrawing(); 
    }
}

BallCollisionDemo::BallCollisionDemo(const int screenWidth, const int screenHeight, const std::string title):Demo(screenWidth, screenHeight, title) {
    ballPosition = { screenWidth / 2.0f, screenHeight / 2.0f };
    ballSpeed = { 5.0f, 4.0f };
    ballRadius = 20;
}

BallCollisionDemo::~BallCollisionDemo() {

}

void BallCollisionDemo::init()
{
    
}

void BallCollisionDemo::shutdown()
{

}

void BallCollisionDemo::update()
{
    ballPosition.x += ballSpeed.x;
    ballPosition.y += ballSpeed.y;

    // Check walls collision for bouncing
    if ((ballPosition.x >= (GetScreenWidth() - ballRadius)) || 
        (ballPosition.x <= ballRadius)) ballSpeed.x = -ballSpeed.x;
    if ((ballPosition.y >= (GetScreenHeight() - ballRadius)) || 
        (ballPosition.y <= ballRadius)) ballSpeed.y = -ballSpeed.y;

}

void BallCollisionDemo::draw()
{
    DrawCircleV(ballPosition, (float)ballRadius, MAROON);
}

#include <cstdlib>
#include <ctime>

NBallsCollisionDemo::NBallsCollisionDemo(const int screenWidth, const int screenHeight, const std::string title, const int MAX_BALLS) :Demo(screenWidth, screenHeight, title), MAX_BALLS(MAX_BALLS) {
    totalCollisions = 0;
    useSpatialHashing = true;
    
    ballRadius = 2;
    ballMinDistance = 2 * ballRadius;
    ballMinDistance2 = ballMinDistance * ballMinDistance;
 
    srand((unsigned)time(NULL));
     
    //initialize ball positions and velocities
    ballPositions.resize(MAX_BALLS);
    ballSpeeds.resize(MAX_BALLS);
    ballColliding.resize(MAX_BALLS);
     

    for (int i = 0; i < MAX_BALLS; i++)
    { 
        ballColliding[i] = false;
        float r1 = float(rand()) / RAND_MAX;
        float r2 = float(rand()) / RAND_MAX;

        float sx = (2.0f * r1) - 1.0f;
        float sy = (2.0f * r2) - 1.0f;

        Vector2 pos { ballRadius + ((screenWidth- (2*ballRadius)) * r1) , ballRadius + ((screenHeight - (2 * ballRadius))* r2) }, 
                speed{ 2*sx, 2*sy}; 

        ballPositions[i] = pos;  
        ballSpeeds[i] = speed;
    } 
}

NBallsCollisionDemo::~NBallsCollisionDemo() {
    
}

void NBallsCollisionDemo::init()
{
     //TODO: do initialization stuff for spatial hashing here
}

void NBallsCollisionDemo::shutdown()
{
    //TODO: do cleanup related to spatial hashing here
    ballPositions.clear();
    ballSpeeds.clear();
}

float Vector2_dot(Vector2& a, Vector2& b)
{
    return a.x * b.x + a.y * b.y;
}
 

void NBallsCollisionDemo::update( )
{
    if (IsKeyPressed(KEY_H)) 
        useSpatialHashing = !useSpatialHashing;
     

    totalCollisions = 0;
    for (int i = 0; i < MAX_BALLS; ++i)
    {
        ballColliding[i] = false;
        Vector2& pos_i = ballPositions[i];
        Vector2& speed_i = ballSpeeds[i];
        
        pos_i.x += speed_i.x;
        pos_i.y += speed_i.y;
         
        //Check collision between balls
        // 
        //interball collision testing using spatial hashing
        if (useSpatialHashing)
        { 
            //TODO: add spatial hashing code here
        }
        else
        {
            //do n^2 comparisons of balls
            for (int j = 0; j < MAX_BALLS; j++)
            {
                if(i==j)
                    continue;
                    
                Vector2& pos_j = ballPositions[j];
                Vector2& speed_j = ballSpeeds[j];

                Vector2 normal = { pos_i.x - pos_j.x, pos_i.y - pos_j.y };
                float d2 = Vector2_dot(normal, normal);

                // are the balls overlapping? 
                if (d2 > 0.0 && d2 < ballMinDistance2)
                { 
                    ballColliding[i] = true;
                    ballColliding[j] = true;
                    totalCollisions++;

                    float d = sqrtf(d2);
                    normal.x /= d;
                    normal.y /= d;

                    // separate the balls 
                    float corr = (ballMinDistance - d) * 0.5f;
                    pos_i.x += normal.x * corr;
                    pos_i.y += normal.y * corr;

                    pos_j.x -= normal.x * corr;
                    pos_j.y -= normal.y * corr;

                    // reflect velocities along normal  
                    float vi = Vector2_dot(speed_i, normal);
                    float vj = Vector2_dot(speed_j, normal);
                    speed_i.x += normal.x * (vj - vi);
                    speed_i.y += normal.y * (vj - vi);

                    speed_j.x += normal.x * (vi - vj);
                    speed_j.y += normal.y * (vi - vj);
                }
            }
        }
        // Check walls collision for bouncing
        if ((pos_i.x  >= (GetScreenWidth() - ballRadius)) ||
            (pos_i.x  <= ballRadius)) 
            speed_i.x = -speed_i.x;
        if ((pos_i.y  >= (GetScreenHeight() - ballRadius)) ||
            (pos_i.y  <= ballRadius)) 
            speed_i.y = -speed_i.y;
    }
}

#include <sstream>

void NBallsCollisionDemo::draw()
{
    int ballsOnScreen = 0;
    for (int i = 0; i < MAX_BALLS; ++i)
    {
        DrawCircleV(ballPositions[i], (float)ballRadius, ballColliding[i] ? GREEN : RED);
        if (ballPositions[i].x >= 0 && ballPositions[i].y >= 0 && ballPositions[i].x <= screenWidth && ballPositions[i].y <= screenHeight)
            ++ballsOnScreen;
    }

    //draw a back drop so text is visible
    DrawRectangle(5, 5, 290, 90, TRANSPARENT_WHITE); 

    //draw the total number of collisions
    std::stringstream s;
    s << "Total collisions: " << totalCollisions;

    DrawText(s.str().c_str(), 10, 30, 20, BLACK);

    //draw the total number of balls on screen
    s.str("");
    s << "Total balls on screen: " << ballsOnScreen;

    DrawText(s.str().c_str(), 10, 50, 20, BLACK);

    //draw if using spatial hashing or not
    s.str(""); 
    s << "using " << (useSpatialHashing? "spatial hashing": "N^2 comparisons");

    DrawText(s.str().c_str(), 10, 70, 20, BLACK);
}