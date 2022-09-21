#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include <cmath>

#include "./vector3d.h"
#include "./utils.h"

/*#define EPSILON 0.001

bool to_the_right = true;
bool AreSame(float a, float b)
{
    return fabs(a - b) < EPSILON;
}
*/

struct Object
{

private:

    std::vector<Point> vertices;

public:

    //Constructors
    Object();
    Object(std::vector<Point>);
    Object(const std::vector<Point>&, const std::vector<int>&);

    //Movement methods
    void move_to_the_right();
    void move_to_the_left();
    void move_down();
    void move_up();
    void move(const Vector3D&);
    void move_in_circle(float);
    void move_in_spiral(float, float);

    //Auxiliar methods
    void print();
    void push(const Point&);
    void stop();

    //Getters
    std::vector<Point> get_vertices();
    size_t size();
    Point& front();

    std::vector<int> indices;
    Point last_Point;
    float increment, angle;
    bool moving, configurated;
};

Object::Object() : last_Point(0.0f, 0.0f), increment(0.0f), angle(0.0f), moving(false), configurated(false)
{}

Object::Object(std::vector<Point> _vertices) : vertices(_vertices), last_Point(0.0f, 0.0f), increment(0.0f), angle(0.0f), moving(false), configurated(false)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        indices.push_back(i);
    }
}

Object::Object(const std::vector<Point>& _vertices, const std::vector<int>& _indices) : vertices(_vertices), indices(_indices), last_Point(0.0f, 0.0f), increment(0.0f), angle(0.0f), moving(false), configurated(false)
{}

void Object::print()
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i].print();
    }
}

void Object::move(const Vector3D& direction)
{
    for (int i = 0; i < vertices.size(); i++)
    {
        vertices[i] += direction;
    }
}

void Object::move_to_the_right()
{
    Vector3D right(0.01f, 0.0f);
    move(right);
}

void Object::move_to_the_left()
{
    Vector3D left(-0.01f, 0.0f);
    move(left);
}

void Object::move_down()
{
    Vector3D down(0.0f, -0.01f);
    move(down);
}

void Object::move_up()
{
    Vector3D up(0.0f, 0.01f);
    move(up);
}

void Object::move_in_circle(float radio)
{
    if (!configurated)
    {
        configurated = true;
        increment = 0.005f;
    }
    float epsilon = 0.00001f;
    Point point_to_go(last_Point);
    if (last_Point.x <= -radio + epsilon) increment = 0.005f;
    if (last_Point.x >= radio - epsilon) increment = -0.005f;
    point_to_go.x += increment;
    point_to_go.y = sqrt((radio*radio) - (point_to_go.x * point_to_go.x));
    if (increment < 0.0f) point_to_go.y *= -1;
    Vector3D direction = point_to_go - last_Point;
    last_Point = point_to_go;
    move(direction);
}

void Object::move_in_spiral(float a, float b)
{
    if (!configurated)
    {
        configurated = true;
        increment = (float) PI / 48;
    }
    angle += increment;
    if (angle > 55.0f) increment = (float)-PI / 48;
    if (angle < 1.0f) increment = (float)PI / 48;
    float x = b * cos(angle + a) * angle;
    float y = b * sin(angle + a) * angle;
    Point point_to_go(x, y);
    Vector3D direction = point_to_go - last_Point;
    last_Point = point_to_go;
    move(direction);
}

void Object::stop()
{
    configurated = false;
    increment = 0.0f;
    //last_Point = Point(0.0f, 0.0f);
}


/*
void Object::move_in_circle(float radio)
{
    if (!configurated)
    {
        configurated = true;
        increment = 0.005f;
    }

    Point point_to_go(last_Point);
    if (to_the_right)
    {
        point_to_go.x += increment;
        if (AreSame(point_to_go.x, radio)) {
            to_the_right = false;
        }
    }
    else
    {
        point_to_go.x -= increment;
        if (AreSame(point_to_go.x, -radio)) {
            to_the_right = true;
        }
    }
    point_to_go.y = sqrt((radio*radio) - (point_to_go.x * point_to_go.x));
    if (!to_the_right) point_to_go.y *= -1;
    Vector3D direction = point_to_go - last_Point;
    last_Point = point_to_go;
    move(direction);
}
*/

void Object::push(const Point& _point)
{
    vertices.emplace_back(_point);
}

std::vector<Point> Object::get_vertices()
{
    return this->vertices;
}

size_t Object::size()
{
    return this->vertices.size();
}

Point& Object::front()
{
    return this->vertices.front();
}

#endif // __OBJECT_H__