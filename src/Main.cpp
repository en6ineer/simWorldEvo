#include "Main.h"
#include <iostream>
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

Main::Main() : window(nullptr), renderer(nullptr), field(nullptr), running(true), limit_tps(60), tick_interval(1000 / 60), last_tick(0) {
    init_sdl();
    init_imgui();
    field = new Field(renderer);
}

Main::~Main() {
    delete field;
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Main::init_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    window = SDL_CreateWindow("MyOwnWorld", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        exit(1);
    }
}

void Main::init_imgui() {
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void Main::draw_gui() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(600, 10));
    ImGui::SetNextWindowSize(ImVec2(190, 580));
    ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    if (ImGui::Button(field->is_simulating() ? "Pause" : "Resume")) {
        field->toggle_simulation();
    }
    ImGui::SameLine(); // Place Restart button next to Pause/Resume
    if (ImGui::Button("Restart")) {
        field->restart();
    }
    ImGui::SliderInt("TPS", &limit_tps, 1, 120);
    tick_interval = 1000 / limit_tps;
    ImGui::Text("Organisms: %u", field->get_organism_count());
    ImGui::Text("Ticks: %u", field->get_tick_count());
    
    // Statistics window
    ImGui::BeginChild("Statistics", ImVec2(0, 100), true);
    ImGui::Text("Statistics");
    ImGui::Separator();
    ImGui::Text("Total Organisms: %u", field->get_organism_count());
    ImGui::EndChild();

    ImGui::End();

    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void Main::handle_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }
}

void Main::run() {
    while (running) {
        handle_input();
        uint32_t current_time = SDL_GetTicks();
        if (current_time - last_tick >= tick_interval) {
            field->tick();
            last_tick = current_time;
        }
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);
        field->draw();
        draw_gui();
        SDL_RenderPresent(renderer);
    }
}

int main() {
    Main app;
    app.run();
    return 0;
}