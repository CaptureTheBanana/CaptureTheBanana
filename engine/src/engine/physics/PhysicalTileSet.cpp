// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <parser/DynamicTilestore.hpp>

#include "engine/physics/PhysicalTileSet.hpp"

namespace ctb {
namespace engine {

PhysicalTileSet::PhysicalTileSet(parser::TilesetConfig& layc,
                                 parser::LevelConfig* levc,
                                 bool flipped)
    : TilesetRenderable(layc, levc, flipped) {}

void PhysicalTileSet::addToWorld(b2World& world, Kinematics& kinematics) {
    b2BodyDef boxdef;

    boxdef.type = b2_staticBody;

    b2PolygonShape shapeBox;

    // length from center
    shapeBox.SetAsBox(convertToWorldCoordinate(m_tiles->getTileWidth() / 2.0),
                      convertToWorldCoordinate(m_tiles->getTileHeight() / 2.0));

    // second step create body

    // third step create shape

    // create fixture
    b2FixtureDef boxFixture;

    boxFixture.shape = &shapeBox;

    // Set main attributes
    //[in kg/m^2]
    boxFixture.density = kinematics.getDensity();
    //[0-1]
    boxFixture.friction = kinematics.getFriction();
    //[0-1]
    boxFixture.restitution = kinematics.getRestitution();

    boxFixture.filter.maskBits = kinematics.getMask();
    boxFixture.filter.categoryBits = kinematics.getCategory();

    // start and end Point to lay an Edge over it
    //-1 is an invalid Point to check if it has to reset start/end
    float32 start = -1.0f;
    float32 end = -1.0f;

    // Create body for every tile in the set

    // Iterate through the Tileset
    for (int i = 0; i < m_tiles->getHeight(); i++) {
        float32 y =
            convertToWorldCoordinate(i * m_tiles->getTileHeight() + m_tiles->getTileHeight() / 2.0);
        float32 y_ground = convertToWorldCoordinate(i * m_tiles->getTileHeight() + 0.1);

        for (int j = 0; j < m_tiles->getWidth(); j++) {
            if (m_tiles->get(j, i)) {
                // compute temporary end Point
                end = float32(j * m_tiles->getTileWidth());

                // set start if not set jet
                if (start == -1.0f) {
                    start = float32(j * m_tiles->getTileWidth());
                    // create an edge if last tile
                } else if (j == m_tiles->getWidth() - 1 && start != -1) {
                    createNewEdge(start, end, world, kinematics, i * m_tiles->getTileHeight());
                    start = -1;
                }
                if (j == m_tiles->getWidth()) {
                    start = -1;
                }

                boxdef.position.Set(convertToWorldCoordinate(j * m_tiles->getTileWidth() +
                                                             m_tiles->getTileWidth() / 2.0),
                                    y);
                b2Body* body = world.CreateBody(&boxdef);
                body->CreateFixture(&boxFixture);

                boxdef.position.Set(
                    convertToWorldCoordinate(j * m_tiles->getTileWidth() +
                                             m_tiles->getTileWidth() / 2.0),
                    /*convertToWorldCoordinate(i * m_tiles->getTileHeight() + 0.1)*/ y_ground);
            } else {
                // if no tile, but start (and automatically end) is set valid create an edge
                if (start != -1.0f) {
                    end = float32(j * m_tiles->getTileWidth());
                    createNewEdge(start, end, world, kinematics, i * m_tiles->getTileHeight());
                    start = -1.0f;
                }
            }
        }
    }
    setCollisionId(kinematics.getId());
}

void PhysicalTileSet::createNewEdge(float32 start,
                                    float32 end,
                                    b2World& world,
                                    Kinematics& kinematics,
                                    int height) {
    start = convertToWorldCoordinate(start + 0.5f);
    end = convertToWorldCoordinate(end - 0.5f);

    b2BodyDef boxdef1;
    boxdef1.type = b2_staticBody;

    b2PolygonShape groundBox1;

    groundBox1.SetAsBox((end - start) / 2, convertToWorldCoordinate(0.2));

    b2FixtureDef groundFixture1;
    groundFixture1.shape = &groundBox1;

    //[in kg/m^2]
    groundFixture1.density = kinematics.getDensity();
    //[0-1]
    groundFixture1.friction = kinematics.getFriction();
    //[0-1]
    groundFixture1.restitution = kinematics.getRestitution();
    groundFixture1.filter.maskBits = kinematics.getMask();
    groundFixture1.filter.categoryBits = kinematics.getCategory();

    boxdef1.position.Set(start + (end - start) / 2, convertToWorldCoordinate(height));
    b2Body* body1 = world.CreateBody(&boxdef1);
    body1->CreateFixture(&groundFixture1);
    body1->SetUserData(this);
}

}  // namespace engine
}  // namespace ctb
