#pragma once
#include "2d/CCSprite.h"
#include "Box2D/Box2D.h"
using namespace cocos2d;

class Tank:public Sprite
{
public:
	enum STATE
	{
		IDLE,
		LEFT,
		RIGHT,
		UP,
		DOWN
	};
	Tank(b2World* world);
	STATE mcurState = LEFT;
	std::map<EventKeyboard::KeyCode, bool> keys;
	float lastFight = 0;

	b2Body *tankBody; // Body của bóng
	b2BodyDef bodyDef; // Định nghĩa cái Body trên
	b2FixtureDef fixtureDef; // Định nghĩa một số thuộc tính tĩnh: ma sát, độ đàn hồi, trọng lượng,v.v.
	b2PolygonShape bodyShape; // Hình khối của body

	b2World *world;

	void update(float delta) override;
	void AddPos(float x, float y);
	void TurnLeft();
	void TurnRight();
	void TurnUp();
	void TurnDown();
	void Fight();
	~Tank();
};

