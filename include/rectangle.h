#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "./object.h"

struct Rectangle : public Object
{
public:
    Rectangle();
    Rectangle(const Point&, float);
    Rectangle(const std::vector<Point>&);
    ~Rectangle();

private:

};

Rectangle::Rectangle()
{}

Rectangle::Rectangle(const Point& center, float separation)
{
    Point top_right(center), top_left(center), down_right(center), down_left(center);
    Vector3D top_right_direction(2 * separation, separation), top_left_direction(-2 * separation, separation), down_right_direction(2 * separation, -separation), down_left_direction(-2 * separation, -separation);
    top_right += top_right_direction;
    top_left += top_left_direction;
    down_right += down_right_direction;
    down_left += down_left_direction;
    push(top_left);
    push(top_right);
    push(down_right);
    push(down_left);
    unsigned int _indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    for (int i = 0; i < 6; i++)
    {
        indices.push_back(_indices[i]);
    }
    last_Point = center;
}


Rectangle::Rectangle(const std::vector<Point>& _vector) : Object(_vector)
{}

Rectangle::~Rectangle()
{
}

#endif // !__RECTANGLE_H__
