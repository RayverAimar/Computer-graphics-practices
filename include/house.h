#ifndef __HOUSE_H__
#define __HOUSE_H__

#include "./object.h"

struct House : public Object
{
public:
	House();
	House(const Point&, float);
	House(const std::vector<Point>&);
	~House();

private:

};

House::House()
{
    Point first(-0.3f, 0.4f, 0.0f), second(-0.3f, 0.0f, 0.0f), third(0.3f, 0.0f, 0.0f), fourth(0.3f, 0.4f, 0.0f), fifth(0.4f, 0.4f, 0.0f), sixth(0.0f, 0.75f, 0.0f), seventh(-0.4f, 0.4f, 0.0f);
    
    unsigned int init[] = {
        5, 6, 4,    //Upper Triangle
        3, 0, 1,    //Bottom right triangle   
        3, 1, 2     //Bottom left triangle
    };

    push(first);
    push(second);
    push(third);
    push(fourth);
    push(fifth);
    push(sixth);
    push(seventh);

    for (int i = 0; i < 9; i++)
    {
        indices.push_back(init[i]);
    }
    last_Point = Point(0.0f, 0.0f);

}

House::House(const Point& center, float separation)
{}

House::House(const std::vector<Point>& _vector) : Object(_vector)
{}

House::~House()
{
}


#endif // !__HOUSE_H__
