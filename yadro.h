#ifndef YADRO_H
#define YADRO_H

#include <vector>
#include <cmath>

using namespace std;


constexpr int n_cells = 8;  //доска n_cells x n_cells


struct King_tower {
    int x = 0, y = 0;           //положение короля
    bool king_moved = false;    //ходил ли король
    bool near_t_moved = false;  //ходила ли ближняя ладья
    bool far_t_moved = false;   //ходила ли дальняя ладья
};


enum class Figures {
    king, queen, elephant, horse, tower, pawn, none
};

enum class FigColor {
    black, white
};

enum class Situation{
    checkmate, pat, shah, none
};


struct Figure {         //сама фигуры
    Figures type;       //тип
    FigColor col;       //цвет

    Figure(Figures t, FigColor c)
        :type{t},col{c}                 
    {}
    Figure()
        :type{Figures::none},col{FigColor::white}
    {}
};


struct One_Move_Core{       //один ход для работы ядра
    int x1,y1,x2,y2;        //откуда, куда
    King_tower white_king;
    King_tower black_king;
    Figure beaten;

    bool on_pass;           //было ли взятие на проходе

    bool operator!=(const One_Move_Core& a) const;
};


struct One_Motion{    //один ход для бота
    int x1,y1,x2,y2;
    Figures type;
};


class Table{
public:
    Table() { start();}

    class No_King{};
    void check_no_king();

    void start();  
    void move(int x1, int y1, int x2, int y2, Figures trans = Figures::none);
    
    bool check_figure(int x, int y) const;
    bool check_move(int x1, int y1, int x2, int y2);

    Figure get(int i, int j) const { return desk[j][i];}
    int get_number_move() const { return number_move;}

    void next_move() { ++number_move;}

    bool attack(int x, int y, FigColor col_attack) const;  //проверка атаки на клетку

    bool reverse_motion();                                 //обратный ход

    One_Move_Core past_moves(unsigned int i) const { return all_moves[all_moves.size() - i];}

    vector <One_Motion> pos_moves(FigColor move_col);      //
    vector <One_Motion> pos_moves_king(int x, int y);      //
    vector <One_Motion> pos_moves_queen(int x, int y);     //
    vector <One_Motion> pos_moves_elephant(int x, int y);  //предполагаемые ходы фигур
    vector <One_Motion> pos_moves_horse(int x, int y);     //
    vector <One_Motion> pos_moves_tower(int x, int y);     //
    vector <One_Motion> pos_moves_pawn(int x, int y);      //

    void display_position();     //вывод позиций в консоль
    Situation desk_situation();  //шах, мат, пат и т.д.

private:
    bool check_move_king(int x1, int y1, int x2, int y2);        //
    bool check_move_queen(int x1, int y1, int x2, int y2);       //
    bool check_move_elephant(int x1, int y1, int x2, int y2);    //проверка возможности хода фигуры
    bool check_move_horse(int x1, int y1, int x2, int y2);       //
    bool check_move_tower(int x1, int y1, int x2, int y2);       //
    bool check_move_pawn(int x1, int y1, int x2, int y2);        //
    bool check_move_pre(int x1, int y1, int x2, int y2);         //
    
    bool check_on_pass(int x1, int y1, int x2, int y2);          //бито на проходе
    void on_pass(int x1, int y1, int x2, int y2);                //

    bool check_castling (FigColor col, int x) const;             //проверка рокировки
    void castling_tower (int x1, int y1, int x2, int y2);        //рокировка

    bool pawn_attack(int x, int y, FigColor col_attack) const;   //
    bool king_attack(int x, int y, FigColor col_attack) const;   //проверка на атаку
    bool horse_attack(int x, int y, FigColor col_attack) const;  //соответствующей фигуры
    bool other_attack(int x, int y, FigColor col_attack) const;  //

    Figure desk[n_cells][n_cells] = {};  //доска
    int number_move = 0;

    vector<One_Move_Core> all_moves;

    bool no_step_over(int x1, int y1, int x2, int y2) const;  //нет перешагивания через фигуру (не нужно для коня)

    King_tower white_king;  //белый и черный король
    King_tower black_king;  //нужны для рокировки

    bool shah(FigColor) const;      //проверка на шах
    bool repeated_moves() const;    //повторение ходов(для ничьи)
};


int sign(int val);  //матемтическая функция signum

#endif // YADRO_H
