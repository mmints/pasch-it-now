class Tetromino : public GameObject
{

public:

	enum TetrominoType
	{
		O_Tetromino,
		L_Tetromino,
		J_Tetromino,
		I_Tetromino,
		Z_Tetromino,
		S_Tetromino,
		T_Tetromino
	};

	TetrominoType tetromino_type;

	virtual void Create(TetrominoType tetromino_type)
	{
		this->tetromino_type = tetromino_type;
	}

	virtual void Init()
	{
		SDL_Log("Tetromino::Init");
		GameObject::Init();
	}
};