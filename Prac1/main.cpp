#include <iostream>
#include "Vector.h"
#include "Matrix.h"

double customAbs(double value) {
    return value >= 0 ? value : -value;
}

int main() {
/* 
    double arr1[] = {0, 0, 0.3};
    double arr2[] = {0, 0 ,0.13};
    Vector v1(3, arr1);
    Vector v2(3, arr2);
    //dot product
    try {
        double dotProduct = v1 * v2;
        std::cout << "Dot product: " << dotProduct << std::endl;
    } catch (const MathExceptions& e) {
    }

    //cross product
    try {
        Vector crossResult = v1.crossProduct(v2);
        std::cout << "Result of cross product:" << std::endl;
        crossResult.print();
    } catch (const MathExceptions& e) {
    }

    //magnitude
    try {
        double magnitude = v1.magnitude();
        std::cout << "Magnitude of the vector: " << magnitude << std::endl;
    } catch (...) {
        std::cerr << "Error during magnitude calculation." << std::endl;
    }

   //addition
    try {
        Vector result = v1 + v2;

        std::cout << "Result of vector addition:" << std::endl;
        result.print();
    } catch (const MathExceptions& e) {
    }

    //subtraction
    try {
        Vector subtractionResult = v1 - v2;

        for (int i = 0; i < subtractionResult.getN(); ++i) {
            subtractionResult[i] = customAbs(subtractionResult[i]);
        }

        std::cout << "Result of vector subtraction:" << std::endl;
        subtractionResult.print();
    } catch (const MathExceptions& e) {
    }

    //scalar multiplication
    double scalar = 2.0;
    try {
        Vector scalarResult = v1 * scalar;

        std::cout << "Result of scalar multiplication:" << std::endl;
        scalarResult.print();
    } catch (const MathExceptions& e) {
    }


    //unit vector
    try {
        Vector unitVec = v1.unitVector();

        std::cout << "Unit vector:" << std::endl;
        unitVec.print();
    } catch (const MathExceptions& e) {
    }
*/
/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX MATRIX XXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*/
    
    double MatrixArr1[2][3] = {{1.0,2.0,3.0},{4.1,5.1,6.1}};
    double MatrixArr2[3][2] = {{7,8},{9,10},{11,12}};
    double* MatrixPtr1[2] = {MatrixArr1[0], MatrixArr1[1]};
    double* MatrixPtr2[3] = {MatrixArr2[0], MatrixArr2[1], MatrixArr2[2]};
    Matrix A(2, 3, MatrixPtr1);
    Matrix B(3, 2, MatrixPtr2);

    //addition
    double MatrixArr3[2][3] = {{-1, 2, -3}, {4, 5, 6}};
    double MatrixArr4[2][3] = {{7, 8, 9}, {10, 11, 12}};
    double* MatrixPtr3[2] = {MatrixArr3[0], MatrixArr3[1]};
    double* MatrixPtr4[2] = {MatrixArr4[0], MatrixArr4[1]};
    Matrix X(2, 3, MatrixPtr3);
    Matrix Y(2, 3, MatrixPtr4);

    //square matrix
    double square[3][3] = {{7,-6,3},{4,-5,-4},{2,1,8}};
    double* sq[3] = {square[0], square[1], square[2]};
    SquareMatrix S(3, sq);

    // multiplication
    // try {
    //     Matrix C(2,2);
    //     C= A*B;
    //     std::cout<<"Matrix multiplication:"<<std::endl;
    //     C.print();
    // } catch (const MathExceptions& e){
    //     std::cout<<"matrix multiplication error"<<std::endl;
    // }

    //scalar multiplication
    double scalar = 3.0;
    try{
        Matrix result = A*scalar;
        std::cout<<"Scalar multiplication:"<<std::endl;
        result.print();
    } catch( const MathExceptions& e){
    }

    //addition
    try{
        Matrix result = X + Y;
        std::cout << "Matrix addition:" << std::endl;
        result.print();
    } catch(const MathExceptions& e){
    }

    //transpose
    Matrix trans = ~A;
    try{
        std::cout<< "Original matrix:"<<std::endl;
        A.print();
        std::cout<<"Transpose matrix:"<<std::endl;
        trans.print();
    }catch (const MathExceptions& e){}

    //Inverse matrix
    try{
        SquareMatrix inverse = !S;
        std::cout << "Inverse Matrix:"<<std::endl;
        inverse.print();
    } catch(const MathExceptions& e){
        std::cout << "Identity Matrix error"<<std::endl;
    }

    //Solve
    double tData[3] = {2, -3, 5};
    Vector t(3, tData);
    try {
        Vector x = S.solve(t);
        std::cout << "Solution vector x:" << std::endl;
        x.print();
    } catch (const MathExceptions &e) {
    }

    //determinant
    try {
        double det = S.determinant();
        std::cout << "Determinant of A: " << det << std::endl;
    } catch (const MathExceptions &e) {
    }

    return 0;
}
