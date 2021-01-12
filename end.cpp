#include "end.h"
#include <iostream>

Chess_End::Chess_End(Situation end, FigColor col)
    :Graph_lib::Window::Window(end_p,end_max_x,end_max_y, "End"),
      ok_button(Point{0, 0}, end_max_x, end_max_y, "Ok", cb_ok)
{
    if (end == Situation::checkmate)
        checkmate(col);
    else
        pat(col);
}

void Chess_End::checkmate(FigColor col)
{
    attach(ok_button);

    Image w_win(Point{0,0}, path_image+"Images/w_win.jpg");
    Image b_win(Point{0,0}, path_image+"Images/b_win.jpg");

    if (col == FigColor::white)
    {
        set_label("White Win!");
        attach(w_win);
    }
    else
    {
        set_label("Black Win!");
        attach(b_win);
    }

    Fl::redraw();
    while (!is_ok && Fl::wait())
        ;
}

void Chess_End::pat(FigColor)
{
    attach(ok_button);

    Image draw(Point{0,0}, path_image+"Images/draw.jpg");
    attach(draw);
    set_label("Draw");

    Fl::redraw();
    while (!is_ok && Fl::wait())
        ;
}
