﻿#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>
#include <vector>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "colors.h"
#include "Matrix.h"
#include "Tetris.h"

using namespace std;


/********************************************************/
/**************** Linux System Functions *********************/
/********************************************************/

char saved_key = 0;
int tty_raw(int fd);	/* put terminal into a raw mode */
int tty_reset(int fd);	/* restore terminal's mode */
  
/* Read 1 character - echo defines echo mode */
char getch() {
  char ch;
  int n;
  while (1) {
    tty_raw(0);
    n = read(0, &ch, 1);
    tty_reset(0);
    if (n > 0)
      break;
    else if (n < 0) {
      if (errno == EINTR) {
        if (saved_key != 0) {
          ch = saved_key;
          saved_key = 0;
          break;
        }
      }
    }
  }
  return ch;
}

void sigint_handler(int signo) {
  // cout << "SIGINT received!" << endl;
  // do nothing;
}

void sigalrm_handler(int signo) {
  alarm(1);
  saved_key = 's';
}

void registerInterrupt() {
  struct sigaction act, oact;
  act.sa_handler = sigint_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGINT, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
}

void registerAlarm() {
  struct sigaction act, oact;
  act.sa_handler = sigalrm_handler;
  sigemptyset(&act.sa_mask);
#ifdef SA_INTERRUPT
  act.sa_flags = SA_INTERRUPT;
#else
  act.sa_flags = 0;
#endif
  if (sigaction(SIGALRM, &act, &oact) < 0) {
    cerr << "sigaction error" << endl;
    exit(1);
  }
  alarm(1);
}

/**************************************************************/
/**************** Tetris Blocks Definitions *******************/
/**************************************************************/
#define MAX_BLK_TYPES 7
#define MAX_BLK_DEGREES 4
#define ROW 0
#define COL 1
#define endl '\n'


int T0D0[] = { 1, 1, 1, 1, -1 };
int T0D1[] = { 1, 1, 1, 1, -1 };
int T0D2[] = { 1, 1, 1, 1, -1 };
int T0D3[] = { 1, 1, 1, 1, -1 };

int T1D0[] = { 0, 1, 0, 1, 1, 1, 0, 0, 0, -1 };
int T1D1[] = { 0, 1, 0, 0, 1, 1, 0, 1, 0, -1 };
int T1D2[] = { 0, 0, 0, 1, 1, 1, 0, 1, 0, -1 };
int T1D3[] = { 0, 1, 0, 1, 1, 0, 0, 1, 0, -1 };

int T2D0[] = { 1, 0, 0, 1, 1, 1, 0, 0, 0, -1 };
int T2D1[] = { 0, 1, 1, 0, 1, 0, 0, 1, 0, -1 };
int T2D2[] = { 0, 0, 0, 1, 1, 1, 0, 0, 1, -1 };
int T2D3[] = { 0, 1, 0, 0, 1, 0, 1, 1, 0, -1 };

int T3D0[] = { 0, 0, 1, 1, 1, 1, 0, 0, 0, -1 };
int T3D1[] = { 0, 1, 0, 0, 1, 0, 0, 1, 1, -1 };
int T3D2[] = { 0, 0, 0, 1, 1, 1, 1, 0, 0, -1 };
int T3D3[] = { 1, 1, 0, 0, 1, 0, 0, 1, 0, -1 };

int T4D0[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D1[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };
int T4D2[] = { 0, 1, 0, 1, 1, 0, 1, 0, 0, -1 };
int T4D3[] = { 1, 1, 0, 0, 1, 1, 0, 0, 0, -1 };

int T5D0[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D1[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };
int T5D2[] = { 0, 1, 0, 0, 1, 1, 0, 0, 1, -1 };
int T5D3[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, -1 };

int T6D0[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D1[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
int T6D2[] = { 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, -1 };
int T6D3[] = { 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, -1 };
  
int *setOfBlockArrays[] = {
  T0D0, T0D1, T0D2, T0D3,
  T1D0, T1D1, T1D2, T1D3,
  T2D0, T2D1, T2D2, T2D3,
  T3D0, T3D1, T3D2, T3D3,
  T4D0, T4D1, T4D2, T4D3,
  T5D0, T5D1, T5D2, T5D3,
  T6D0, T6D1, T6D2, T6D3,
};

void drawScreen(Matrix *screen, int wall_depth)
{
  int dy = screen->get_dy();
  int dx = screen->get_dx();
  int dw = wall_depth;
  int **array = screen->get_array();

  for (int x = 0; x < dx - 2*dw; x++) {
    string digit = to_string(x) + " ";
    cout << digit;
  }
  cout << '/' << endl;

  for (int y = dw; y < dy - dw; y++) {
    for (int x = dw; x < dx - dw; x++) {
      if (array[y][x] == 0)
	      cout << "□ ";
      else if (array[y][x] == 1)
	      cout << "■ ";
      else if (array[y][x] == 10)
	      cout << "◈ ";
      else if (array[y][x] == 20)
	      cout << "★ ";
      else if (array[y][x] == 30)
	      cout << "● ";
      else if (array[y][x] == 40)
	      cout << "◆ ";
      else if (array[y][x] == 50)
	      cout << "▲ ";
      else if (array[y][x] == 60)
	      cout << "♣ ";
      else if (array[y][x] == 70)
	      cout << "♥ ";
      else
	      cout << "X ";
    }
    string digit = to_string(y-dw) + " ";
    cout << digit << endl;
  }
}
  
/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

Matrix *myDeleteFullLines(Matrix *screen, Matrix *blk, int top, int left, int dw) {
	Matrix *rowZero, *colZero, *rowLine, *colLine;
  vector<pair<int, int>> deleteLineNum;
  int colLenth = screen->get_dx() - (2*dw); int rowLenth = screen->get_dy() - (2*dw);
	rowZero = new Matrix(1, colLenth); colZero = new Matrix(rowLenth, 1);

  for (int i = 0; i < dw; i++) {
    rowLine = new Matrix(); colLine = new Matrix();
    int rowInd = top + i; int colInd = left + i;
    if (dw <= rowInd && rowInd < rowLenth + dw){delete rowLine; rowLine = screen->clip(rowInd, dw, rowInd+1, dw + colLenth);}
    if (dw <= colInd && colInd < colLenth + dw){delete colLine; colLine = screen->clip(dw, colInd, dw + rowLenth, colInd+1);}
    if (rowLine->sum() == colLenth) deleteLineNum.push_back(make_pair(ROW, rowInd-dw));
    if (colLine->sum() == rowLenth) deleteLineNum.push_back(make_pair(COL, colInd-dw));
    delete rowLine; delete colLine;
  }

	for (auto lineInfo: deleteLineNum) {
		int lineType = lineInfo.first; int lineInd = lineInfo.second;
		if (lineType == ROW) screen->paste(rowZero, dw+lineInd, dw);
		else if (lineType == COL) screen->paste(colZero, dw, dw+lineInd);
	}

	delete rowZero; delete colZero;
	return screen;
}

class myOnUp : public ActionHandler {
	public:
    	void run(Tetris *t, char key) {
        t->top--;
			  if (t->top < 0) t->top++;
    	  return;
    	}
};

class myOnLeft : public ActionHandler {
	public:
    	void run(Tetris *t, char key) {
        t->left--;
			  if (t->left < 0) t->left++;
    	  return;
    	}
};

class myOnRight : public ActionHandler {
	public:
    	void run(Tetris *t, char key) {
        t->left++;
			  if (t->left + t->currBlk->get_dx() > t->cols) t->left--;
    	  return;
    	}
};

class myOnDown : public ActionHandler {
	public:
    	void run(Tetris *t, char key) {
        t->top++;
			  if (t->top + t->currBlk->get_dy() > t->rows) t->top--;
    	  return;
    	}
};

class onPass : public ActionHandler {
	public:
    	void run(Tetris *t, char key) {return;}
};

class myOnNewBlock : public ActionHandler {
	public:
		void run(Tetris *t, char key) {
			if (t->currBlk != NULL)
            t->oScreen = myDeleteFullLines(t->oScreen, t->currBlk, t->top, t->left, t->wallDepth);
			t->iScreen->paste(t->oScreen, 0, 0);
			t->type = key - '0';
			t->degree = 0;
			t->top = t->wallDepth; 
			t->left = t->cols/2 - t->wallDepth/2;
			t->currBlk = t->setOfBlockObjects[t->type][t->degree];
      		return; 
		}
};

int main(int argc, char *argv[]) {  
  char key;
  srand((unsigned int)time(NULL));
  
  TetrisState state;
  
  /////////////////////////////////////////////////////////////////////////
  Tetris::setOperation('a', TetrisState::Running,  new myOnLeft(),      TetrisState::Running,  new onPass(),      TetrisState::Running);
  Tetris::setOperation('d', TetrisState::Running,  new myOnRight(),     TetrisState::Running,  new onPass(),      TetrisState::Running);
  Tetris::setOperation('s', TetrisState::Running,  new myOnDown(),      TetrisState::Running,  new onPass(),      TetrisState::Running);
  Tetris::setOperation('e', TetrisState::Running,  new myOnUp(), 	      TetrisState::Running,  new onPass(),      TetrisState::Running);
  Tetris::setOperation(' ', TetrisState::Running,  new onPass(),        TetrisState::NewBlock, new onPass(), 	    TetrisState::Running);
  Tetris::setOperation('1', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('2', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('3', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('4', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('5', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('6', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('0', TetrisState::NewBlock, new myOnNewBlock(),  TetrisState::Running,  new OnFinished(),  TetrisState::Finished);
  Tetris::setOperation('w', TetrisState::Running,  new OnClockWise(),   TetrisState::Running,  new onPass(),  	  TetrisState::Running);
  /////////////////////////////////////////////////////////////////////////

  Tetris::init(setOfBlockArrays, MAX_BLK_TYPES, MAX_BLK_DEGREES);
  Tetris *board = new Tetris(10, 10);
  key = (char) ('0' + rand() % board->get_numTypes());
  board->accept(key);
  drawScreen(board->get_oScreen(), board->get_wallDepth()); cout << endl;

  while ((key = getch()) != 'q') {
    state = board->accept(key);
    drawScreen(board->get_oScreen(), board->get_wallDepth()); cout << endl;
    if (state == TetrisState::NewBlock) {
      key = (char) ('0' + rand() % board->get_numTypes());
      state = board->accept(key);
      drawScreen(board->get_oScreen(), board->get_wallDepth()); cout << endl;
      if (state == TetrisState::Finished) 
        break;
    }
  }

  delete board;

  Tetris::deinit();
  cout << "(nAlloc, nFree) = (" << Matrix::get_nAlloc() << ',' << Matrix::get_nFree() << ")" << endl;  
  cout << "Program terminated!" << endl;
  return 0;
}
