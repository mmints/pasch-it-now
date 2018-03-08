class Generator : public GameObject
{
public:
	virtual void Init()
	{
		SDL_Log("GUI is loaded");
		GameObject::Init();
	}

	virtual void Destroy()
	{
		SDL_Log("GUI::Destroy");
		GameObject::Destroy();

	}
};

class GeneratorBehaviourComponent : public Component
{
	float time_fire_pressed = -10000.f;	// time from the last time the fire button was pressed
	ObjectPool<Tetromino> * tetromino_pool;
	b2World * world;
	Tetromino * tetromino;
	PhysicsComponent * physic;
	ControlComponent * controller;

	// float stack_hight; // This is a really messy hack <----!!!  HUGE TODO!

public:
	virtual void Create(AvancezLib* system, b2World* world, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<Tetromino> * tetromino_pool)
	{
		Component::Create(system, go, game_objects);

		this->tetromino_pool = tetromino_pool;
		this->world = world;
	}

	virtual void Init()
	{
	//	stack_hight = 90.f;// This is a really messy hack <----!!!  HUGE TODO!
	//	SDL_Log("Generator::Init() -> stack_hight = %.0f", stack_hight);
		Component::Init();
	}

	virtual void Update(float dt)
	{

		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		tetromino = tetromino_pool->FirstAvailable();

		// TODO: Automatic Tetromino spawn after. Currently the player has to spawn them by himself
		if (keys.fire)
		{
			if (CanFire())
			{
				if (tetromino != NULL)
				{
					generate();
				}
			}
		}
	}

	virtual void generate()
	{
		physic = new PhysicsComponent();
		physic->Create(system, world, tetromino, game_objects, b2_dynamicBody, 224, 576 - 32, (rand() % 4) * 90.f);

		controller = new ControlComponent();
		controller->Create(system, tetromino, game_objects, physic); // This is a really messy hack <----!!!  HUGE TODO!

		tetromino->AddComponent(physic);
		tetromino->AddComponent(controller);
		tetromino->Init();
		game_objects->insert(tetromino);

	//	SDL_Log("Generator::Update() -> stack_hight = %.0f", stack_hight);
	}

	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_fire_pressed) < 1.f)
			return false;

		time_fire_pressed = system->getElapsedTime();

		return true;
	}
};