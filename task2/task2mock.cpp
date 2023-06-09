#include "CTetris.h"

TetrisState CTetris::accept(char key) {
    Matrix *tempBlk, *tempBlk2;

    TetrisState _state = Tetris::accept(key);

    currCBlk = setOfColorBlockObjects[type][degree];

    tempBlk = iCScreen->clip(top, left, top+currCBlk->get_dy(), left + currCBlk->get_dx());
    tempBlk2 = tempBlk->add(currCBlk);
    delete tempBlk;

    oCScreen->paste(iCScreen, 0, 0);
    oCScreen->paste(tempBlk2, top, left);
    delete tempBlk2;

    if (_state == TetrisState::NewBlock) {
        oCScreen = deleteFullLines(oCScreen, currCBlk, top, wallDepth);
        iCScreen->paste(oCScreen, 0, 0);
    }
    return _state;
}

Matrix *** CTetris::setOfColorBlockObjects = nullptr;

void CTetris::init(int **setOfColorBlockArrays, int nTypes, int nDegrees) {
    if (setOfBlockObjects != nullptr) deinit();

    Tetris::init(setOfColorBlockArrays, nTypes, nDegrees);

    setOfColorBlockObjects = new Matrix**[numTypes];

    for (int t = 0; t < numTypes; t++)
        setOfColorBlockObjects[t] = new Matrix* [numDegrees];

    for (int t = 0; t < numTypes; t++) {
        int size = setOfBlockObjects[t][0]->get_dy();
        for (int d = 0; d < numDegrees; d++) {
            setOfColorBlockObjects[t][d] = new Matrix(setOfColorBlockArrays[numTypes*t + d], size, size);
        }
    }
}

