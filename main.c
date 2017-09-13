#include <stdio.h>
#include <spl/cslib.h>
#include <spl/gwindow.h>
#include <spl/gevents.h>
#include <spl/gobjects.h>

#define WIDTH 400
#define HEIGHT 400
#define CELLWIDTH 40
#define ROWS (HEIGHT/CELLWIDTH)
#define COLS (WIDTH/CELLWIDTH)


//struct for holding info about each cell
struct cell {
    int i;
    int j;
    int walls[4];
};
// new type
typedef struct cell cell;
// greating an array of cells
cell grid[ROWS * COLS];
// creating the panel
GWindow panel;


// function to inti the grid
void init_grid(cell grid[ROWS * COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            cell temp = {i, j, {1, 1, 1, 1}};
            grid[j + i * COLS] = temp;
        }
    }
}

void draw_cell(cell cell) {
    int x = cell.j * CELLWIDTH;
    int y = cell.i * CELLWIDTH;

    GLine linetop = newGLine(x, y, x + CELLWIDTH, y);
    GLine lineright = newGLine(x + CELLWIDTH, y, x + CELLWIDTH, y + CELLWIDTH);
    GLine linedown = newGLine(x + CELLWIDTH, y + CELLWIDTH, x, y + CELLWIDTH);
    GLine lineleft = newGLine(x, y + CELLWIDTH, x, y);
    if (cell.walls[0]) {
        add(panel, linetop);
    } else {
        removeGWindow(panel, linetop);
    }
    if (cell.walls[1]) {
        add(panel, lineright);
    } else {
        removeGWindow(panel, lineright);
    }
    if (cell.walls[2]) {
        add(panel, linedown);
    } else {
        removeGWindow(panel, linedown);
    }
    if (cell.walls[3]) {
        add(panel, lineleft);
    } else {
        removeGWindow(panel, lineleft);

    }
}



int main() {
    panel = newGWindow(WIDTH, HEIGHT);
    init_grid(grid);
    for (int i = 0; i < COLS * ROWS; ++i) {
        draw_cell(grid[i]);
    }

}