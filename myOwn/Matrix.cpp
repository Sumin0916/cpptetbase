#include "Matrix.h"

int Matrix::nAlloc = 0;
int Matrix::nFree = 0;

int Matrix::get_nFree() { return nFree; }

int Matrix::get_nAlloc() { return nAlloc; }

int Matrix::get_dy() const { return dy; }

int Matrix::get_dx() const { return dx; }

int **Matrix::get_array() const { return array; }

void Matrix::alloc(int cy, int cx){
    if ((cy <= 0) || (cx <= 0)) {
        dy = 0;
        dx = 0;
        array = nullptr;
        nAlloc++;
        return;
    }
    dy = cy;
    dx = cx;
    array = new int *[dy];
    for (int y = 0; y < dy; y++)
        array[y] = new int[dx];
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            array[y][x] = 0;
    nAlloc++;
    return;
}

void dealloc() {
    if (array != nullptr) {
        for (int y = 0; y < dy; y++)
            delete[] array[y];
        delete[] array;
        array = nullptr;
    }
    nFree++;
}

Matrix::~Matrix() { dealloc(); }

Matrix::Matrix() { alloc(0, 0); }

Matrix::Matrix(int cy, int cx) { alloc(cy, cx); }

Matrix::Matrix(const Matrix *obj) {
    alloc(obj->dy, obj->dx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            array[y][x] = obj->array[y][x];
}

Matrix::Matrix(const Matrix &obj) {
    alloc(obj.dy, obj.dx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            arraty[y][x] = obj.array[y][x];
}

Matrix::Matrix(int *arr, int col, int row) {
    alloc(col, row);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            array[y][x] = arr[y*dx + x];
}

Matrix *Matrix::clip(int top, int left, int bottom, int right) {
    int cy = bottom - top;
    int cx = right - left;
    Matrix *temp = new Matrix(cy, cx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            if ((top + y >= 0) && (top + y < dy) && 
                (left + x >= 0) && (left + x < dx))
                temp->array[y][x] = array[top + y][left + x];
            else {
                cerr << "Invalid matrix range";
                delete temp;
                return nullptr;
            }
    return temp;
}

Matrix *Matrix::clip_(int top, int left, int bottom, int right) {
    int cy = bottom - top;
    int cx = right - left;
    Matrix temp = Matrix(cy, cx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            if ((top + y >= 0) && (top + y < dy) && (left + x >= 0) && (left + x < dx))
                temp.array[y][x] = array[top + y][left + x];
            else {
                cerr << "Invalid matrix range";
                return Matrix();
            }
    return temp;
}

void Matrix::paste(const Matrix *obj, int top, int left) {
    for (int y = 0; y < obj->dy; y++)
        for (int x = 0; x < obj->dx; x++) {
            int diff_y = top + y; int diff_x = left + x;
            if ((diff_y >= 0) && (diff_y < dy) && (diff_x >= 0) && (diff_x < dx))
                array[diff_y][diff_x] = obj->array[y][x];
        }
    return;
}

void Matrix::paste(const Matrix &obj, int top, int left) {
    for (int y = 0; y < obj.dy; y++)
        for (int x = 0; x < obj.dx; x++) {
            int diff_y = top + y; int diff_x = left + x;
            if ((diff_y >= 0) && (diff_y < dy) && (diff_x >= 0) && (diff_x < dx))
                array[diff_y][diff_x] = obj.array[y][x];
        }
    return;
}

Matrix *Matrix::add(const Matirx *obj) {
    if ((dy != obj->dy) || (dx != obj->dx)) return NULL;
    Matrix *temp = new Matrix(dy, dx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            temp->array[y][x] = array[y][x] + obj->array[y][x];
    return temp;
}

Matrix *Matrix::add(const Matirx &obj) {
    if ((dy != obj.dy) || (dx != obj.dx)) return NULL;
    Matrix *temp = new Matrix(dy, dx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            temp->array[y][x] = array[y][x] + obj.array[y][x];
    return temp;
}

const Matrix Matrix::operator+(const Matrix &obj) const {
    if ((dy != obj.dy) || (dx != obj.dx)) return Matrix();
    Matrix temp = Matrix(dy, dx);
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            temp.array[y][x] = array[y][x] + obj.array[y][x];
    return temp;
}

int Matrix::sum() {
    int res = 0;
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            res += array[y][x];
    return res;
}

void Matrix::mulc(int coef) {
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            array[y][x] *= coef;
    return;
}

int *Matrix::int2bool() {
    Matrix *temp = new Matrix(dy, dx);
    int **t_array = temp->get_array();
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            t_array[y][x] = (array[y][x] != 0 ? 1 : 0);
    return temp;
}

bool Matrix::anyGreaterThan(int val) {
    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            if (array[y][x] > val) return true;
    return false;
}

void Matrix::print() {
    cout<< "Matrix(" << dy << ',' << dx << ')' <<'\n';
    for (int y = 0; y < dy; y++) {
        for (int x = 0; x < dx; x++) {
            cout<<array[y][x]<<' ';
        }
    cout<<'\n';
    }
}

ostream& operator<<(ostream& out, const Matrix& obj) {
    out << "Matrix(" << obj.dy << ',' << obj.dx << ')' << '\n';
    for (int y = 0; y < obj.dy; y++) {
        for (int x = 0; x < obj.dx; x++) {
            out << obj.array[y][x] << ' ';
        }
        out << '\n';
    }
    out << '\n';
    return out;
}

Matrix&  Matrix::operator=(const Matrix &obj) {
    if (this == &obj) { return *this; }
    if ((dy != obj.dy) || (dx != obj.dx)) {
        if (this != nullptr) dealloc();
        alloc(obj.dy, obj.dx);
    }

    for (int y = 0; y < dy; y++)
        for (int x = 0; x < dx; x++)
            array[y][x] = obj.array[y][x];

    return *this;
}