#include "charactercollision.h"

CharacterCollision::CharacterCollision(const b2Vec2 &gravity, QObject *parent) :
    QBox2DTest(gravity, parent)
{
    // Ground body
    {
        b2BodyDef bd;
        QBox2DBody* ground = createBody(&bd);

        b2EdgeShape shape;
        shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
        ground->createFixture(&shape, 0.0f);
    }

    // Collinear edges with no adjacency information.
    // This shows the problematic case where a box shape can hit
    // an internal vertex.
    {
        b2BodyDef bd;
        QBox2DBody* ground = createBody(&bd);

        b2EdgeShape shape;
        shape.Set(b2Vec2(-8.0f, 1.0f), b2Vec2(-6.0f, 1.0f));
        ground->createFixture(&shape, 0.0f);
        shape.Set(b2Vec2(-6.0f, 1.0f), b2Vec2(-4.0f, 1.0f));
        ground->createFixture(&shape, 0.0f);
        shape.Set(b2Vec2(-4.0f, 1.0f), b2Vec2(-2.0f, 1.0f));
        ground->createFixture(&shape, 0.0f);
    }

    // Chain shape
    {
        b2BodyDef bd;
        bd.angle = 0.25f * b2_pi;
        QBox2DBody* ground = createBody(&bd);

        b2Vec2 vs[4];
        vs[0].Set(5.0f, 7.0f);
        vs[1].Set(6.0f, 8.0f);
        vs[2].Set(7.0f, 8.0f);
        vs[3].Set(8.0f, 7.0f);
        b2ChainShape shape;
        shape.CreateChain(vs, 4);
        ground->createFixture(&shape, 0.0f);
    }

    // Square tiles. This shows that adjacency shapes may
    // have non-smooth collision. There is no solution
    // to this problem.
    {
        b2BodyDef bd;
        QBox2DBody* ground = createBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 1.0f, b2Vec2(4.0f, 3.0f), 0.0f);
        ground->createFixture(&shape, 0.0f);
        shape.SetAsBox(1.0f, 1.0f, b2Vec2(6.0f, 3.0f), 0.0f);
        ground->createFixture(&shape, 0.0f);
        shape.SetAsBox(1.0f, 1.0f, b2Vec2(8.0f, 3.0f), 0.0f);
        ground->createFixture(&shape, 0.0f);
    }

    // Square made from an edge loop. Collision should be smooth.
    {
        b2BodyDef bd;
        QBox2DBody* ground = createBody(&bd);

        b2Vec2 vs[4];
        vs[0].Set(-1.0f, 3.0f);
        vs[1].Set(1.0f, 3.0f);
        vs[2].Set(1.0f, 5.0f);
        vs[3].Set(-1.0f, 5.0f);
        b2ChainShape shape;
        shape.CreateLoop(vs, 4);
        ground->createFixture(&shape, 0.0f);
    }

    // Edge loop. Collision should be smooth.
    {
        b2BodyDef bd;
        bd.position.Set(-10.0f, 4.0f);
        QBox2DBody* ground = createBody(&bd);

        b2Vec2 vs[10];
        vs[0].Set(0.0f, 0.0f);
        vs[1].Set(6.0f, 0.0f);
        vs[2].Set(6.0f, 2.0f);
        vs[3].Set(4.0f, 1.0f);
        vs[4].Set(2.0f, 2.0f);
        vs[5].Set(0.0f, 2.0f);
        vs[6].Set(-2.0f, 2.0f);
        vs[7].Set(-4.0f, 3.0f);
        vs[8].Set(-6.0f, 2.0f);
        vs[9].Set(-6.0f, 0.0f);
        b2ChainShape shape;
        shape.CreateLoop(vs, 10);
        ground->createFixture(&shape, 0.0f);
    }

    // Square character 1
    {
        b2BodyDef bd;
        bd.position.Set(-3.0f, 8.0f);
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true;
        bd.allowSleep = false;

        QBox2DBody* body = createBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(0.5f, 0.5f);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 20.0f;
        body->createFixture(&fd);
    }

    // Square character 2
    {
        b2BodyDef bd;
        bd.position.Set(-5.0f, 5.0f);
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true;
        bd.allowSleep = false;

        QBox2DBody* body = createBody(&bd);

        b2PolygonShape shape;
        shape.SetAsBox(0.25f, 0.25f);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 20.0f;
        body->createFixture(&fd);
    }

    // Hexagon character
    {
        b2BodyDef bd;
        bd.position.Set(-5.0f, 8.0f);
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true;
        bd.allowSleep = false;

        QBox2DBody* body = createBody(&bd);

        float32 angle = 0.0f;
        float32 delta = b2_pi / 3.0f;
        b2Vec2 vertices[6];
        for (int32 i = 0; i < 6; ++i)
        {
            vertices[i].Set(0.5f * cosf(angle), 0.5f * sinf(angle));
            angle += delta;
        }

        b2PolygonShape shape;
        shape.Set(vertices, 6);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 20.0f;
        body->createFixture(&fd);
    }

    // Circle character
    {
        b2BodyDef bd;
        bd.position.Set(3.0f, 5.0f);
        bd.type = b2_dynamicBody;
        bd.fixedRotation = true;
        bd.allowSleep = false;

        QBox2DBody* body = createBody(&bd);

        b2CircleShape shape;
        shape.m_radius = 0.5f;

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 20.0f;
        body->createFixture(&fd);
    }

    // Circle character
    {
        b2BodyDef bd;
        bd.position.Set(-7.0f, 6.0f);
        bd.type = b2_dynamicBody;
        bd.allowSleep = false;

        m_character = createBody(&bd);

        b2CircleShape shape;
        shape.m_radius = 0.25f;

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.density = 20.0f;
        fd.friction = 1.0f;
        m_character->createFixture(&fd);
    }
}