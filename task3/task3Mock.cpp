#pragma once
#include <iostream>
#include <cstdlib>
#include "Matrix.h"
#include<vector>

vector<int> *getFullLines(Matrix *screen, Matrix *blk, int top, int dw) {
    vector<int> *p_vector = new vector<int>;

    Matrix *line, *bline, *zero, *temp;
    int cy, y;
    int nDeleted, nScanned;
    int ws_dy = screen->get_dy() - 2*dw;
    int ws_dx = screen->get_dx() - 2*dw;

    if (top + blk->get_dy() > ws_dy + dw) nScanned = ws_dy + dw - top;
    else nScanned = blk->get_dy();

    for (y = nScanned - 1; nDeleted = 0; y >= 0; y--) {
        cy = top + y;
        if (cy < dw || cy  >= dw + ws_dy) continue;
        line = screen->clip(cy, dw, cy+1, dw+ws_dx);
        bline = line->int2bool();
        delete line;

        if (bline->sum() == ws_dx) {
            p_vector->push_back(cy);
            nDeleted++;
        }
        delete bline;
    }
    return p_vector;
}

Matrix *myDeleteFullLines(Matrix *screen, Matrix *blk, int top, int left, int dw) {
    vector<int> *y_indices;
    vector<int> *x_indices;
    int ws_dy = screen->get_dy() - 2*dw;
    int ws_dx = screen->get_dx() - 2*dw;
    Matrix *h_zero = new Matrix(1, ws_dx);
    Matrix *v_zero = new Matrix(ws_dy, 1);

    y_indices = getFullLines(screen, blk, top, dw);
    Matrix *t_screen = transpose(screen);
    Matrix *t_blk = transpose(blk);
    x_indices = getFullLines(t_screen, t_blk, left, dw);

    cout << "y_indices = ";
    for (int n = 0; n < y_indices->size(); n++) {
        int cy = (*y_indices)[n];
        screen->paste(h_zero, cy, dw);
        cout << cy <<  ',';
    }
    cout << endl;

    cout << "x_indices = ";
    for (int n = 0; n < x_indices->size(); n++) {
        int cx = (*x_indices)[n];
        screen->paste(v_zero, dw, cx);
        cout << cx << ',';
    }
    cout << endl;

    delete y_indices;
    delete x_indices;
    delete h_zero;
    delete v_zero;
    delete t_screen;
    delete t_blk;

    return screen;
}

Tetris::setOperation('a', TetrisState::Running, new MyOnLeft(), TetrisState::Running, new MyDummy(), TetrisState::Running);