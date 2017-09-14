#include <stdio.h>
#include <spl/cslib.h>
#include <spl/gwindow.h>
#include <spl/gobjects.h>
#include <spl/stack.h>

#define WIDTH 400
#define HEIGHT 400
#define CELLWIDTH 40
#define ROWS 10//(HEIGHT/CELLWIDTH)
#define COLS 10//(WIDTH/CELLWIDTH)


//struct for holding info about each cell
struct cell {
    int i;
    int j;
    bool walls[4];
    bool visited;
    bool deadend;
};
// new type
typedef struct cell cell;
// greating an array of cells
cell grid[ROWS * COLS];
// creating the panel
GWindow panel;
// creating current
cell *current;
Stack movedCells;
Stack moves;

int get_index(int i, int j) {
    if (i < 0 || j < 0 || i > ROWS - 1 || j > COLS - 1)
        return -1;
    else
        return j + i * COLS;
}

void init_grid(cell grid[ROWS * COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cell temp = {i, j, {true, true, true, true}, false, false};
            grid[get_index(i, j)] = temp;
        }
    }
}

void draw_cell(cell *cell) {
    int x = cell->j * CELLWIDTH;
    int y = cell->i * CELLWIDTH;

    if (cell->visited) {
        //fillRect(panel, x, y, CELLWIDTH, CELLWIDTH);
        GRect temp = newGRect(x, y, CELLWIDTH, CELLWIDTH);
        setFillColor(temp, "WHITE");
        setFilled(temp, true);
        add(panel, temp);
    }
    GLine linetop = newGLine(x, y, x + CELLWIDTH, y);
    GLine lineright = newGLine(x + CELLWIDTH, y, x + CELLWIDTH, y + CELLWIDTH);
    GLine linedown = newGLine(x + CELLWIDTH, y + CELLWIDTH, x, y + CELLWIDTH);
    GLine lineleft = newGLine(x, y + CELLWIDTH, x, y);

    if (cell->walls[0]) {
        setColor(linetop, "BLACK");
        add(panel, linetop);
    } else {
        setColor(linetop, "WHITE");
        add(panel, linetop);
    }
    if (cell->walls[1]) {
        setColor(lineright, "BLACK");
        add(panel, lineright);
    } else {
        setColor(lineright, "WHITE");
        add(panel, lineright);
    }
    if (cell->walls[2]) {
        setColor(linedown, "BLACK");
        add(panel, linedown);
    } else {
        setColor(linedown, "WHITE");
        add(panel, linedown);
    }
    if (cell->walls[3]) {
        setColor(lineleft, "BLACK");
        add(panel, lineleft);
    } else {
        setColor(lineleft, "WHITE");
        add(panel, lineleft);
    }


}

void draw_current_cell(cell *current) {
    int x = current->j * CELLWIDTH;
    int y = current->i * CELLWIDTH;
    /*GRect temp = newGRect(x, y, CELLWIDTH, CELLWIDTH);
    setColorGObject(temp, "RED");
    add(panel,temp);*/
    GRect temp = newGRect(x, y, CELLWIDTH, CELLWIDTH);
    setFillColor(temp, "RED");
    setFilled(temp, true);
    add(panel, temp);
}

int get_a_neighbor(cell *cell) {
    int picked;
    bool arr[4] = {false, false, false, false};
    int top = get_index(cell->i - 1, cell->j);
    int right = get_index(cell->i, cell->j + 1);
    int down = get_index(cell->i + 1, cell->j);
    int left = get_index(cell->i, cell->j - 1);
    int arrvalues[4] = {top, right, down, left};
    if ((top != -1))
        if (!grid[top].visited)
            arr[0] = true;
    if ((right != -1))
        if (!grid[right].visited)
            arr[1] = true;
    if ((down != -1))
        if (!grid[down].visited)
            arr[2] = true;
    if ((left != -1))
        if (!grid[left].visited)
            arr[3] = true;
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (arr[i] == true)
            break;
    }
    if (i == 4)
        return -1;
    picked = rand() % 4;
    while (!arr[picked]) {
        picked = rand() % 4;
    }
    return arrvalues[picked];

}

void remove_walls(cell *current, cell *next) {
    int x = current->i - next->i;
    if (x == 1) {
        current->walls[0] = false;
        next->walls[2] = false;
    } else if (x == -1) {
        current->walls[2] = false;
        next->walls[0] = false;
    }
    int y = current->j - next->j;
    if (y == 1) {
        current->walls[3] = false;
        next->walls[1] = false;

    } else if (y == -1) {
        current->walls[1] = false;
        next->walls[3] = false;
    }
}


///////////////////////////////////////////////////////////////////////////////
////////////////////////*solving the maze now*////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int move_up(int i, int j) {
    printf("moving top\n");
    return get_index(i - 1, j);
}

int move_right(int i, int j) {
    printf("moving right\n");
    return get_index(i, j + 1);
}

int move_down(int i, int j) {
    printf("moving down\n");
    return get_index(i + 1, j);
}

int move_left(int i, int j) {
    printf("moving left\n");
    return get_index(i, j - 1);
}

int get_up_index(int i, int j) {

    return get_index(i - 1, j);
}

int get_right_index(int i, int j) {
    return get_index(i, j + 1);
}

int get_down_index(int i, int j) {
    return get_index(i + 1, j);
}

int get_left_index(int i, int j) {
    return get_index(i, j - 1);
}

bool is_full_closed(cell *cell1) {
    bool top = false, right = false, down = false, left = false;
    if (get_up_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[0]) && (grid[get_up_index(cell1->i, cell1->j)].walls[2])) {
            top = true;
        }
    } else {
        top = true;
    }
    if (get_right_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[1]) && (grid[get_right_index(cell1->i, cell1->j)].walls[3])) {
            right = true;
        }
    } else {
        right = true;
    }
    if (get_down_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[2]) && (grid[get_down_index(cell1->i, cell1->j)].walls[0])) {
            down = true;
        }
    } else {
        down = true;
    }
    if (get_left_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[3]) && (grid[get_left_index(cell1->i, cell1->j)].walls[1])) {
            left = true;
        }
    } else {
        left = true;
    }

    if(top && left && right && down)
        return true;
    else
        return false;

}

int is_half_closed(cell *cell1) {
    bool top = false, right = false, down = false, left = false;
    if (get_up_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[0]) && (grid[get_up_index(cell1->i, cell1->j)].walls[2])) {
            top = true;
        }
    } else {
        top = true;
    }
    if (get_right_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[1]) && (grid[get_right_index(cell1->i, cell1->j)].walls[3])) {
            right = true;
        }
    } else {
        right = true;
    }
    if (get_down_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[2]) && (grid[get_down_index(cell1->i, cell1->j)].walls[0])) {
            down = true;
        }
    } else {
        down = true;
    }
    if (get_left_index(cell1->i, cell1->j) != -1) {
        if ((cell1->walls[3]) && (grid[get_left_index(cell1->i, cell1->j)].walls[1])) {
            left = true;
        }
    } else {
        left = true;
    }
    if(!top){
        return 0;
    } else if(!right){
        return 1;
    } else if(!down){
        return 2;
    } else if(!left){
        return 3;
    } else{
        return -1;
    }
}


cell *move_agent(cell *cell1) {
    if (get_index(cell1->i, cell1->j) == ((COLS * ROWS) - 1)) {
        printf("you have reached\n");
        return NULL;
    }
    cell1->visited = true;
    int number_of_opened_walls = 0;
    bool trap = false;
    int move_from = 0;
    for (int i = 0; i < 4; i++) {
        if (!cell1->walls[i])
            number_of_opened_walls++;
    }
    if (number_of_opened_walls == 1) {
        ///////////* getting which wall is opened*//////////////////////////////
        for (int i = 0; i < 4; i++) {
            if (!cell1->walls[i]) {
                move_from = i;
                break;
            }
        }
        ////////////////////////////////* setting the move *////////////////////////
        if (move_from == 0) {
            if (!grid[get_up_index(cell1->i, cell1->j)].visited) {
                push(movedCells, cell1);
                push(moves, "top");
                cell1->walls[move_from] = true;
                grid[get_up_index(cell1->i, cell1->j)].walls[2] = true;
                return (&grid[move_up(cell1->i, cell1->j)]);
            } else {
                trap = true;
            }
        } else if (move_from == 1) {
            if (!grid[get_right_index(cell1->i, cell1->j)].visited) {
                push(movedCells, cell1);
                push(moves, "right");
                cell1->walls[move_from] = true;
                grid[get_right_index(cell1->i, cell1->j)].walls[3] = true;
                return (&grid[move_right(cell1->i, cell1->j)]);
            } else {
                trap = true;
            }
        } else if (move_from == 2) {
            if (!grid[get_down_index(cell1->i, cell1->j)].visited) {
                push(movedCells, cell1);
                push(moves, "down");
                cell1->walls[move_from] = true;
                grid[get_down_index(cell1->i, cell1->j)].walls[0] = true;
                return (&grid[move_down(cell1->i, cell1->j)]);
            } else {
                trap = true;
            }
        } else if (move_from == 3) {
            if (!grid[get_left_index(cell1->i, cell1->j)].visited) {
                push(movedCells, cell1);
                push(moves, "left");
                cell1->walls[move_from] = true;
                grid[get_left_index(cell1->i, cell1->j)].walls[1] = true;
                return (&grid[move_left(cell1->i, cell1->j)]);
            } else {
                trap = true;
            }
        }
    } else if (number_of_opened_walls == 2) {
        push(movedCells, cell1);
        int picked = rand() % 4;
        while (cell1->walls[picked])
            picked = rand() % 4;
        cell1->walls[picked] = true;
        current = cell1;
    } else {
        int halfClosedindex = is_half_closed(cell1);
        if (halfClosedindex != -1) {
            cell1->walls[halfClosedindex] = false;
        }
        if (is_full_closed(cell1)) {
            cell1->deadend = true;
            string temp = pop(moves);
            if (temp == "top") {
                printf("moving down\n");
            } else if (temp == "right") {
                printf("moving left\n");
            } else if (temp == "down") {
                printf("moving top\n");
            } else if (temp == "left") {
                printf("moving right\n");
            }
        }
        int x = cell1->j*CELLWIDTH;
        int y = cell1->i*CELLWIDTH;
        GOval tempRect = newGOval(x+10, y+10,CELLWIDTH-20, CELLWIDTH-20 );
        setFillColor(tempRect, "RED");
        setFilled(tempRect, true);
        add(panel, tempRect);
        current = pop(movedCells);
    }

}

void draw_for_solution(cell* temp){
    int x = temp->j*CELLWIDTH;
    int y = temp->i*CELLWIDTH;
    if(!temp->visited){
        GOval tempRect = newGOval(x+10, y+10,CELLWIDTH-20, CELLWIDTH-20 );
        setFillColor(tempRect, "GREEN");
        setFilled(tempRect, true);
        add(panel, tempRect);
    } else{
        GOval tempRect = newGOval(x+10, y+10,CELLWIDTH-20, CELLWIDTH-20 );
        setFillColor(tempRect, "WHITE");
        setFilled(tempRect, true);
        add(panel, tempRect);
    }
    if(get_index(temp->i, temp->j)==(COLS*ROWS-1)){
        GOval tempRect = newGOval(x+10, y+10,CELLWIDTH-20, CELLWIDTH-20 );
        setFillColor(tempRect, "BLACK");
        setFilled(tempRect, true);
        add(panel, tempRect);
    }
    if(get_index(temp->i, temp->j)==(0)){
        GOval tempRect = newGOval(x+10, y+10,CELLWIDTH-20, CELLWIDTH-20 );
        setFillColor(tempRect, "WHITE");
        setFilled(tempRect, true);
        add(panel, tempRect);
    }

}

int main() {
    srand(8);
    current = NULL;
    Stack local = newStack();
    movedCells = newStack();
    moves = newStack();
    panel = newGWindow(WIDTH, HEIGHT);
    init_grid(grid);
    for (int i = 0; i < COLS * ROWS; ++i) {
        draw_cell(&grid[i]);
        //pause(1);
    }
    grid[0].visited = true;
    current = &grid[0];
    push(local, current);
    while (!isEmpty(local)) {
        int c;
        if ((c = get_a_neighbor(current)) != -1) {
            // STEP 1 mark as viited
            grid[c].visited = true;

            push(local, current);
            //STEP 3 remove walls
            remove_walls(current, &grid[c]);
            // sTEP 4
            current = &grid[c];
        } else if (!isEmpty(local)) {
            current = pop(local);
        }

        draw_current_cell(current);
        //pause(2);
        draw_cell(current);

    }
    // pause(1000);
    //closeGWindow(panel);



    //////////////////* remarking alt the cells as topunvisited again *////////////////

    for (int j = 0; j < ROWS * COLS; ++j) {
        grid[j].visited = false;
    }


    srand(2);
    current = &grid[0];
    while (1) {
        draw_for_solution(current);
        cell* temp = move_agent(current);
        if(temp==NULL)
            break;
        else
            current = temp;
        pause(12);


    }


    return 0;
}




































/*  thinking not working good*/////
/*
int move_from_1_walls(cell cell1){
    int i = 0;
    for (i = 0; i < 4; i++) {
        if (!cell1.walls[i]) {
            break;
        }
    }
    if (i == 0) {
        {
            return move_up(cell1.i, cell1.j);
        }
    } else if (i == 1) {
        return move_right(cell1.i, cell1.j);
    } else if (i == 2) {
        return move_down(cell1.i, cell1.j);
    } else if (i == 3) {
        return move_left(cell1.i, cell1.j);
    }

}

int move_from_2_walls(cell cell1){
    int firstWall = 0;
    int secondWall = 0;
    for (int i = 0; i < 4; i++) {
        if (!cell1.walls[i]) {
            firstWall = i;
            break;
        }
    }
    for (int i = 0; i < 4; ++i) {
        if ((!(cell1.walls[i])) && (firstWall < i)) {
            secondWall = i;
            break;
        }
    }
    if (firstWall == 0) {
        if (!grid[get_up_index(cell1.i, cell1.j)].visited) {
            return move_up(cell1.i, cell1.j);
        } else {
            if (secondWall == 1) {
                return move_right(cell1.i, cell1.j);
            } else if (secondWall == 2) {
                return move_down(cell1.i, cell1.j);
            } else if (secondWall == 3) {
                return move_left(cell1.i, cell1.j);
            }
        }
    } else if (firstWall == 1) {
        if (!grid[get_right_index(cell1.i, cell1.j)].visited) {
            return move_right(cell1.i, cell1.j);
        } else {
            if (secondWall == 0) {
                return move_up(cell1.i, cell1.j);
            } else if (secondWall == 2) {
                return move_down(cell1.i, cell1.j);
            } else if (secondWall == 3) {
                return move_left(cell1.i, cell1.j);
            }
        }
    } else if (firstWall == 2) {
        if (!grid[get_down_index(cell1.i, cell1.j)].visited) {
            return move_down(cell1.i, cell1.j);
        } else {
            if (secondWall == 0) {
                return move_up(cell1.i, cell1.j);
            } else if (secondWall == 1) {
                return move_right(cell1.i, cell1.j);
            } else if (secondWall == 3) {
                return move_left(cell1.i, cell1.j);
            }
        }} else if (firstWall == 3) {
        if (!grid[get_left_index(cell1.i, cell1.j)].visited) {
            return move_left(cell1.i, cell1.j);
        } else {
            if (secondWall == 0) {
                return move_up(cell1.i, cell1.j);
            } else if (secondWall == 1) {
                return move_right(cell1.i, cell1.j);
            } else if (secondWall == 2) {
                return move_right(cell1.i, cell1.j);
            }
        }
    }
}

int move_from_3_walls(cell cell1){
    int first_wall = 0;
    int secod_wall = 0;
    int third_wall = 0;
    for (int i = 0; i <4 ; i++) {
        if(!cell1.walls[i]){
            first_wall=i;
            break;
        }
    }
    for (int i = 0; i <4; i++) {
        if ((!(cell1.walls[i])) && (first_wall < i)) {
            secod_wall = i;
            break;
        }
    }
    for (int i = 0; i < 4; ++i) {
        if ((!(cell1.walls[i])) && (secod_wall < i)) {
            third_wall = i;
            break;
        }
    }




}

int move_agent(cell *cell1) {
    int counter = 0;
    if(get_index(cell1->i, cell1->j)==(COLS*ROWS-1))
    {
        printf("Your reached your dis");
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        if (!cell1->walls[i])
            counter++;
    }
    if (counter == 1) {
        move_from_1_walls(*cell1);
    } else if (counter == 2) {
        move_from_2_walls(*cell1);

    }

}

*/
