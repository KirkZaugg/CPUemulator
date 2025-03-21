#include"Interface.h"
#include<fstream>

Interface::Interface() {
    //initialize SDL
    window = nullptr;
    renderer = nullptr;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(256*4, 240*4, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 4, 4);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    //load palettes into the palette array
    std::ifstream inputFileStream("Nintendulator.pal", std::ios::in | std::ios::binary);
    inputFileStream.seekg(std::ios::beg);
    char input[3];
    for (int i = 0; i < 64; i++) {
        inputFileStream.read(input, 3);
        palette[0][i] = reinterpret_cast<uint8_t&>(input[0]);
        palette[1][i] = reinterpret_cast<uint8_t&>(input[1]);
        palette[2][i] = reinterpret_cast<uint8_t&>(input[2]);
    }
    inputFileStream.close();
}

void Interface::drawPix(uint8_t xpos, uint8_t ypos, uint8_t color) {
    uint8_t rgb[3] = {palette[0][color], palette[1][color], palette[2][color]};
    SDL_SetRenderDrawColor(renderer, rgb[0], rgb[1], rgb[2],255);
    SDL_RenderDrawPoint(renderer, xpos, ypos);

}

void Interface::PresentFrame() {
    SDL_RenderPresent(renderer);
}
