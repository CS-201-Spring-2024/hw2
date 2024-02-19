#ifndef FRAMEWORK_H
#define FRAMEWORK_H
#pragma once
#include <string>
#include "raylib.h"  

class Demo {

public:
	Demo(const int screenWidth=1024, const int screenHeight=768, 
		const std::string title="SimpleFramework::Demo");
	virtual ~Demo();

	virtual void init()     = 0;
	virtual void shutdown() = 0;
	virtual void draw()		= 0;
	virtual void update( )	= 0;

	virtual void run();

protected:
	int screenWidth, screenHeight;
	int framesCounter;
	bool pause;

	const Color TRANSPARENT_WHITE;
};

class BallCollisionDemo :public Demo
{
public:
	BallCollisionDemo(const int screenWidth, const int screenHeight, const std::string title);
	virtual ~BallCollisionDemo();

	void init();
	void shutdown();
	void draw();
	void update( );

private:
	Vector2 ballPosition;
	Vector2 ballSpeed;
	int ballRadius;
};

#include <vector>

class NBallsCollisionDemo :public Demo
{
public:
	NBallsCollisionDemo(const int screenWidth, const int screenHeight, const std::string title, const int MAX_BALLS=5000);
	virtual ~NBallsCollisionDemo();

	void init();
	void shutdown();
	void draw();
	void update();

private:
	std::vector<Vector2> ballPositions;
	std::vector<Vector2> ballSpeeds;
	std::vector<bool>    ballColliding;
	int 				 ballRadius, 
						 ballMinDistance, 
						 ballMinDistance2, 
						 totalCollisions;
	const int 			 MAX_BALLS;
	bool 				 useSpatialHashing; 

};
#endif
