#include <iostream>
#include <cmath>

class Point {
    double x_, y_;
public:
    // конструктор с параметрами
    explicit Point(double x = 0, double y = 0) : x_(x), y_(y) {}
    // геттеры
    double getX() const { return x_; }
    double getY() const { return y_; }
    // установить значения точки (сеттер через числа)
    void setCoords(double x, double y) {
        x_ = x; y_ = y;
    }
    // оператор присваивания копированием
    Point& operator=(const Point& other) {
        x_ = other.x_; y_ = other.y_;
        return *this;
    }
    // перегрузка оператора ==
    bool operator==(const Point& other) const {
        return x_ == other.x_ && y_ == other.y_;
    }
    // найти расстояние между точками
    double getDistance(const Point& other) const {
        // возведение в степень: pow(a, n) , n - степень
        return sqrt( 
            (x_ - other.x_) * (x_ - other.x_) +
            (y_ - other.y_) * (y_ - other.y_)
        );
    }
    // перегрузка оператора << (не является членом класса)
    friend std::ostream& operator<<(std::ostream& os, const Point& point) {
        return os << "(" << point.x_ << "," << point.y_ << ")";
    }
    // перегрузка оператора << (является членом класса)
    std::ostream& operator<<(std::ostream& os) {
        return os << "(" << x_ << "," << y_ << ")";
    }
};

class Shape {
    Point gravity_center_;
public:
    Shape(const Point& p = Point(0, 0)) : gravity_center_(p) {}
    void moveShape(const Point& other) { gravity_center_ = other; }
    virtual ~Shape() {} // ОБЯЗАТЕЛЬНО VIRTUAL
};

// окружность
class Circle : public Shape {
private:
    double r_;
    Point p_;
public:
    Circle(const Point& p = Point(0,0), double r = 1) : p_(p), r_(r) {
        Shape(p_);
    }
};

// прямоугольник
class Rectangle : public Shape {

};

// треугольник
class Triangle : public Shape {

};


int main() {
    Circle c;
    return 0;
}

