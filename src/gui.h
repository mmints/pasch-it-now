class GUI : public GameObject
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