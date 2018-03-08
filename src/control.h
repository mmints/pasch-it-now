class ControlComponent : public Component
{
	PhysicsComponent * physic;
	b2Body * body;

	float time_fire_pressed = 0;	// time from the last time the fire button was pressed
	float newTime;

	bool enabled;

public:
	virtual void Create(AvancezLib* system, Tetromino * tetromino, std::set<GameObject*> * game_objects, PhysicsComponent * physic)
	{
		Component::Create(system, tetromino, game_objects);
		this->body = physic->body;
	}

	virtual void Init()
	{
//		SDL_Log("ControlComponent::Init() -> set on_stack = false");
		enabled = true;
		SDL_Log("Controller Enabled");

		newTime = 0.f;

		Component::Init();
	}

	float prev_y = -100.f;

	virtual void Update(float dt)
	{
		if (enabled)
		{		
		  SDL_Log("Y Coordinate of Tetromino: %.6f", go->verticalPosition);

		{ // Timestemp for moving control
			if (prev_y != go->verticalPosition)
			{
				newTime = system->getElapsedTime();
			}

			if (((system->getElapsedTime()) - newTime) > 0.1f)
			{
				SDL_Log("Controller Disabled");
				enabled = false;
			}
			prev_y = go->verticalPosition;
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
		}
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