#include "Vector.h"

Vector::Vector(int size) {
    //paramter check
    if (size <=0){
        throw MathExceptions::InvalidVectorSize;
    }
    n = size;
    arr = new double[n];
    for (int i=0; i<n; ++i){
        arr[i] = 0.0;
    }
}

Vector::Vector(int size, double* elements) {
    //parameter check
    if (size <= 0) {
        throw MathExceptions::InvalidVectorSize;
    }
    else{
    n = size;
    arr = new double[n]; 
    for (int i = 0; i < n; ++i) {
        arr[i] = elements[i];
    }}
}
Vector::~Vector(){
    delete[] arr;
}

Vector::Vector(const Vector& index){
    n = index.n;
    arr = new double[n];
    for(int i=0; i<n;++i){
        arr[i]=index.arr[i];
    }
}

Vector::operator Matrix() const{
    Matrix result(n,1);
    for (int i=0; i<n; ++i){
        result[i][0] = arr[i];
    }
    return result;
}

int Vector::getN() const{
    return n;
}

double Vector::magnitude() const {
    double sum = 0.0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i] * arr[i];
    }
       return std::sqrt(sum);
}

/*
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXX OPERATIONS XXXXXXXXXXXXXXXX
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*/

Vector Vector::operator+(const Vector other) const{
    if (n!=other.n){
        throw MathExceptions::InvalidVectorAddition;
    }
    Vector result(n);
    for (int i=0;i<n;++i){
        result.arr[i] = arr[i] + other.arr[i];
    }
    return result;
}

Vector Vector::operator-(const Vector other) const{
    if (n!=other.n){
        throw MathExceptions::InvalidVectorAddition;
    }
    double* result = new double[n];
    for (int i=0; i<n; ++i){
        result[i] = arr[i] - other.arr[i];
    }
    return Vector(n,result);
}

Vector Vector::operator*(double scalar) const {
    Vector scaledVector(n, arr); 

    for (int i = 0; i < n; ++i) {
        scaledVector[i] *= scalar;
    }
    return scaledVector;
}

double Vector::operator*(const Vector vec) const{
    if(n!=vec.n){
        throw MathExceptions::InvalidDotProduct;
    }
    double result =0.0;
    for(int i=0; i<n ; ++i){
        result +=arr[i]*vec.arr[i];
    }
    return result;
}

Vector Vector::crossProduct(const Vector r) const{
    if(n!=3 || r.getN() != 3){
        throw MathExceptions::InvalidCrossProduct;
    }
    else{
        double x= arr[1]*r[2] - arr[2]*r[1];
        double y= arr[2]*r[0] - arr[0]*r[2];
        double z= arr[0]*r[1] - arr[1]*r[0];
        double result[] = {x,y,z};
        return Vector(3,result);
    }

}

Vector Vector::unitVector() const{
    double magnitudes = magnitude();
    if (magnitudes == 0){
        throw MathExceptions::InvalidUnitVector;
    }
    Vector unitV(n);
    for (int i=0; i<n;++i){
        unitV[i] = (*this)[i] /magnitudes;
    }
    return unitV;
}
