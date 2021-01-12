#include "yadro.h"
#include <iostream>


int sign(int val) {
    if (val == 0)  return 0;
    if (val > 0)  return 1;
    return -1;
}

void Table::check_no_king()
{
    int n{0};
    for (int i{0}; i < n_cells; ++i)
        for(Figure f : desk[i])
            if (f.type == Figures::king)
                ++n;
    if(n != 2)
        throw No_King{};
}

bool One_Move_Core::operator!=(const One_Move_Core& a) const{
    if(x1 == a.x2 && y1 == a.y2 && x2 == a.x1 && y2 == a.y1)
        return true;
    return false;
}

void Table::start(){
    all_moves.clear();
    number_move = 0;
    black_king.x = 4;
    black_king.y = 0;
    black_king.king_moved = false;
    black_king.far_t_moved =false;
    black_king.near_t_moved = false;
    desk[0][0] = Figure{Figures::tower, FigColor::black};
    desk[0][1] = Figure{Figures::horse, FigColor::black};
    desk[0][2] = Figure{Figures::elephant, FigColor::black};
    desk[0][3] = Figure{Figures::queen, FigColor::black};
    desk[0][4] = Figure{Figures::king, FigColor::black};
    desk[0][5] = Figure{Figures::elephant, FigColor::black};
    desk[0][6] = Figure{Figures::horse, FigColor::black};
    desk[0][7] = Figure{Figures::tower, FigColor::black};

    for(unsigned int i {0}; i < n_cells; ++i)
        desk[1][i] = Figure{Figures::pawn, FigColor::black};

    for (unsigned int j {2}; j <= 5; ++j)
        for(unsigned int i {0}; i < n_cells; ++i)
            desk[j][i] = Figure{Figures::none, FigColor::white};

    for(unsigned int i {0}; i < n_cells; ++i)
        desk[6][i] = Figure{Figures::pawn, FigColor::white};

    white_king.x = 4;
    white_king.y = 7;

    white_king.king_moved = false;
    white_king.far_t_moved =false;
    white_king.near_t_moved = false;
    desk[7][0] = Figure{Figures::tower, FigColor::white};
    desk[7][1] = Figure{Figures::horse, FigColor::white};
    desk[7][2] = Figure{Figures::elephant, FigColor::white};
    desk[7][3] = Figure{Figures::queen, FigColor::white};
    desk[7][4] = Figure{Figures::king, FigColor::white};
    desk[7][5] = Figure{Figures::elephant, FigColor::white};
    desk[7][6] = Figure{Figures::horse, FigColor::white};
    desk[7][7] = Figure{Figures::tower, FigColor::white};
}

void Table::move(int x1, int y1, int x2, int y2, Figures trans){
    if (get(x1, y1).type == Figures::pawn  && x1!=x2 &&/* get(x2, y2).type == Figures::none*/ check_on_pass(x1, y1, x2, y2))
    {
        if (get(x1, y1).col == FigColor::white)
            all_moves.push_back(One_Move_Core{x1,y1,x2,y2,white_king,black_king, get(x2, y2+1), true});
        else
            all_moves.push_back(One_Move_Core{x1,y1,x2,y2,white_king,black_king, get(x2, y2-1), true});
        on_pass(x1, y1, x2, y2);
    }
    else
        all_moves.push_back(One_Move_Core{x1,y1,x2,y2,white_king,black_king, desk[y2][x2], false});


    if (y1 == y2  &&  abs(x1 - x2) == 2  &&  get(x1, y1).type == Figures::king)
        castling_tower(x1, y1, x2, y2);

    if(trans != Figures::none)
        desk[y2][x2].type = trans;
    else
        desk[y2][x2].type = desk[y1][x1].type;
    desk[y2][x2].col = desk[y1][x1].col;
    desk[y1][x1].type = Figures::none;
    desk[y1][x1].col = FigColor::white;
    ++number_move;



    if (desk[y2][x2].type == Figures::king)
    {
        if (desk[y2][x2].col == FigColor::white)
        {
            white_king.x = x2;
            white_king.y = y2;
            white_king.king_moved = true;
        }
        else
        {
            black_king.x = x2;
            black_king.y = y2;
            black_king.king_moved = true;
        }
    }
    else if (desk[y2][x2].type == Figures::tower)
    {
        if (desk[y2][x2].col == FigColor::black)
        {
            if (x1 == 0)
                black_king.far_t_moved = true;
            else
                black_king.near_t_moved = true;
        }
        else {
            if (x1 == 0)
                white_king.far_t_moved = true;
            else
                white_king.near_t_moved = true;
        }
    }
}

void Table::castling_tower (int x1, int y1, int x2, int){
    if (desk[y1][x1].col == FigColor::white){
        if (x2 == 2){
            desk[7][0].type = Figures::none;
            desk[7][0].col = FigColor::white;
            desk[7][3].type = Figures::tower;
            desk[7][3].col = FigColor::white;
            white_king.far_t_moved = true;
        }
        else {
            desk[7][7].type = Figures::none;
            desk[7][7].col = FigColor::white;
            desk[7][5].type = Figures::tower;
            desk[7][5].col = FigColor::white;
            white_king.near_t_moved = true;
        }
    }
    else {
        if (x2 == 2){
            desk[0][0].type = Figures::none;
            desk[0][0].col = FigColor::white;
            desk[0][3].type = Figures::tower;
            desk[0][3].col = FigColor::black;
            black_king.far_t_moved = true;
        }
        else {
            desk[0][7].type = Figures::none;
            desk[0][7].col = FigColor::white;
            desk[0][5].type = Figures::tower;
            desk[0][5].col = FigColor::black;
            black_king.near_t_moved = true;
        }
    }
}

bool Table::reverse_motion(){
    if (all_moves.size() == 0)
        return false;

    int x1 = all_moves[all_moves.size()-1].x1;
    int y1 = all_moves[all_moves.size()-1].y1;
    int x2 = all_moves[all_moves.size()-1].x2;
    int y2 = all_moves[all_moves.size()-1].y2;
    Figure beaten = all_moves[all_moves.size()-1].beaten;

    if (all_moves[all_moves.size()-1].on_pass)
    {
        desk[y1][x1].type = desk[y2][x2].type;
        desk[y1][x1].col = desk[y2][x2].col;
        desk[y1][x2].type = beaten.type;
        desk[y1][x2].col = beaten.col;
        desk[y2][x2].type = Figures::none;
        desk[y2][x2].col = FigColor::white;
    }
    else
    {
        if(desk[y2][x2].type == Figures::king && abs(x2 - x1) == 2)
        {
            if (x2 > x1)
            {
                desk[y1][7].type = desk[y2][x2-1].type;
                desk[y1][7].col = desk[y2][x2-1].col;
                desk[y2][x2-1].type = Figures::none;
                desk[y2][x2-1].col = FigColor::white;
            }
            else
            {
                desk[y1][0].type = desk[y2][x2+1].type;
                desk[y1][0].col = desk[y2][x2+1].col;
                desk[y2][x2+1].type = Figures::none;
                desk[y2][x2+1].col = FigColor::white;
            }
        }
        desk[y1][x1].type = desk[y2][x2].type;
        desk[y1][x1].col = desk[y2][x2].col;
        desk[y2][x2].type = beaten.type;
        desk[y2][x2].col = beaten.col;
    }
    --number_move;


    white_king = all_moves[all_moves.size()-1].white_king;
    black_king = all_moves[all_moves.size()-1].black_king;
    all_moves.pop_back();
    return true;
}

void Table::display_position(){
    for(unsigned i{0}; i < n_cells; ++i)
    {
        for(unsigned j{0}; j < n_cells; ++j)
        {
            if (desk[i][j].type == Figures::none)
                cout << "- ";
            else
                cout << int(desk[i][j].type) << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << endl;
}

bool Table::check_figure (int x, int y) const {
    Figure f = get(x, y);
    if (f.col != FigColor::white && number_move%2 == 1 && f.type != Figures::none)
        return true;
    if (f.col != FigColor::black && number_move%2 == 0 && f.type != Figures::none)
        return true;
    return false;
}

bool Table::check_castling(FigColor col, int x) const{
    if (col == FigColor::white){
        if (x == 2)
            if (!white_king.king_moved && !white_king.far_t_moved){
                for (int i = 0; i < 5; i++)
                    if (attack(i, 7, FigColor::black)  || (get(i, 7).type != Figures::none && i != 0 && i != 4))
                        return false;
                return true;
            }
        if (!white_king.king_moved && !white_king.near_t_moved){
            for (int i = 7; i > 3; i--)
            {
                if (attack(i, 7, FigColor::black)  ||  (get(i, 7).type != Figures::none && i != 4 && i != 7))
                    return false;
            }
            return true;
        }
    }
    else{
        if (x == 2){
            if (!black_king.king_moved && !black_king.far_t_moved){
                for (int i = 0; i < 5; i++)
                    if (attack(i, 0, FigColor::white)  ||  (get(i, 0).type != Figures::none && i != 4 && i != 0))
                        return false;
                return true;
            }
        }
        if (!black_king.king_moved && !black_king.near_t_moved){
            for (int i = 7; i > 3; i--)
                if (attack(i, 0, FigColor::white)  ||  (get(i, 0).type != Figures::none && i != 4 && i != 7))
                    return false;
            return true;
        }
    }
    return false;
}

bool Table::check_move(int x1, int y1, int x2, int y2){
    if (x1 == x2  &&  y1 == y2)
        return false;

    Figure f1 = get(x1, y1);
    if (f1.type != Figures::king)
        if (!check_move_pre(x1,y1,x2,y2))
            return false;

    switch (f1.type){
    case Figures::king:
        return check_move_king(x1,y1,x2,y2);
    case Figures::queen:
        return check_move_queen(x1,y1,x2,y2);
    case Figures::elephant:
        return check_move_elephant(x1,y1,x2,y2);
    case Figures::horse:
        return check_move_horse(x1,y1,x2,y2);
    case Figures::tower:
        return check_move_tower(x1,y1,x2,y2);
    case Figures::pawn:
        return check_move_pawn(x1,y1,x2,y2);
    default:
        return false;
    }
}

bool Table::check_move_pre(int x1, int y1, int x2, int y2){
    Figure f1 = get(x1, y1);
    Figure f2 = get(x2, y2);

    if (f2.col == f1.col && f2.type != Figures::none)
        return false;

    if(!no_step_over(x1,y1,x2,y2))
        return false;

    move(x1,y1,x2,y2);
    if (number_move % 2 == 1)
    {
        if(shah(FigColor::white))
        {
            reverse_motion();
            return false;
        }
    }
    else
        if(shah(FigColor::black))
        {
            reverse_motion();
            return false;
        }
    reverse_motion();
    return true;
}

bool Table::check_move_king(int x1, int y1, int x2, int y2){
    if (y1 == y2  &&  abs(x2 - x1) == 2)
    {
        if (check_castling(get(x1, y1).col, x2))
            return true;
        else
            return false;
    }
    if (!check_move_pre(x1,y1,x2,y2))
        return false;
    if (abs(y1 - y2) <= 1  &&   abs(x1 - x2) <= 1)
        return true;
    return false;
}

bool Table::check_move_queen(int x1, int y1, int x2, int y2){
    return ( abs(y1 - y2) == abs(x1 - x2))  ||  (y1 == y2 )   ||  (x1 == x2);
}

bool Table::check_move_elephant(int x1, int y1, int x2, int y2){
    return (abs(y1 - y2) == abs(x1 - x2));
}

bool Table::check_move_horse(int x1, int y1, int x2, int y2){
    return (abs(x1 - x2) == 2  &&  abs(y1 - y2) == 1)  || (abs(x1 - x2) == 1  &&  abs(y1 - y2) == 2);
}

bool Table::check_move_tower(int x1, int y1, int x2, int y2){
    return y1 == y2  ||  x1 == x2;
}

bool Table::check_move_pawn(int x1, int y1, int x2, int y2){
    Figure f1 = get(x1, y1);
    Figure f2 = get(x2, y2);
    if (x1 == x2)
    {
        if (f2.type != Figures::none)
            return false;
        else if (f1.col == FigColor::white)
        {
            if ((y1 == 6  &&  y1 - y2 == 2)  ||  y1 - y2 == 1)
                return true;
            else
                return false;
        }
        else
        {
            if ((y1 == 1  &&  y2 - y1 == 2)  ||  y2 - y1 == 1)
                return true;
            else
                return false;
        }
    }
    else if (abs(x1 - x2) == 1)
    {
        if (check_on_pass(x1, y1, x2, y2))
            return true;
        if (f2.type == Figures::none)
            return false;
        else if (f1.col == FigColor::white)
        {
            if (y1 - y2 == 1)
                return true;
            else
                return false;
        }
        else if (y1 - y2 == -1)
            return true;
        else
            return false;
    }
    return false;
}

bool Table::check_on_pass(int x1, int y1, int x2, int){
    if (abs(x1 - x2) == 0)
        return false;
    if(all_moves.size() == 0)
        return false;
    One_Move_Core m = all_moves[all_moves.size()-1];
    
    if (get(x1, y1).col == FigColor::white  &&  y1 == 3)
        if (get(m.x2, m.y2).type == Figures::pawn  &&  get(m.x2, m.y2).col == FigColor::black)
            if (m.x1 == m.x2  &&  m.x2 == x2  &&  abs(m.y1 - m.y2) == 2)
                return true;
    
    if (get(x1, y1).col == FigColor::black  &&  y1 == 4)
        if (get(m.x2, m.y2).type == Figures::pawn  &&  get(m.x2, m.y2).col == FigColor::white)
            if (m.x1 == m.x2  &&  m.x2 == x2  &&  abs(m.y1 - m.y2) == 2)
                return true;

    return false;
}

void Table::on_pass(int, int y1, int x2, int){
    desk[y1][x2].type = Figures::none;
    desk[y1][x2].col = FigColor::white;
}

bool Table::attack(int x, int y, FigColor col_attack) const{
    //если атакующая фигура стоит на клетке, то клетка под атакой

    if (pawn_attack(x, y, col_attack))
        return true;

    if (king_attack(x, y, col_attack))
        return true;

    if (horse_attack(x, y, col_attack))
        return true;

    if(other_attack(x, y, col_attack))
        return true;

    return false;
}

bool Table::pawn_attack(int x, int y, FigColor col_attack) const{
    if (col_attack == FigColor::white)
    {
        if (x-1 >= 0 && y+1 < n_cells
                && get(x-1, y+1).type == Figures::pawn && get(x-1, y+1).col == col_attack)
            return true;
        if (x+1 < n_cells && y+1 < n_cells
                && get(x+1, y+1).type == Figures::pawn && get(x+1, y+1).col == col_attack)
            return true;
    }
    else
    {
        if (x-1 >= 0 && y-1 >= 0
                && get(x-1, y-1).type == Figures::pawn && get(x-1, y-1).col == col_attack)
            return true;
        if (x+1 < n_cells && y-1 >= 0
                && get(x+1, y-1).type == Figures::pawn && get(x+1, y-1).col == col_attack)
            return true;
    }
    return false;
}

bool Table::king_attack(int x, int y, FigColor col_attack) const{
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j)
            if (x+j >= 0 && y+i >= 0 && x+j < n_cells && y+i < n_cells)
                if (i != 0 || j != 0)
                    if (get(x+j, y+i).type == Figures::king && get(x+j, y+i).col == col_attack)
                        return true;
    return false;
}

bool Table::horse_attack(int x, int y, FigColor col_attack) const{
    if (x+2 < n_cells && y+1 < n_cells)
        if (get(x+2, y+1).type == Figures::horse && get(x+2, y+1).col == col_attack)
            return true;

    if (x+2 < n_cells && y-1 >= 0)
        if (get(x+2, y-1).type == Figures::horse && get(x+2, y-1).col == col_attack)
            return true;

    if (x-2 >= 0 && y+1 < n_cells)
        if (get(x-2, y+1).type == Figures::horse && get(x-2, y+1).col == col_attack)
            return true;

    if (x-2 >= 0 && y-1 >= 0)
        if (get(x-2, y-1).type == Figures::horse && get(x-2, y-1).col == col_attack)
            return true;

    if (x+1 < n_cells && y+2 < n_cells)
        if (get(x+1, y+2).type == Figures::horse && get(x+1, y+2).col == col_attack)
            return true;

    if (x+1 < n_cells && y-2  >= 0)
        if (get(x+1, y-2).type == Figures::horse && get(x+1, y-2).col == col_attack)
            return true;

    if (x-1 >= 0 && y+2 < n_cells)
        if (get(x-1, y+2).type == Figures::horse && get(x-1, y+2).col == col_attack)
            return true;

    if (x-1 >= 0 && y-2  >= 0)
        if (get(x-1, y-2).type == Figures::horse && get(x-1, y-2).col == col_attack)
            return true;
    return false;
}

bool Table::other_attack(int x, int y, FigColor col_attack) const{
    //вверх
    bool vert = true, diag1= true, diag2 = true;
    for(int i = 1; y-i >= 0; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (vert && (get(x, y-i).type == Figures::queen || get(x, y-i).type == Figures::tower) && get(x, y-i).col == col_attack)
            return true;
        if (diag1)
            if ((get(x-i, y-i).type == Figures::queen || get(x-i, y-i).type == Figures::elephant)  && get(x-i, y-i).col == col_attack)
                return true;
        if (diag2)
            if ((get(x+i, y-i).type == Figures::queen || get(x+i, y-i).type == Figures::elephant) && get(x+i, y-i).col == col_attack)
                return true;

        if (get(x, y-i).type !=  Figures::none)
            vert = false;
        if (get(x-i, y-i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y-i).type !=  Figures::none)
            diag2 = false;
    }

    vert = true;
    diag1= true;
    diag2 = true;

    //вниз
    for(int i = 1; y+i < n_cells; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (vert)
            if ((get(x, y+i).type == Figures::queen || get(x, y+i).type == Figures::tower) && get(x, y+i).col == col_attack)
                return true;
        if (diag1)
            if ((get(x-i, y+i).type == Figures::queen || get(x-i, y+i).type == Figures::elephant) && get(x-i, y+i).col == col_attack)
                return true;
        if (diag2)
            if ((get(x+i, y+i).type == Figures::queen || get(x+i, y+i).type == Figures::elephant) && get(x+i, y+i).col == col_attack)
                return true;

        if (get(x, y+i).type !=  Figures::none)
            vert = false;
        if (get(x-i, y+i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y+i).type !=  Figures::none)
            diag2 = false;
    }

    //горизонталь
    bool l = true;
    bool r = true;
    for (int i = 1;l || r; ++i)
    {
        if (x-i < 0)
            l = false;
        if (x+i > n_cells-1)
            r = false;


        if (l && (get(x-i, y).type == Figures::queen || get(x-i, y).type == Figures::tower) && get(x-i, y).col == col_attack)
            return true;
        if (r && (get(x+i, y).type == Figures::queen || get(x+i, y).type == Figures::tower) && get(x+i, y).col == col_attack)
            return true;

        if (get(x-i, y).type !=  Figures::none)
            l = false;
        if (get(x+i, y).type !=  Figures::none)
            r = false;
    }
    return false;
}

bool Table::no_step_over(int x1, int y1, int x2, int y2) const{
    if (x1 == x2)
        for (int i = 1; i < abs(y1 - y2); ++i)
            if (get(x1,y1 + i*sign(y2-y1)).type != Figures::none)
                return false;
    if (y1 == y2)
        for (int i = 1; i < abs(x1 - x2); ++i)
            if (get(x1 + i*sign(x2-x1),y1).type != Figures::none)
                return false;
    if (abs(y1-y2) == abs(x1-x2))
        for (int i = 1; i < abs(y1 - y2); ++i)
            if (get(x1 + i*sign(x2-x1), y1 + i*sign(y2-y1)).type != Figures::none)
                return false;
    return true;
}

bool Table::shah(FigColor col) const{
    if (col == FigColor::white)
        return attack(white_king.x, white_king.y, FigColor::black);
    else
        return attack(black_king.x, black_king.y, FigColor::white);
}

vector <One_Motion> Table::pos_moves(FigColor move_col){
    vector <One_Motion> res;
    for (int j{0}; j < n_cells; ++j)
        for (int i{0}; i < n_cells; ++i)
        {
            if (get(i, j).col == move_col)
                switch (get(i, j).type)
                {
                case Figures::king:
                    for(One_Motion g : pos_moves_king(i,j))
                        res.push_back(g);
                    break;
                case Figures::queen:
                    for(One_Motion g : pos_moves_queen(i,j))
                        res.push_back(g);
                    break;
                case Figures::elephant:
                    for(One_Motion g : pos_moves_elephant(i,j))
                        res.push_back(g);
                    break;
                case Figures::horse:
                    for(One_Motion g : pos_moves_horse(i,j))
                        res.push_back(g);
                    break;
                case Figures::tower:
                    for(One_Motion g : pos_moves_tower(i,j))
                        res.push_back(g);
                    break;
                case Figures::pawn:
                    for(One_Motion g : pos_moves_pawn(i,j))
                        res.push_back(g);
                    break;
                default:
                    break;
                }
        }
    return res;
}

vector <One_Motion> Table::pos_moves_king(int x, int y){
    vector <One_Motion> res;
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j)
            if (x+j >= 0 && y+i >= 0 && x+j < n_cells && y+i < n_cells)
                if (i != 0 || j != 0)
                    if (check_move_pre(x,y,x+j,y+i))
                        res.push_back(One_Motion{x, y, x+j, y+i ,Figures :: king});
    if (check_move_king(x,y,x+2,y))
        res.push_back(One_Motion{x, y, x+2, y ,Figures :: king});
    if (check_move_king(x,y,x-2,y))
        res.push_back(One_Motion{x, y, x-2, y ,Figures :: king});
    return res;
}

vector <One_Motion> Table::pos_moves_queen(int x, int y){
    vector <One_Motion> res;
    //вверх
    bool vert = true, diag1= true, diag2 = true;
    for(int i = 1; y-i >= 0; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (vert && check_move_pre(x, y, x, y-i))
            res.push_back(One_Motion{x, y, x, y-i,Figures :: queen});
        if (diag1 && check_move_pre(x, y, x-i, y-i))
            res.push_back(One_Motion{x, y, x-i, y-i ,Figures :: queen });
        if (diag2 && check_move_pre(x, y, x+i, y-i))
            res.push_back(One_Motion{x, y, x+i, y-i , Figures :: queen});

        if (get(x, y-i).type !=  Figures::none)
            vert = false;
        if (get(x-i, y-i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y-i).type !=  Figures::none)
            diag2 = false;
    }

    vert = true;
    diag1= true;
    diag2 = true;

    //вниз
    for(int i = 1; y+i < n_cells; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (vert && check_move_pre(x, y, x, y+i))
            res.push_back(One_Motion{x, y, x, y+i,  Figures :: queen});
        if (diag1 && check_move_pre(x, y, x-i, y+i))
            res.push_back(One_Motion{x, y, x-i, y+i,Figures :: queen});
        if (diag2 && check_move_pre(x, y, x+i, y+i))
            res.push_back(One_Motion{x, y, x+i, y+i,Figures :: queen});

        if (get(x, y+i).type !=   Figures::none)
            vert = false;
        if (get(x-i, y+i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y+i).type !=  Figures::none)
            diag2 = false;
    }

    //горизонталь
    bool l = true;
    bool r = true;
    for (int i = 1;l || r; ++i)
    {
        if (x-i < 0)
            l = false;
        if (x+i > n_cells-1)
            r = false;


        if (l && check_move_pre(x, y, x-i, y))
            res.push_back(One_Motion{x, y, x-i, y,Figures :: queen});
        if (r && check_move_pre(x, y, x+i, y))
            res.push_back(One_Motion{x, y, x+i, y,Figures :: queen});

        if (get(x-i, y).type !=  Figures::none)
            l = false;
        if (get(x+i, y).type !=  Figures::none)
            r = false;
    }
    return res;
}

vector <One_Motion> Table::pos_moves_elephant(int x, int y){
    vector <One_Motion> res;
    //вверх
    bool diag1= true, diag2 = true;
    for(int i = 1; y-i >= 0; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (diag1 && check_move_pre(x, y, x-i, y-i))
            res.push_back(One_Motion{x, y, x-i, y-i,Figures :: elephant});
        if (diag2 && check_move_pre(x, y, x+i, y-i))
            res.push_back(One_Motion{x, y, x+i, y-i,Figures :: elephant});

        if (get(x-i, y-i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y-i).type !=  Figures::none)
            diag2 = false;
    }

    diag1= true;
    diag2 = true;

    //вниз
    for(int i = 1; y+i < n_cells; ++i)
    {
        if (x-i < 0)
            diag1 = false;
        if (x+i > n_cells-1)
            diag2 = false;

        if (diag1 && check_move_pre(x, y, x-i, y+i))
            res.push_back(One_Motion{x, y, x-i, y+i , Figures :: elephant});
        if (diag2 && check_move_pre(x, y, x+i, y+i))
            res.push_back(One_Motion{x, y, x+i, y+i , Figures :: elephant} );

        if (get(x-i, y+i).type !=  Figures::none)
            diag1 = false;
        if (get(x+i, y+i).type !=  Figures::none)
            diag2 = false;
    }
    return res;
}

vector <One_Motion> Table::pos_moves_horse(int x, int y){
    vector <One_Motion> res;
    if (x+2 < n_cells && y+1 < n_cells)
        if (check_move_pre(x,y,x+2,y+1))
            res.push_back(One_Motion{x, y, x+2, y+1 , Figures :: horse});

    if (x+2 < n_cells && y-1 >= 0)
        if (check_move_pre(x,y,x+2,y-1))
            res.push_back(One_Motion{x, y, x+2, y-1 , Figures :: horse});

    if (x-2 >= 0 && y+1 < n_cells)
        if (check_move_pre(x,y,x-2,y+1))
            res.push_back(One_Motion{x, y, x-2, y+1 , Figures :: horse});

    if (x-2 >= 0 && y-1 >= 0)
        if (check_move_pre(x,y,x-2,y-1))
            res.push_back(One_Motion{x, y, x-2, y-1 , Figures :: horse});

    if (x+1 < n_cells && y+2 < n_cells)
        if (check_move_pre(x,y,x+1,y+2))
            res.push_back(One_Motion{x, y, x+1, y+2 , Figures :: horse});

    if (x+1 < n_cells && y-2  >= 0)
        if (check_move_pre(x,y,x+1,y-2))
            res.push_back(One_Motion{x, y, x+1, y-2 , Figures :: horse});
    if (x-1 >= 0 && y+2 < n_cells)
        if (check_move_pre(x,y,x-1,y+2))
            res.push_back(One_Motion{x, y, x-1, y+2 , Figures :: horse});

    if (x-1 >= 0 && y-2  >= 0)
        if (check_move_pre(x,y,x-1,y-2))
            res.push_back(One_Motion{x, y, x-1, y-2 , Figures :: horse});
    return res;
}

vector <One_Motion> Table::pos_moves_tower(int x, int y){
    vector <One_Motion> res;
    //вверх
    bool vert = true;
    for(int i = 1; y-i >= 0; ++i)
    {
        if (vert && check_move_pre(x, y, x, y-i))
            res.push_back(One_Motion{x, y, x, y-i,Figures :: tower});

        if (get(x, y-i).type !=  Figures::none)
            vert = false;
    }

    vert = true;

    //вниз
    for(int i = 1; y+i < n_cells; ++i)
    {
        if (vert && check_move_pre(x, y, x, y+i))
            res.push_back(One_Motion{x, y, x, y+i,Figures :: tower});

        if (get(x, y+i).type !=  Figures::none)
            vert = false;
    }

    //горизонталь
    bool l = true;
    bool r = true;
    for (int i = 1;l || r; ++i)
    {
        if (x-i < 0)
            l = false;
        if (x+i > n_cells-1)
            r = false;

        if (l && check_move_pre(x, y, x-i, y))
            res.push_back(One_Motion{x, y, x-i, y,Figures :: tower});
        if (r && check_move_pre(x, y, x+i, y))
            res.push_back(One_Motion{x, y, x+i, y,Figures :: tower});

        if (get(x-i, y).type !=  Figures::none)
            l = false;
        if (get(x+i, y).type !=  Figures::none)
            r = false;
    }
    return res;
}

vector <One_Motion> Table::pos_moves_pawn(int x, int y){
    vector <One_Motion> res;
    if (get(x,y).col == FigColor::white)
    {
        if (check_move_pre(x,y,x,y-1) && check_move_pawn(x,y,x,y-1))
            res.push_back(One_Motion{x, y, x, y-1,Figures :: pawn});
        if (check_move_pre(x,y,x,y-2) && check_move_pawn(x,y,x,y-2))
            res.push_back(One_Motion{x, y, x, y-2,Figures :: pawn});

        if (x+1 < n_cells)
            if(check_move_pre(x,y,x+1,y-1) && check_move_pawn(x,y,x+1,y-1))
                res.push_back(One_Motion{x, y, x+1, y-1,Figures :: pawn});
        if (x-1 >= 0)
            if(check_move_pre(x,y,x-1,y-1) && check_move_pawn(x,y,x-1,y-1))
                res.push_back(One_Motion{x, y, x-1, y-1,Figures :: pawn});
    }
    else
    {
        if (check_move_pre(x,y,x,y+1) && check_move_pawn(x,y,x,y+1))
            res.push_back(One_Motion{x, y, x, y+1,Figures :: pawn});
        if (check_move_pre(x,y,x,y+2) && check_move_pawn(x,y,x,y+2))
            res.push_back(One_Motion{x, y, x, y+2,Figures :: pawn});

        if (x+1 < n_cells)
            if(check_move_pre(x,y,x+1,y+1) && check_move_pawn(x,y,x+1,y+1))
                res.push_back(One_Motion{x, y, x+1, y+1,Figures :: pawn});
        if (x-1 >= 0)
            if(check_move_pre(x,y,x-1,y+1) && check_move_pawn(x,y,x-1,y+1))
                res.push_back(One_Motion{x, y, x-1, y+1,Figures :: pawn});
    }

    return res;
}

bool Table::repeated_moves() const {
    if (all_moves.size() >= 8)
        if(all_moves[all_moves.size() - 1] != all_moves[all_moves.size() - 3]
                && all_moves[all_moves.size() - 3] != all_moves[all_moves.size() - 5]
                && all_moves[all_moves.size() - 5] != all_moves[all_moves.size() - 7])
            if(all_moves[all_moves.size() - 2] != all_moves[all_moves.size() - 4]
                    && all_moves[all_moves.size() - 4] != all_moves[all_moves.size() - 6]
                    && all_moves[all_moves.size() - 6] != all_moves[all_moves.size() - 8])
                return true;
    return false;
}

Situation Table::desk_situation() {
    if (number_move % 2 == 0)
    {
        if (pos_moves(FigColor::white).size() == 0)
        {
            if (shah(FigColor::white))
                return Situation::checkmate;
            else
                return Situation::pat;
        }

        if (repeated_moves())
            return Situation::pat;

        if (shah(FigColor::white))
            return Situation::shah;
    }
    else
    {
        if (pos_moves(FigColor::black).size() == 0)
        {
            if (shah(FigColor::black))
                return Situation::checkmate;
            else
                return Situation::pat;
        }

        if (repeated_moves())
            return Situation::pat;

        if (shah(FigColor::black))
            return Situation::shah;
    }
    return Situation::none;
}
