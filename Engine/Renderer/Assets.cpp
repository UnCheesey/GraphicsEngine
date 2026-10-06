#include "Assets.h"

using namespace nu;

namespace assets {
	Mesh playerMesh{
		{ 
			Vector2{ 2, 0 }, 
			Vector2{ -2, 2 }, 
			Vector2{ -1, 0 }, 
			Vector2{ -2, -2 }, 
			Vector2{ 2, 0 }
		}, 
		Color{ 1.0f, 1.0f, 1.0f }
	};

	std::shared_ptr<Model> playerModel = std::make_shared<Model>( std::vector<Mesh>{ playerMesh } );

	Mesh enemyMesh{
		{
			Vector2{ 2, 0 },
			Vector2{ -2, 2 },
			Vector2{ -1, 0 },
			Vector2{ -2, -2 },
			Vector2{ 2, 0 }
		},
		Color{ 1.0f, 0.0f, 0.0f }
	};

	std::shared_ptr<Model> enemyModel = std::make_shared<Model>(std::vector<Mesh>{ enemyMesh } );

	Mesh bulletMesh{
		{
			Vector2{ -1, -1 },
			Vector2{ 1, 0 },
			Vector2{ -1, 1 },
			Vector2{ -1, -1 },
		},
		Color{ 1.0f, 0.0f, 0.0f }
	};

	std::shared_ptr<Model> bulletModel = std::make_shared<Model>(std::vector<Mesh>{ bulletMesh } );
}