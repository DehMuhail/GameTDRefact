#pragma once

#include <iostream>
#include <cmath>
using namespace std;
/* Tasks 1 and 2 from 1st list and 1st tast from 2nd list were  references */
const double Eps = 0.00001;
class shape
{
protected:
    double square;
    double perimeter;
    double** points;
    double* sides;
    double** vectors;
public:
    virtual double getSquare() = 0;
    virtual double getPerimeter() = 0;
    virtual void calculateSides() = 0;
    virtual void calculateVectors() = 0;
    virtual void setPoint() = 0;
    virtual bool isConvex() = 0;
    virtual bool operator>(shape&);
    friend bool operator<(shape&, shape&);
    friend bool operator==(shape&, shape&);
    friend bool operator!=(shape&, shape&);
    friend ostream& operator<<(ostream& c, const shape& s);
    friend void swap(shape*, shape*);
  

};

bool shape::operator>(shape& t)
{
    getSquare();
    if (square > t.getSquare())
        return true;
    else return false;
};

bool operator<(shape& t1, shape& t2)
{
    if (t1.getSquare() < t2.getSquare())
        return true;
    else return false;

};

bool operator==(shape& t1, shape& t2)
{
    if (t1.getSquare() == t2.getSquare())
        return true;
    else return false;

};

bool operator!=(shape& t1, shape& t2)
{
    if (t1.getSquare() != t2.getSquare())
        return true;
    else return false;

};


void swap(shape* q, shape* p)
{

    for (int i = 0; i < 5; i++)
    {
        double t = q->points[i][0];
        q->points[i][0] = p->points[i][0];
        p->points[i][0] = t;

        t = q->points[i][1];
        q->points[i][1] = p->points[i][1];
        p->points[i][1] = t;

        t = q->vectors[i][0];
        q->vectors[i][0] = p->vectors[i][0];
        p->vectors[i][0] = t;

        t = q->vectors[i][1];
        q->vectors[i][1] = p->vectors[i][1];
        p->vectors[i][1] = t;

        t = q->sides[i];
        q->sides[i] = p->sides[i];
        p->sides[i] = t;
    }
    double t = q->square;
    q->square = p->square;
    p->square = t;

    t = q->perimeter;
    q->perimeter = p->perimeter;
    p->perimeter = t;

};

ostream& operator<<(ostream& c, const shape& s)
{
    c << s.square;
    return c;
};

class triangle :public shape
{
public:
    double getterSquare()
    {
        return square;
    }
    void setSquare(double s)
    {
        this->square = s;
    }
    void setPoint() override 
    {
        cout << "enter points" << endl;
        for (int i = 0; i < 3; i++)
        {
            cout << " enter X" << endl;
            cin >> points[i][0];
            cout << "enter Y" << endl;
            cin >> points[i][1];
        }

    };
    void setPoints(int x1, int y1, int x2, int y2, int x3, int y3)
    {
        points[0][0] = x1;
        points[0][1] = y1;
        points[1][0] = x2;
        points[1][1] = y2;
        points[2][0] = x3;
        points[2][1] = y3;

    };

    void calculateSides() override
    {
        for (int i = 0; i < 2; i++)
        {
            sides[i] = sqrt(pow(points[i][0] - points[i + 1][0], 2) + pow(points[i][1] - points[i + 1][1], 2));
        };
        sides[2] = sqrt(pow(points[2][0] - points[0][0], 2) + pow(points[2][1] - points[0][1], 2));
    };
    double getPerimeter() override
    {
        calculateSides();
        perimeter = 0;
        for (int i = 0; i < 3; i++)
        {
            perimeter += sides[i];
        }
        return perimeter;
    };
    double getSquare() override
    {
        double p = getPerimeter() / 2;
        square = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - sides[2]));
        return square;

    };
    void calculateVectors() override
    {
        for (int i = 0; i < 2; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[i][0];
            vectors[i][1] = points[i + 1][1] - points[i][1];
        }
        vectors[2][0] = points[0][0] - points[2][0];
        vectors[2][1] = points[0][1] - points[2][1];
    };

    bool isRectangular()
    {
        calculateVectors();
        if ((vectors[0][0] * vectors[1][0] + vectors[0][1] * vectors[1][1] == 0) || (vectors[1][0] * vectors[2][0] + vectors[1][1] * vectors[2][1] == 0)
            || (vectors[0][0] * vectors[2][0] + vectors[0][1] * vectors[2][1] == 0))
        {
            return true;
        }
        else return false;
    };

    bool isIsosceles()
    {
        if (fabs(sides[0] - sides[1]) < Eps || fabs(sides[1] - sides[2]) < Eps || fabs(sides[2] - sides[0]) < Eps)
        {
            return true;
        }
        else return false;
    };

    bool isEquilateral()
    {
        if (fabs(sides[0] - sides[1]) < Eps && fabs(sides[1] - sides[2]) < Eps)
        {
            return true;
        }
        else return false;
    };

    bool isConvex() { return true; };
    

    bool operator>(const triangle&);
    friend bool operator<(const triangle&, const triangle&);
    triangle();
    ~triangle();
    triangle(const triangle&);
    triangle& operator=(const triangle&);
};

triangle::triangle()
{
    sides = new double[3];
    points = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        vectors[i] = new double[2];
    }
};
triangle::~triangle()
{
    delete[]sides;
    for (int i = 0; i < 3; i++)
    {
        delete[]points[i];
    }
    delete[]points;

    for (int i = 0; i < 3; i++)
    {
        delete[]vectors[i];
    }
    delete[]vectors;
};

triangle::triangle(const triangle& x) {
    sides = new double[3];
    points = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [3];
    for (int i = 0; i < 3; i++)
    {
        vectors[i] = new double[2];
    }

    for (int i = 0; i < 3; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 3; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
}
triangle& triangle::operator=(const triangle& x) {
    for (int i = 0; i < 3; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 3; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
    return *this;
}


class quadrangle :public shape
{
public:
    void setPoint() override
    {
        cout << "enter points" << endl;
        for (int i = 0; i < 4; i++)
        {
            cout << " enter X" << endl;
            cin >> points[i][0];
            cout << "enter Y" << endl;
            cin >> points[i][1];
        }

    };

    void calculateSides() override
    {
        for (int i = 0; i < 3; i++)
        {
            sides[i] = sqrt(pow(points[i][0] - points[i + 1][0], 2) + pow(points[i][1] - points[i + 1][1], 2));
        };
        sides[3] = sqrt(pow(points[3][0] - points[0][0], 2) + pow(points[3][1] - points[0][1], 2));

    };

    double getPerimeter() override
    {
        calculateSides();
        perimeter = 0;
        for (int i = 0; i < 4; i++)
        {
            perimeter += sides[i];
        }
        return perimeter;
    };

    void calculateVectors() override
    {
        for (int i = 0; i < 3; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[i][0];
            vectors[i][1] = points[i + 1][1] - points[i][1];
        }
        vectors[3][0] = points[0][0] - points[3][0];
        vectors[3][1] = points[0][1] - points[3][1];
    };

    bool isTrapezium()
    {
        calculateVectors();
        if ((vectors[0][1] * vectors[2][0] == vectors[0][2] * vectors[2][1]) || (vectors[1][1] * vectors[3][0] == vectors[1][2] * vectors[3][1]))
        {
            return true;
        }
        else false;
    };

    bool isParallelogram()
    {
        calculateVectors();
        if ((vectors[0][1] * vectors[2][0] == vectors[0][0] * vectors[2][1]) && (vectors[1][1] * vectors[3][0] == vectors[1][0] * vectors[3][1]))
        {
            return true;
        }
        else false;
    };

    bool isDiamond()
    {
        for (int i = 0; i < 3; i++)
        {
            if (fabs(sides[i] - sides[i + 1]) >= Eps)
                return false;
        }
        return true;


    }

    bool isRectangularQuadrangale()
    {
        calculateVectors();
        if ((vectors[0][0] * vectors[1][0] + vectors[0][1] * vectors[1][1] == 0) && (vectors[2][0] * vectors[3][0] + vectors[2][1] * vectors[3][1] == 0)
            && (vectors[1][0] * vectors[2][0] + vectors[1][1] * vectors[2][1] == 0))
        {
            return true;
        }
        else return false;
    };

    bool isSquare()
    {
        if (isRectangularQuadrangale() && isDiamond())
        {
            return true;
        }
        else return false;
    };

    bool isConvex()
    {
        calculateVectors();
        for (int i = 0; i < 3; i++)
        {
            if (vectors[i][0] * vectors[i + 1][1] - vectors[i][1] * vectors[i + 1][0] < 0)
                return false;
        }
        if (vectors[3][0] * vectors[0][1] - vectors[3][1] * vectors[0][0] < 0)
            return false;
        return true;
    };



    double getSquare() override
    {
        calculateSides();
        if (isSquare())
        {
            square = pow(sides[0], 2);
            return square;
        }
        else if (isDiamond())
        {
            square = (sqrt((pow(points[2][0] - points[0][0], 2)) + (pow(points[2][1] - points[0][1], 2))) *
                sqrt((pow(points[3][0] - points[1][0], 2)) + (pow(points[3][1] - points[1][1], 2)))) / 2;
            return square;
        }
        else if (isRectangularQuadrangale())
        {
            square = sides[0] * sides[1];
            return square;
        }
        else if (isParallelogram())
        {
            square = fabs(vectors[0][0] * vectors[1][1] - vectors[0][1] * vectors[1][0]);
            return square;
        }
        else if (isConvex() || (vectors[1][0] * vectors[2][1] - vectors[1][1] * vectors[2][0] < 0)
            || (vectors[3][0] * vectors[0][1] - vectors[3][1] * vectors[0][0] < 0))
        {
            double d;
            d = sqrt(pow(points[2][0] - points[0][0], 2) + pow(points[2][1] - points[0][1], 2));
            double p = getPerimeter() / 2 - (sides[3] / 2) - (sides[2] / 2) + d / 2;
            square = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - d));
            double p2 = getPerimeter() / 2 - (sides[0] / 2) - (sides[1] / 2) + d / 2;
            square += sqrt(p * (p - sides[2]) * (p - sides[3]) * (p - d));
            return square;
        }
        else
        {
            double d;
            d = sqrt(pow(points[2][0] - points[0][0], 2) + pow(points[2][1] - points[0][1], 2));
            double p = getPerimeter() / 2 - (sides[3] / 2) - (sides[2] / 2) + d / 2;
            square = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - d));
            double p2 = getPerimeter() / 2 - (sides[0] / 2) - (sides[1] / 2) + d / 2;
            square -= sqrt(p * (p - sides[2]) * (p - sides[3]) * (p - d));
            square = fabs(square);
            return square;
        }
    };
    // bool operator>(const triangle&);
    // friend bool operator<(const triangle&, const triangle&);

    quadrangle();
    ~quadrangle();
    quadrangle(const quadrangle&);
    quadrangle& operator=(const quadrangle&);
};

quadrangle::quadrangle()
{
    sides = new double[4];
    points = new double* [4];
    for (int i = 0; i < 4; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [4];
    for (int i = 0; i < 4; i++)
    {
        vectors[i] = new double[2];
    }
};

quadrangle::~quadrangle()
{
    delete[]sides;
    for (int i = 0; i < 4; i++)
    {
        delete[]points[i];
    }
    delete[]points;

    for (int i = 0; i < 4; i++)
    {
        delete[]vectors[i];
    }
    delete[]vectors;
};

quadrangle::quadrangle(const quadrangle& x) {
    sides = new double[4];
    points = new double* [4];
    for (int i = 0; i < 4; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [4];
    for (int i = 0; i < 4; i++)
    {
        vectors[i] = new double[2];
    }

    for (int i = 0; i < 4; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 4; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
}
quadrangle& quadrangle::operator=(const quadrangle& x) {
    for (int i = 0; i < 4; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 4; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
    return *this;
}
/*
bool triangle::operator>(const triangle& t) override
{
    getSquare();
    if (square > t.getSquare())
        return true;
    else return false;
};

bool operator<(const triangle& t1, const triangle& t2)
{
    if (t1.getSquare() < t2.getSquare())
        return true;
    else return false;

};*/



class pentagon : public shape
{
public:
    void setPoint() override
    {
        cout << "enter points" << endl;
        for (int i = 0; i < 5; i++)
        {
            cout << " enter X" << endl;
            cin >> points[i][0];
            cout << "enter Y" << endl;
            cin >> points[i][1];
        }

    };
    void calculateSides() override
    {
        for (int i = 0; i < 4; i++)
        {
            sides[i] = sqrt(pow(points[i][0] - points[i + 1][0], 2) + pow(points[i][1] - points[i + 1][1], 2));
        };
        sides[4] = sqrt(pow(points[4][0] - points[0][0], 2) + pow(points[4][1] - points[0][1], 2));

    };
    double getPerimeter() override
    {
        calculateSides();
        perimeter = 0;
        for (int i = 0; i < 4; i++)
        {
            perimeter += sides[i];
        }
        return perimeter;
    };

    void calculateVectors() override
    {
        for (int i = 0; i < 4; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[i][0];
            vectors[i][1] = points[i + 1][1] - points[i][1];
        }
        vectors[4][0] = points[0][0] - points[4][0];
        vectors[4][1] = points[0][1] - points[4][1];

    };


    double getSquare() override
    {
        square = 0;
        //double p = getPerimeter() / 2;

        //calculateVectors();
        for (int i = 0; i < 4; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[0][0];
            vectors[i][1] = points[i + 1][1] - points[0][1];
        }

        for (int i = 0; i < 3; i++)
        {
            square += ((vectors[i][0] * vectors[i + 1][1] - vectors[i][1] * vectors[i + 1][0]) / 2);
        }

        return square;
    };
    bool isConvex() {
        calculateVectors();
        for (int i = 0; i < 4; i++)
        {
            if (vectors[i][0] * vectors[i + 1][1] - vectors[i][1] * vectors[i + 1][0] < 0)
                return false;
        }
        if (vectors[4][0] * vectors[0][1] - vectors[4][1] * vectors[0][0] < 0)
            return false;
        return true;
    }
    pentagon();
    ~pentagon();
    pentagon(const pentagon&);//конструктор копіювання
    pentagon& operator=(const pentagon&);
};
pentagon::pentagon()
{
    sides = new double[5];
    points = new double* [5];
    for (int i = 0; i < 5; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [5];
    for (int i = 0; i < 5; i++)
    {
        vectors[i] = new double[2];
    }
};

pentagon::~pentagon()
{
    delete[]sides;
    for (int i = 0; i < 5; i++)
    {
        delete[]points[i];
    }
    delete[]points;

    for (int i = 0; i < 5; i++)
    {
        delete[]vectors[i];
    }
    delete[]vectors;
};
pentagon::pentagon(const pentagon& x) {
    sides = new double[5];
    points = new double* [5];
    for (int i = 0; i < 5; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [5];
    for (int i = 0; i < 5; i++)
    {
        vectors[i] = new double[2];
    }

    for (int i = 0; i < 5; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 5; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
}
pentagon& pentagon::operator=(const pentagon& x) {
    for (int i = 0; i < 5; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < 5; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
    return *this;
}

template<int P>
class polygon :public shape
{
public:
    void setPoint() override
    {
        cout << "enter points" << endl;
        for (int i = 0; i < P; i++)
        {
            cout << " enter X" << endl;
            cin >> points[i][0];
            cout << "enter Y" << endl;
            cin >> points[i][1];
        }

    };
    void calculateSides() override
    {
        for (int i = 0; i < P - 1; i++)
        {
            sides[i] = sqrt(pow(points[i][0] - points[i + 1][0], 2) + pow(points[i][1] - points[i + 1][1], 2));
        };
        sides[P - 1] = sqrt(pow(points[P - 1][0] - points[0][0], 2) + pow(points[P - 1][1] - points[0][1], 2));

    };
    double getPerimeter() override
    {
        calculateSides();
        perimeter = 0;
        for (int i = 0; i < P - 1; i++)
        {
            perimeter += sides[i];
        }
        return perimeter;
    };

    void calculateVectors() override
    {
        for (int i = 0; i < P - 1; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[i][0];
            vectors[i][1] = points[i + 1][1] - points[i][1];
        }
        vectors[P - 1][0] = points[0][0] - points[P - 1][0];
        vectors[P - 1][1] = points[0][1] - points[P - 1][1];

    };


    double getSquare() override
    {
        square = 0;
        //double p = getPerimeter() / 2;

        //calculateVectors();
        for (int i = 0; i < P - 1; i++)
        {
            vectors[i][0] = points[i + 1][0] - points[0][0];
            vectors[i][1] = points[i + 1][1] - points[0][1];
        }

        for (int i = 0; i < P - 2; i++)
        {
            square += ((vectors[i][0] * vectors[i + 1][1] - vectors[i][1] * vectors[i + 1][0]) / 2);
        }

        return square;
    };
    bool isConvex() {
        calculateVectors();
        for (int i = 0; i < P - 1; i++)
        {
            if (vectors[i][0] * vectors[i + 1][1] - vectors[i][1] * vectors[i + 1][0] < 0)
                return false;
        }
        if (vectors[P - 1][0] * vectors[0][1] - vectors[P - 1][1] * vectors[0][0] < 0)
            return false;
        return true;
    }
    polygon();
    ~polygon();
    polygon(const polygon&);//конструктор копіювання
    polygon& operator=(const polygon&);
};
template<int P>
polygon<P>::polygon()
{
    sides = new double[P];
    points = new double* [P];
    for (int i = 0; i < P; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [P];
    for (int i = 0; i < P; i++)
    {
        vectors[i] = new double[2];
    }
};

template<int P>
polygon<P>::~polygon()
{

    delete[]sides;
    for (int i = 0; i < P; i++)
    {
        delete[]points[i];
    }
    delete[]points;

    for (int i = 0; i < P; i++)
    {
        delete[]vectors[i];
    }
    delete[]vectors;

};
template<int P>
polygon<P>::polygon(const polygon<P>& x) {
    sides = new double[P];
    points = new double* [P];
    for (int i = 0; i < P; i++)
    {
        points[i] = new double[2];
    }
    vectors = new double* [P];
    for (int i = 0; i < P; i++)
    {
        vectors[i] = new double[2];
    }

    for (int i = 0; i < P; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < P; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
}
template<int P>
polygon<P>& polygon<P>::operator=(const polygon<P>& x) {
    for (int i = 0; i < P; i++) {
        sides[i] = x.sides[i];
    }
    for (int i = 0; i < P; i++)
    {
        points[i][0] = x.points[i][0];
        points[i][1] = x.points[i][1];
        vectors[i][0] = x.vectors[i][0];
        vectors[i][1] = x.vectors[i][1];
    }
    return *this;
}



template <typename T>
struct node {
    T val;
    node<T>* next;
};

template <typename T>
class list {
private:
    node<T>* head;
public:
    list() {
        head = nullptr;
    };
    ~list();
    void addNode(T);
    T operator() (int i);
    int certainSearch(T val);
    void sort();
    void show();
};

template <typename T>
void list <T>::show()
{
    node<T>* p = head;
    while (p != nullptr)
    {
        cout << p->val << " ";
        p = p->next;
    }
    cout << endl;
};

template <typename T>
list<T>::~list() {
    while (head != nullptr) {
        node<T>* temp = head->next;
        delete head;
        head = temp;
    }

};
template <typename T>
void list<T>::addNode(T val) {
    node<T>* p = head;
    if (p == nullptr)
    {
        head = new node<T>;
        head->val = val;
        head->next = nullptr;
        return;
    }
    else
    {
        while (p->next != nullptr)
        {
            p = p->next;
        }
        p->next = new node<T>;
        p = p->next;
        p->val = val;
        p->next = nullptr;
    }

};

template <typename T>
T list<T>::operator()(int i) {
    node<T>* p = head;
    while (i) {
        if (p->next == nullptr) {
            cout << "error";
            return 0;
        }
        else {
            p = p->next;
            i--;
        }
    }
    return p->val;
}

template <typename T>
int list<T>::certainSearch(T val) {
    node<T>* p = head;
    int i = 0;
    while (p != nullptr && p->val != val) {
        p = p->next;
        i++;
    }
    if (p == nullptr) {
        return -1;
    }
    else
    {
        return i;
    }
};

template <typename T>
void list<T>::sort()
{
    node<T>* p = head;
    node<T>* q = head;
    int n = 0;
    double max = 0;
    while (p != nullptr)
    {
        p = p->next;
        n++;
    }
    if (n <= 1)
        return;
    for (int i = 0; i < n; i++)
    {
        max = (head->val).getSquare();
        q = head;
        p = head;
        for (int j = 0; j < n - i - 1; j++)
        {
            p = p->next;
            if ((p->val).getSquare() > max)
            {
                max = (p->val).getSquare();
                q = p;
            }
        }
        if (p != q)
        {
            swap((shape*)&(q->val), (shape*)&(p->val));
        }
    }

}


