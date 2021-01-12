#ifndef END_H
#define END_H

#include "../Graph_lib/Window.h"
#include "../Graph_lib/Graph.h"
#include "../Graph_lib/GUI.h"
#include "yadro.h"
#include "Graph_chess.h"

using namespace Graph_lib;

constexpr int end_max_x = 500;
constexpr int end_max_y = 300;
const Point end_p {1000,400};

class Chess_End : public Graph_lib::Window
{
public:
    Chess_End(Situation end, FigColor col);
    void checkmate(FigColor col);
    void pat(FigColor col);
    void ok()
    {is_ok = true;}

    void hide() override
    {
        is_ok = true;
        Fl_Widget::hide();
    }
private:
    Button ok_button;
    static void cb_ok (Address, Address addr)
    {
        Graph_lib::reference_to<Chess_End>(addr).ok();
    }
    bool is_ok = false;
};

#endif // END_H
