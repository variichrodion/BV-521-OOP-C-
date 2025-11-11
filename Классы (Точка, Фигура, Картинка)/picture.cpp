#include <iostream>
#include <utility>
#include <memory>
#include <string>
#include <vector>

// класс "Точка"
class Point {
	double x, y;
public:
	static int count;
	Point(double xP = 0, double yP = 0) : x(xP), y(yP) { count++; }
	// 1. конструктор копирования
	Point(const Point& other) : x(other.x), y(other.y) {}
	// 2. оператор копирования
	Point& operator=(const Point& other) {
		if (this != &other) {
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	// 3. деструктор
	~Point() {}
	// 4. конструктор перемещения
	Point(Point&& other) noexcept {
		x = std::exchange(other.x, 0);
		y = std::exchange(other.y, 0);
	}
	// 5. оператор присваивания перемещением
	Point& operator=(Point&& other) noexcept {
		if (this != &other) {
			x = std::exchange(other.x, 0);
			y = std::exchange(other.y, 0);
		}
		return *this;
	}
	void setX(double& xP) { x = xP; }
	void setY(double& yP) { x = yP; }
	double getX() const { return x; }
	double getY() const { return y; }
	void showPoint() const {
		std::cout << "(" << x << ", " << y << ")";
	}
};

// класс "Фигура"
class Shape {
	Point def_point;
	std::string color;
public:
	Shape(Point p = Point(), std::string color = "black") : def_point(p), color(color) {}
	// 1. конструктор копирования
	Shape(const Shape& other) : def_point(other.def_point), color(other.color) {}
	// 2. оператор копирования
	Shape& operator=(const Shape& other) {
		if (this != &other) {
			def_point = other.def_point;
			color = other.color;
		}
		return *this;
	}
	// 3. деструктор
	~Shape() {}
	// 4. конструктор перемещения
	Shape(Shape&& other) noexcept {
		def_point = std::exchange(other.def_point, 0);
		color = std::exchange(other.color, 0);
	}
	// 5. оператор присваивания перемещением
	Shape& operator=(Shape&& other) noexcept {
		if (this != &other) {
			def_point = std::exchange(other.def_point, 0);
			color = std::exchange(other.color, 0);
		}
		return *this;
	}

	void showShape() {
		std::cout << "Фигура с координатами: ";
		def_point.showPoint();
		std::cout << " с цветом: " << color;
	}
};

class Picture {
private:

	std::string pic_name;
	static int pic_count;
public:
	std::vector<std::unique_ptr<Shape>> shapes;
	Picture(std::string name = "Picture") : pic_name(name) { pic_count++; };

	void addFigure(std::unique_ptr<Shape> shape) {
		shapes.push_back(std::move(shape));
	}
};

int Point::count = 0;
int Picture::pic_count = 0;

int main() {
	setlocale(LC_ALL, "");
	Picture pic("green");
	std::unique_ptr<Shape> shape_temp = std::make_unique<Shape>(Shape());
	pic.addFigure(std::move(shape_temp));
	pic.shapes[0]->showShape();
	return 0;
}