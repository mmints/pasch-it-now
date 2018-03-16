class GUI : public GameObject
{
public:
	void Init()
	{
		SDL_Log("GUI::Init");
		GameObject::Init();
	}

	void Destroy()
	{
		SDL_Log("GUI::Destroy");
		GameObject::Destroy();
	}
};