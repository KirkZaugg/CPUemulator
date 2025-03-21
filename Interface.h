#pragma once
#include<SDL2/SDL.h>

class Interface {
private:

    SDL_Window* window;
    SDL_Renderer* renderer;



    uint8_t palette[3][64];

public:
    Interface();

    void drawPix(uint8_t xpos, uint8_t ypos, uint8_t color);
    void PresentFrame();
};