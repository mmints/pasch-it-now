class Generator : public GameObject
{
public:
	bool can_spawn;

	virtual void Init()
	{
		SDL_Log("Generator::Init");
		GameObject::Init();
	}

	virtual void Destroy()
	{
		SDL_Log("Generator::Destroy");
		GameObject::Destroy();
	}
};

class GeneratorBehaviourComponent : public Component
{
	ObjectPool<Tetromino> * tetromino_pool;
	b2World * world;
	Tetromino * tetromino;

	PhysicsComponent * physic;
	ControlComponent * controller;

	Generator * generator;

public:
	void Create(AvancezLib* system, b2World* world, Generator * generator, std::set<GameObject*> * game_objects, ObjectPool<Tetromino> * tetromino_pool)
	{
		Component::Create(system, generator, game_objects);

		this->tetromino_pool = tetromino_pool;
		this->world = world;
		this->generator = generator;
	}

	void Init()
	{
		tetromino = tetromino_pool->FirstAvailable();
		generate();
		Component::Init();
	}

	void Update(float dt)
	{
		AvancezLib::KeyStatus keys;
		system->getKeyStatus(keys);
		tetromino = tetromino_pool->FirstAvailable();

		if (CanSpawn())
			{
				if (tetromino != NULL)
				{
					generate();
				}
			}
	}

	// Generates/Spawn the Tetraminos with a random angle
	void generate()
	{
		physic = new PhysicsComponent();
		physic->Create(system, world, tetromino, game_objects, b2_dynamicBody, 224, 576 - 32, (rand() % 4) * 90.f);

		controller = new ControlComponent();
		controller->Create(system, tetromino, game_objects, physic);

		tetromino->AddComponent(physic);
		tetromino->AddComponent(controller);
		tetromino->Init();
		game_objects->insert(tetromino);
	}

	bool CanSpawn()
	{
		if (!(controller->enabled))
		{
			generator->can_spawn = true;
			return true;
		}

		generator->can_spawn = false;
		return false;
	}
};