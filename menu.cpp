#include "menu.h"


Chess_Menu::Chess_Menu(int i)
    : Graph_lib::Window::Window(menu_p,menu_max_x,menu_max_y, "Menu"),
      background{ Point{0,0}, path_image + "Images/menu" + char(i + int('0')) + ".jpg"},
      play_pp_img{ play_pp, path_image + "Images/play.jpg"},
      play_pc_img{ play_pc, path_image + "Images/Player_comp.png", Suffix::png},
      quit_img{ quit_pp, path_image + "Images/quit.jpg"},
      go_play_pp_button{ play_pp, play_w, play_h, "Play", cb_go_play_pp},
      go_play_pc_button{ play_pc, play_pc_w, play_pc_h, "Play", cb_go_play_pc},
      quit_button{ quit_pp, quit_w, quit_h, "Quit", cb_quit}
{
    start();
}


void Chess_Menu::start()
{
    attach_all();
    while(!is_quit)
    {
        while (!is_quit && !is_play_pc && !is_play_pp && Fl::wait());
        if(is_play_pc)
        {
            is_play_pc = false;
            Settings set;
            FigColor col = set.setting();
            if (set.is_quit)
                continue;
            My_Chessboard desk;
            End_Return sit = desk.play_bot(col);
            if (!desk.quit_is)
                end(sit);
            desk.hide();
        }
        else if(is_play_pp)
        {
            is_play_pp = false;
            My_Chessboard desk;
            End_Return sit = desk.play();
            if (!desk.quit_is)
                end(sit);
            desk.hide();
        }
    }
}


void Chess_Menu::end(End_Return sit)
{
    if (sit.sit == Situation::checkmate)
    {
        if (sit.win_color == FigColor::white)
            Chess_End end(Situation::checkmate, FigColor::black);
        else
            Chess_End end(Situation::checkmate, FigColor::white);
    }
    else if (sit.sit == Situation::pat)
    {
        if (sit.win_color == FigColor::white)
            Chess_End end(Situation::pat, FigColor::black);
        else
            Chess_End end(Situation::pat, FigColor::white);
    }
}


void Chess_Menu::attach_all()
{
    attach(background);
    attach(play_pp_img);
    attach(play_pc_img);
    attach(quit_img);
    attach(go_play_pp_button);
    attach(go_play_pc_button);
    attach(quit_button);
    Fl::redraw();
}




Settings::Settings()
    : Graph_lib::Window::Window(sett_p, sett_max_x, sett_max_y, "Setting"),
      message{Point{0,15},"Сhoose color"},
      white_button{ Point{0, 15}, weight_cell, weight_cell, "", cb_white},
      black_button{ Point{x_max()-weight_cell, 15}, weight_cell, weight_cell, "", cb_black}
{}


FigColor Settings::setting()
{
    Image w{Point(0, 15), path_image+"figures/wP.png", Suffix::png};
    Image b{Point(x_max()-weight_cell, 15), path_image+"figures/bP.png", Suffix::png};
    attach(w);
    attach(b);
    attach(white_button);
    attach(black_button);
    attach(message);
    Fl::redraw();

    while (!is_click && Fl::wait() && !is_quit);
    Fl_Window::hide();
    return choice;
}
