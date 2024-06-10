#include <raylib.h>
#include <string>
#include "game.hpp"

//handle the format of game score
std::string FormatWithLeadingZero(int number, int width) {
    std::string scoreText = std::to_string(number);
    int leadingZero = width - scoreText.length();
    return scoreText = std::string(leadingZero, '0') + scoreText;
}

int main()
{
    //initialize
    Color gray = {29,29,27,255};
    Color yellow = {243,216,63,255};
    int offset = 50;
    int windowWidth = 750, windowHeight = 700;

    InitWindow(windowWidth + offset, windowHeight + 2 * offset, "Space Invaders");
    InitAudioDevice();
    Font font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImage = LoadTexture("Graphics/spaceship.png");
    SetTargetFPS(60);
    Game game;

    //game loop
    while(WindowShouldClose() == false) {
        UpdateMusicStream(game.music);
        game.HandleInput();
        game.Update();

        BeginDrawing();
        ClearBackground(gray);
        DrawRectangleRoundedLines({10,10,780,780}, 0.18f, 20, 2, yellow);
        DrawLineEx({25,730}, {775,730}, 3, yellow);

        if(game.run) {
            DrawTextEx(font, "LEVEL 01", {570,740}, 34, 2, yellow);
        }
        else {
            DrawTextEx(font, "GAME OVER", {570,740}, 34, 2, yellow);
        }

        float x = 50.0;
        for(int i = 0;i < game.lives; i++) {
            DrawTextureV(spaceshipImage, {x,745}, WHITE);
            x += 50.0;
        }

        DrawTextEx(font, "SCORE", {50,15}, 34, 2, yellow);
        std::string scoreText = FormatWithLeadingZero(game.score, 5);
        DrawTextEx(font, scoreText.c_str(), {50,40}, 34, 2, yellow);

        DrawTextEx(font, "HIGH-SCORE", {570,15}, 34, 2, yellow);
        std::string highscoreText = FormatWithLeadingZero(game.highscore, 5);
        DrawTextEx(font, highscoreText.c_str(), {655,40}, 34, 2, yellow);

        game.Draw();
        EndDrawing();
    }

    //close the game
    CloseWindow();
    CloseAudioDevice();
}