class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	// TODO: Integrate world into AnvancezLib
	AvancezLib* system;
	b2World * world;

	GameObject * canvas;
	GUI * gui;
	
	Generator * generator;
	ObjectPool<Tetromino> tetromino_pool;

	bool game_over;

	Tetromino * base;

public:
	virtual void Create(AvancezLib* system)
	{
		SDL_Log("Game::Create");
		this->system = system;

		// initialize physics system
		b2Vec2 gravity(0.0f, -0.25f);
		world = new b2World(gravity);

		{ // canvas for physic enviroment
			canvas = new GameObject();

			PhysicsComponent * base_line = new PhysicsComponent();
			base_line->CreateEdge(system, world, canvas, &game_objects, 0, 0, 640, 0);

			PhysicsComponent * base_left = new PhysicsComponent();
			base_left->CreateEdge(system, world, canvas, &game_objects, 64, 0, 64, 576);

			PhysicsComponent * base_right = new PhysicsComponent();
			base_right->CreateEdge(system, world, canvas, &game_objects, 384, 0, 384, 576);

			canvas->Create();
			canvas->AddComponent(base_line);
			canvas->AddComponent(base_left);
			canvas->AddComponent(base_right);
			game_objects.insert(canvas);
		}

		gui = new GUI();
		
		RenderComponent * background = new RenderComponent();
		background->Create(system, gui, &game_objects, "data/Background.png");

		gui->Create();
		gui->AddComponent(background);
		game_objects.insert(gui);

		{ // Create a basement for constructing the tower
			base = new Tetromino();
			base->Create(Tetromino::O_Tetromino);

			PhysicsComponent * base_physics = new PhysicsComponent();
			base_physics->Create(system, world, base, &game_objects, b2_staticBody, 224, 32, 0);

			RenderComponent * base_render = new RenderComponent();
			base_render->Create(system, base, &game_objects, "data/base.png");

			base->AddComponent(base_physics);
			base->AddComponent(base_render);
			game_objects.insert(base);

		}

		generator = new Generator();
		GeneratorBehaviourComponent * generator_behaviour = new GeneratorBehaviourComponent();
		generator_behaviour->Create(system, world, generator, &game_objects, &tetromino_pool);

		generator->Create();
		generator->AddComponent(generator_behaviour);
		game_objects.insert(generator);

		// generate a object pool of random tetrominos
		tetromino_pool.Create(100);
		for (auto tetromino = tetromino_pool.pool.begin(); tetromino != tetromino_pool.pool.end(); tetromino++)
		{
			int r = rand() % 7;
			Tetromino::TetrominoType tetromino_type = Tetromino::TetrominoType(r);
			(*tetromino)->Create(tetromino_type);

			RenderComponent * render = new RenderComponent();
			switch (tetromino_type)
			{
			case Tetromino::O_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/O_64x64.png");
				break;

			case Tetromino::L_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/L_64x96.png");
				break;

			case Tetromino::J_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/J_64x96.png");
				break;

			case Tetromino::I_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/I_128x32.png");
				break;

			case Tetromino::Z_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/Z_96x64.png");
				break;

			case Tetromino::S_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/S_96x64.png");
				break;

			case Tetromino::T_Tetromino:
				render->Create(system, *tetromino, &game_objects, "data/T_96x64.png");
				break;
			}
			(*tetromino)->AddComponent(render);
			(*tetromino)->AddReceiver(this);
		}
	}

	virtual void Init()
	{
		game_over = false;
		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		{
			(*go)->Init();
		}

		enabled = true;
	}

	virtual void Update(float delta_time)
	{
		if (IsGameOver())
		{
			delta_time = 0.f;
		}

		float32 timeStep = 1.0f / 60.0f;

		int32 velocityIterations = 6;
		int32 positionIterations = 2;
		world->Step(timeStep, velocityIterations, positionIterations);

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
		{
			(*go)->Update(delta_time);
		}
	}

	virtual void Draw()
	{
		char msg[1024];
		sprintf_s(msg, "Test Enviroment");
		system->drawText(450, 112, msg);

		if (IsGameOver())
		{
			sprintf_s(msg, "*** G A M E  O V E R ***");
			system->drawText(128, 112, msg);
		}

	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
			game_over = true;
	}

	bool IsGameOver()
	{
		return game_over;
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		delete world;
	}
};