class PhysicsComponent : public Component
{
	b2World* world;

	float size_x, size_y;
	float block;

public:

	b2Body * body;

	virtual void Create(AvancezLib* system, b2World* world, Tetromino * tetromino, std::set<GameObject*> * game_objects, b2BodyType type, float pos_x, float pos_y, float block_size, float angle)
	{
		Component::Create(system, tetromino, game_objects);

		this->world = world;
		this->block = block_size / 2.f;

		b2BodyDef def;
		def.type = type;
		body = world->CreateBody(&def);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.8f;
		fixtureDef.friction = 1.0f;
		fixtureDef.restitution = 0.01f; // Just for fun

		b2PolygonShape shape;

		b2Vec2 center;

		switch (tetromino->tetromino_type)
		{
		case Tetromino::O_Tetromino:
			shape.SetAsBox(2.f * block, 2.f * block);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			center = body->GetLocalCenter();

			this->size_x = 2.f * block;
			this->size_y = 2.f * block;

			break;

		case Tetromino::L_Tetromino:
			shape.SetAsBox(2.f * block, block, b2Vec2(0, -(2.f * block)), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			shape.SetAsBox(block, 2.f * block, b2Vec2(-block, block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			center = body->GetLocalCenter();

			this->size_x = 2.f * block;
			this->size_y = 3.f * block;

			break;

		case Tetromino::J_Tetromino:
			shape.SetAsBox(2.f * block, block, b2Vec2(0, 2 * block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			shape.SetAsBox(block, 2.f * block, b2Vec2(-block, -block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			center = body->GetLocalCenter();

			this->size_x = 2.f * block;
			this->size_y = 3.f * block;

			break;

		case Tetromino::I_Tetromino:
			shape.SetAsBox(4.f * block, block);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			center = body->GetLocalCenter();

			this->size_x = 4.f * block;
			this->size_y = 1.f * block;

			break;

		case Tetromino::Z_Tetromino:
			shape.SetAsBox(2.f * block, block, b2Vec2(block, -block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			shape.SetAsBox(2.f * block, block, b2Vec2(-block, block), 0);
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
			center = body->GetLocalCenter();

			this->size_x = 3.f * block;
			this->size_y = 2.f * block;

			break;

		case Tetromino::S_Tetromino:
			shape.SetAsBox(2.f * block, block, b2Vec2(-block, -block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			shape.SetAsBox(2.f * block, block, b2Vec2(block, block), 0);
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
			center = body->GetLocalCenter();

			this->size_x = 3.f * block;
			this->size_y = 2.f * block;

			break;

		case Tetromino::T_Tetromino:
			shape.SetAsBox(3.f * block, block, b2Vec2(0, -block), 0);
			fixtureDef.shape = &shape;
			body->CreateFixture(&fixtureDef);

			shape.SetAsBox(block, block, b2Vec2(0, block), 0);
			fixtureDef.shape = &shape;

			body->CreateFixture(&fixtureDef);
			center = body->GetLocalCenter();

			this->size_x = 3.f * block;
			this->size_y = 2.f * block;

			break;
		}
		body->SetTransform(b2Vec2(pos_x, pos_y), angle * (3.14 / 180));
	}

	virtual void Create(AvancezLib* system, b2World* world, Tetromino * tetromino, std::set<GameObject*> * game_objects, b2BodyType type, float pos_x, float pos_y, float angle)
	{
		float block_size = 32.f;
		Create(system, world, tetromino, game_objects, type, pos_x, pos_y, block_size, angle);
	}

	virtual void CreateEdge(AvancezLib* system, b2World* world, GameObject * go, std::set<GameObject*> * game_objects, float begin_x, float begin_y, float end_x, float end_y)
	{
		Component::Create(system, go, game_objects);

		this->world = world;

		b2BodyDef bd;
		body = world->CreateBody(&bd);

		b2FixtureDef fixtureDef;
		fixtureDef.density = 0.8f;
		fixtureDef.friction = 0.0f;


		b2EdgeShape shape;
		shape.Set(b2Vec2(begin_x, begin_y), b2Vec2(end_x, end_y));
		fixtureDef.shape = &shape;
		body->CreateFixture(&fixtureDef);
	}

	virtual void Update(float dt)
	{

		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

		go->horizontalPosition = position.x - size_x;
		go->verticalPosition = 576 - (position.y + size_y);
		go->angle = angle * (-180 / 3.14f);

		if (position.y < 0) // When the box reaches the bottom of the screen, it disappears.
		{
			go->enabled = false;
			go->Send(GAME_OVER);
		}
	}
};