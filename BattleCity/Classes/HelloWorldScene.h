#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include <map>
#include "cocos2d.h"
#include "proj.win32/Tank.h"
#include "Box2D/Box2D.h"
#include "proj.win32/GLES-Render.h"
using namespace cocos2d;
using namespace std;
class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
	b2World * world;
	enum TypeBody
	{
		BRICK,
		BULLET
	};
	Tank* a;
	map<EventKeyboard::KeyCode, bool> keys;


	virtual bool init();
	void update(float delta) override;
	
	void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	void addWallFromMap(TMXTiledMap* tiled_map);
	void addWall(float w, float h, float px, float py); // Tạo 1 khung Wall bao quanh màn hình để cho quả bóng va chạm
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
