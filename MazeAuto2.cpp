#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <time.h>

using namespace std;

struct Cell
{
    int x, y;       // 해당 cell의 위치
    int union_code; // 집합 번호
    bool up, down, right, left;
};

int input;           // 입력 값
int union_count = 1; // 집합 갯수
int current_col = 0; // 현재 y의 위치
int dest[100][100];
Cell cells[50][50];

unordered_map<int, vector<Cell *>> unions;
vector<Cell *>::iterator dest_end, cpy_start, cpy_end;

void set_row();
void random_union();
void set_union(int current, int target);
void set_union_members(int union_code);

void set_row()
{
    for (int i = 0; i < input; i++)
    {
        cells[current_col][i].x = i;
        cells[current_col][i].y = current_col;

        if (cells[current_col][i].union_code == 0)
        {                                                   // 해당 cell에 집합이 지정되지 않을 경우
            cells[current_col][i].union_code = union_count; // 집합코드 지정

            unions.insert(make_pair(union_count++, vector<Cell *>())); // 새로운 집합 구성
            unions[union_count].push_back(&cells[current_col][i]);
        }
    }
}

void random_union()
{
    int current_union_code; // 현재 집합 코드
    int next_union_code;
    int next_index = 0;

    for (int i = 0; i < input - 1; i++)
    {
        bool flag = false;

        current_union_code = cells[current_col][i].union_code; // 현재 집합 코드 설정

        if (rand() % 2)
        {
            if (cells[current_col][next_index].union_code != current_union_code)
            {
                // 오른쪽 합집합
                next_index = i + 1;
                next_union_code = cells[current_col][next_index].union_code;

                if (i + 1 < input)
                { // 범위 검사
                    flag = true;
                }
            }
        }

        if (flag)
        {
            cells[current_col][i].right = true;
            cells[current_col][next_index].left = true;

            set_union(current_union_code, next_union_code);

            unions.erase(next_union_code);
        }
    }
}

void set_union(int current, int target)
{
    dest_end = unions[current].begin();
    cpy_start = unions[target].begin();
    cpy_end = unions[target].end();

    unions[current].insert(dest_end, cpy_start, cpy_end);

    set_union_members(current);
}

void set_union_members(int union_code)
{
    unordered_map<int, vector<Cell *>>::iterator iter = unions.find(union_code);

    for (int i = 0; i < iter->second.size(); i++)
    {
        iter->second[i]->union_code = union_code;
    }
}

void down_cell()
{
    int current_down_move_count;
    int v_size;
    unordered_map<int, vector<Cell *>>::iterator iter;

    for (iter = unions.begin(); iter != unions.end(); iter++)
    {
        current_down_move_count = 0;
        v_size = iter->second.size();

        for (int i = 0; i < v_size; i++)
        {
            if (iter->second[i]->y == current_col)
            {
                if (rand() % 2)
                {
                    cells[current_col + 1][iter->second[i]->x].union_code = cells[current_col][iter->second[i]->x].union_code;

                    cells[current_col + 1][iter->second[i]->x].up = true;
                    cells[current_col][iter->second[i]->x].down = true;

                    unions[iter->first].push_back(&cells[current_col + 1][iter->second[i]->x]);

                    current_down_move_count++;
                }
            }
        }

        if (current_down_move_count == 0 && v_size != 0)
        {
            int random_value = rand() % v_size;
            int x = iter->second[random_value]->x;

            cells[current_col + 1][x].union_code = cells[current_col][x].union_code;

            cells[current_col + 1][x].up = true;
            cells[current_col][x].down = true;

            unions[iter->first].push_back(&cells[current_col + 1][x]);
        }
    }
}

void print_code()
{
    for (int i = 0; i < input; i++)
    {
        for (int j = 0; j < input; j++)
        {
            {
                printf("%3d", cells[i][j].union_code);
            }
        }
        cout << endl;
    }

    cout << endl;
}

void print_maze()
{
    for (int i = 0; i < input * 2 + 1; i++)
    {
        for (int j = 0; j < input * 2 + 1; j++)
        {
            dest[i][j] = 1;
        }
    }
    for (int i = 0; i < input; i++)
    {
        for (int j = 0; j < input; j++)
        {
            dest[i * 2 + 1][j * 2 + 1] = 0;
            if (cells[i][j].up)
                dest[i * 2][j * 2 + 1] = 0;
            if (cells[i][j].down)
                dest[i * 2 + 2][j * 2 + 1] = 0;
            if (cells[i][j].left)
                dest[i * 2 + 1][j * 2] = 0;
            if (cells[i][j].right)
                dest[i * 2 + 1][j * 2 + 2] = 0;
        }
    }
    for (int i = 0; i < input * 2 + 1; i++)
    {
        for (int j = 0; j < input * 2 + 1; j++)
        {
            if (dest[i][j] > 0)
            {
                cout << "■" << ' ';
            }
            else
                cout << "  ";
        }
        cout << endl;
    }
}

int main()
{
    cin >> input;

    // srand(time(NULL));

    for (int i = 0; i < input - 1; i++)
    {
        set_row();
        random_union();
        down_cell();

        current_col++;

        print_maze();
        print_code();
    }

    return 0;
}