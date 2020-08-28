#include "SDL.h"
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#define FPS 60
#define PI 3.1415926535897932384626433

class gui
{
private:
    static const int START = 15001;
    static const int STOP = 15002;
    static const int RADIUS = 15003;
    static const int DELQA = 15004;
    static const int DELQB = 15005;
    static const int OBSTACLE_START = 15006;
    static const int OBSTACLE_END = 15007;

    SDL_Window *window;
    SDL_Event event;
    SDL_Surface *screen;
    SDL_Renderer *renderer;
    Uint32 white_color;
    int window_width;
    int window_height;
    void cap_frame_rate(Uint32 starting_tick);
    int start_x, start_y, end_x, end_y, radius, del_q_x1, del_q_y1, del_q, del_q_x2, del_q_y2;
    int stage;
    void set_window_heightnwidth();
    void handle_button_click(int x, int y);
    void draw_circle(int x, int y, int radius, uint8_t R, uint8_t G, uint8_t B, uint8_t A);
    void draw_interest_points(bool scenario);
    std::vector<Rectangle> obstacles;
    int temp_obx, temp_oby;
    void draw_lines(std::vector<std::vector<point>> lines);
    bool result_reached(point p);

public:
    gui();
    ~gui();
    bool get_user_input();
    void get_window_dimensions(int &height, int &width)
    {
        width = window_width;
        height = window_height;
    };
    void get_start_point(int &x, int &y)
    {
        x = start_x;
        y = start_y;
    }
    void get_end_point(int &x, int &y, double &r)
    {
        x = end_x;
        y = end_y;
        r = radius;
    }
    void get_del_q(double &x) { x = del_q; }
    std::vector<Rectangle> get_obstacles() { return obstacles; }
    void plot_results(std::vector<std::vector<point>> lines);
};

void gui::set_window_heightnwidth()
{
    std::string input;
    std::cout << "enter screen height" << std::endl;
    std::cin >> input;
    window_height = std::stoi(input);
    std::cout << "enter screen width" << std::endl;
    std::cin >> input;
    window_width = std::stoi(input);
}

gui::gui()
{
    stage = START;
    set_window_heightnwidth();
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
}

void gui::handle_button_click(int x, int y)
{
    switch (stage)
    {
    case START:
        start_x = x;
        start_y = y;
        stage = STOP;
        break;
    case STOP:
        end_x = x;
        end_y = y;
        stage = RADIUS;
        break;
    case RADIUS:
        radius = sqrt(pow(x - end_x, 2) + pow(y - end_y, 2));
        stage = DELQA;
        break;
    case DELQA:
        del_q_x1 = x;
        del_q_y1 = y;
        stage = DELQB;
        break;
    case DELQB:
        del_q_x2 = x;
        del_q_y2 = y;
        del_q = sqrt(pow(x - del_q_x1, 2) + pow(y - del_q_y1, 2));
        stage = OBSTACLE_START;
        break;
    case OBSTACLE_START:
        temp_obx = x;
        temp_oby = y;
        stage = OBSTACLE_END;
        break;
    case OBSTACLE_END:
    {
        stage = OBSTACLE_START;
        Rectangle rect(temp_obx, temp_oby, x, y);
        obstacles.push_back(rect);
    }
    break;
    default:
        break;
    }
}

void gui::draw_interest_points(bool scenario)
{
    if (scenario)
    {
        if (stage == STOP)
        {
            draw_circle(start_x, start_y, 5, 0, 255, 0, 255);
        }
        if (stage == RADIUS)
        {
            draw_circle(start_x, start_y, 5, 0, 255, 0, 255);
            draw_circle(end_x, end_y, 5, 255, 0, 0, 255);
        }
        if (stage == DELQA)
        {
            draw_circle(start_x, start_y, 5, 0, 255, 0, 255);
            draw_circle(end_x, end_y, 5, 255, 0, 0, 255);
            draw_circle(end_x, end_y, radius, 255, 0, 0, 255);
        }

        if (stage == DELQB)
        {
            draw_circle(start_x, start_y, 5, 0, 255, 0, 255);
            draw_circle(end_x, end_y, radius, 255, 0, 0, 255);
            draw_circle(del_q_x1, del_q_y1, 5, 0, 0, 255, 255);
        }
    }

    if (stage == OBSTACLE_START || stage == OBSTACLE_END)
    {
        draw_circle(start_x, start_y, 5, 0, 255, 0, 255);
        draw_circle(end_x, end_y, radius, 255, 0, 0, 255);
        if (scenario)
        {
            draw_circle(del_q_x1, del_q_y1, 5, 0, 0, 255, 255);
            draw_circle(del_q_x2, del_q_y2, 5, 0, 0, 255, 255);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, del_q_x1, del_q_y1, del_q_x2, del_q_y2);
        }

        if (obstacles.size() > 0)
        {
            SDL_Rect *rectangles = new SDL_Rect[obstacles.size()];
            for (int i = 0; i < obstacles.size(); ++i)
            {
                rectangles[i] = {obstacles[i].x1, obstacles[i].y1, obstacles[i].w, obstacles[i].h};
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderFillRects(renderer, rectangles, obstacles.size());
            delete[] rectangles;
        }
    }
}

void gui::draw_circle(int x, int y, int radius, uint8_t R, uint8_t G, uint8_t B, uint8_t A)
{

    for (int i = 0; i < 360; ++i)
    {
        SDL_SetRenderDrawColor(renderer, R, G, B, A);
        if (i < 359)
        {
            SDL_RenderDrawLineF(renderer, x + radius * (cos(PI / 180 * i)), y + radius * (sin(PI / 180 * i)),
                                x + radius * (cos(PI / 180 * (i + 1))), y + radius * (sin(PI / 180 * (i + 1))));
        }
        else
        {
            SDL_RenderDrawLineF(renderer, x + radius * (cos(PI / 180 * i)), y + radius * (sin(PI / 180 * i)),
                                x + radius, y);
        }
    }
}

bool gui::get_user_input()
{
    window = SDL_CreateWindow("RRT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bool running = true;
    Uint32 starting_tick;
    while (running)
    {
        starting_tick = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    handle_button_click(event.button.x, event.button.y);
                }
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        draw_interest_points(true);

        SDL_RenderPresent(renderer);
        cap_frame_rate(starting_tick);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return (stage == OBSTACLE_END || stage == OBSTACLE_START);
}

void gui::plot_results(std::vector<std::vector<point>> lines)
{
    window = SDL_CreateWindow("RRT", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    bool running = true;
    Uint32 starting_tick;
    while (running)
    {
        starting_tick = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw_interest_points(false);
        draw_lines(lines);
        SDL_RenderPresent(renderer);
        cap_frame_rate(starting_tick);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

bool gui::result_reached(point p)
{
    if(pow(p.x - end_x,2) + pow(p.y - end_y,2) < pow(radius,2))
    {
        return true;
    }
    return false;
}

void gui::draw_lines(std::vector<std::vector<point>> lines)
{
    for (std::vector<point> line : lines)
    {
        int R = 0 , G = 0, B = 0;
        if(result_reached(line[line.size()-1]))
        {
            R = 255;
        }
        draw_circle(line[0].x, line[0].y, 5, R, G, B, 1);
        for (int i = 0; i < line.size() - 1; ++i)
        {
            draw_circle(line[i + 1].x, line[i + 1].y, 5, R, G, B, 1);
            SDL_SetRenderDrawColor(renderer, R, G, B, 1);
            SDL_RenderDrawLineF(renderer, line[i].x, line[i].y, line[i + 1].x, line[i + 1].y);
        }
    }
}
void gui::cap_frame_rate(Uint32 starting_tick)
{
    if ((1000 / FPS) > SDL_GetTicks() - starting_tick)
    {
        SDL_Delay(1000 / FPS - (SDL_GetTicks() - starting_tick));
    }
}
gui::~gui()
{
    SDL_Quit();
}
