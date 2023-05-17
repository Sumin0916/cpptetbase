#pragma once
#include <iostream>
#include <cstdlib>
#include "Matrix.h"

using namespace std;

#define MAX_TETRIS_OPERATIONS 100

enum class TetrisState {
    NewBlock,
    Running,
    Finished,
};

extern Matrix *deleteFullLines(Matrix *screen, Matrix *blk, int top, int dw);
extern bool anyConflict(Matrix *tempBlk);
//extern int *allocArrayScreen(int dy, int dx, int dw);
//extern void deallocArrayScreen(int *array);

class Tetris; // 이렇게 해줘야 한다. 실제 선언은 뒤에 있다. run에서 미리 tetris를 쓰기 떄문에 이렇게 시그니처를 적는다.

class ActionHandler { //이음새를 정의하는 클래스 body가 없다 순수 가상 함수다 =0으로 정의 얘는 추상클래스라 객채를 못 만든다. 결국 얘를 상속받아서 재정의를 해야한다.
public:
    virtual void run(Tetris *t, char key) = 0;
};


class TetrisOperation {
public:
    char key;
    ActionHandler *hAction; // 상속받아서 자식 객채들이 만들어질 것인데 그 자식 객체들이 등록이 될 거라는 뜻이다 그냥 액션핸들러는 객채생성을 하지 못한다!
    ActionHandler *hCounterAction;
    TetrisState preState;
    TetrisState postAState;
    TetrisState postCState;
    TetrisOperation(char ch, TetrisState s0, ActionHandler *h1,
        TetrisState s1, ActionHandler *h2, TetrisState s2) {
        key = ch;
        hAction = h1;
        hCounterAction = h2;
        preState = s0;
        postAState = s1;
        postCState = s2;
    }
    virtual ~TetrisOperation() { //여기서 virtual를 빼먹었다 교수님은
        delete hAction;
        delete hCounterAction;  
    }
};

class Tetris {
protected:
    // TetrisOperation related variables
    static int nOps;
    static TetrisOperation *operations[MAX_TETRIS_OPERATIONS]; //객체 포인터 배열
    static int findOpIdxByKey(char key);
    static void setDefaultOperations(void);

public:
    static void setOperation(char key, TetrisState s0, ActionHandler *h1,
			   TetrisState s1, ActionHandler *h2, TetrisState s2);

    // Tetris game related variables
    static Matrix ***setOfBlockObjects;
    static int numTypes;
    static int numDegrees;
    static int wallDepth;

    int rows; // rows of screen = dy + 2*wallDepth
    int cols; // columns of screen = dx + 2*wallDepth
    int type;
    int degree;
    int top; 
    int left;

    TetrisState state;
    Matrix *iScreen;
    Matrix *oScreen;
    Matrix *currBlk;

    static void init(int **setOfBlockArrays, int nTypes, int nDegrees);
    static void deinit(void);
    Tetris(int cy, int cx);
    ~Tetris();

    // accessors
    static int get_wallDepth(void)  {  return wallDepth; }
    static int get_numTypes(void)  {  return numTypes; }    
    Matrix *get_oScreen(void) const {  return oScreen; }
    Matrix *overlap_currBlk(void);

    // mutators
    void update_oScreen(Matrix *tempBlk, int y, int x);
    TetrisState accept(char key);
};

//////////////////////////////////////////////////////////////////
/// Examples of ActionHandlers
//////////////////////////////////////////////////////////////////

class OnLeft : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->left = t->left - 1;
        return;
    }
};

class OnRight : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->left = t->left + 1;
        return;
    }
};

class OnDown : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->top = t->top + 1;
        return;
    }
};

class OnUp : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->top = t->top - 1;
        return;
    }
};

class OnDrop : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        Matrix *tempBlk;
        while (true) { 
            t->top = t->top + 1; 
            tempBlk = t->overlap_currBlk();
            if (anyConflict(tempBlk) == true) {
                delete tempBlk;
                break;
            }
            delete tempBlk;
        } 
        return;
    }
};

class OnClockWise : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->degree = (t->degree + 1) % t->numDegrees;
        t->currBlk = t->setOfBlockObjects[t->type][t->degree];
        return;
    }    
};

class OnCounterClockWise : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        t->degree = (t->degree + 3) % t->numDegrees;
        t->currBlk = t->setOfBlockObjects[t->type][t->degree];
        return;
    }    
};

class OnNewBlock : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        if (t->currBlk != NULL) // why test currBlk != NULL?
            t->oScreen = deleteFullLines(t->oScreen, t->currBlk, t->top, t->wallDepth);
        t->iScreen->paste(t->oScreen, 0, 0);
        // select a new block
        t->type = key - '0';
        t->degree = 0;
        t->top = t->wallDepth; 
        t->left = t->cols/2 - t->wallDepth/2;
        t->currBlk = t->setOfBlockObjects[t->type][t->degree];
        return;
    }    
};

class OnFinished : public ActionHandler {
public:
    void run(Tetris *t, char key) {
        cout << "OnFinished.run() called" << endl;
        return;
    }    
};
