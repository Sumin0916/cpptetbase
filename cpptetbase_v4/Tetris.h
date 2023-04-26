#pragma once
#include <iostream>
#include <cstdlib>
#include "Matrix.h"

using namespace std;

enum class TetrisState {
    NewBlock,
    Running,
    Finished,
};

class Tetris {
private:
    // static members
    static Matrix ***setOfBlockObjects;
    static int numTypes;
    static int numDegrees;
    static int wallDepth;

    // dynamic members
    int rows; // rows of screen = dy + wallDepth
    int cols; // rows of columns = dx + 2*wallDepth

    int type;
    int degree;
    int top; 
    int left;

    TetrisState state;

    Matrix *iScreen;
    Matrix *oScreen;
    Matrix *currBlk;
    
    int *allocArrayScreen(int dy, int dx, int dw);
    void deallocArrayScreen(int *array);

public:
    static void init(int **setOfBlockArrays, int nTypes, int nDegrees);
    static void deinit(void);
    Tetris(int cy, int cx);
    ~Tetris();

    // accessors
    static int get_wallDepth(void)  {  return wallDepth; }
    static int get_numTypes(void)  {  return numTypes; }    
    Matrix *get_oScreen(void) const {  return oScreen; }

    // mutators
    TetrisState accept(char key);
};