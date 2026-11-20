///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Physics.hpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////


#pragma once

#include "Entity.hpp"
#include <cmath>


struct Intersect 
{ 
    bool result = false; 
    Vec2f pos; 
};
                    
namespace Physics
{    
    inline Vec2f GetOverlap( const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
    {
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>()) { return Vec2f(0, 0); }
        auto& aPos = a->get<CTransform>().pos;
        auto& bPos = b->get<CTransform>().pos;
        auto& aHalf = a->get<CBoundingBox>().halfSize;
        auto& bHalf = b->get<CBoundingBox>().halfSize;

        float overlapX = aHalf.x + bHalf.x - std::abs(aPos.x - bPos.x);
        float overlapY = aHalf.y + bHalf.y - std::abs(aPos.y - bPos.y);

        return Vec2f(overlapX, overlapY);
    }

    inline Vec2f GetPreviousOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
    {
        if (!a->has<CBoundingBox>() || !b->has<CBoundingBox>()) { return Vec2f(0, 0); }

        auto& aPos = a->get<CTransform>().prevPos;
        auto& bPos = b->get<CTransform>().prevPos;
        auto& aHalf = a->get<CBoundingBox>().halfSize;
        auto& bHalf = b->get<CBoundingBox>().halfSize;

        float overlapX = aHalf.x + bHalf.x - std::abs(aPos.x - bPos.x);
        float overlapY = aHalf.y + bHalf.y - std::abs(aPos.y - bPos.y);

        return Vec2f(overlapX, overlapY);
    }

    inline bool IsInside(const Vec2f& pos, const std::shared_ptr<Entity>& e)
    {
        if (!e->has<CBoundingBox>()) { return false; }

        const auto& ePos = e->get<CTransform>().pos;
        const auto& eHalf = e->get<CBoundingBox>().halfSize;

        return std::abs(pos.x - ePos.x) < eHalf.x &&
            std::abs(pos.y - ePos.y) < eHalf.y;
    }

    inline Intersect LineIntersect(const Vec2f& a, const Vec2f& b, const Vec2f& c, const Vec2f& d)
    {
        Vec2f r = b - a;
        Vec2f s = d - c;

        float rxs = r * s;
        if (std::abs(rxs) < 0.00001f) { return {}; }

        Vec2f ca = c - a;

        float t = (ca * s) / rxs;
        float u = (ca * r) / rxs;

        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) { return { true, a + r * t }; }

        return {};
    }

         
    inline bool EntityIntersect(const Vec2f& a, const Vec2f& b, const std::shared_ptr<Entity>& e)
    {
        if (!e->has<CBoundingBox>()) { return false; }
        if (IsInside(a, e) || IsInside(b, e)) { return true; }

        const auto& pos = e->get<CTransform>().pos;
        const auto& half = e->get<CBoundingBox>().halfSize;

        Vec2f tl(pos.x - half.x, pos.y - half.y);
        Vec2f tr(pos.x + half.x, pos.y - half.y);
        Vec2f bl(pos.x - half.x, pos.y + half.y);
        Vec2f br(pos.x + half.x, pos.y + half.y);

        return LineIntersect(a, b, tl, tr).result || LineIntersect(a, b, tr, br).result || LineIntersect(a, b, br, bl).result || LineIntersect(a, b, bl, tl).result;
    }

}



// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
