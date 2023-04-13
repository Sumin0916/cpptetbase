#include <iostream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <termios.h>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#include "colors.h"
#include "Matrix.h"

using namespace std;


/**************************************************************/
/**************** Linux System Functions **********************/
/**************************************************************/

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
const int MAX_BLK_TYPES = 7;
const int MAX_BLK_DEGREES = 4;

int TRow[MAX_BLK_TYPES] = {2, 3, 3, 3, 3, 3, 4};
int TCol[MAX_BLK_TYPES] = {2, 3, 3, 3, 3, 3, 4};

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

  for (int y = 0; y < dy - dw + 1; y++) {
    for (int x = dw - 1; x < dx - dw + 1; x++) {
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
    cout << '\n';
  }
}

/**************************************************************/
/******************** Tetris Main Loop ************************/
/**************************************************************/

const int SCREEN_DY = 10;
const int SCREEN_DX = 10;
const int SCREEN_DW = 4;

const int ARRAY_DY = SCREEN_DY + SCREEN_DW;
const int ARRAY_DX = SCREEN_DX + 2*SCREEN_DW;

int arrayScreen[ARRAY_DY][ARRAY_DX] = {
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },  
};

int arrayBlk[3][3] = {
  { 0, 1, 0 },
  { 1, 1, 1 },
  { 0, 0, 0 },
};

void deleteFullLines(Matrix *screen)
{
  for (int count = 0; count < SCREEN_DY; count++){
    const int idxDepth = SCREEN_DY-count-1;
    Matrix *blkLine = screen->clip(idxDepth, SCREEN_DW, idxDepth+1, SCREEN_DX+SCREEN_DW);
    if (blkLine->sum() == SCREEN_DX){
      Matrix *upcleaned = new Matrix(idxDepth+1, SCREEN_DX);
      Matrix *upRemain = screen->clip(0, SCREEN_DW, idxDepth, SCREEN_DX+SCREEN_DW);
      screen->paste(upcleaned, 0, SCREEN_DW);
      screen->paste(upRemain, 1, SCREEN_DW);
      delete upcleaned;  
      delete upRemain;
      count = -1;
    }
    delete blkLine;
  }
}

int main(int argc, char *argv[]) {
  srand((unsigned int)time(NULL));
  char key;
  int top = 0, left = ARRAY_DX/2-2;
  int idxBlockDegree = 0;
  int blkType = rand() % MAX_BLK_TYPES;

  Matrix *setOfBlockObjects[MAX_BLK_TYPES][MAX_BLK_DEGREES];
  Matrix *typeOfBlks[MAX_BLK_TYPES];
  for (int i = 0; i < MAX_BLK_TYPES; i++)for(int j = 0; j < MAX_BLK_DEGREES; j++){setOfBlockObjects[i][j] = new Matrix((int *) setOfBlockArrays[i*MAX_BLK_DEGREES+j], TCol[i], TRow[i]);}
  for (int i = 0; i < MAX_BLK_TYPES; i++){typeOfBlks[i] = new Matrix(setOfBlockObjects[i][0]);}

  Matrix *iScreen = new Matrix((int *) arrayScreen, ARRAY_DY, ARRAY_DX);
  Matrix *currBlk = new Matrix(typeOfBlks[blkType]);
  Matrix *tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
  Matrix *tempBlk2 = tempBlk->add(currBlk);
  delete tempBlk;

  Matrix *oScreen = new Matrix(iScreen);
  oScreen->paste(tempBlk2, top, left);
  delete tempBlk2;
  drawScreen(oScreen, SCREEN_DW);
  delete oScreen;

  while ((key = getch()) != 'q') {
    switch (key) {
      case 'a': left--; break;
      case 'd': left++; break;
      case 's': top++; break;
      case 'w':
      delete currBlk;
      idxBlockDegree = (idxBlockDegree + 1) % MAX_BLK_DEGREES;
      currBlk = new Matrix(setOfBlockObjects[blkType][idxBlockDegree]);
      break;
      case ' ':
      while(true){
        top++;
        tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
        tempBlk2 = tempBlk->add(currBlk);
        delete tempBlk;
        if (tempBlk2->anyGreaterThan(1)){delete tempBlk2;break;}delete tempBlk2;
      }
      break;
      default: cout << "wrong key input" << '\n';
    }
    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    tempBlk2 = tempBlk->add(currBlk);
    delete tempBlk;
    if (tempBlk2->anyGreaterThan(1)) {
      delete tempBlk2;
      switch (key) {
          case 'a': left++; break;
          case 'd': left--; break;
          case 'w':
          idxBlockDegree = (idxBlockDegree+3) % MAX_BLK_DEGREES;
          delete currBlk;
          currBlk = new Matrix(setOfBlockObjects[blkType][idxBlockDegree]);
          break;
          case 's':
          case ' ':
          top--;
          tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
          tempBlk2 = tempBlk->add(currBlk);
          iScreen->paste(tempBlk2, top, left);
          delete tempBlk;delete tempBlk2;
          deleteFullLines(iScreen);
          blkType = rand() % MAX_BLK_TYPES;
          idxBlockDegree = 0;
          delete currBlk;
          currBlk = new Matrix(typeOfBlks[blkType]);
          top = 0, left = ARRAY_DX/2-2;
          break;
      }
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);
      delete tempBlk;
    }
    if(tempBlk2->anyGreaterThan(1)){
      cout<<"========================Game Over!!========================"<<'\n';
      delete tempBlk2;
      break;
    }
    oScreen = new Matrix(iScreen);
    oScreen->paste(tempBlk2, top, left);
    delete tempBlk2;
    drawScreen(oScreen, SCREEN_DW);
    cout<<'\n';
    delete oScreen;
  }

  delete iScreen;
  delete currBlk;
  for (int i = 0; i < MAX_BLK_TYPES; i++){delete typeOfBlks[i];}
  for (int i = 0; i < MAX_BLK_TYPES; i++)for(int j = 0; j < MAX_BLK_DEGREES; j++){delete setOfBlockObjects[i][j];}
  cout << "(nAlloc, nFree) = (" << Matrix::get_nAlloc() << ',' << Matrix::get_nFree() << ")" << '\n';  
  cout << "Program terminated!" << endl;

  return 0;
}
