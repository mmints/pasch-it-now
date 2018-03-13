#include <ctime>
#include <iostream>
#include <fstream>

// External Libraries
#include "Box2D\Box2D.h"

// Framework
#include "avancezlib.h"
#include "game_object.h"
#include "object_pool.h"
#include "component.h"

// Game Objects
#include "tetromino.h"
#include "gui.h"
// Additional Components
#include "physic.h"
#include "control.h"

#include "generator.h"

// Main game file
#include "game.h"



int main( int argc, char** argv)
{
	std::srand(time(NULL));

	AvancezLib system;

	system.init(640, 576);

	Game game;
	game.Create(&system);
	game.Init();

	float lastTime = system.getElapsedTime();
	while (system.update())
	{
		float newTime = system.getElapsedTime();
		float dt = newTime - lastTime;
		lastTime = newTime;

		game.Draw();
		game.Update(dt);

	}

	// clean up
	game.Destroy();
	system.destroy();

	return 0;
}