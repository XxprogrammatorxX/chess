#ifndef MENU_H
#define MENU_H
#include "../Graph_lib/Window.h"
#include "../Graph_lib/Graph.h"
#include "../Graph_lib/GUI.h"
#include "end.h"

using namespace Graph_lib;


const Point menu_p {400,100};
constexpr int menu_max_x = 900;
constexpr int menu_max_y = 550;

const Point sett_p {500,300};
constexpr int sett_max_x = 180;
constexpr int sett_max_y = 95;

const Point quit_pp {menu_max_x-150, 50};
constexpr int quit_h = 50;
constexpr int quit_w = 100;

const Point play_pp {menu_max_x-150, 100};
constexpr int play_h = 50;
constexpr int play_w = 100;

const Point play_pc {menu_max_x-150, 150};
constexpr int play_pc_h = 70;
constexpr int play_pc_w = 100;


class Chess_Menu : public Graph_lib::Window
{
public:
    Chess_Menu(int img = 1);
    void start();

    bool exit() const {return is_quit;}

    void go_play_pp() {is_play_pp = true;}
    void go_play_pc() {is_play_pc = true;}

    void quit () {is_quit = true; Fl_Window::hide();}

    void hide() override
    {
        Fl_Window::hide();
        is_quit = true;
    }

private:
    void attach_all();

    Image background;
    Image play_pp_img;
    Image play_pc_img;
    Image quit_img;

    void end(End_Return sit);

    Button go_play_pp_button;
    static void cb_go_play_pp (Address, Address addr)
    {
        Graph_lib::reference_to<Chess_Menu>(addr).go_play_pp();
    }

    bool is_play_pp = false;

    Button go_play_pc_button;
    static void cb_go_play_pc (Address, Address addr)
    {
        Graph_lib::reference_to<Chess_Menu>(addr).go_play_pc();
    }

    bool is_play_pc = false;

    Button quit_button;
    static void cb_quit (Address, Address addr)
    {
        Graph_lib::reference_to<Chess_Menu>(addr).quit();
    }
    bool is_quit = false;
};


class Settings : public Graph_lib::Window
{
public:
    Settings();
    FigColor setting();

    void hide() override
    {
        Fl_Window::hide();
        is_quit = true;
    }
    bool is_quit = false;

private:
    Text message;

    Button white_button;
    static void cb_white (Address, Address addr)
    {
        Graph_lib::reference_to<Settings>(addr).ch_white();
    }
    void ch_white()
    {
        choice = FigColor::black;
        is_click = true;
    }

    Button black_button;
    static void cb_black (Address, Address addr)
    {
        Graph_lib::reference_to<Settings>(addr).ch_black();
    }
    void ch_black()
    {
        choice = FigColor::white;
        is_click = true;
    }

    bool is_click = false;
    FigColor choice = FigColor::white;
};

#endif // MENU_H
