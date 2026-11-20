///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Vec2.hpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>

template <typename T>
class Vec2
{
public:
                    
    T x = 0;
    T y = 0;

    Vec2() = default;
                    
    Vec2(T xin, T yin)
        : x(xin), y(yin)
    { }

    // constructor to convert from sf::Vector2
    Vec2(const sf::Vector2<T>& vec)
        : x(vec.x), y(vec.y)
    { }

                    
    // allow automatic conversion to sf::Vector2
    // this lets us pass Vec2f into sfml functions
    operator sf::Vector2<T>() const
    {
        return sf::Vector2<T>(x, y);
    }
                    
    Vec2 operator + (const Vec2& rhs) const
    {
        return Vec2(x + rhs.x, y + rhs.y);
    }

    Vec2 operator - (const Vec2& rhs) const
    {
        return Vec2(x - rhs.x, y - rhs.y);
    }

    Vec2 operator / (const T val) const
    {
        return Vec2(x / val, y / val);
    }

    Vec2 operator * (const T val) const
    {
        return Vec2(x * val, y * val);
    }

    T operator * (const Vec2& rhs) const
    {
        return x * rhs.y - rhs.x * y;
    }

    bool operator == (const Vec2& rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }
                    
    bool operator != (const Vec2& rhs) const
    {
        return !(*this == rhs);
    }

    void operator += (const Vec2& rhs)
    {
        x += rhs.x; y += rhs.y;
    }
                    
    void operator -= (const Vec2& rhs)
    {
        x -= rhs.x; y -= rhs.y;
    }
                    
    void operator *= (const T val)
    {
        x *= val; y *= val;
    }

    void operator /= (const T val)
    {
        x /= val; y /= val;
    }
                    
    float dist(const Vec2& rhs) const
    {
        return sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
    }

    Vec2 abs() const
    {
        return Vec2(x < 0 ? -x : x, y < 0 ? -y : y);
    }
};
                    
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;

// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
