#ifndef GRAPH_CHESS_H
#define GRAPH_CHESS_H

#include "../Graph_lib/Window.h"
#include "../Graph_lib/Graph.h"
#include "../Graph_lib/GUI.h"
#include <iostream>
#include <vector>
#include <map>

#include "yadro.h"
#include "bot_chess.h"


using namespace Graph_lib;

const Point win_p {500,20};
constexpr int win_max_x = 1000;
constexpr int win_max_y = 700;
constexpr int indent_x = 100;
constexpr int indent_y = 20;
constexpr int weight_cell = 80;
const int col_cell1 = 175;
const int col_cell2 = 65;

const Color select_col1 = Color::blue;
const Color select_col2 = Color::green;
const Color select_col3 = Color::red;

const string path_image = "D:\\inf\\c++\\projects\\chess\\";


std::string letters ();
std::string digits1 ();
std::string digits2 ();

const map <Figures, string> fig_type {
    {Figures::king, "K.png"},
    {Figures::queen, "Q.png"},
    {Figures::elephant, "B.png"},
    {Figures::horse, "N.png"},
    {Figures::tower, "R.png"},
    {Figures::pawn, "P.png"},
};

const map <FigColor, string> fig_col {
    {FigColor::black, "figures/b"},
    {FigColor::white, "figures/w"}
};


struct Print_motion
{
    vector<Out_box*> out;
    vector<string> str;

    Print_motion();
};


struct End_Return
{
    Situation sit;
    FigColor win_color;
};


class My_Window : public Graph_lib::Window
{
public:
    My_Window();

    //Переопределение Fl_Window::hide
    void hide() override { quit(); }

    bool quit_is = false;
protected:
    //Реализованный выход
    Button quit_button;
    static void cb_quit (Address, Address addr)
    {
      Graph_lib::reference_to<My_Window>(addr).quit();
    }
    void quit () { quit_is = true; Fl_Window::hide(); }

    //Сброс игры
    Button reset_button;
    static void cb_reset (Address, Address addr)
    {
      Graph_lib::reference_to<My_Window>(addr).reset_play();
    }
    virtual void reset_play () = 0;
    bool reset = false;

    //Отмена хода
    Button reverse_motion_button;
    static void cb_reverse_motion (Address, Address addr)
    {
      Graph_lib::reference_to<My_Window>(addr).reverse_motion();
    }
    virtual void reverse_motion () = 0;
    bool reverse = false;

    //Ничья
    Button draw_button;
    static void cb_draw (Address, Address addr)
    {
      Graph_lib::reference_to<My_Window>(addr).go_draw();
    }
    void go_draw () {draw_is = true;}
    bool draw_is = false;
};


class Chessboard : public My_Window
{
public:
    Chessboard();

    void click(Address);

protected:
    //Объекты на доске
    Table desk;
    std::vector <Image*> figures;
    Vector_ref <Button> b_cells;
    Vector_ref <Shape> cells;

    //Нажатие кнопок
    bool recovery_button();
    bool is_click() const { return clicked;}
    Point cell_click();
    bool clicked = false;
    int click_x = -1;
    int click_y = -1;

    //Буковки и циферки
    Graph_lib::Marks x_labels_w;
    Graph_lib::Marks x_labels_b;
    Graph_lib::Marks y_labels_w;
    Graph_lib::Marks y_labels_b;


    //Превращение пешки
    static void cb_trans(Address addr1, Address addr2)
    {
        Graph_lib::reference_to<Chessboard>(addr2).trans(addr1);
    }
    void trans(Address addr1)
    {
        x_trans = Graph_lib::reference_to<Fl_Widget>(addr1).x();
        y_trans = Graph_lib::reference_to<Fl_Widget>(addr1).y();
        is_cl_trans = true;
    }
    bool is_cl_trans = false;
    int x_trans = -1;
    int y_trans = -1;

    //Вывод прошлых ходов
    Print_motion moves;
    void print_moves();

    Out_box* out;
};


class My_Chessboard : public Chessboard
{
public:
    My_Chessboard();

    End_Return play();

    End_Return play_bot(FigColor bot_col);
private:

    //Вывод на экран
    void attach_all();
    void attach_start();
    void position();
    void detach_posit();

    //Выделение и его сброс
    void selection(int x, int y);
    void unselection();
    std::vector <Shape*> select;

    void reset_play () override;
    void reverse_motion () override;
    Figures transformation(FigColor col);

    End_Return check_sit(Situation sit = Situation::none);
};


struct Cell : Button
{
    Cell(Point p)
        :Button{ p, weight_cell, weight_cell, "", cb_click}
    {}
    static void cb_click(Address button, Address win)
    {
        Graph_lib::reference_to<Chessboard>(win).click(button);
    }
};


const map <int, string> str_leters {
    {0, "a"},
    {1, "b"},
    {2, "c"},
    {3, "d"},
    {4, "e"},
    {5, "f"},
    {6, "g"},
    {7, "h"}
};

#endif // GRAPH_CHESS_H
