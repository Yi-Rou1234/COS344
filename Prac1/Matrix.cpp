#include "Matrix.h"
#include "MathExceptions.h"
#include <cmath>
/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
MATRIX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*/
SquareMatrix submatrix(const SquareMatrix& matrix, int row, int col);

void custom_swap(double* a,double* b);
int determ(double** arr, int n);

Matrix::Matrix(int row, int col){
    if (row <=0 || col<=0){
        throw MathExceptions::InvalidMatrixSize;
    }
    n = row;
    m = col;
    arr = new double*[n];
    for(int i=0; i<n; ++i){
        arr[i] = new double[m];
    }
    for(int i=0; i<n; ++i){
        for (int j=0; j<m;++j){
            arr[i][j] = 0.0;
        }
    }
}

Matrix::Matrix(int row, int col, double** elements){
    n=row;
    m=col;
    arr = new double*[n];
    for (int i = 0; i < n; ++i) {
        arr[i] = new double[m];
        for (int j = 0; j < m; ++j) {
            arr[i][j] = elements[i][j];
        }
    }
}

Matrix::Matrix(const Matrix &num){
    n=num.n;
    m=num.m;
    arr= new double*[n];
    for(int i=0;i<n;++i){
        arr[i] = new double[m];
        for (int j=0; j<m;++j){
            arr[i][j] = num.arr[i][j];
        }
    }
}

Matrix::~Matrix(){
    if (arr != nullptr) {
        for(int i = 0; i < n; ++i){
            delete[] arr[i];
        }
        delete[] arr;
        arr = nullptr; 
    }
}


/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
SQUARE MATRIX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*/

SquareMatrix::SquareMatrix(int index) : Matrix(index, index){
    if(index <=0){
        throw MathExceptions::InvalidMatrixSize;
    }
}

SquareMatrix::SquareMatrix(int size, double** elements) : Matrix(size, size, elements) {
    /*nothing*/
}

SquareMatrix::~SquareMatrix() {
    // No need to implement anything here, the destructor of the base class Matrix will be invoked automatically
}

int Matrix::getN() const{
    return n;
}

int Matrix::getM() const{
    return m;
}


IdentityMatrix::IdentityMatrix(int size) : SquareMatrix(size){
    for (int i = 0; i< size; ++i){
        for (int m = 0 ; m <size; ++m){
            if(i==m)
                (*this)[i][m] = 1.0;
            else
                (*this)[i][m] = 0.0;
        }
    }
}

IdentityMatrix::~IdentityMatrix() {
}

/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX OPERATIONS XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*/

Matrix Matrix::operator*(const Matrix second) const {
    if (m != second.getN()) {
        throw MathExceptions::InvalidMatrixMultiplication;
    }
    Matrix mul(n, second.getM());
        
        for(int i=0; i<n; ++i){
            for (int k=0; k<second.getM(); ++k){
                // mul[i][k] =0.0;
                for (int p=0; p<second.getN();++p){
                    mul[i][k] += arr[i][p] *second[p][k];
                }
            }
        }
        return mul;
}

Matrix Matrix::operator*(const double scalar) const{
    Matrix result(n,m);
    for (int row=0; row<n; ++row){
        for (int col=0; col<m; ++col){
            result[row][col] = arr[row][col] * scalar;
        }
    }
    return result;
}

Matrix Matrix::operator+(const Matrix mat)const{
    if(n != mat.n || m!= mat.m){
        throw MathExceptions::InvalidMatrixAddition;
    }
    Matrix result(n,m);
    for(int row=0; row<n; ++row){
        for (int col=0; col<m; ++col){
            result[row][col] = arr[row][col] +mat.arr[row][col];
        }
    }
    return result;
}

Matrix Matrix::operator~() const{
    Matrix trans(m,n);
    for(int row=0; row<n; ++row){
        for (int col=0;col<m ; ++col){
            trans[col][row]=arr[row][col];
        }
    }
    return trans;
}

SquareMatrix SquareMatrix::operator!() const {
    double det = determinant();
        if (det == 0) {
            std::cout<<"Matrix is singular, cannot find inverse."<<std::endl;
        }

        double scalar = 1 / det;
        SquareMatrix augmented(n);
        // Initialize augmented matrix with cofactors
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                double cofactor = pow(-1, i + j) * submatrix(*this, i, j).determinant();
                augmented[j][i] = cofactor;
            }
        }

        // Multiply by scalar to get inverse
        SquareMatrix inverse(n);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                inverse[i][j] = augmented[i][j] * scalar;
            }
        }
        return inverse;
}

//global function
SquareMatrix submatrix(const SquareMatrix& matrix, int row, int col) {
    int size = matrix.getN() - 1;
    SquareMatrix sub(size);
    int subRow = 0;
    for (int i = 0; i < matrix.getN(); ++i) {
        if (i == row) continue;
        int subCol = 0;
        for (int j = 0; j < matrix.getN(); ++j) {
            if (j == col) continue;
            sub[subRow][subCol] = matrix[i][j];
            ++subCol;
        }
        ++subRow;
    }
    return sub;
}

Vector SquareMatrix::solve(const Vector t) const {
    if (n <=0 || m<=0) {
        throw MathExceptions::InvalidVectorSize;
    }

    Matrix augmentedMatrix(n, n + 1);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = arr[i][j];
        }
        augmentedMatrix[i][n] = t[i];
    }

    for (int i = 0; i < n; ++i) {
        int maxRowIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (std::abs(augmentedMatrix[j][i]) > std::abs(augmentedMatrix[maxRowIndex][i])) {
                maxRowIndex = j;
            }
        }
        if (augmentedMatrix[maxRowIndex][i] == 0) {
            throw MathExceptions::InvalidMatrixSize;
        }

        if (maxRowIndex != i) {
            std::swap(augmentedMatrix[i], augmentedMatrix[maxRowIndex]);
        }

        for (int j = i + 1; j < n; ++j) {
            double factor = augmentedMatrix[j][i] / augmentedMatrix[i][i];
            for (int k = i; k <= n; ++k) {
                augmentedMatrix[j][k] -= factor * augmentedMatrix[i][k];
            }
        }
    }

    Vector solution(n);
    for (int i = n - 1; i >= 0; --i) {
        double sum = 0.0;
        for (int j = i + 1; j < n; ++j) {
            sum += augmentedMatrix[i][j] * solution[j];
        }
        solution[i] = (augmentedMatrix[i][n] - sum) / augmentedMatrix[i][i];
    }

    return solution;
}

double SquareMatrix::determinant()const {
    // if( n!= m){
    //     throw MathExceptions::InvalidMatrixSize;
    // }
    // double det = 1.0;
    // for (int i = 0; i < n; i++) {
    //     int pivot = i;
    //     for (int j = i + 1; j < n; j++) {
    //         if (abs(arr[j][i]) > abs(arr[pivot][i])) {
    //             pivot = j;
    //         }
    //     }
    //     if (pivot != i) {
    //         custom_swap(arr[i], arr[pivot]);
    //         det *= -1;
    //     }
    //     if (arr[i][i] == 0) {
    //         return 0;
    //     }
    //     det *= arr[i][i];
    //     for (int j = i + 1; j < n; j++) {
    //         double factor = arr[j][i] / arr[i][i];
    //         for (int k = i + 1; k < n; k++) {
    //             arr[j][k] -= factor * arr[i][k];
    //         }
    //     }
    // }
    // return det;
    double deter = determ(arr, n);
    return deter;
}


int determ(double** arr,int n) {
    int det=0, p, h, k, i, j;
    double** temp = new double*[n-1];
    for (int i = 0; i < n - 1; ++i) {
        temp[i] = new double[n - 1];
    }
    if(n==1) {
        return arr[0][0];
    } else if(n==2) {
    det=(arr[0][0]*arr[1][1]-arr[0][1]*arr[1][0]);
    return det;
    } else {
    for(p=0;p<n;p++) {
        h = 0;
        k = 0;
        for(i=1;i<n;i++) {
            for( j=0;j<n;j++) {
                if(j==p) {
                continue;
            }
            temp[h][k] = arr[i][j];
            k++;
            if(k==n-1) {
                h++;
                k = 0;
            }
            }
        }
        det=det+arr[0][p]*pow(-1,p)*determ(temp,n-1);
    }
    return det;
    }
}

void custom_swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
