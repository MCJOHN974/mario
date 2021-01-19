#include <iostream>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <fileapi.h>
#include <string>


const int HIGH = 25;
const int LENGTH = 1000;
const int LENGTH_OF_SHOW = 50;
const int PLAYER_POS = 25;
FILE* stream;



enum cell_types {
    empty, ground, man_body, man_left_hand, man_right_hand, man_head, vertical_wall, turbo_up
};



class field {
private:
    int player_y = -1;
    int player_x = -1;
    int player_leg_pos = 1;
    int player_arm_pos = 1;
    int player_move_up_power = 0;
    int POS_OF_SHOW = 0;
    cell_types cells[HIGH][LENGTH];

public:
    field(std::vector<std::string> s) {
        for (int i = 0; i < HIGH; i++) {
            for (int j = 0; j < LENGTH; j++) {
                switch (s[i][j]) {
                case('e'): {
                    cells[i][j] = empty;
                    break;
                }
                case('='): {
                    cells[i][j] = ground;
                    break;
                }
                case('|'): {
                    cells[i][j] = vertical_wall;
                    break;
                }
                case('^'): {
                    cells[i][j] = turbo_up;
                    break;
                }
                }
            }
        }
    }

    void move_map_right() {
       /* 
       cell_types output[HIGH][LENGTH];
        for (int y = 0; y < HIGH; y++) {
            for (int x = 0; x < LENGTH; x++) {
                if (x > 0) {
                    output[y][x] = cells[y][x - 1];
                }
                else {
                    output[y][x] = empty;
                }
            }
        }
        for (int y = 0; y < HIGH; y++) {
            for (int x = 0; x < LENGTH; x++) {
                cells[y][x] = output[y][x];
            }
        }
        player_x++;
        */
        POS_OF_SHOW--;
    }

    void move_map_left() {
        /*
        cell_types output[HIGH][LENGTH];
        for (int y = 0; y < HIGH; y++) {
            for (int x = 0; x < LENGTH; x++) {
                if (x < LENGTH - 1) {
                    output[y][x] = cells[y][x + 1];
                }
                else {
                    output[y][x] = empty;
                }
            }
        }
        for (int y = 0; y < HIGH; y++) {
            for (int x = 0; x < LENGTH; x++) {
                cells[y][x] = output[y][x];
            }
        }
        player_x--;
        */
        POS_OF_SHOW++;
    }

    bool spawn_player(int x, int y, int arm_pos, int leg_pos) {
        bool succes = true;
        succes = cells[y - 2][x - 1] == empty && cells[y - 2][x] == empty && cells[y - 2][x + 1] == empty
            && cells[y - 1][x - 1] == empty && cells[y - 1][x] == empty && cells[y - 1][x + 1] == empty &&
            cells[y][x - 1] == empty && cells[y][x] == empty && cells[y][x + 1] == empty;
            if (succes) {
                player_x = x;
                player_y = y;
                player_arm_pos = arm_pos;
                player_leg_pos = leg_pos;
                switch (arm_pos) {
                case(1): {
                    cells[y - 2][x - 1] = empty;
                    cells[y - 2][x + 1] = empty;
                    cells[y - 1][x - 1] = man_left_hand;
                    cells[y - 1][x + 1] = man_right_hand;
                    break;
                }
                case(2): {
                    cells[y - 2][x - 1] = man_right_hand;
                    cells[y - 2][x + 1] = man_left_hand;
                    cells[y - 1][x - 1] = empty;
                    cells[y - 1][x + 1] = empty;
                    break;
                }
                }
                cells[y - 2][x] = man_head;
                cells[y - 1][x] = man_body;
                switch (leg_pos) {
                case(1): {
                    cells[y][x - 1] = man_left_hand;
                    cells[y][x] = man_right_hand;
                    cells[y][x + 1] = empty;
                    break;
                }
                case(2): {
                    cells[y][x] = man_left_hand;
                    cells[y][x + 1] = man_right_hand;
                    cells[y][x - 1] = empty;
                    break;
                }
                }
                return true;
            }
        return false;
    }

    void despawn_player(int x, int y) {
        for (int i = y - 2; i <= y; i++) {
            for (int j = x - 1; j <= x + 1; j++)
                cells[i][j] = empty;
        }
    }

    bool move_player_right() {
        if (player_x < LENGTH - 2) {
            despawn_player(player_x, player_y);
            bool ans = spawn_player(player_x + 1, player_y, player_arm_pos, 3 - player_leg_pos);
            if (!ans) {
                spawn_player(player_x, player_y, player_arm_pos, 3 - player_leg_pos);
            }
            else {
                move_map_left();
            }
            return ans;
        }
        else {
            return false;
        }
    }

    bool move_player_left() {
        if (player_x > 1) {
            despawn_player(player_x, player_y);
            bool ans = spawn_player(player_x - 1, player_y, player_arm_pos, 3 - player_leg_pos);
            if (!ans) {
                spawn_player(player_x, player_y, player_arm_pos, 3 - player_leg_pos);
            }
            else {
                move_map_right();
            }
            return ans;
        }
        else {
            return false;
        }
    }

    bool player_fall() {
        bool ans = cells[player_y + 1][player_x] == empty && cells[player_y + 1][player_x - 1] == empty &&
            cells[player_y + 1][player_x + 1] == empty;
        if (ans) {
            despawn_player(player_x, player_y);
            spawn_player(player_x, player_y + 1, player_arm_pos, player_leg_pos);
        }
        return ans;
    }

    void player_jump() {
        if(cells[player_y + 1][player_x] == ground)
        player_move_up_power = 5;
    }

    bool player_move_up() {
        bool succes = player_move_up_power > 0;
        for (int j = player_x - 1; j <= player_x + 1; j++)
            succes = succes && cells[player_y - 3][j] == empty;
        if (succes) {
            player_move_up_power--;
            despawn_player(player_x, player_y);
            spawn_player(player_x, player_y - 1, player_arm_pos, player_leg_pos);
        }
        else
            player_move_up_power = 0;
        return succes;
    }


    void game() {

        spawn_player(15, 15, 1, 1);
        while (true) {
            if (player_move_up_power > 0)
                player_move_up();
            else
                player_fall();

            if (_kbhit())
            {

                switch (_getch())
                {
                case 75:
                    move_player_left();
                    break;
                case 77:
                    move_player_right();
                    break;
                case 32:
                    player_jump();
                    break;
                case 72:
                    player_arm_pos = 2;
                    despawn_player(player_x, player_y);
                    spawn_player(player_x, player_y, player_arm_pos, player_leg_pos);
                    break;
                case 80:
                    player_arm_pos = 1;
                    despawn_player(player_x, player_y);
                    spawn_player(player_x, player_y, player_arm_pos, player_leg_pos);
                    break;
                case 83:
                    despawn_player(player_x, player_y);
                    break;
                }
            }

            if (cells[player_y + 1][player_x] == turbo_up) {
                player_move_up_power = 15;
            }


            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0, 0 });
            std::cout << *this;
        }
    }




    friend std::ostream& operator<< (std::ostream& out, const field& f) {
        for (int i = 0; i < HIGH; i++) {
            for (int j = max(f.POS_OF_SHOW, 0); j < min(f.POS_OF_SHOW + LENGTH_OF_SHOW, LENGTH); j++) {
                switch (f.cells[i][j]) {
                case(empty): {
                    out << ' ';
                    break;
                }
                case(ground): {
                    out << 'Z';
                    break;
                }
                case(man_body): {
                    out << 'Q';
                    break;
                }
                case(man_head): {
                    out << 'o';
                    break;
                }
                case(man_right_hand): {
                    out << '\\';
                    break;
                }
                case(man_left_hand): {
                    out << '/';
                    break;
                }
                case(vertical_wall): {
                    out << '|';
                    break;
                }
                case(turbo_up): {
                    out << '^';
                    break;
                }
                }
            }
            out << '\n';
        }
        return out;
    }
};


int main()
{
    CONSOLE_CURSOR_INFO curs = { 0 };
    curs.dwSize = sizeof(curs);
    curs.bVisible = FALSE;
    ::SetConsoleCursorInfo(::GetStdHandle(STD_OUTPUT_HANDLE), &curs);

    freopen_s(&stream, "map.txt", "r", stdin);
    std::vector<std::string> v;
    for (int i = 0; i < HIGH; i++) {
        std::string s;
        std::getline(std::cin, s);
        v.push_back(s);
    }
    field f = field(v);
    f.game();
    return 0;
}

