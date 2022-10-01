#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "./object.h"

struct Square : public Object
{
public:
	Square();
	Square(const Point&, float);
	Square(const std::vector<Point>&);
	~Square();

private:

};

Square::Square()
{}

Square::Square(const Point& center, float separation)
{
    Point top_right(center), top_left(center), down_right(center), down_left(center);
    Vector3D top_right_direction(separation, separation), top_left_direction(-separation, separation), down_right_direction(separation, -separation), down_left_direction(-separation, -separation);
    top_right += top_right_direction;
    top_left += top_left_direction;
    down_right += down_right_direction;
    down_left += down_left_direction;
    push(top_left);
    push(top_right);
    push(down_right);
    push(down_left);
    for (int i = 0; i < this->size(); i++)
    {
        indices.push_back(i);
    }
    last_Point = center;
}


Square::Square(const std::vector<Point>& _vector) : Object(_vector)
{}

Square::~Square()
{
}

#endif // !__SQUARE_H__
