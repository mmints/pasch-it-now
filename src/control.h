class ControlComponent : public Component
{
	PhysicsComponent * physic;
	b2Body * body;
//	bool on_stack; // This is a really messy hack <----!!!  HUGE TODO!

//	float * stack_hight; // This is a really messy hack <----!!!  HUGE TODO!
	float time_fire_pressed = 0.00001f;	// time from the last time the fire button was pressed


public:
	bool enabled;
	virtual void Create(AvancezLib* system, Tetromino * tetromino, std::set<GameObject*> * game_objects, PhysicsComponent * physic)
	{
		Component::Create(system, tetromino, game_objects);
		this->body = physic->body;
//		this->stack_hight = stack_hight; // This is a really messy hack <----!!!  HUGE TODO!
	}

	virtual void Init()
	{
//		SDL_Log("ControlComponent::Init() -> set on_stack = false");
//		on_stack = false; // This is a really messy hack <----!!!HUGE TODO!
		if (enabled == false)
		{
			SDL_Log("Controller Disabled");
		}
		enabled = true;
		SDL_Log("Controller Enabled");

		Component::Init();
	}

	float prev_y = 0;
	float newTime;

	virtual void Update(float dt)
	{
		if (prev_y != go->verticalPosition)
		{
			newTime = system->getElapsedTime();
		}

		if (((system->getElapsedTime()) - newTime) > 0.5f)
		{
			enabled = false;
		}
		
		b2Vec2 position = body->GetPosition();
		float32 angle = body->GetAngle();

			AvancezLib::KeyStatus keys;
			system->getKeyStatus(keys);

			if (enabled)
			{
				//SDL_Log("Controller Is Enabled");
			//	printf("Enabled \n");
				if (keys.right)
				{
					body->SetTransform(b2Vec2(position.x + 0.2f, position.y), angle);
				}
				else if (keys.left)
				{
					body->SetTransform(b2Vec2(position.x - 0.2f, position.y), angle);
				}


				if (keys.up)
				{
					if (CanFlip())
					{
						//			SDL_Log("ControlComponent::Update() -> angle = %.3f", angle * (-180 / 3.14f));
						angle += 90.f * (3.14f / 180.f);
						//			SDL_Log("ControlComponent::Update() -> angle after flipping = %.3f", angle * (-180 / 3.14f));

						body->SetTransform(b2Vec2(position.x, position.y), angle);
					}
				}
				else if (keys.down)
				{
					if (CanFlip())
					{
						angle -= (90.f * (3.14f / 180.f));
						body->SetTransform(b2Vec2(position.x, position.y), angle);
					}
				}

			}

		prev_y = go->verticalPosition;
	}

	//virtual void Update(float dt)
	//{
	//	Component::Update(dt);

	//	b2Vec2 position = body->GetPosition();
	//	float32 angle = body->GetAngle();

	//	AvancezLib::KeyStatus keys;
	//	system->getKeyStatus(keys);
	//	if (keys.right && !on_stack)
	//	{
	//		body->SetTransform(b2Vec2(position.x + 0.2f, position.y), angle);
	//	}
	//	else if (keys.left && !on_stack)
	//	{
	//		body->SetTransform(b2Vec2(position.x - 0.2f, position.y), angle);
	//	}

	//	if (keys.up && !on_stack)
	//	{
	//		if (CanFlip())
	//		{
	////			SDL_Log("ControlComponent::Update() -> angle = %.3f", angle * (-180 / 3.14f));
	//			angle += 90.f * (3.14f/180.f);
	////			SDL_Log("ControlComponent::Update() -> angle after flipping = %.3f", angle * (-180 / 3.14f));

	//			body->SetTransform(b2Vec2(position.x, position.y), angle);
	//		}
	//	}
	//	else if (keys.down && !on_stack)
	//	{
	//		if (CanFlip())
	//		{
	//			angle -= (90.f * (3.14f / 180.f));
	//			body->SetTransform(b2Vec2(position.x, position.y), angle);
	//		}
	//	}

	////	if (!on_stack && (position.y < (*stack_hight))) // This is a really messy hack <----!!!  HUGE TODO!
	////	{
	//////		SDL_Log("ControlComponent::Update() -> set on_stack = true");
	////		on_stack = true;
	////		(*stack_hight) += 50.f; // This is a really messy hack <----!!!  HUGE TODO!
	//////		SDL_Log("ControlComponent::Update() -> stack_hight = %.2f", (*stack_hight));
	////	}

	//}



	bool CanFlip()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_fire_pressed) < 0.2f)
			return false;

		time_fire_pressed = system->getElapsedTime();

		return true;
	}
};