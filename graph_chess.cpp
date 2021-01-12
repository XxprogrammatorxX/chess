#include <iostream>
#include <string>
#include "graph_chess.h"


std::string letters ()
{
    std::string s(n_cells, '\0');
    for (unsigned int i = 0; i < n_cells; ++i)
        s[i] = char(int('a') + i);
    return s;
}

std::string digits ()
{
    std::string s(n_cells, '\0');
    for (unsigned int i = 0; i < n_cells; ++i)
        s[i] = char(int('0') + n_cells - i);
    return s;
}

Print_motion::Print_motion()
    :out{ new Out_box{ Point{win_max_x-140, 120},  140, 20, ""},
          new Out_box{ Point{win_max_x-140, 140},  140, 20, ""},
          new Out_box{ Point{win_max_x-140, 160},  140, 20, ""},
          new Out_box{ Point{win_max_x-140, 180},  140, 20, ""},
          new Out_box{ Point{win_max_x-140, 200},  140, 20, ""} }
{
    for(int i{0}; i < 5; ++i)
        str.push_back("");
}

My_Window::My_Window()
    :Graph_lib::Window::Window(win_p,win_max_x,win_max_y, "Chess"),
      quit_button{ Point{x_max()-140, 20}, 140, 20, "Quit", cb_quit},
      reset_button{ Point{x_max()-140, 40}, 140, 20, "Reset play", cb_reset},
      reverse_motion_button{ Point{x_max()-140, 60}, 140, 20, "Reverse_motion", cb_reverse_motion},
      draw_button{ Point{x_max()-140, 80}, 140, 20, "Draw", cb_draw}
{}

My_Chessboard::My_Chessboard()
    :Chessboard()
{attach_start();}

End_Return My_Chessboard::play()
{
    while (!quit_is)
    {
        try
        {
            desk.check_no_king();
        }
        catch(Table::No_King)
        {
            cerr << "pfnhf[fk e;t, Gde korol\n";
            return End_Return{Situation::none, FigColor::white};
        }
        position();
        while (!is_click() && Fl::wait() && !reset && !reverse && !quit_is && !draw_is);
        if (recovery_button())
            continue;

        if (draw_is)
        {
            return check_sit(Situation::pat);
            break;
        }

        int flip;
        if (desk.get_number_move()%2 == 0)
            flip = 0;
        else
            flip = n_cells-1;

        Point click1 = cell_click();
        if (desk.check_figure(abs(flip -click1.x), abs(flip - click1.y)))
        {
            selection(abs(flip -click1.x), abs(flip - click1.y));
            while (!is_click() && Fl::wait() && !reset && !reverse && !quit_is && !draw_is);
            if (recovery_button())
                continue;

            if (draw_is)
                return check_sit(Situation::pat);

            Point click2 = cell_click();
            int x1 = abs(flip - click1.x);
            int y1 = abs(flip - click1.y);
            int x2 = abs(flip - click2.x);
            int y2 = abs(flip - click2.y);
            if (desk.check_move(x1, y1, x2, y2))
            {
                if (desk.get(x1,y1).type == Figures::pawn && (y2 == 7 || y2 == 0))
                    desk.move(x1, y1, x2, y2, transformation(desk.get(x1,y1).col));
                else
                    desk.move(x1, y1,x2, y2);
                print_moves();


                End_Return situat = check_sit();
                if(situat.sit != Situation::none && situat.sit != Situation::shah)
                {
                    unselection();
                    position();
                    return situat;
                }
            }
            unselection();
        }
    }
    return End_Return{Situation::none, FigColor::white};
}

End_Return My_Chessboard::play_bot(FigColor bot_col)
{
    Bot bot(desk, bot_col);
    if (bot_col == FigColor::white)
    {
        bot.move();
        print_moves();
    }
    while (!quit_is)
    {
        try
        {
            desk.check_no_king();
        }
        catch(Table::No_King)
        {
            cerr << "pfnhf[fk e;t, gde korol";
            return End_Return{Situation::none, FigColor::white};
        }

        position();
        while (!is_click() && Fl::wait() && !reset && !reverse && !quit_is && !draw_is);

        if(reverse)
        {
            reverse_motion();
            reverse = false;
        }
        if (reset == true)
        {
            reset = false;
            if (bot_col == FigColor::white)
            {
                bot.move();
                print_moves();
            }
            continue;
        }
        if(quit_is)
            continue;
        if (draw_is)
            return check_sit(Situation::pat);

        int flip;
        if (desk.get_number_move()%2 == 0)
            flip = 0;
        else
            flip = n_cells-1;

        Point click1 = cell_click();
        if (desk.check_figure(abs(flip -click1.x), abs(flip - click1.y)))
        {
            selection(abs(flip -click1.x), abs(flip - click1.y));
            while (!is_click() && Fl::wait() && !reset && !reverse && !quit_is && !draw_is);

            if(reverse)
            {
                reverse_motion();
                reverse = false;
            }
            if (reset == true)
            {
                reset = false;
                if (bot_col == FigColor::white)
                {
                    bot.move();
                    print_moves();
                }
                continue;
            }
            if(quit_is)
                continue;
            if (draw_is)
                return check_sit(Situation::pat);

            Point click2 = cell_click();
            int x1 = abs(flip -click1.x);
            int y1 = abs(flip - click1.y);
            int x2 = abs(flip -click2.x);
            int y2 = abs(flip - click2.y);
            if (desk.check_move(x1, y1,x2, y2))
            {
                if (desk.get(x1,y1).type == Figures::pawn && (y2 == 7 || y2 == 0))
                    desk.move(x1, y1,x2, y2, transformation(desk.get(x1,y1).col));
                else
                    desk.move(x1, y1,x2, y2);
                unselection();
                print_moves();

                End_Return situat = check_sit();
                if(situat.sit != Situation::none && situat.sit != Situation::shah)
                {
                    desk.next_move();
                    unselection();
                    position();
                    return situat;
                }
                bot.move();
                print_moves();
                situat = check_sit();
                if(situat.sit != Situation::none && situat.sit != Situation::shah)
                {

                    position();
                    return situat;
                }
            }
            unselection();
        }
    }
    return End_Return{Situation::none, FigColor::white};
}

void My_Chessboard::reverse_motion()
{
    if(!desk.reverse_motion())
        return;

    unselection();

    One_Move_Core last = desk.past_moves(1);
    if (desk.get_number_move() % 2 == 1)
    {
        string s = to_string(desk.get_number_move()/2 + 1);
        s += " " + str_leters.at(last.x1) + char(n_cells - last.y1 + int('0')) + "-"
                + str_leters.at(last.x2) + char(n_cells - last.y2 + int('0')) + "    ";
        moves.str[0] = s;
    }
    else
    {
        moves.str[0] = moves.str[1];
        moves.str[1] = moves.str[2];
        moves.str[2] = moves.str[3];
        moves.str[3] = moves.str[4];
        moves.str[4] = "";
    }

    reverse = true;
}

void My_Chessboard::reset_play ()
{
    desk.start();
    unselection();

    reset = true;
}

Figures My_Chessboard::transformation(FigColor col)
{
    Button queen{Point{x_max() - weight_cell, 300}, weight_cell, weight_cell, "", cb_trans};
    Button elephant{Point{x_max() - weight_cell, 300-weight_cell}, weight_cell, weight_cell, "", cb_trans};
    Button horse{Point{x_max()-weight_cell*2, 300-weight_cell}, weight_cell, weight_cell, "", cb_trans};
    Button tower{Point{x_max()-weight_cell*2, 300}, weight_cell, weight_cell, "", cb_trans};

    attach(queen);
    attach(elephant);
    attach(horse);
    attach(tower);

    Image* q = new Image(Point{x_max() - weight_cell, 300},
                         path_image+fig_col.at(col)+ "Q.png",
                         Suffix::png
                         );
    Image* e = new Image(Point{x_max() - weight_cell, 300-weight_cell},
                         path_image+fig_col.at(col)+ "B.png",
                         Suffix::png
                         );
    Image* h = new Image(Point{x_max() - weight_cell*2, 300-weight_cell},
                         path_image+fig_col.at(col)+ "N.png",
                         Suffix::png
                         );
    Image* t = new Image(Point{x_max() - weight_cell*2, 300},
                         path_image+fig_col.at(col)+ "R.png",
                         Suffix::png
                         );

    attach(*q);
    attach(*e);
    attach(*h);
    attach(*t);
    Fl::redraw();

    while(!is_cl_trans && Fl::wait());

    is_cl_trans = false;

    Figures res;

    if(x_trans == x_max() - weight_cell)
    {
        if(y_trans == 300)
            res = Figures::queen;
        else
            res = Figures::elephant;
    }
    else
    {
        if(y_trans == 300)
            res = Figures::tower;
        else
            res = Figures::horse;
    }

    detach(queen);
    detach(elephant);
    detach(horse);
    detach(tower);
    detach(*q);
    detach(*e);
    detach(*h);
    detach(*t);
    return res;
}

End_Return My_Chessboard::check_sit(Situation sit)
{
    Situation d;
    if (sit == Situation::none)
        d = desk.desk_situation();
    else
        d = sit;

    if (d == Situation::checkmate)
    {
        if (desk.get_number_move()%2 == 1)
            return {Situation::checkmate, FigColor::black};
        else
            return {Situation::checkmate, FigColor::white};
    }
    else if (d == Situation::pat)
    {
        if (desk.get_number_move()%2 == 1)
            return {Situation::pat, FigColor::black};
        else
            return {Situation::pat, FigColor::white};
    }
    else if (d == Situation::shah)
        out->put("Shah");
    return {Situation::none, FigColor::white};
}

Chessboard::Chessboard()
    : My_Window(),
      x_labels_w{ letters() }, x_labels_b{ letters() },
      y_labels_w{ digits() }, y_labels_b{ digits() },
      out{new Out_box{Point{win_max_x-200, win_max_y-40},  140, 20, ""}}
{
    for (int i{0}; i < n_cells; ++i)
    {
        for (int j{0}; j < n_cells; ++j)
        {
            Point p = Point(indent_x+weight_cell*j, indent_y+weight_cell*i);
            b_cells.push_back(new Cell(p));
            cells.push_back(new Graph_lib::Rectangle(p,weight_cell,weight_cell));
            if ((i+j)%2 == 0)
                cells[cells.size()-1].set_fill_color(col_cell1);
            else
                cells[cells.size()-1].set_fill_color(col_cell2);
        }

        x_labels_w.add(Point{indent_x + weight_cell/2 + weight_cell*i,
                             indent_y + weight_cell*n_cells + 10});
        x_labels_b.add(Point{indent_x + weight_cell/2 + weight_cell*(n_cells - i - 1),
                             indent_y + weight_cell*n_cells + 10});
        y_labels_w.add(Point{indent_x - 10,
                             indent_y + weight_cell/2 + weight_cell*i});
        y_labels_b.add(Point{indent_x - 10,
                             indent_y + weight_cell/2 + weight_cell*(n_cells - i - 1)});
    }
}

void My_Chessboard::attach_all()
{
    for (unsigned int i{0}; i < select.size(); ++i)
        attach(*select[i]);

    for (unsigned int i{0}; i < figures.size(); ++i)
        attach(*figures[i]);

    if (desk.get_number_move()%2 == 0)
    {
        attach(x_labels_w);
        attach(y_labels_w);
    }
    else
    {
        attach(y_labels_b);
        attach(x_labels_b);
    }
    moves.out[4]->put(moves.str[4]);
    moves.out[3]->put(moves.str[3]);
    moves.out[2]->put(moves.str[2]);
    moves.out[1]->put(moves.str[1]);
    moves.out[0]->put(moves.str[0]);
    redraw();
}

void My_Chessboard::attach_start()
{
    for (int i{0}; i < b_cells.size(); ++i)
        attach(b_cells[i]);
    for (int i{0}; i < cells.size(); ++i)
        attach(cells[i]);

    attach(quit_button);
    attach(reset_button);
    attach(reverse_motion_button);
    for (Out_box* i : moves.out)
        attach(*i);
    attach(*out);
    attach(draw_button);
}

void My_Chessboard::position()
{
    detach_posit();
    int flip;
    if (desk.get_number_move()%2 == 0)
        flip = 0;
    else
        flip = n_cells-1;

    for (int i{0}; i < n_cells; ++i)
        for (int j{0}; j < n_cells; ++j)
        {
            Figure f = desk.get(j,i);
            if (f.type != Figures::none)
            {
                figures.push_back(new Image(Point{weight_cell*abs(flip -j) + indent_x,
                                                  weight_cell*abs(flip - i)+indent_y},
                                            path_image+fig_col.at(f.col)+fig_type.at(f.type),
                                            Suffix::png
                                            ));
            }
        }
    attach_all();
}

void My_Chessboard::detach_posit()
{
    for (unsigned long long i = figures.size(); i >= 1; --i)
    {
        detach(*figures[i-1]);
        figures.pop_back();
    }

    if (desk.get_number_move() % 2 == 1)
    {
        detach(x_labels_w);
        detach(y_labels_w);
    }
    else
    {
        detach(x_labels_b);
        detach(y_labels_b);
    }

}

void My_Chessboard::selection(int x, int y)
{
    vector <One_Motion> res;
    switch (desk.get(x, y).type)
    {
    case Figures::king:
        res = desk.pos_moves_king(x,y);
        break;
    case Figures::queen:
        res = desk.pos_moves_queen(x,y);
        break;
    case Figures::elephant:
        res = desk.pos_moves_elephant(x,y);
        break;
    case Figures::horse:
        res = desk.pos_moves_horse(x,y);
        break;
    case Figures::tower:
        res = desk.pos_moves_tower(x,y);
        break;
    case Figures::pawn:
        res = desk.pos_moves_pawn(x,y);
        break;
    default:
        break;
    }

    int flip;
    if (desk.get_number_move()%2 == 0)
        flip = 0;
    else
        flip = n_cells-1;

    for (One_Motion i : res)
    {
        select.push_back(new Graph_lib::Rectangle{Point{ abs(flip - i.x2)*weight_cell + indent_x,
                                                         abs(flip - i.y2)*weight_cell + indent_y},
                                                  weight_cell, weight_cell});
        if (desk.get(i.x2, i.y2).type == Figures::none)
            select[select.size() - 1]->set_fill_color(select_col2);
        else
            select[select.size() - 1]->set_fill_color(select_col3);
    }

    select.push_back(new Graph_lib::Rectangle{Point{ abs(flip - x)*weight_cell + indent_x,
                                                     abs(flip - y)*weight_cell + indent_y},
                                              weight_cell, weight_cell});
    select[select.size() - 1]->set_fill_color(select_col1);

    position();
}

void My_Chessboard::unselection()
{
    for (int i = select.size()-1; i >= 0; --i)
    {
        detach(*select[i]);
        select.pop_back();
    }
}

void Chessboard::print_moves()
{
    One_Move_Core last = desk.past_moves(1);
    if (desk.get_number_move() % 2 == 1)
    {
        moves.str[4] = moves.str[3];
        moves.str[3] = moves.str[2];
        moves.str[2] = moves.str[1];
        moves.str[1] = moves.str[0];
        string s = std::to_string(desk.get_number_move()/2+1);
        s += " " + str_leters.at(last.x1) + char(n_cells - last.y1 + int('0')) + "-"
                 + str_leters.at(last.x2) + char(n_cells - last.y2 + int('0')) + "    ";
        moves.str[0] = s;

    }
    else
    {
        string s = moves.str[0];
        s += " " + str_leters.at(last.x1) + char(n_cells - last.y1 + int('0')) + "-"
                 + str_leters.at(last.x2) + char(n_cells - last.y2 + int('0')) + " ";
        moves.str[0] = s;
    }
}


void Chessboard::click(Address button)
{
    click_x = (Graph_lib::reference_to<Fl_Widget>(button).x() - indent_x)/weight_cell;
    click_y = (Graph_lib::reference_to<Fl_Widget>(button).y() - indent_y)/weight_cell;
    clicked = true;
}

Point Chessboard::cell_click()
{
    clicked = false;
    return Point{click_x,click_y};
}

bool Chessboard::recovery_button()
{
    if (reset == true)
    {
        reset = false;
        return true;
    }
    if (reverse == true)
    {
        reverse = false;
        return true;
    }
    if(quit_is)
        return true;
    return false;
}
