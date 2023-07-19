#pragma once

#include <initializer_list>
#include <cmath>
#include <iostream>

namespace mmath
{
    constexpr float PI = 3.14159265359f;

    template <typename ElementType>
    struct Vec2
    {
        ElementType x, y;

        
        ElementType L2Norm() const { return x * x + y * y; }
        float length() const { return (float)sqrt((float)L2Norm()); }

        Vec2() noexcept : x(0), y(0) {};
        Vec2(ElementType x) noexcept : x(x), y(x) {};
        Vec2(ElementType x, ElementType y) noexcept : x(x), y(y) {};
    };
    
    template <typename ElementType>
    struct Vec3
    {
        ElementType x, y, z;

        ElementType L2Norm() const { return x * x + y * y + z * z; }
        float length() const { return (float)sqrt((float)L2Norm()); }

        Vec3<float> normalize() const
        {
            float len = length();
            return {x / len, y / len, z / len};
        }

        constexpr ElementType& operator[](const int index)
        {
            switch(index)
            {
                case 0: return x; case 1: return y; case 2: return z;
            }
        }
        constexpr const ElementType& operator[](const int index) const
        {
            switch(index)
            {
                case 0: return x; case 1: return y; case 2: return z;
            }
        }

        template <typename T>
        Vec3<ElementType> operator/(const T rhs) const
        {
            return {x / rhs, y / rhs, z / rhs};   
        }

        template <typename T>
        operator Vec3<T>() { return Vec3<T>(x, y, z); }

        Vec3() noexcept : x(0), y(0), z(0) {};
        Vec3(ElementType x) noexcept : x(x), y(x), z(x) {};
        Vec3(ElementType x, ElementType y, ElementType z = 0) noexcept
            : x(x), y(y), z(z) {};
    };

    template <typename ElementType>
    struct Vec4
    {
        ElementType x, y, z, w;
        
        constexpr ElementType& operator[](const int index)
        {
            switch(index)
            {
                case 0: return x;   case 1: return y;
                case 2: return z;   case 3: return w;

            }
        }
        constexpr const ElementType& operator[](const int index) const
        {
            switch(index)
            {
                case 0: return x;   case 1: return y;
                case 2: return z;   case 3: return w;
            }
        }

        template <typename T>
        Vec4<ElementType> operator/(const T rhs) const
        {
            return {x / rhs, y / rhs, z / rhs, w / rhs};   
        }

        template <typename T>
        operator Vec3<T>() { return Vec3<T>(x, y, z); }

        template <typename T>
        operator Vec4<T>() { return Vec4<T>(x, y, z, w); }

        Vec4() noexcept : x(0), y(0), z(0), w(0) {};
        Vec4(ElementType x) noexcept : x(x),  y(x), z(x), w(x) {};
        Vec4(Vec3<ElementType> v, ElementType w = 0) noexcept
            : x(v.x), y(v.y), z(v.z), w(w) {};
        Vec4(ElementType x, ElementType y,
            ElementType z = 0, ElementType w = 0) noexcept
            : x(x), y(y), z(z), w(w) {};
    };

    template <typename ElementType>
    struct Mat3x3
    {
        Vec3<ElementType> v[3];

        Vec3<ElementType>& operator[](const int index) { return v[index]; }
        const Vec3<ElementType>& operator[](const int index) const { return v[index]; }

        inline ElementType det()
        {
            return v[0][0] * (v[1][1] * v[2][2] - v[2][1] * v[1][2]) -
             v[0][1] * (v[1][0] * v[2][2] - v[1][2] * v[2][0]) +
             v[0][2] * (v[1][0] * v[2][1] - v[1][1] * v[2][0]);
        }

        inline Mat3x3<float> inverse()
        {
            float w = 1.0 / det();
            return
            {
                (v[1][1] * v[2][2] - v[2][1] * v[1][2]) * w,
                (v[0][2] * v[2][1] - v[0][1] * v[2][2]) * w,
                (v[0][1] * v[1][2] - v[0][2] * v[1][1]) * w,
                (v[1][2] * v[2][0] - v[1][0] * v[2][2]) * w,
                (v[0][0] * v[2][2] - v[0][2] * v[2][0]) * w,
                (v[1][0] * v[0][2] - v[0][0] * v[1][2]) * w,
                (v[1][0] * v[2][1] - v[2][0] * v[1][1]) * w,
                (v[2][0] * v[0][1] - v[0][0] * v[2][1]) * w,
                (v[0][0] * v[1][1] - v[1][0] * v[0][1]) * w
            };
        }

        inline Mat3x3<ElementType> transpose()
        {
            return
            {
                v[0][0], v[1][0], v[2][0],
                v[0][1], v[1][1], v[2][1],
                v[0][2], v[1][2], v[2][2]
            };
        }


        template <typename T>
        Vec3<ElementType> operator*(const Vec3<T>& rhs)
        {
            Vec3<ElementType> result;
            for (int i = 0; i < 3; i++)
                for (int j = 0; j < 3; j++)
                    result[i] += v[i][j] * rhs[j];
            
            return result;
        }

        template <typename T>
        Mat3x3<ElementType> operator*(const Mat3x3<T>& rhs)
        {
            Mat3x3<ElementType> result;
            for (int i = 0; i < 3; i++) result[i] = this * rhs[i];
            return result;
        }

        Mat3x3() : Mat3x3{ 0, 0, 0, 0 }
        {
            for (int i = 0; i < 3; i++) v[i][i] = 1;
        }

        Mat3x3(const std::initializer_list<ElementType>& list)
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                v[t / 3][t % 3] = *i;
                if (t++ == 9) break;
            }
            for (; t < 9; t++) v[t / 3][t % 3] = 0;
        }

        Mat3x3(const std::initializer_list< Vec3<ElementType> >& list)
        {
            int t = 0;
            for (const auto& i : list)
            {
                v[t] = i;
                if (t++ == 3) break;
            }
            for (; t < 3; t++) v[t] = Vec3<ElementType>();
        }
    };

    template <typename ElementType>
    struct Mat4x4
    {
        Vec4<ElementType> v[4];

        Vec4<ElementType>& operator[](const int index) { return v[index]; }
        const Vec4<ElementType>& operator[](const int index) const { return v[index]; }
        
        inline Mat4x4<ElementType> transpose()
        {
            return
            {
                v[0][0], v[1][0], v[2][0], v[3][0], 
                v[0][1], v[1][1], v[2][1], v[3][1], 
                v[0][2], v[1][2], v[2][2], v[3][2], 
                v[0][3], v[1][3], v[2][3], v[3][3], 
            };
        }

        template <typename T>
        Vec4<ElementType> operator*(const Vec4<T>& rhs)
        {
            Vec4<ElementType> result;
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    result[i] += v[i][j] * rhs[j];

            return result;
        }

        template <typename T>
        Mat4x4<ElementType> operator*(const Mat4x4<T>& rhs)
        {
            Mat4x4<ElementType> result = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    for (int k = 0; k < 4; k++)
                        result[i][j] += v[i][k] * rhs.v[k][j];
            return result;
        }

        template <typename T>
        operator Mat3x3<T>()
        {
            return {
                mmath::Vec3<T>(v[0]), mmath::Vec3<T>(v[1]), mmath::Vec3<T>(v[2]) 
            };
        }

        Mat4x4() noexcept : Mat4x4{ 0, 0, 0, 0 }
        {
            for (int i = 0; i < 4; i++) v[i][i] = 1;
        }

        Mat4x4(std::initializer_list<ElementType> list) noexcept
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                v[t / 4][t % 4] = *i;
                if (t++ == 16) break;
            }
            for (; t < 16; t++) v[t / 4][t % 4] = 0;
        }

        Mat4x4(std::initializer_list< Vec4<ElementType> > list) noexcept
        {
            int t = 0;
            for (auto i = list.begin(); i != list.end(); i++)
            {
                v[t] = *i;
                if (t++ == 4) break;
            }
            for (; t < 4; t++) v[t] = Vec4<ElementType>();
        }
    };

    /***** vector operation *****/

    template <typename ElementType>
    inline Vec2<ElementType> mult_element_wise(
        Vec2<ElementType> u, Vec2<ElementType> v)
    {
        return { u.x * v.x, u.y * v.y };
    }

    template <typename ElementType>
    inline Vec3<ElementType> mult_element_wise(
        Vec3<ElementType> u, Vec3<ElementType> v)
    {
        return { u.x * v.x, u.y * v.y, u.z * v.z };
    }
    
    template <typename ElementType>
    inline Vec4<ElementType> mult_element_wise(
        Vec4<ElementType> u, Vec4<ElementType> v)
    {
        return { u.x * v.x, u.y * v.y, u.z * v.z, u.w * v.w};
    }

    template <typename ElementType>
    ElementType dot(Vec2<ElementType> u, Vec2<ElementType> v)
    {
        return u[0] * v[0] + u[1] * v[1];
    }

    template <typename ElementType>
    ElementType dot(Vec3<ElementType> u, Vec3<ElementType> v)
    {
        return u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
    }

    template <typename ElementType>
    ElementType dot(Vec4<ElementType> u, Vec4<ElementType> v)
    {
        return u[0] * v[0] + u[1] * v[1] + u[2] * v[2] + u[3] * v[3];
    }

    template <typename VectorType>
    VectorType reflect(VectorType I, VectorType N)
    {
        return I - 2.0f * dot(N, I) * N;
    }

    // Returns the z elements of cross product result vector.
    template <typename ElementType>
    float cross(Vec2<ElementType> u, Vec2<ElementType> v)
    {
        return u.x * v.y - u.y * v.x;
    }

    template <typename ElementType>
    Vec3<float> cross(Vec3<ElementType> u, Vec3<ElementType> v)
    {
        return {
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        };
    }


    /***** definition of operator about vectors *****/
    template <typename T>
    Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y};
    }
    
    template <typename T>
    Vec3<T> operator+(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }
    
    template <typename T>
    Vec4<T> operator+(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w};
    }

    template <typename T>
    Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y};
    }
    
    template <typename T>
    Vec3<T> operator-(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }
    
    template <typename T>
    Vec4<T> operator-(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w};
    }

    template <typename T>
    Vec2<T> operator*(const T scalar, const Vec2<T>& v)
    {
        return {scalar * v.x, scalar * v.y};
    }
    
    template <typename T>
    Vec3<T> operator*(const T scalar, const Vec3<T>& v)
    {
        return {scalar * v.x, scalar * v.y, scalar * v.z};
    }
    
    template <typename T>
    Vec4<T> operator*(const T scalar, const Vec4<T>& v)
    {
        return {scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w};
    }

    template <typename T>
    Vec2<T> operator*(const Vec2<T>& v, const T scalar)
    {
        return {scalar * v.x, scalar * v.y};
    }
    
    template <typename T>
    Vec3<T> operator*(const Vec3<T>& v, const T scalar)
    {
        return {scalar * v.x, scalar * v.y, scalar * v.z};
    }
    
    template <typename T>
    Vec4<T> operator*(const Vec4<T>& v, const T scalar)
    {
        return {scalar * v.x, scalar * v.y, scalar * v.z, scalar * v.w};
    }


    /***** transform matrices ****/

    inline Mat4x4<float> translate(const Vec3<float>& v)
    {
        return
        {
            1, 0, 0, (float)v[0],
            0, 1, 0, (float)v[1],
            0, 0, 1, (float)v[2],
            0, 0, 0, 1,
        };
    }

    inline Mat4x4<float> scale(const Vec3<float>& v)
    {
        return
        {
            (float)v[0], 0, 0, 0,
            0, (float)v[1], 0, 0,
            0, 0, (float)v[2], 0,
            0, 0, 0, 1,
        };
    }

    inline Mat4x4<float> rotate(const Vec3<float>& v, const float radian)
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
        float h = near * tan(radian(fovy / 2.0));
        float w = h * aspect;
        float d = (far - near);
        return {
            near / w, 0, 0, 0,
            0, near / h, 0, 0,
            0, 0, -(far + near) / d, -2 * far * near / d,
            0, 0, -1, 0
        };
    }

    template <typename ElementType>
    ElementType interpolate(ElementType a, ElementType b, ElementType c, Vec3<float> weight)
    {
        return a * weight[0] + b * weight[1] + c * weight[2];
    }
    
    template <typename ElementType>
    ElementType interpolate(ElementType a, ElementType b, float a_w, float b_w)
    {
        return a * a_w + b * b_w;
    }


    // function for debug(print in console)

    template <typename ElementType>
    std::ostream& operator<<(std::ostream &os, const Vec3<ElementType> &v)
    {
        return os << "[" << v[0] << ", " << v[1] << ", " << v[2] << "]";
    }
    
    template <typename ElementType>
    std::ostream& operator<<(std::ostream &os, const Vec4<ElementType> &v)
    {
        os << "[";
        for(int i = 0; i < 3; i++) os << v[i] << ", ";
        os << v[3] << "]";
        return os;
    }

    template <typename ElementType>
    std::ostream& operator<<(std::ostream &os, const Mat4x4<ElementType> &mat)
    {
        os << "[";
        for(int i = 0; i < 3; i++) os << mat[i] << ", \n";
        os << mat[3] << "]";
        return os;
    }    
}