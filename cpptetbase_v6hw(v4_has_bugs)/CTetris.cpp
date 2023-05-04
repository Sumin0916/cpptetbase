#include "CTetris.h"

using namespace std;

///**************************************************************
/// static member variables and functions
///**************************************************************

Matrix *** CTetris::setOfColorBlockObjects = NULL;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) { //deinit을 참고해서 구현, or 부모의 init와 구조 비슷
  if (setOfColorBlockObjects != NULL) // already allocated?
    deinit();

  Tetris::init(setOfColorBlockArrays, nTypes, nDegrees); // call superclass' function 이 구조는 유지해야한다.

  // write the rest of this function!!
}

void CTetris::deinit(void) {
  Tetris::deinit(); // call superclass' function

  for (int t = 0; t < numTypes; t++)
    for (int d = 0; d < numDegrees; d++)
      delete setOfColorBlockObjects[t][d]; // deallocate matrix objects

  for (int t = 0; t < numTypes; t++)
    delete [] setOfColorBlockObjects[t]; // deallocate 1d array of Matrix pointers

  delete []  setOfColorBlockObjects; // deallocate 1d array of pointers to Matrix pointer arrays

  setOfColorBlockObjects = NULL;
}

///**************************************************************
/// dynamic member variables and functions
///**************************************************************

/// constructors and destructor

CTetris::CTetris(int cy, int cx) : Tetris(cy, cx) {
  iCScreen = new Matrix(iScreen);
  oCScreen = new Matrix(oScreen);
  currCBlk = NULL;
}

CTetris::~CTetris() {
  delete iCScreen;
  delete oCScreen;
}

/// mutators
TetrisState CTetris::accept(char key) {

  TetrisState _state = Tetris::accept(key); // call superclass' function 이것도 구조를 유지해야한다.
  // 웬만하면 부보 클래스한테 다 떠넘길 수 있으면 좋은거다.
  // 충돌테스트는 어디에 ?
  // 무식한 방법: 자식의 accept를 거창하게 작성한다. 부모의 accept를 안 부르게 된다. 색상값에 대해서 충돌 테스트를 어떻게 할지?
  // 부모의 accept를 최대한 사용해야 한다.
  // write the rest of this function!!

  // you can use the following code if you want to
  // oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);

  return _state;
}
