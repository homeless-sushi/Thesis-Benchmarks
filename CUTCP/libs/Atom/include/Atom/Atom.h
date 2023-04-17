#ifndef ATOM_ATOM_H
#define ATOM_ATOM_H

namespace Atom
{
    class Vec3
    {
        public:
            float x;
            float y;
            float z;

            Vec3() = default;
            Vec3(float u) :
                x{u}, y{u}, z{u}
            {};
            Vec3(float x, float y, float z) :
                x{x}, y{y}, z{z}
            {};

            Vec3 operator+(const Vec3& other){ return Vec3(x+other.x, y+other.y, z+other.z); };
            Vec3 operator-(const Vec3& other){ return Vec3(x-other.x, y-other.y, z-other.z); };
            Vec3 operator-(){ return Vec3(-x, -y, -z); };
            Vec3 operator*(float a){ return Vec3(a*x, a*y, a*z); };
    };

    class Atom
    {
        public:
            float x;
            float y;
            float z;
            float q;

            Atom() = default;
            Atom(float x, float y, float z, float q) :
                x{x}, y{y}, z{z}, q{q}
            {};
    };
}

#endif //ATOM_ATOM_H