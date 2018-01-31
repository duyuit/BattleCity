#include "Tank.h"
#define SCALE_RATIO 32.0


Tank::Tank(b2World* world)
{
	this->initWithFile("tank.png");
	this->setScale(3, 3);
	this->setPosition(100, 200);
	bodyShape.SetAsBox((this->getContentSize().width+5)/SCALE_RATIO, (this->getContentSize().height+5) / SCALE_RATIO);
	fixtureDef.density = 10; // Trọng lượng
	fixtureDef.friction = 0.8; // Ma sát
	fixtureDef.restitution = 0.6; // Đàn hồi
	fixtureDef.shape = &bodyShape; // Trỏ vào bodyShape

	bodyDef.type = b2_dynamicBody; // Va chạm động
	bodyDef.fixedRotation=true;
	bodyDef.userData = this; // gắn với Sprite ball

							 // Đặt vị trí, và nhớ chuyển đổi đơn vị
	bodyDef.position.Set(this->getPosition().x / SCALE_RATIO, this->getPosition().y / SCALE_RATIO);
	tankBody = world->CreateBody(&bodyDef); // Tạo Body
	tankBody->CreateFixture(&fixtureDef); // Tạo các thuộc tính tĩnh

	this->world = world;
}

void Tank::update(float delta)
{
	if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW])
		this->TurnLeft();
	else if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW])
		this->TurnRight();
	else if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW])
		this->TurnUp();
	else if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW])
		this->TurnDown();
	else this->mcurState = IDLE;
	if (keys[EventKeyboard::KeyCode::KEY_SPACE])
	{
		if(GetTickCount()- lastFight>=500)
		this->Fight();
	}
	switch (mcurState) {
	case IDLE:
		tankBody->SetLinearVelocity(b2Vec2(0, 0));
		break;
	case LEFT:
		tankBody->SetLinearVelocity(b2Vec2(-100 / SCALE_RATIO, 0));
		break;
	case RIGHT:
		tankBody->SetLinearVelocity(b2Vec2(100 / SCALE_RATIO, 0));
		break;
	case UP:
		tankBody->SetLinearVelocity(b2Vec2(0, 100 / SCALE_RATIO));
		break;
	case DOWN:
		tankBody->SetLinearVelocity(b2Vec2(0, -100 / SCALE_RATIO));
		break;
	default: break; }
}

void Tank::AddPos(float x,float y)
{
	this->setPosition(this->getPosition() + Vec2(x,y));
}

void Tank::TurnLeft()
{
	if (mcurState == LEFT) return;
	mcurState = LEFT;
	this->setRotation(-90);
}

void Tank::TurnRight()
{
	if (mcurState == RIGHT) return;
	mcurState = RIGHT;
	this->setRotation(90);
}

void Tank::TurnUp()
{
	if (mcurState == UP) return;
	mcurState = UP;
	this->setRotation(0);
}

void Tank::TurnDown()
{
	if (mcurState == DOWN) return;
	mcurState = DOWN;
	this->setRotation(180);
}

void Tank::Fight()
{
	Sprite* bullet = Sprite::create("60016.png", Rect(323, 102, 3, 4));
	bullet->setScale(3, 3);

	b2Body* bullet_body;
	b2PolygonShape bullet_bodyshape;
	b2BodyDef bullet_bodydef;
	b2FixtureDef bullet_fixtureDef;



	bullet_bodyshape.SetAsBox(5 / SCALE_RATIO, 5/ SCALE_RATIO);

	bullet_fixtureDef.density = 10; // Trọng lượng
	bullet_fixtureDef.friction = 0.8; // Ma sát
	bullet_fixtureDef.restitution = 0.6; // Đàn hồi
	bullet_fixtureDef.shape = &bullet_bodyshape; // Trỏ vào bodyShape

	bullet_bodydef.type = b2_dynamicBody; // Va chạm động
	bullet_bodydef.fixedRotation = true;
	bullet_bodydef.userData = bullet;
	//bullet_bodydef.position.Set((this->getPosition().x+100)/SCALE_RATIO, this->getPosition().y/SCALE_RATIO);

	


	

	this->getParent()->addChild(bullet);
	int x = this->getPosition().x;
	switch ((int)this->getRotation()) 
	{ 
	case 90:
		bullet_bodydef.position.Set((x + 40)/ SCALE_RATIO, this->getPosition().y / SCALE_RATIO);
		bullet_body = this->world->CreateBody(&bullet_bodydef); // Tạo Body
		bullet_body->CreateFixture(&bullet_fixtureDef);
		bullet_body->SetLinearVelocity(b2Vec2(300 / SCALE_RATIO, 0));
		break;
	case 180:
		bullet_bodydef.position.Set(x / SCALE_RATIO, (this->getPosition().y- 35) / SCALE_RATIO);
		bullet_body = this->world->CreateBody(&bullet_bodydef); // Tạo Body
		bullet_body->CreateFixture(&bullet_fixtureDef);
		bullet_body->SetLinearVelocity(b2Vec2(0, -300 / SCALE_RATIO));
		break;
	case -90:
		bullet_bodydef.position.Set((x- 35) / SCALE_RATIO, this->getPosition().y / SCALE_RATIO);
		bullet_body = this->world->CreateBody(&bullet_bodydef); // Tạo Body
		bullet_body->CreateFixture(&bullet_fixtureDef);
		bullet_body->SetLinearVelocity(b2Vec2(-300 / SCALE_RATIO, 0));
		break;
	case 0:
		bullet_bodydef.position.Set(x / SCALE_RATIO, (this->getPosition().y+35) / SCALE_RATIO);
		bullet_body = this->world->CreateBody(&bullet_bodydef); // Tạo Body
		bullet_body->CreateFixture(&bullet_fixtureDef);
		bullet_body->SetLinearVelocity(b2Vec2(0,300 / SCALE_RATIO));
		break;
	default: ;
	}
	lastFight = GetTickCount();
	bullet->setRotation(this->getRotation());
	
}


Tank::~Tank()
{
}
