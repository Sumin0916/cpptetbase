#include "Tetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** Tetris::setOfBlockObjects = NULL;
int Tetris::numTypes = 0;
int Tetris::numDegrees = 0;
int Tetris::wallDepth = 0;

void Tetris::init(int **setOfBlockArrays, int nTypes, int nDegrees) {
  if (setOfBlockObjects != NULL) // already allocated?
    deinit();

  numTypes = nTypes;
  numDegrees = nDegrees;

  setOfBlockObjects = new Matrix**[numTypes]; // allocate 1d array of pointers to Matrix pointer arrays

  for (int t = 0; t < numTypes; t++)
    setOfBlockObjects[t] = new Matrix*[numDegrees]; // allocate 1d array of Matrix pointers

  for (int t = 0; t < numTypes; t++) {
    int *array = setOfBlockArrays[numDegrees * t];
    int idx, size;
    for (idx = 0; array[idx] != -1 ; idx++); // find the element of -1 in array[] / 원소의 개수 카운트
    for (size = 0; size*size < idx; size++); // comupte the square root of idx / 루트를 씌운다(수학함수 쓰기 싫어서.. 이렇게) 숫자 싫어! 하드코딩 실렁
    wallDepth = (size > wallDepth ? size : wallDepth); 
    for (int d = 0; d < numDegrees; d++) { // allocate matrix objects
      setOfBlockObjects[t][d] = new Matrix(setOfBlockArrays[numDegrees * t + d], size, size);
      //cout << *setOfBlockObjects[t][d] << endl;
    }
  }
}

void Tetris::deinit(void) {
  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

void Tetris::deallocArrayScreen(int *array1d) {
  delete[] array1d;
}

int *Tetris::allocArrayScreen(int dy, int dx, int dw) {
  int **array2d = new int*[dy + dw];
  int y, x;

  for (y = 0; y < dy+dw; y++) // alloc array2d
    array2d[y] = new int[dx + 2*dw];
  
  for (y = 0; y < dy+dw; y++) {
    for (x = 0; x < dw; x++) {
      array2d[y][x] = 1; // left wall
      array2d[y][dw+dx+x] = 1; // right wall
    }
    if (y < dy) continue;

    for (x = 0; x < dx; x++)
      array2d[y][dw+x] = 1; // bottom ground
  }

  int *array1d = new int[(dy+dw)*(dx+2*dw)]; // alloc array2d
  for (y = 0; y < dy+dw; y++)
    for (x = 0; x < dx+2*dw; x++)
      array1d[(dx+2*dw)*y + x] = array2d[y][x]; // copy 2d to 1d

  for (int y = 0; y < dy+dw; y++) // dealloc array2d
    delete [] array2d[y];
  delete [] array2d;

  return array1d;  
}

Tetris::Tetris(int cy, int cx) {
  rows = cy + wallDepth; 
  cols = cx + 2*wallDepth; 
  type = -1; // unknown as of now
  degree = 0;
  top = 0;
  left = wallDepth + cols/2 - wallDepth/2; // wallDepth equals the size of the largest block.

  int *arrayScreen = allocArrayScreen(cy, cx, wallDepth);
  iScreen = new Matrix(arrayScreen, rows, cols);
  oScreen = new Matrix(iScreen);
  currBlk = NULL;
  state = TetrisState::NewBlock;

  //cout << *iScreen << endl; 디버깅용 코드
  deallocArrayScreen(arrayScreen);
}

Tetris::~Tetris() {
  delete iScreen;
  delete oScreen;
}

/// non-member functions
Matrix *deleteFullLines(Matrix *screen, Matrix *blk, int top, int dw) {
  Matrix *line, *zero, *temp;
  int cy, y;
  int nDeleted, nScanned;
  int ws_dy = screen->get_dy() - dw;
  int ws_dx = screen->get_dx() - 2*dw;

  if (top + blk->get_dy() > ws_dy)
    nScanned = ws_dy - top;
  else
    nScanned = blk->get_dy();
  
  zero = new Matrix(1, ws_dx);
  for (y = nScanned - 1, nDeleted = 0; y >= 0; y--) {
    cy = top + y + nDeleted;
    line = screen->clip(cy, dw, cy+1, dw + ws_dx);
    if (line->sum() == ws_dx) {
      temp = screen->clip(0, dw, cy, dw + ws_dx);
      screen->paste(temp, 1, dw);
      screen->paste(zero, 0, dw);
      nDeleted++;
      delete temp;
    }
    delete line; 
  }
  delete zero;
  return screen;
}

/// mutators
TetrisState Tetris::accept(char key) {

  if (state == TetrisState::Finished)
    return state;

  else if (state == TetrisState::NewBlock) {
  
    int idx = key - '0';
    if (idx < 0 || idx >= numTypes) {
      cout << "Tetris::accept: wrong block index!" << endl;
      return state = TetrisState::NewBlock;
    }

    state = TetrisState::Running;

    // select a new block
    type = idx;
    degree = 0;
    top = 0; 
    left = cols/2 - wallDepth/2;

    // init variables for screen refresh with the new block
    currBlk = setOfBlockObjects[type][degree];
    Matrix *tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    Matrix *tempBlk2 = tempBlk->add(currBlk);
    delete tempBlk;

    // update oScreen before conflict test
    oScreen->paste(iScreen, 0, 0);
    oScreen->paste(tempBlk2, top, left);
    if (tempBlk2->anyGreaterThan(1)) // exit the game
      state = TetrisState::Finished;
    delete tempBlk2;

    return state; // = Running or Finished
  }
  else if (state == TetrisState::Running) {

    state = TetrisState::Running;
    bool touchDown = false;
    Matrix *tempBlk, *tempBlk2;

    switch (key) { // perform the requested action
      case 'a': left--; break;
      case 'd': left++; break;
      case 'w': 
        degree = (degree + 1) % numDegrees; 
        currBlk = setOfBlockObjects[type][degree]; 
        break;
      case 's': top++; break;
      case ' ': 
        while (true) {
          top++;
          tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
          tempBlk2 = tempBlk->add(currBlk);
          delete tempBlk;
          if (tempBlk2->anyGreaterThan(1)) {
            delete tempBlk2;
            break;
          }
          delete tempBlk2;
        }
        break;
      default: cout << "Tetris::accept: wrong key input" << endl;
    }

    tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
    tempBlk2 = tempBlk->add(currBlk);
    delete tempBlk;
    if (tempBlk2->anyGreaterThan(1)) {
      switch (key) { // undo the requested action
        case 'a': left++; break;
        case 'd': left--; break;
        case 'w': 
          degree = (degree + 3) % numDegrees; 
          currBlk = setOfBlockObjects[type][degree]; 
          break;
        case 's': top--; touchDown = true; break;
        case ' ': top--; touchDown = true; break;
      }
      delete tempBlk2;
      tempBlk = iScreen->clip(top, left, top + currBlk->get_dy(), left + currBlk->get_dx());
      tempBlk2 = tempBlk->add(currBlk);    
      delete tempBlk;
    }

    // update oScreen
    oScreen->paste(iScreen, 0, 0);
    oScreen->paste(tempBlk2, top, left);
    delete tempBlk2;

    if (touchDown) {
      oScreen = deleteFullLines(oScreen, currBlk, top, wallDepth);
      iScreen->paste(oScreen, 0, 0);
      state = TetrisState::NewBlock;
    }

    return state; // = Running or NewBlock
  }
  
  return state; // not reachable
}
