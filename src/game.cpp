#include "game.hpp"
#include <iostream>
#include <fstream>

//create game entity
Game::Game()
{
    music = LoadMusicStream("Sounds/music.ogg");
    explosionSound = LoadSound("Sounds/explosion.ogg");
    PlayMusicStream(music);
    InitGame();
}

//destruct game entity
Game::~Game()
{
    Alien::UnloadImages();
    UnloadMusicStream(music);
    UnloadSound(explosionSound);
}

//draw the game objects on the screen
void Game::Draw()
{
    spaceship.Draw();

    for(auto& laser : spaceship.lasers) {
        laser.Draw();
    }

    for(auto& obstacle : obstacles) {
        obstacle.Draw();
    }

    for(auto& alien : aliens) {
        alien.Draw();
    }

    for (auto& laser : alienLasers) {
        laser.Draw();
    }

    mysteryship.Draw();
}

//update all the game objects state
void Game::Update()
{
    if(run) {
        currentTime += GetFrameTime();
        if(currentTime - timeLastSpawn > mysteryShipSpawnInterval) {
            mysteryship.Spawn();
            timeLastSpawn = currentTime;
            mysteryShipSpawnInterval = GetRandomValue(10,20);
        }

        for(auto& laser : spaceship.lasers) {
            laser.Update();
        }

        MoveAliens();
        AlienShootLaser();

        for(auto& laser : alienLasers) {
            laser.Update();
        }

        DeleteInactiveLasers();
        mysteryship.Update();
        CheckForCollisions();
    }
    else {
        if(IsKeyDown(KEY_R)) {
            Reset();
            InitGame();
        }
    }
}

//handle and response to the player keyboard input
void Game::HandleInput()
{
    if(run) {
        if(IsKeyDown(KEY_A)) {
            spaceship.MoveLeft();
        }

        if(IsKeyDown(KEY_D)) {
            spaceship.MoveRight();
        }

        if(IsKeyDown(KEY_SPACE)) {
            spaceship.FireLaser();
        }
    }
}

//delete the laser that hit target or move outside the window
void Game::DeleteInactiveLasers()
{
    for(auto iterator = spaceship.lasers.begin(); iterator != spaceship.lasers.end();) {
        if(!iterator -> active) {
            iterator = spaceship.lasers.erase(iterator);
        }
        else {
            iterator++;
        }
    }

    for(auto iterator = alienLasers.begin(); iterator != alienLasers.end();) {
        if(!iterator -> active) {
            iterator = alienLasers.erase(iterator);
        }
        else {
            iterator++;
        }
    }
}

//create the obstacle object
std::vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;

    for(int i = 0;i < 4; i++) {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX,float(GetScreenHeight() - 200)}));
    }
    return obstacles;
}

//create aliens object
std::vector<Alien> Game::CreateAliens()
{
    std::vector<Alien> aliens;
    for(int row = 0; row < 5; row++) {
        for(int col = 0; col < 11; col++) {
            int alienType;
            if(row == 0) {
                alienType = 3;
            }
            else if(row == 1 || row == 2) {
                alienType = 2;
            }
            else {
                alienType = 1;
            }

            float x_pos = 75 + col * 55;
            float y_pos = 110 + row * 55;
            aliens.push_back(Alien(alienType, {x_pos,y_pos}));
        }
    }
    return aliens;
}

//move the aliens on the screen
void Game::MoveAliens()
{
    for(auto& alien : aliens) {
        if(alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth() - 9) {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if(alien.position.x < 9) {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

//move the aliens down
void Game::MoveDownAliens(int distance)
{
    for(auto& alien : aliens) {
        alien.position.y += distance;
    }
}

//alien shoot lasers to attack
void Game::AlienShootLaser()
{
    if(currentTime - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty()) {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2,alien.position.y + alien.alienImages[alien.type - 1].height}, 6));
        timeLastAlienFired = currentTime;
    }
}

//check all the collisions between corresponding game object interaction
void Game::CheckForCollisions()
{
    //spaceship lasers
    for(auto& laser : spaceship.lasers) {
        //the collision to aliens
        auto iterator = aliens.begin();
        while(iterator != aliens.end()) {
            if(CheckCollisionRecs(iterator -> GetRect(), laser.GetRect())) {
                PlaySound(explosionSound);

                if(iterator -> type == 1) {
                    score += 100;
                }
                else if(iterator -> type == 2) {
                    score += 200;
                }
                else {
                    score += 300;
                }

                CheckForHighscore();
                iterator = aliens.erase(iterator);
                laser.active = false;
            }
            else {
                iterator++;
            }
        }

        //the collision to obstacles
        for(auto& obstacle : obstacles) {
            auto iterator = obstacle.blocks.begin();
            while(iterator != obstacle.blocks.end()) {
                if(CheckCollisionRecs(iterator -> GetRect(), laser.GetRect())) {
                    iterator = obstacle.blocks.erase(iterator);
                    laser.active = false;
                }
                else {
                    iterator++;
                }
            }
        }

        //the collision to mystery ship
        if(CheckCollisionRecs(mysteryship.GetRect(), laser.GetRect())) {
            mysteryship.alive = false;
            laser.active = false;
            score += 500;
            CheckForHighscore();
            PlaySound(explosionSound);
        }
    }

    //alien lasers
    for(auto& laser : alienLasers) {
        //the collision to the player
        if(CheckCollisionRecs(laser.GetRect(), spaceship.GetRect())) {
            laser.active = false;
            lives--;
            if(lives == 0) {
                GameOver();
            }
        }

        //the collision to obstacles
        for(auto& obstacle : obstacles) {
            auto iterator = obstacle.blocks.begin();
            while(iterator != obstacle.blocks.end()) {
                if(CheckCollisionRecs(iterator -> GetRect(), laser.GetRect())) {
                    iterator = obstacle.blocks.erase(iterator);
                    laser.active = false;
                }
                else {
                    iterator++;
                }
            }
        }
    }

    //aliens
    for(auto& alien : aliens) {
        //the collision to obstacles
        for(auto& obstacle : obstacles) {
            auto iterator = obstacle.blocks.begin();
            while(iterator != obstacle.blocks.end()) {
                if(CheckCollisionRecs(iterator -> GetRect(), alien.GetRect())) {
                    iterator = obstacle.blocks.erase(iterator);
                }
                else {
                    iterator++;
                }
            }
        }

        //the collision to the player
        if(CheckCollisionRecs(alien.GetRect(), spaceship.GetRect())) {
            GameOver();
        }
    }
}

//game over functionallity
void Game::GameOver()
{
    run = false;
}

//reset the game after restart the game
void Game::Reset()
{
    spaceship.Reset();
    aliens.clear();
    alienLasers.clear();
    obstacles.clear();
    mysteryship.alive = false;
}

//initialize the game
void Game::InitGame()
{
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    currentTime = 0.0;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    lives = 3;
    score = 0;
    highscore = LoadHighscore();
    run = true;
    mysteryShipSpawnInterval = GetRandomValue(10,20);
}

//check and change the highscore value
void Game::CheckForHighscore()
{
    if(score > highscore) {
        highscore = score;
        SaveHighscore(highscore);
    }
}

//save the highscore value
void Game::SaveHighscore(int highscore)
{
    std::ofstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile << highscore;
        highscoreFile.close();
    }
    else {
        std::cerr << "Failed to save highscore to file" << std::endl;
    }
}

//load the highscore into the game from file
int Game::LoadHighscore()
{
    int loadedHighscore = 0;
    std::ifstream highscoreFile("highscore.txt");
    if(highscoreFile.is_open()) {
        highscoreFile >> loadedHighscore;
        highscoreFile.close();
    }
    else {
        std::cerr << "Failed to load highscore from file" << std::endl;
    }
    return loadedHighscore;
}