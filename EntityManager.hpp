///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       EntityManager.hpp
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

#include <memory>
#include <vector>
#include <algorithm>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;
                    
class EntityManager
{
    EntityVec   m_entities;
    EntityVec   m_entitiesToAdd;
    EntityMap   m_entityMap;
    size_t      m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec)
    {
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [](auto& e) { return !e->isActive(); }), vec.end());
    }

public:

    EntityManager() = default;

    void update()
    {
        // add all the entities that are pending
        for (auto e : m_entitiesToAdd)
        {
            m_entities.push_back(e);
            m_entityMap[e->tag()].push_back(e);
        }

        m_entitiesToAdd.clear();

        // remove dead entities from the vector of all entities
        removeDeadEntities(m_entities);

        // remove dead entities from each vector in the entity map
        // C++20 way of iterating through [key,value] pairs in a map
        for (auto& [tag, entityVec] : m_entityMap)
        {
            removeDeadEntities(entityVec);
        }
    }

    std::shared_ptr<Entity> addEntity(const std::string& tag)
    {
        auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
        m_entitiesToAdd.push_back(entity);
        return entity;
    }

    const EntityVec& getEntities() const
    {
        return m_entities;
    }

    const EntityVec& getEntities(const std::string& tag)
    {
        if (m_entityMap.find(tag) == m_entityMap.end()) { m_entityMap[tag] = EntityVec(); }
        return m_entityMap[tag];
    }

    const EntityMap& getEntityMap()
    {
        return m_entityMap;
    }

    void destroyEntities(const std::string& tag)
    {
        for (const auto& e : m_entitiesToAdd)
        {
            if (e->tag() == tag) { e->destroy(); }
        }

        auto it = m_entityMap.find(tag);
        if (it != m_entityMap.end())
        {
            for (const auto& e : it->second) { e->destroy(); }
        }
    }
};


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
