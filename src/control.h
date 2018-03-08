class ControlComponent : public Component
{
	PhysicsComponent * physic;
	b2Body * body;

	float time_fire_pressed = 0;	// time from the last time the fire button was pressed
	float newTime;
	float prev_y;

public:
	bool enabled;


	virtual void Create(AvancezLib* system, Tetromino * tetromino, std::set<GameObject*> * game_objects, PhysicsComponent * physic)
	{
		Component::Create(system, tetromino, game_objects);
		this->body = physic->body;
	}

	virtual void Init()
	{
		enabled = true;
		SDL_Log("Controller Enabled");

		newTime = 0.f;
		prev_y = -100.f;
		Component::Init();
	}



	virtual void Update(float dt)
	{
		if (enabled)
		{		
		
			{ // Timestemp for moving control
				if (prev_y != go->verticalPosition)
				{
					newTime = system->getElapsedTime();
				}

				if (((system->getElapsedTime()) - newTime) > 0.1f)
				{
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
						angle += 90.f * (3.14f / 180.f);
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

	bool CanFlip()
	{
		// shoot just if enough time passed by
		if ((system->getElapsedTime() - time_fire_pressed) < 0.2f)
			return false;

		time_fire_pressed = system->getElapsedTime();

		return true;
	}
};