#pragma once
#include <iostream> 
using namespace std;
template<typename T>
class matrix
{
public:
    T** a;
    int m;
    int n;
    matrix();
    matrix(int n, int m);
    ~matrix();
    matrix(const matrix<T>& M);
    matrix<T> inverse();
    matrix<T> transpond();
    matrix<T>& operator=(const matrix<T>& M);
    friend matrix<T> operator*(matrix<T>& M1, matrix<T>& M2)
    {
        matrix<T> temp(M1.n, M2.m);
        if (M1.m != M2.n)
        {
            cout << "error *" << endl;
            return temp;
        }
        for (int i = 0; i < M1.n; i++)
        {
            for (int j = 0; j < M2.m; j++)
            {
                for (int k = 0; k < M1.m; k++)
                    temp.a[i][j] += M1.a[i][k] * M2.a[k][j];
            }
        }
        return temp;
    };
    friend ostream& operator<<<>(ostream& o, const matrix<T>& M);
};

template<typename T>
ostream& operator<<(ostream& o, const matrix<T>& M)
{
    for (int i = 0; i < M.n; i++)
    {
        for (int j = 0; j < M.m; j++)
        {
            o << M.a[i][j] << '\t';
        }
        o << endl;
    }
    o << endl;
    return o;
};

template <typename T>
matrix<T>::matrix()
{
    a = nullptr;
    m = 0;
    n = 0;
};

template <typename T>
matrix<T>::matrix(int n, int m)
{
    this->n = n;
    this->m = m;
    a = new T * [n];
    for (int i = 0; i < n; i++)
    {
        a[i] = new T[m];
        for (int j = 0; j < m; j++)
        {
            a[i][j] = 0;
        }
    }
};

template <typename T>
matrix<T>::~matrix()
{
    if (a != nullptr)
    {
        for (int i = 0; i < n; i++)
        {
            delete[] a[i];
        }
        delete[] a;
        a = nullptr;
    }
};
