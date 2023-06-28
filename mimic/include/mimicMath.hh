#pragma once

#include <initializer_list>

namespace mmath
{
    template <typename ElementType>
    struct Vec3
    {
        ElementType elements[3];

        ElementType& operator[](const int index) { return elements[index]; }
        const ElementType& operator[](const int index) const { return elements[index]; }

        template <typename T>
        operator Vec3<T>() { return Vec3<T>(elements[0], elements[1], elements[2]); }

        Vec3() : elements{0, 0, 0} {};
        Vec3(ElementType x) : elements{ x, x, x } {};
        Vec3(ElementType x, ElementType y, ElementType z = 0) : elements{ x, y, z } {};
    };

    template <typename ElementType>
    struct Vec4
    {
        ElementType elements[4];

        ElementType& operator[](const int index) { return elements[index]; }
        const ElementType& operator[](const int index) const { return elements[index]; }

        template <typename T>
        operator Vec3<T>() { return Vec3<T>(elements[0], elements[1], elements[2]); }

        template <typename T>
        operator Vec4<T>() { return Vec4<T>(elements[0], elements[1], elements[2], elements[3]); }

        Vec4() : elements{ 0, 0, 0, 0 } {};
        Vec4(ElementType x) : elements{x, x, x, x} {};
        Vec4(Vec3<ElementType> v, ElementType a) : elements{ v[0], v[1], v[2], a } {};
        Vec4(ElementType x, ElementType y, ElementType z = 0, ElementType a = 0) : elements{ x, y, z, a } {};
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
            Mat4x4<ElementType> result;
            for (int i = 0; i < 4; i++) result[i] = this * v[i];
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
}