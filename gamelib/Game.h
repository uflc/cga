#pragma once

#include "GraphicsDevice.h"
#include "GameObject.h"
#include <vector>
//#include <string>
using namespace std;

class Game
{
	HWND hWnd;
	GraphicsDevice *pGD;
	LPD3DXSPRITE sprite;
	LPCWSTR title;

	//todo timer
	DWORD EnemiesSpawnTick;

	Player *player;
	std::vector<Line> parL;
	std::vector<Enemy *> enemies;//

	D3DXMATRIX viewMatrix;

public:
	float gameboxWidth, gameboxHeight;


	Game(LPCWSTR title, float w, float h);
	~Game();

	bool Initialize();
	void Run();
	void Update();
	void Draw();

	//todo safe
	template<class T>
	Enemy *GetEnemy() //
	{
		for (auto enemy : enemies)
			if (!enemy->IsActive() && typeid(*enemy) == typeid(T) && !enemy->IsExp())
			{
				enemy->SetActive(true);
				return enemy;
			}
		Enemy *temp = new T; //
		temp->SetActive(true);
		enemies.push_back(temp);
		return temp;
	}

	LRESULT WINAPI WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

	//
	bool LoadTexture(LPCWSTR texStr, GameTextureID texID);
	//void AddEnemy();

	void PlayerHit();
};

