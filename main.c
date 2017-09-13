#include <stdio.h>
#include <spl/cslib.h>
#include <spl/gwindow.h>
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/stack.h>

#define WIDTH 400
#define HEIGHT 400
#define CELLWIDTH 50
#define ROWS (HEIGHT/CELLWIDTH)
#define COLS (WIDTH/CELLWIDTH)


//struct for holding info about each cell
struct cell {
    int i;
    int j;
    bool walls[4];
    bool visited;
};
// new type
typedef struct cell cell;
// greating an array of cells
cell grid[ROWS * COLS];
// creating the panel
GWindow panel;
// creating current
cell *current;
// function to inti the grid

int get_index(int i, int j) {
    if (i < 0 || j < 0 || i > ROWS - 1 || j > COLS - 1)
        return -1;
    else
        return j + i * COLS;
}

void init_grid(cell grid[ROWS * COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cell temp = {i, j, {true, true, true, true}, false};
            grid[get_index(i, j)] = temp;
        }
    }
}

void draw_cell(cell *cell) {
    int x = cell->j * CELLWIDTH;
    int y = cell->i * CELLWIDTH;

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
    /*if (cell->visited) {
        fillRect(panel, x, y, CELLWIDTH, CELLWIDTH);
    }*/

}

void draw_current_cell(cell *current) {
    int x = current->j * CELLWIDTH;
    int y = current->i * CELLWIDTH;
    GRect temp = newGRect(x, y, CELLWIDTH, CELLWIDTH);
    setFillColor(temp, "GREEN");
    setFilled(temp, true);
    add(panel, temp);
}

int get_a_neighbor(cell *cell) {
    int picked;
    bool arr[4]= {false, false, false, false};
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
        if(arr[i]==true)
            break;
    }
    if (i ==4)
        return -1;
    picked = rand()%4;
    while(!arr[picked]){
        picked = rand()%4;
    }
    return arrvalues[picked];

}

void remove_walls(cell* current, cell* next){
    int x = current->i - next->i;
    if(x==1){
        current->walls[0] = false;
        next->walls[2] = false;
    } else if(x==-1){
        current->walls[2]= false;
        next->walls[0] = false;
    }
    int y = current->j - next->j;
    if(y==1){
        current->walls[3] = false;
        next->walls[1]= false;

    } else if(y==-1){
        current->walls[1] = false;
        next->walls[3] = false;
    }
}
int main() {
    srand(23);
    Stack local = newStack();
    panel = newGWindow(WIDTH, HEIGHT);
    init_grid(grid);
    for (int i = 0; i < COLS * ROWS; ++i) {
        draw_cell(&grid[i]);
        //pause(1);
    }
    draw_current_cell(&grid[0]);
    grid[0].visited = true;
    current = &grid[0];
    while (1) {
        int c;
        if ((c = get_a_neighbor(current)) != -1) {
            // STEP 1 mark as viited
            grid[c].visited = true;

            push(local, current);
            //STEP 3 remove walls
            remove_walls(current, &grid[c]);
            // sTEP 4
            current = &grid[c];
        }
        else if(!isEmpty(local)){
            current = pop(local);
        }
        if(isEmpty(local))
            break;
        draw_cell(current);
    }

}