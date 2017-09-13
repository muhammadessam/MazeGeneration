#include <stdio.h>
#include <spl/cslib.h>
#include <spl/gwindow.h>
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/stack.h>

#define WIDTH 400
#define HEIGHT 400
#define CELLWIDTH 20
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
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
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
        add(panel, linetop);
    } else {
        removeGWindow(panel, linetop);
    }
    if (cell->walls[1]) {
        add(panel, lineright);
    } else {
        removeGWindow(panel, lineright);
    }
    if (cell->walls[2]) {
        add(panel, linedown);
    } else {
        removeGWindow(panel, linedown);
    }
    if (cell->walls[3]) {
        add(panel, lineleft);
    } else {
        removeGWindow(panel, lineleft);
    }
    if(cell->visited){
        setColor(linetop, "RED");
        setColor(lineright, "RED");
        setColor(linedown, "RED");
        setColor(lineleft, "RED");
    }
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

int main() {
    srand(23);
    panel = newGWindow(WIDTH, HEIGHT);
    init_grid(grid);
    for (int i = 0; i < COLS * ROWS; ++i) {
        draw_cell(&grid[i]);
    }
    draw_current_cell(&grid[0]);
    grid[0].visited = true;
    current = &grid[0];
    while (1) {
        int c;
        draw_cell(current);
        if ((c = get_a_neighbor(current)) != -1) {
            grid[c].visited = true;
            current = &grid[c];
        }
        draw_current_cell(current);
    }

}