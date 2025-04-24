#pragma once

#include <SDL2/SDL.h>
#include "imgui.h"
#include "implot.h"
#include "Field.h"

class Main {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Field* field;
    bool running;
    int limit_tps;
    int tick_interval;
    uint32_t last_tick;

    void init_sdl();
    void init_imgui();
    void draw_gui();
    void handle_input();

public:
    Main();
    ~Main();
    void run();
};