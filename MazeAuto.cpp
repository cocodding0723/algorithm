#include <iostream>
#include <vector>
#include <map>
#include <time.h>

using namespace std;

struct Cell
{
    int x, y;
    int group;
    bool up = true;
    bool right = true;
    bool left = true;
    bool down = true;
};

struct GroupInfo{
    vector<Cell> members;
    int branch_count = 0;
};

int n;
Cell cells[25][25] = {0};
int dest[60][60];
map<int, GroupInfo> groups;
map<int, GroupInfo>::iterator iter;
int current_col = 0;
int group_count = 1;

void set_row()
{
    // groups.clear();

    for (int i = 0; i < n; i++)
    {
        cells[current_col][i].x = i;
        cells[current_col][i].y = current_col;

        if (cells[current_col][i].group == 0)
        {
            cells[current_col][i].group = group_count;

            if (groups.find(group_count) == groups.end()){
                groups.insert(make_pair(group_count, GroupInfo()));
            }
            groups[group_count].members.push_back(cells[current_col][i]);

            group_count++;
        }
        else{
            if (groups.find(cells[current_col][i].group) == groups.end()){
                groups.insert(make_pair(cells[current_col][i].group, GroupInfo()));
            }
            groups[cells[current_col][i].group].members.push_back(cells[current_col][i]);
        }
    }
}

void rand_union()
{
    int current_group = -1;
    int prev_group = -1;

    for (int i = 0; i < n - 1; i++)
    {
        if (rand() % 2 == 0 && cells[current_col][i].group != cells[current_col][i + 1].group)        // 50% 확률로 옆 블럭과 joint
        {
            current_group = cells[current_col][i].group;
            prev_group = cells[current_col][i + 1].group;

            // cells[current_col][i + 1].group = current_group;
            cells[current_col][i + 1].left = false;
            cells[current_col][i].right = false;

            groups[current_group].members.insert(groups[current_group].members.begin(), groups[prev_group].members.begin(), groups[prev_group].members.end());

            groups.erase(prev_group);
        }
    }
}

void down_cell()
{
    int current_cell_index = -1;
    int rnd_select = -1;
    int rnd = -1;

    for (iter = groups.begin(); iter != groups.end(); iter++)
    {
        if (iter->second.members.size() != 0)
        {
            for (int i = 0; i < iter->second.members.size(); i++){
                
                if (rand() % 2 == 0 || iter->second.members.size() == 1)        // 50% 확률로 아래 블럭과 joint, 혹은 맴버 크기가 1개일 경우 아래블럭과 joint
                {
                    current_cell_index = iter->second.members[i].x;
                    // cells[current_col + 1][current_cell_index].group = cells[current_col][current_cell_index].group;
                    cells[current_col + 1][current_cell_index].up = false;
                    cells[current_col][current_cell_index].down = false;

                    groups[cells[current_col][current_cell_index].group].members.push_back(cells[current_col + 1][current_cell_index]);

                    iter->second.branch_count++;
                }
            }

            if (iter->second.branch_count == 0){
                current_cell_index = iter->second.members[rand() % iter->second.members.size()].x;
                // cells[current_col + 1][current_cell_index].group = cells[current_col][current_cell_index].group;
                cells[current_col + 1][current_cell_index].up = false;
                cells[current_col][current_cell_index].down = false;

                groups[cells[current_col][current_cell_index].group].members.push_back(cells[current_col + 1][current_cell_index]);
            }   
        }
    }

    current_col++;
}

void print_maze()
{
    for (int i = 0; i < n * 2 + 1; i++){
        for (int j = 0; j < n * 2 + 1; j++){
            dest[i][j] = 1;
        }
    }
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            dest[i * 2 + 1][j * 2 + 1] = 0;
            if (!cells[i][j].up) dest[i * 2][j * 2 + 1] = 0;
            if (!cells[i][j].down) dest[i * 2 + 2][j * 2 + 1] = 0;
            if (!cells[i][j].left) dest[i * 2 + 1][j * 2] = 0;
            if (!cells[i][j].right) dest[i * 2 + 1][j * 2 + 2] = 0;
        }
    }
    for (int i = 0; i < n * 2 + 1; i++){
        for (int j = 0; j < n * 2 + 1; j++){
            if (dest[i][j] > 0){
                cout << "■" << ' ';
            }
            else 
                cout << "  ";
        }
        cout << endl;
    }

    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (cells[i][j].group > 0){
                printf("%3d", cells[i][j].group);
            }
            else{
                cout << "  ";
            }
        }
        cout << endl;
    }
}

void set_last_line(){
    set_row();

    for (int i = 0; i < n - 1; i++){
        if (cells[current_col][i].group != cells[current_col][i + 1].group){
            cells[current_col][i].right = false;
            cells[current_col][i + 1].left = false;
        }
    }
}

void regroup(){
     for (iter = groups.begin(); iter != groups.end(); iter++){
        for (int i = 0; i < iter->second.members.size(); i++){
            iter->second.members[i].group = iter->first;
        }
     }
}

int main()
{
    cin >> n;

    srand(time(NULL));

    for (int i = 0; i < n - 1; i++){
        set_row();
        rand_union();
        down_cell();
        regroup();
        print_maze();
    }
    set_last_line();
    print_maze();

    return 0;
}