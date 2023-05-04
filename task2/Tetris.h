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

extern Matrix *deleteFullLines(Matrix *screen, Matrix *blk, int top, int dw); //extern선언 : colortet쪽에서도 이게 필요해서 호출하기 위해 붙여줌
//extern int *allocArrayScreen(int dy, int dx, int dw);
//extern void deallocArrayScreen(int *array);

class Tetris {
protected: // 자식이 상속받고 쓸 수 있도록 protedted로 선언해줌 (v4)와 차이점
    // static members
    static Matrix ***setOfBlockObjects;
    static int numTypes;
    static int numDegrees;
    static int wallDepth;

    // dynamic members
    int rows; // rows of screen = dy + wallDepth
    int cols; // columns of screen = dx + 2*wallDepth
    int type;
    int degree;
    int top; 
    int left;

    TetrisState state;
    Matrix *iScreen;
    Matrix *oScreen;
    Matrix *currBlk;

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