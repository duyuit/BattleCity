#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32/Tank.h"

USING_NS_CC;
#define SCALE_RATIO 32.0


Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
	auto _tileMap = new TMXTiledMap();
	//Khai bao Map
	{
		_tileMap->setPosition(Vec2(0,visibleSize.height));
		_tileMap->setAnchorPoint(Vec2(0,1));
		_tileMap->initWithTMXFile("untitled.tmx");
		_tileMap->getLayer("Tile Layer 1")->setGlobalZOrder(2);
		this->addChild(_tileMap);
	} 

	b2Vec2 gravity = b2Vec2(0.0f, 0.0f); // Vector gia tốc ( dấu - là chỉ hướng xuống, vì trục y hướng lên trên)
	world = new b2World(gravity); // Tạo world với vector gia tố

	a = new Tank(world);
	this->addChild(a,2);
	addWallFromMap(_tileMap);


	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = true;
	};
	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		keys[keyCode] = false;
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);


	GLESDebugDraw* _debugDraw = new GLESDebugDraw(32.0f); // change 30 to your ptm ratio
	_debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit);
	world->SetDebugDraw(_debugDraw);
	this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float delta)
{
	a->keys = keys;
	a->update(delta);
	int velocityIterations = 10;   // 
	int positionIterations = 10;   // 

	 // Bước thời gian
	world->Step(delta, velocityIterations, positionIterations);

	// Duyệt tất cả body của world
	for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
	{
		// Xét những body có gắn vào Sprite
		if (body->GetUserData())
		{

			// Trả về sprite quả bóng ( có mỗi sprite trong bài này )
			Sprite *sprite = (Sprite *)body->GetUserData();
			// Đặt lại vị trí của Sprite này theo vị trí của body ( body sẽ bị rơi dần theo time), nhớ nhân RATIO để chuyển sang tọa độ pixel
			sprite->setPosition(Point(body->GetPosition().x * SCALE_RATIO, body->GetPosition().y * SCALE_RATIO));

		}
	}
	for (b2Contact* contact = world->GetContactList(); contact; contact = contact->GetNext())
	{

		if (contact->GetFixtureA()->GetBody())
			world->DestroyBody(contact->GetFixtureA()->GetBody());
		world->Step(delta, velocityIterations, positionIterations);
		/*contact->GetFixtureA()->GetBody()->DestroyFixture(contact->GetFixtureA());*/
	}

	
	world->ClearForces(); // Xóa mọi áp đặt cho Body

}
void HelloWorld::addWall(float w, float h, float px, float py) {

	b2PolygonShape floorShape; // Hình dạng Sàn

	floorShape.SetAsBox(w / SCALE_RATIO, h / SCALE_RATIO); // Hình vuông, hoặc chữ nhật
	b2FixtureDef floorFixture;

	floorFixture.density = 0;
	floorFixture.friction = 10;
	floorFixture.restitution = 0.5;
	floorFixture.shape = &floorShape;

	b2BodyDef floorBodyDef;
	floorBodyDef.type = b2_staticBody;
	floorBodyDef.position.Set(px / SCALE_RATIO, py / SCALE_RATIO);


	b2Body *floorBody = world->CreateBody(&floorBodyDef);
	floorBody->CreateFixture(&floorFixture);
}

void HelloWorld::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
	Layer::draw(renderer, transform, flags);
	world->DrawDebugData();
}

void HelloWorld::addWallFromMap(TMXTiledMap* tiled_map)
{
	TMXObjectGroup *objectGroup = tiled_map->getObjectGroup("Objects");


	for (int i = 0; i<objectGroup->getObjects().size(); i++)
	{

		Value objectemp = objectGroup->getObjects().at(i);

		float wi_box = objectemp.asValueMap().at("width").asFloat()/2;
		float he_box = objectemp.asValueMap().at("height").asFloat()/2;
		float x_box = objectemp.asValueMap().at("x").asFloat()+ wi_box;
		float y_box = objectemp.asValueMap().at("y").asFloat()+he_box;
		this->addWall(wi_box, he_box, x_box, y_box);
	}
}


