#include "Vector.hpp"
#include <iostream>
#ifndef Collision_hpp
#define Collision_hpp

namespace ez{

class Sphere
{
private:
    Vector3<float> _center = Vector3<float>(0, 0, 0);
    float _radius=0;
public:
    Sphere() {}
    Sphere(Vector3<float> center, float radius) : _center(center), _radius(radius) {}
    Vector3<float> getCenter() { return _center; }
    float getRadius() { return _radius; }
    void setRadius(float radius) { _radius = radius; }
    void setCenter(Vector3<float> center) { _center = center; }
};

class Collision {
public:
    static bool isPointInsideSphere(Sphere sphere, Vector3<float> point) 
    {
        point -= sphere.getCenter();
        point.x() *= point.x();
        point.y() *= point.y();
        point.z() *= point.z();

        if (sqrt(point.x() + point.y() + point.z()) < sphere.getRadius()) 
            return true;
        return false;
    };

    static bool intersect(Sphere sphere, Sphere other) 
    {
        Vector3<float> pos = sphere.getCenter();

        pos -= other.getCenter();
        pos *= pos;
        if (sqrt(pos.x() + pos.y() + pos.z()) < sphere.getRadius() + other.getRadius())
            return true;
        return false;
    };
};
}

#endif