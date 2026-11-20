///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////
//
//  Assignment       COMP4300 - Assignment 4
//  Professor:       David Churchill
//  Year / Term:     2026-09
//  File Name:       Scene_Zelda.cpp
// 
//  Student Name:    Zahra Khan
//  Student User:    zahrak
//  Student Email:   zahrak@mun.ca
//  Student ID:      202252265
//  Group Member(s): [enter student name(s)]
//
///\/\/\\\//\\\/\//\/////\/\//////\/\/\\\//\\\/\//\/////\/\/////

#include "Scene_Zelda.h"
#include "Physics.hpp"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

#include "imgui.h"
#include "imgui-SFML.h"
#include <cmath>
#include <algorithm>
#include <random>

Scene_Zelda::Scene_Zelda(GameEngine& game, const std::string& levelPath)
    : Scene(game)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Zelda::init(const std::string& levelPath)
{
    loadLevel(levelPath);

    m_game.stopSound("MusicTitle");
    m_game.playSound("MusicPlay");

    registerAction(sf::Keyboard::Scancode::P, "PAUSE");
    registerAction(sf::Keyboard::Scancode::Escape, "QUIT");
    registerAction(sf::Keyboard::Scancode::Y, "TOGGLE_FOLLOW");       // Toggle drawing Camera
    registerAction(sf::Keyboard::Scancode::T, "TOGGLE_TEXTURE");      // Toggle drawing (T)extures
    registerAction(sf::Keyboard::Scancode::C, "TOGGLE_COLLISION");    // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::Scancode::G, "TOGGLE_GRID");         // Toggle drawing (G)rid         
    registerAction(sf::Keyboard::Scancode::W, "UP");
    registerAction(sf::Keyboard::Scancode::S, "DOWN");
    registerAction(sf::Keyboard::Scancode::A, "LEFT");
    registerAction(sf::Keyboard::Scancode::D, "RIGHT");
    registerAction(sf::Keyboard::Scancode::Space, "ATTACK");
}

void Scene_Zelda::loadLevel(const std::string& filename)
{
    m_entityManager = EntityManager();
    m_player.reset();

    std::ifstream file(filename);
    std::string type;

    if (!file.is_open())
    {
        throw std::runtime_error("Could not open level file: " + filename);
    }
    bool playerLoaded = false;

    while (file >> type)
    {
        if (type == "Tile")
        {
            std::string name;
            int rx, ry, tx, ty, bm, bv;
            file >> name >> rx >> ry >> tx >> ty >> bm >> bv;

            auto entity = m_entityManager.addEntity("tile");
            entity->add<CAnimation>(Assets::Instance().getAnimation(name), true);

            Vec2f pos = getPosition(rx, ry, tx, ty);
            entity->add<CTransform>(pos, Vec2f(0, 0), Vec2f(1, 1), 0);

            auto size = entity->get<CAnimation>().animation.getRect().size;
            entity->add<CBoundingBox>(Vec2f(size.x, size.y), bm, bv);
        }
        else if (type == "NPC")
        {
            std::string name, ai;
            int rx, ry, tx, ty, bm, bv, health, damage;
            file >> name >> rx >> ry >> tx >> ty >> bm >> bv >> health >> damage >> ai;

            auto entity = m_entityManager.addEntity("npc");
            entity->add<CAnimation>(Assets::Instance().getAnimation(name), true);

            Vec2f pos = getPosition(rx, ry, tx, ty);
            entity->add<CTransform>(pos, Vec2f(0, 0), Vec2f(1, 1), 0);

            auto size = entity->get<CAnimation>().animation.getRect().size;
            entity->add<CBoundingBox>(Vec2f(size.x, size.y), bm, bv);
            entity->add<CHealth>(health, health);
            entity->add<CDamage>(damage);

            if (ai == "Follow")
            {
                float speed;
                file >> speed;
                entity->add<CFollowPlayer>(pos, speed);
            }
            else if (ai == "Patrol")
            {
                float speed;
                int count;
                file >> speed >> count;

                std::vector<Vec2f> positions;

                for (int i = 0; i < count; i++)
                {
                    int x, y;
                    file >> x >> y;
                    positions.push_back(getPosition(rx, ry, x, y));
                }

                entity->add<CPatrol>(positions, speed);
            }
        }
        else if (type == "Player")
        {
            if (!(file >> m_playerConfig.X >> m_playerConfig.Y
                >> m_playerConfig.CX >> m_playerConfig.CY
                >> m_playerConfig.SPEED >> m_playerConfig.HEALTH)
                || m_playerConfig.CX <= 0 || m_playerConfig.CY <= 0
                || m_playerConfig.SPEED < 0 || m_playerConfig.HEALTH <= 0)
            {
                throw std::runtime_error("Invalid player configuration");
            }

            playerLoaded = true;
        }
    }
    if (!playerLoaded)
    {
        throw std::runtime_error("Missing Player configuration");
    }
    spawnPlayer();
}

Vec2f Scene_Zelda::getPosition(int rx, int ry, int tx, int ty) const
{
    float x = rx * float(width()) + tx * m_gridSize.x + m_gridSize.x / 2.0f;
    float y = ry * float(height()) + ty * m_gridSize.y + m_gridSize.y / 2.0f;

    return Vec2f(x, y);
}

void Scene_Zelda::spawnPlayer()
{
    m_player = m_entityManager.addEntity("player");

    m_player->add<CTransform>( Vec2f(m_playerConfig.X, m_playerConfig.Y)) ;
    m_player->add<CAnimation>( Assets::Instance().getAnimation("StandDown"), true);
    m_player->add<CBoundingBox>( Vec2f(m_playerConfig.CX, m_playerConfig.CY), true, false );
    m_player->add<CHealth>( m_playerConfig.HEALTH, m_playerConfig.HEALTH );
    m_player->add<CInput>();
}

void Scene_Zelda::spawnSword()
{
    auto sword = m_entityManager.addEntity("sword");
    sword->add<CTransform>(m_player->get<CTransform>().pos);
    sword->add<CDamage>(1);
    sword->add<CLifeSpan>(10, static_cast<int>(m_currentFrame));
    placeSword(sword);
    m_game.playSound("Slash");
}

void Scene_Zelda::placeSword(const std::shared_ptr<Entity>& sword)
{
    const auto& player = m_player->get<CTransform>();
    const auto& facing = player.facing;
    const std::string anim = facing.y != 0 ? "SwordUp" : "SwordRight";

    if (!sword->has<CAnimation>() || sword->get<CAnimation>().animation.getName() != anim)
    {
        sword->add<CAnimation>(Assets::Instance().getAnimation(anim), true);
        const auto size = sword->get<CAnimation>().animation.getRect().size;
        sword->add<CBoundingBox>(Vec2f(size.x, size.y), false, false);
    }

    auto& t = sword->get<CTransform>();
    t.prevPos = t.pos;
    t.pos = player.pos + facing * m_gridSize.x;
    t.scale = Vec2f( facing.x < 0 ? -1.0f : 1.0f, facing.y > 0 ? -1.0f : 1.0f );
}

void Scene_Zelda::update()
{
    m_entityManager.update();

    if (!m_paused)
    {
        sAI();
        sMovement();
        sStatus();
        sCollision();
        sAnimation();
        sCamera();

        m_currentFrame++;
    }

    sGUI();
}
                    
void Scene_Zelda::sMovement()
{
    if (!m_player) { return; }

    auto& input = m_player->get<CInput>();
    auto& transform = m_player->get<CTransform>();

    const float speed = m_playerConfig.SPEED;
    transform.velocity = Vec2f(0, 0);

    if (input.left && !input.right)
    {
        transform.velocity.x = -speed;
        transform.facing = Vec2f(-1, 0);
    }
    else if (input.right && !input.left)
    {
        transform.velocity.x = speed;
        transform.facing = Vec2f(1, 0);
    }
    else if (input.up && !input.down)
    {
        transform.velocity.y = -speed;
        transform.facing = Vec2f(0, -1);
    }
    else if (input.down && !input.up)
    {
        transform.velocity.y = speed;
        transform.facing = Vec2f(0, 1);
    }
    transform.scale.x = transform.facing.x < 0 ? -1.0f : 1.0f; 

    if (input.shoot && input.canShoot && m_entityManager.getEntities("sword").empty())
    { 
        spawnSword();
      input.canShoot = false;
    }

    for (const auto& entity : m_entityManager.getEntities())
    {
        if (entity->isActive() && entity->has<CTransform>())
        {
            auto& t = entity->get<CTransform>();
            t.prevPos = t.pos;
            t.pos += t.velocity;
        }
    }
}

void Scene_Zelda::syncSwords()
{
    if (!m_player || !m_player->isActive()) { return; }

    for (const auto& sword : m_entityManager.getEntities("sword"))
    {
        if (sword->isActive()) { placeSword(sword); }
    }
}

void Scene_Zelda::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "TOGGLE_FOLLOW") { m_follow = !m_follow; }
        else if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }

        else if (m_player && !m_paused)
        {
            auto& input = m_player->get<CInput>();

            if (action.name() == "UP") { input.up = true; }
            else if (action.name() == "DOWN") { input.down = true; }
            else if (action.name() == "RIGHT") { input.right = true; }
            else if (action.name() == "LEFT") { input.left = true; }
            else if (action.name() == "ATTACK") { input.shoot = true; }
        }
    }
    else if (action.type() == "END" && m_player)
    {
        auto& input = m_player->get<CInput>();
        if (action.name() == "UP") { input.up = false; }
        else if (action.name() == "DOWN") { input.down = false; }
        else if (action.name() == "RIGHT") { input.right = false; }
        else if (action.name() == "LEFT") { input.left = false; }
        else if (action.name() == "ATTACK") { input.shoot = false; input.canShoot = true; }
    }
}

bool Scene_Zelda::hasLineOfSight(const std::shared_ptr<Entity>& npc) const
{
    if (!m_player || !m_player->isActive()) { return false; }

    const auto& start = npc->get<CTransform>().pos;
    const auto& end = m_player->get<CTransform>().pos;

    for (const auto& entity : m_entityManager.getEntities())
    {
        if (!entity->isActive() || entity == npc || entity == m_player) { continue; }
        if (!entity->has<CBoundingBox>()) { continue; }
        if (!entity->get<CBoundingBox>().blockVision) { continue; }

        if (Physics::EntityIntersect(start, end, entity)) { return false; }
    }

    return true;
}

void Scene_Zelda::sAI()
{
    if (!m_player) { return; }

    for (const auto& npc : m_entityManager.getEntities("npc"))
    {
        if (!npc->isActive()) { continue; }

        auto& transform = npc->get<CTransform>();
        transform.velocity = Vec2f(0, 0);

        Vec2f target;
        float speed = 0;
        bool returningHome = false;

        if (npc->has<CPatrol>())
        {
            auto& patrol = npc->get<CPatrol>();

            if (patrol.positions.empty()) { continue; }

            target = patrol.positions[patrol.currentPosition];
            speed = patrol.speed;

            if (transform.pos.dist(target) <= 5.0f)
            {
                patrol.currentPosition = (patrol.currentPosition + 1) % patrol.positions.size();
                target = patrol.positions[patrol.currentPosition];
            }
        }
        else if (npc->has<CFollowPlayer>())
        {
            auto& follow = npc->get<CFollowPlayer>();

            if (hasLineOfSight(npc))
            {
                target = m_player->get<CTransform>().pos;
            }
            else
            {
                target = follow.home;
                returningHome = true;
            }

            speed = follow.speed;
        }
        else 
        { continue; }

        Vec2f direction = target - transform.pos;
        float distance = transform.pos.dist(target);

        if (returningHome && distance <= 5.0f) { continue; }
 
        if (distance > 0)
        {
            transform.velocity = direction / distance * std::min(speed, distance);
        }
    }
}

void Scene_Zelda::sStatus()
{
    for (const auto& entity : m_entityManager.getEntities())
    {
        if (!entity->isActive()) { continue; }

        if (entity->has<CLifeSpan>())
        {
            auto& life = entity->get<CLifeSpan>();

            if (static_cast<int>(m_currentFrame) - life.frameCreated > life.lifespan) 
            { 
                entity->destroy();
            }
        }

        if (entity->has<CInvincibility>())
        {
            auto& invincibility = entity->get<CInvincibility>();
            invincibility.iframes--;

            if (invincibility.iframes <= 0) { entity->remove<CInvincibility>();}
        }
    }
}

void Scene_Zelda::sCollision()
{
    if (!m_player || !m_player->isActive()) { return; }

    const auto& tiles = m_entityManager.getEntities("tile");
    const auto& npcs = m_entityManager.getEntities("npc");

    auto resolveTileCollisions = [&](const std::shared_ptr<Entity>& entity)
        {
            if (!entity->isActive()) { return; }

            auto& transform = entity->get<CTransform>();

            for (const auto& tile : tiles)
            {
                if (!tile->isActive() || !tile->get<CBoundingBox>().blockMove) { continue; }

                Vec2f overlap = Physics::GetOverlap(entity, tile);
                if (overlap.x <= 0 || overlap.y <= 0) { continue; }

                Vec2f previous = Physics::GetPreviousOverlap(entity, tile);
                const auto& tilePos = tile->get<CTransform>().pos;

                bool vertical = previous.y <= 0 && previous.x > 0;
                bool horizontal = previous.x <= 0 && previous.y > 0;

                if (!vertical && !horizontal) { vertical = overlap.y < overlap.x; }

                if (vertical)
                {
                    transform.pos.y += transform.pos.y < tilePos.y ? -overlap.y : overlap.y;
                    transform.velocity.y = 0;
                }
                else
                {
                    transform.pos.x += transform.pos.x < tilePos.x ? -overlap.x : overlap.x;
                    transform.velocity.x = 0;
                }
            }
        };
    resolveTileCollisions(m_player);

    for (const auto& npc : npcs) { resolveTileCollisions(npc); }

    syncSwords();

    // npc/hearts
    for (const auto& tile : tiles)
    {
        if (!tile->isActive()) { continue; }
        if (tile->get<CAnimation>().animation.getName() != "Heart") { continue; }

        for (const auto& npc : npcs)
        {
            if (!npc->isActive()) { continue; }

            Vec2f overlap = Physics::GetOverlap(npc, tile);

            if (overlap.x > 0 && overlap.y > 0) //assuming that enemies with full health can destroy hearts too
            {
                auto& health = npc->get<CHealth>();
                health.current = health.max;
                tile->destroy();
                break;
            }
        }
    }
    // player/npc
    if (!m_player->has<CInvincibility>())
    {
        for (const auto& npc : npcs)
        {
            if (!npc->isActive()) { continue; }

            Vec2f overlap = Physics::GetOverlap(m_player, npc);
            if (overlap.x <= 0 || overlap.y <= 0) { continue; }

            auto& health = m_player->get<CHealth>();
            health.current -= npc->get<CDamage>().damage;

            m_game.playSound("LinkHurt");

            if (health.current <= 0)
            {
                m_game.playSound("LinkDie");

                const CInput held = m_player->get<CInput>();

                m_entityManager.destroyEntities("sword");
                m_player->destroy();
                spawnPlayer();

                m_player->get<CInput>() = held;
                m_player->add<CInvincibility>(30);
                return;
            }

            m_player->add<CInvincibility>(30);
            break;
        }
    }
    for (const auto& sword : m_entityManager.getEntities("sword"))
    {
        if (!sword->isActive() || !sword->has<CDamage>()) { continue; }

        for (const auto& npc : npcs)
        {
            if (!npc->isActive()) { continue; }

            Vec2f overlap = Physics::GetOverlap(sword, npc);
            if (overlap.x <= 0 || overlap.y <= 0) { continue; }

            auto& health = npc->get<CHealth>();
            health.current -= sword->get<CDamage>().damage;

            sword->remove<CDamage>();
            m_game.playSound("EnemyHit");

            if (health.current <= 0) {npc->destroy(); m_game.playSound("EnemyDie"); }

            break;
        }
    }
    // player/tiles
    for (const auto& tile : tiles)
    {
        if (!tile->isActive()) { continue; }
        if (tile->get<CAnimation>().animation.getName() != "Black") { continue; }

        Vec2f overlap = Physics::GetOverlap(m_player, tile);
        Vec2f previous = Physics::GetPreviousOverlap(m_player, tile);

        if (overlap.x <= 0 || overlap.y <= 0) { continue; }
        if (previous.x > 0 && previous.y > 0) { continue; }

        std::vector<std::shared_ptr<Entity>> destinations;

        for (const auto& other : tiles)
        {
            if (!other->isActive() || other == tile) { continue; }
            if (other->get<CAnimation>().animation.getName() == "Black") {destinations.push_back(other); }
        }

        if (destinations.empty()) { break; }

        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<size_t> dist(0, destinations.size() - 1);

        auto& transform = m_player->get<CTransform>();

        transform.pos = destinations[dist(rng)]->get<CTransform>().pos;
        transform.prevPos = transform.pos;
        transform.velocity = Vec2f(0, 0);
        syncSwords();

        break;
    }
}

void Scene_Zelda::sAnimation()
{
    if (m_player && m_player->isActive())
    {
        const auto& transform = m_player->get<CTransform>();
        std::string direction = "Right";

        if (transform.facing.y < 0) { direction = "Up"; }
        else if (transform.facing.y > 0) { direction = "Down"; }

        bool attacking = false;

        for (const auto& sword : m_entityManager.getEntities("sword"))
        {
            if (sword->isActive())   { attacking = true; break; }
        }

        std::string name = "Stand" + direction;

        if (attacking) { name = "Atk" + direction; }
        else if (transform.velocity != Vec2f(0, 0)) { name = "Run" + direction; }

        auto& animation = m_player->get<CAnimation>().animation;

        if (animation.getName() != name)
        {
            m_player->add<CAnimation>( Assets::Instance().getAnimation(name), true);
        }
    }
    for (const auto& entity : m_entityManager.getEntities())
    {
        if (!entity->isActive() || !entity->has<CAnimation>()) { continue; }

        auto& anim = entity->get<CAnimation>();
        anim.animation.update();

        if (!anim.repeat && anim.animation.hasEnded()) { entity->destroy(); }
    }
}

void Scene_Zelda::sCamera()
{
    if (!m_player) { return; }

    sf::View view = m_game.window().getView();
    const auto& pos = m_player->get<CTransform>().pos;

    if (m_follow)
    {
        view.setCenter(pos);
    }
    else
    {
        float w = float(width());
        float h = float(height());

        int rx = int(std::floor(pos.x / w));
        int ry = int(std::floor(pos.y / h));

        view.setCenter(Vec2f(
            rx * w + w / 2.0f,
            ry * h + h / 2.0f
        ));
    }

    m_game.window().setView(view);
}

void Scene_Zelda::onEnd()
{
    m_hasEnded = true;

    m_game.stopSound("MusicPlay");
    m_game.playSound("MusicTitle");
    m_game.changeScene("MENU", nullptr, true);
}

void Scene_Zelda::sRender()
{                    
    m_game.window().clear(sf::Color(255, 192, 122));
    sf::RectangleShape tick({ 1.0f, 6.0f });
    tick.setFillColor(sf::Color::Black);

    sf::Text gridText(Assets::Instance().getFont("Tech"));
    gridText.setCharacterSize(12);
                    
    if (m_drawTextures)
    {
        for ( const auto& e : m_entityManager.getEntities())
        {
            if (!e->isActive()) { continue; }
            auto& transform = e->get<CTransform>();
            sf::Color c = sf::Color::White;
            if (e->has<CInvincibility>())
            {
                c = sf::Color(255, 255, 255, 128);
            }

            if (e->has<CAnimation>())
            {
                sf::Sprite sprite = e->get<CAnimation>().animation.getSprite();
                sprite.setRotation(sf::degrees(transform.angle));
                sprite.setPosition(transform.pos);
                sprite.setScale(transform.scale);
                sprite.setColor(c);

                m_game.window().draw(sprite);
            }
        }

        for (const auto& e : m_entityManager.getEntities())
        {
            if (!e->isActive()) { continue; }
            auto& transform = e->get<CTransform>();
            if (e->has<CHealth>())
            {
                auto& h = e->get<CHealth>();
                Vec2f size(64, 6);
                sf::RectangleShape rect({ size.x, size.y });
                rect.setPosition({ transform.pos.x - 32, transform.pos.y - 48 });
                rect.setFillColor(sf::Color(96, 96, 96));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(2);
                m_game.window().draw(rect);

                float ratio = (float)(h.current) / h.max;
                size.x *= ratio;
                rect.setSize({ size.x, size.y });
                rect.setFillColor(sf::Color(255, 0, 0));
                rect.setOutlineThickness(0);
                m_game.window().draw(rect);

                for (int i = 0; i < h.max; i++)
                {
                    tick.setPosition(rect.getPosition() + sf::Vector2f(i * 64 * 1 / h.max, 0));
                    m_game.window().draw(tick);
                }
            }
        }
    }
        // draw the grid so that students can easily debug
        if (m_drawGrid)
        {
            float leftX = m_game.window().getView().getCenter().x - width() / 2;
            float rightX = leftX + width() + m_gridSize.x;
            float topY = m_game.window().getView().getCenter().y - height() / 2;
            float bottomY = topY + height() + m_gridSize.y;
            float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);
            float nextGridY = topY - ((int)topY % (int)m_gridSize.y);
                    

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                drawLine(Vec2f(x, topY), Vec2f(x, bottomY), sf::Color::White);
            }

            for (float y = nextGridY; y < bottomY; y += m_gridSize.y)
            {
                drawLine(Vec2f(leftX, y), Vec2f(rightX, y), sf::Color::White);

                for (float x = nextGridX; x < rightX; x += m_gridSize.x)
                {
                    std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                    std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                    gridText.setString("(" + xCell + "," + yCell + ")");
                    gridText.setPosition({ x + 3, y + 2 });
                    m_game.window().draw(gridText);
                }
            }
        }
    // draw all Entity collision bounding boxes with a rectangleshape
    if (m_drawCollision)
    {
        sf::CircleShape dot(4);
        dot.setFillColor(sf::Color::Black);
        for (const auto& e : m_entityManager.getEntities())
        {
            if (!e->isActive()) { continue; }
            if (e->has<CBoundingBox>())
            {
                auto& box = e->get<CBoundingBox>();
                auto& transform = e->get<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos);
                rect.setFillColor(sf::Color(0, 0, 0, 0));

                if (box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Black); }
                if (box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::Blue); }
                if (!box.blockMove && box.blockVision) { rect.setOutlineColor(sf::Color::Red); }
                if (!box.blockMove && !box.blockVision) { rect.setOutlineColor(sf::Color::White); }
                rect.setOutlineThickness(1);
                m_game.window().draw(rect);
            }
                    
            if (e->has<CPatrol>())
            {
                auto& patrol = e->get<CPatrol>().positions;
                for (size_t p = 0; p < patrol.size(); p++)
                {
                    dot.setPosition(patrol[p]);
                    m_game.window().draw(dot);
                }
            }
            if (e->has<CFollowPlayer>() && e->isActive())
            {
                sf::Color color = hasLineOfSight(e) ? sf::Color::Green : sf::Color::Red;

                drawLine(e->get<CTransform>().pos,m_player->get<CTransform>().pos, color);

                dot.setPosition(e->get<CFollowPlayer>().home);
                m_game.window().draw(dot);
            }

        }
    }
    if (m_drawMinimap)
    {
        drawMinimap();
    }
}

void Scene_Zelda::drawMinimap()
{
    if (!m_player || !m_player->isActive()) { return; }

    auto& window = m_game.window();
    const sf::View oldView = window.getView();
    window.setView(window.getDefaultView());

    const float roomW = float(width());
    const float roomH = float(height());
    const float scale = 0.16f;
    const float margin = 20.0f;
    const float mapW = roomW * scale;
    const float mapH = roomH * scale;
    const float left = roomW - mapW - margin;
    const float top = roomH - mapH - margin;
    const auto& playerPos = m_player->get<CTransform>().pos;
    const Vec2f roomOrigin(std::floor(playerPos.x / roomW) * roomW,
        std::floor(playerPos.y / roomH) * roomH);

    auto toMap = [&](const Vec2f& relative) { return Vec2f(left + relative.x * scale, top + relative.y * scale); };

    sf::RectangleShape background({ mapW, mapH });
    background.setPosition({ left, top });
    background.setFillColor(sf::Color(235, 205, 155, 230));
    background.setOutlineColor(sf::Color(110, 75, 45));
    background.setOutlineThickness(2.0f);
    window.draw(background);

    sf::RectangleShape marker; 
    for (const auto& entity : m_entityManager.getEntities())
    {
        if (!entity->isActive() || !entity->has<CTransform>()) { continue; }

        const Vec2f relative = entity->get<CTransform>().pos - roomOrigin;
        if (relative.x < 0 || relative.x >= roomW || relative.y < 0 || relative.y >= roomH) { continue; }

        sf::Color color;
        Vec2f size(8.0f, 8.0f);

        if (entity->tag() == "tile")
        {
            const auto& name = entity->get<CAnimation>().animation.getName();
            const auto& box = entity->get<CBoundingBox>();

            if (name == "Heart") { color = sf::Color::Green; }
            else if (name == "Black") { color = sf::Color(180, 100, 255); }
            else if (box.blockMove) { color = sf::Color(64, 89, 70); size = box.size * scale; }
            else { continue; }
        }
        else if (entity->tag() == "npc") { color = sf::Color::Red; }
        else { continue; }   

        marker.setSize(size);
        marker.setOrigin(size / 2.0f);
        marker.setPosition(toMap(relative));
        marker.setFillColor(color);
        window.draw(marker);
    }
    sf::CircleShape playerMarker(6.0f);
    playerMarker.setOrigin({ 6.0f, 6.0f });
    playerMarker.setPosition(toMap(playerPos - roomOrigin));
    playerMarker.setFillColor(sf::Color(113, 150, 122));
    playerMarker.setOutlineColor(sf::Color::White);
    playerMarker.setOutlineThickness(2.0f);
    window.draw(playerMarker);
    window.setView(oldView);
}
void Scene_Zelda::sGUI()
{
    ImGui::Begin("Scene Properties");

    if (ImGui::BeginTabBar("MyTabBar"))
    {
        if (ImGui::BeginTabItem("Debug"))
        {
            ImGui::Checkbox("Draw Grid", &m_drawGrid);
            ImGui::Checkbox("Draw Textures", &m_drawTextures);
            ImGui::Checkbox("Draw Debug", &m_drawCollision);
            ImGui::Checkbox("Follow Cam", &m_follow);
            ImGui::Checkbox("Show Minimap", &m_drawMinimap);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Animations"))
        {
            int count = 0;
            const auto& animations = Assets::Instance().getAnimations();

            for (const auto& [name, animation] : animations)
            {
                ImGui::ImageButton(name.c_str(), animation.getSprite(), sf::Vector2f(32, 32));

                if (ImGui::IsItemHovered()) { ImGui::SetTooltip("%s", name.c_str()); }

                count++;
                if (count % 6 != 0 && count < animations.size()) {ImGui::SameLine(); }
            }
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Entity Manager"))
        {
            ImGui::Text("Total Entities: %zu", m_entityManager.getEntities().size());

            for (const auto& [tag, entities] : m_entityManager.getEntityMap())
            {
                std::string label = tag + " (" + std::to_string(entities.size()) + ")";

                if (ImGui::TreeNode(tag.c_str(), "%s", label.c_str()))
                {
                    for (const auto& entity : entities)
                    {
                        if (!entity->isActive()) { continue; }

                        ImGui::PushID(static_cast<int>(entity->id()));

                        const auto& pos = entity->get<CTransform>().pos;

                        if (entity->has<CAnimation>())
                        {
                            const auto& animation = entity->get<CAnimation>().animation;

                            ImGui::ImageButton("##sprite", animation.getSprite(), sf::Vector2f(32, 32));
                            ImGui::SameLine();

							ImGui::Text("ID %zu | (%.0f, %.0f) | %s", entity->id(), pos.x, pos.y, animation.getName().c_str());
                        }
                        else
                        {
                            ImGui::Text("ID %zu | (%.0f, %.0f)", entity->id(), pos.x, pos.y);
                        }
                        ImGui::PopID();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
    ImGui::End();
}

void Scene_Zelda::drawLine(const Vec2f& p1, const Vec2f& p2, const sf::Color& color)
{
    sf::Vertex line[] = { { p1, color }, { p2, color } };
    m_game.window().draw(line, 2, sf::PrimitiveType::Lines);
}


// Copyright (C) David Churchill - All Rights Reserved
// COMP4300 - 2026-09 - Assignment 4
// Written by David Churchill (dave.churchill@gmail.com)
// Unauthorized copying of these files are strictly prohibited
// Distributed only for course work at Memorial University
// If you see this file online please contact email above
