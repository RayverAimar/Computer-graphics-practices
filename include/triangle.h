#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "object.h"

struct Triangle : public Object
{
    Triangle();
    Triangle(const Point&, float);
    Triangle(const std::vector<Point>&);
};

Triangle::Triangle()
{}

Triangle::Triangle(const Point& center, float separation = 2.0f) : Object(center)
{
    Point top(center), down_right(center), down_left(center);
    Vector3D top_direction(0.0f, separation), down_right_direction(separation, -separation), down_left_direction(-separation, -separation);
    top += top_direction;
    down_right += down_right_direction;
    down_left += down_left_direction;
    push(top);
    push(down_right);
    push(down_left);
    for (int i = 0; i < size(); i++)
    {
        indices.push_back(i);
    }
    last_Point = center;
}

Triangle::Triangle(const std::vector<Point>& _vector) : Object(_vector)
{}

#endif // __TRIANGLE_H__
