#pragma once

#include <initializer_list>
#include <cmath>
#include <iostream>


namespace mmath
{
    constexpr float PI = 3.14159265359f;
    
    template <typename ElementType>
    struct Vec3
    {
        ElementType elems[3];

        ElementType L2Norm() const
        {
            return elems[0] * elems[0] + elems[1] * elems[1]
                + elems[2] * elems[2];
        }

        float length() const { return (float)sqrt((double)L2Norm()); }

        Vec3<float> normalize() const
        {
            float len = length();
            return {elems[0] / len, elems[1] / len, elems[2] / len};
        }

        ElementType& operator[](const int index) { return elems[index]; }
        const ElementType& operator[](const int index) const { return elems[index]; }

        template <typename T>
        Vec3<ElementType> operator-(const Vec3<T>& rhs) const
        {
            return {elems[0] - rhs[0], elems[1] - rhs[1], elems[2] - rhs[2]};   
        }

        template <typename T>
        Vec3<ElementType> operator*(const T rhs) const
        {
            return {elems[0] * rhs, elems[1] * rhs, elems[2] * rhs};   
        }

        template <typename T>
        Vec3<ElementType> operator/(const T rhs) const
        {
            return {elems[0] / rhs, elems[1] / rhs, elems[2] / rhs};   
        }



        template <typename T>
        operator Vec3<T>() { return Vec3<T>(elems[0], elems[1], elems[2]); }

        Vec3() : elems{0, 0, 0} {};
        Vec3(ElementType x) : elems{ x, x, x } {};
        Vec3(ElementType x, ElementType y, ElementType z = 0) : elems{ x, y, z } {};
    };

    template <typename ElementType>
    struct Vec4
    {
        ElementType elems[4];

        ElementType& operator[](const int index) { return elems[index]; }
        const ElementType& operator[](const int index) const { return elems[index]; }

        
        template <typename T>
        Vec4<ElementType> operator-(const Vec4<T>& rhs) const
        {
            return {elems[0] - rhs[0], elems[1] - rhs[1], elems[2] - rhs[2], elems[3] - rhs[3]};   
        }

        template <typename T>
        Vec4<ElementType> operator*(const T rhs) const
        {
            return {elems[0] * rhs, elems[1] * rhs, elems[2] * rhs, elems[3] * rhs};   
        }

        template <typename T>
        Vec4<ElementType> operator/(const T rhs) const
        {
            return {elems[0] / rhs, elems[1] / rhs, elems[2] / rhs, elems[3] / rhs};   
        }


        template <typename T>
        operator Vec3<T>() { return Vec3<T>(elems[0], elems[1], elems[2]); }

        template <typename T>
        operator Vec4<T>() { return Vec4<T>(elems[0], elems[1], elems[2], elems[3]); }

        Vec4() : elems{ 0, 0, 0, 0 } {};
        Vec4(ElementType x) : elems{x, x, x, x} {};
        Vec4(Vec3<ElementType> v, ElementType a) : elems{ v[0], v[1], v[2], a } {};
        Vec4(ElementType x, ElementType y, ElementType z = 0, ElementType a = 0) : elems{ x, y, z, a } {};
    };

    template <typename ElementType>
    struct Mat3x3
    {
        Vec3<ElementType> vecs[3];

        Vec3<ElementType>& operator[](const int index) { return vecs[index]; }
        const Vec3<ElementType>& operator[](const int index) const { return vecs[index]; }

        template <typename T>
        Vec3<ElementType> operator*(const Vec3<T>& v)
        {
            Vec3<ElementType> result;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    result[i] += vecs[i][j] * v[j];
            
            return result;
        }

        template <typename T>
        Mat3x3<ElementType> operator*(const Mat3x3<T>& v)
        {
            Mat3x3<ElementType> result;
            for (int i = 0; i < 3; i++) result[i] = this * v[i];
            return result;
        }

        Mat3x3() : Mat3x3{ 0, 0, 0, 0 }
        {
            for (int i = 0; i < 3; i++) vecs[i][i] = 1;
        }

        Mat3x3(const std::initializer_list<ElementType>& list)
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                vecs[t / 3][t % 3] = *i;
                if (t++ == 9) break;
            }
            for (; t < 9; t++) vecs[t / 3][t % 3] = 0;
        }

        Mat3x3(const std::initializer_list< Vec3<ElementType> >& list)
        {
            int t = 0;
            for (const auto& i : list)
            {
                vecs[t] = i;
                if (t++ == 3) break;
            }
            for (; t < 3; t++) vecs[t] = Vec3<ElementType>();
        }
    };

    template <typename ElementType>
    struct Mat4x4
    {
        Vec4<ElementType> vecs[4];

        Vec4<ElementType>& operator[](const int index) { return vecs[index]; }
        const Vec4<ElementType>& operator[](const int index) const { return vecs[index]; }
        
        template <typename T>
        Vec4<ElementType> operator*(const Vec4<T>& v)
        {
            Vec4<ElementType> result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    result[i] += vecs[i][j] * v[j];

            return result;
        }

        template <typename T>
        Mat4x4<ElementType> operator*(const Mat4x4<T>& v)
        {
            Mat4x4<ElementType> result = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        result[i][j] += vecs[i][k] * v.vecs[k][j];
            return result;
        }

        Mat4x4() : Mat4x4{ 0, 0, 0, 0 }
        {
            for (int i = 0; i < 4; i++) vecs[i][i] = 1;
        }

        Mat4x4(std::initializer_list<ElementType> list)
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                vecs[t / 4][t % 4] = *i;
                if (t++ == 16) break;
            }
            for (; t < 16; t++) vecs[t / 4][t % 4] = 0;
        }

        Mat4x4(std::initializer_list< Vec4<ElementType> > list)
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                vecs[t] = *i;
                if (t++ == 4) break;
            }
            for (; t < 4; t++) vecs[t] = Vec4<ElementType>();
        }
    };
    
    template <typename ElementType>
    ElementType dot(Vec3<ElementType> u, Vec3<ElementType> v)
    {
        return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
    }

    template <typename ElementType>
    Vec3<ElementType> cross(Vec3<ElementType> u, Vec3<ElementType> v)
    {
        return { u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2],
            u[0] * v[1] - u[1] * v[0] };
    }

    template <typename IndexedVectorType>
    Mat4x4<float> translate(const IndexedVectorType& v)
    {
        return
        {
            1, 0, 0, (float)v[0],
            0, 1, 0, (float)v[1],
            0, 0, 1, (float)v[2],
            0, 0, 0, 1,
        };
    }

    template <typename IndexedVectorType>
    Mat4x4<float> scale(const IndexedVectorType& v)
    {
        return
        {
            (float)v[0], 0, 0, 0,
            0, (float)v[1], 0, 0,
            0, 0, (float)v[2], 0,
            0, 0, 0, 1,
        };
    }

    inline Mat4x4<float> rotate(const Vec3<float>& v, float radian)
    {
        float c = cos(radian);
        float s = sin(radian);
        float c2 = 1 - c;

        Vec3<float> nv = v.normalize(); 

        const float& x = v[0];
        const float& y = v[1];
        const float& z = v[2];

        return
        {
            c + x * x * c2,     x * y * c2 - z * s, x * z * c2 + y * s, 0,
            y * x * c2 + z * s, c + y * y * c2,     y * z * c2 - x * s, 0,
            z * x * c2 - y * s, z * y * c2 + x * s, c + z * z * c2, 0,
            0, 0, 0, 1
        };
    }

    inline Mat4x4<float> lookat(const Vec3<float>& eye,
        const Vec3<float>& ref, const Vec3<float>& up)
    {
        const Vec3<float> w = (eye - ref).normalize();
        const Vec3<float> u = cross(up, w).normalize();
        const Vec3<float> v = cross(w, u);
        return
        {
            Vec4<float>(u, -dot(u, eye)),
            Vec4<float>(v, -dot(v, eye)),
            Vec4<float>(w, -dot(w, eye)),
            Vec4<float>(0, 0, 0, 1)
        };
    }

    inline Mat4x4<float> ortho(float left, float right,
        float bottom, float top, float zNear, float zFar)
    {
        return {
            2 / (right - left), 0, 0, -(right + left) / (right - left),
            0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
            0, 0, -2 / (zFar - zNear), -(zFar + zNear) / (zFar - zNear),
            0, 0, 0, 1
        };
    }

    inline float radian(float degree) { return degree * (PI / 180); }

    inline Mat4x4<float> perspective(float fovy, float aspect,
        float near, float far)
    {
        float top = tan((float)(radian(fovy) / 2.0)), bottom = -top;
        float right = top * aspect, left = -right;
        return {
            2 * near / (right - left), 0, 0, 0,
            0, 2 * near / (top - bottom), 0, 0,
            0, 0, -(far + near) / (far - near), -2 * far * near / (far - near),
            0, 0, -1, 0
        };
    }
    
    template <typename ElementType>
    std::ostream& operator<<(std::ostream &os, Vec4<ElementType> &v)
    {
        os << "[";
        for(int i = 0; i < 3; i++) os << v[i] << ", ";
        os << v[3] << "]";
        return os;
    }

    template <typename ElementType>
    std::ostream& operator<<(std::ostream &os, Mat4x4<ElementType> &mat)
    {
        os << "[";
        for(int i = 0; i < 3; i++) os << mat[i] << ", \n";
        os << mat[3] << "]";
        return os;
    }
    
}