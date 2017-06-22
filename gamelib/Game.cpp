#include "Game.h" 
using namespace std;


//todo 전역변수. main에서 new. 싱글턴?
Game *game;

LPDIRECT3DTEXTURE9 texes[TEX_ID_MAX] = { NULL };

//wc 초기화시 이 함수를 보내서 CreateWindow()에 의한 WM_CREATE 등등의 메시지를 DefWindowProc()이 특정 HWND로 처리하도록 함.
//또한 Game::Run() 이외의 상황에 다시 이 함수로 Proc을 바꿈.
static LRESULT WINAPI DummyProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hWnd, msg, wparam, lparam);
}

//실제 WndProc(). 이 함수가 받은 파라미터들을 Game::WndProc()에 넘겨서 Game 클래스 안에서 Game의 멤버와 관련된 작업을 쉽게 처리하도록 함.
static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return game->WndProc(msg, wparam, lparam);
}


//생성자
Game::Game(LPCWSTR title, float w, float h) //todo
	: title(title), hWnd(NULL), pGD(NULL), sprite(NULL), player(nullptr), gameboxWidth(w), gameboxHeight(h)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, DummyProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		title, NULL
	};
	RegisterClassEx(&wc);

	hWnd = CreateWindow(title, title, WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, w, h, NULL, NULL, wc.hInstance, NULL); //todo flexible window
}


//소멸자
Game::~Game() //todo: safe delete
{
	delete pGD;
	sprite->Release();
	

	for (auto tex : texes)
		if (tex)
			tex->Release();
	
	delete player;

	for (auto enemy : enemies)
		delete enemy;
}


//초기화.
bool Game::Initialize() //todo safe
{
	//init GD
	pGD = new GraphicsDevice();

	if (!pGD->Initialize(hWnd, true)) return false;


	//init sprite
	D3DXCreateSprite(pGD->GetDevice(), &sprite);


	//init textures
	LoadTexture(L"PlayerShip.png", PShip);
	LoadTexture(L"ETriangle.png", ETriangle);
	LoadTexture(L"Lightning Segment.png", GOLSegment);
	LoadTexture(L"Half Circle.png", GOLHalfCircle);
	LoadTexture(L"Background.png", Background);
	LoadTexture(L"Bullet.png", GOBullet);


	//init player //test //to fn?
	player = new Player(gameboxWidth / 2, gameboxHeight / 2, 20.0f, 0.0f, 0.0f, sprite);
	player->SetTexure(texes[PShip]);
	player->SetActive(true);

	//
	EnemiesSpawnTick = 2000;
	
	for (int i = 0; i < 100; i++)
	{
		Line line;
		parL.push_back(line);
	}

	return true;
}

void Game::Run()
{
	SetWindowLong(hWnd, GWL_WNDPROC, (long)::WndProc);


	MSG msg;
	ZeroMemory(&msg, sizeof(msg));


	while (msg.message != WM_QUIT)
	{
		//DWORD tickcount = GetTickCount();


		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		player->Input();
		Update();
		Draw();
		

		//while ((GetTickCount() - tickcount < 10));
	}


	SetWindowLong(hWnd, GWL_WNDPROC, (long)::DummyProc);

}

void Game::Draw()
{
	pGD->Clear();

	pGD->Begin();	

		sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE | D3DXSPRITE_OBJECTSPACE);

		//set 2D-camera
		D3DXMATRIX orthographicMatrix;
		D3DXMATRIX identityMatrix;

		D3DXMatrixOrthoLH(&orthographicMatrix, gameboxWidth, -gameboxHeight, 0.0f, 1.0f); //
		D3DXMatrixIdentity(&identityMatrix);
		
		LPDIRECT3DDEVICE9 dev = pGD->GetDevice();
		dev->SetTransform(D3DTS_PROJECTION, &orthographicMatrix);
		dev->SetTransform(D3DTS_WORLD, &identityMatrix);
		dev->SetTransform(D3DTS_VIEW, &viewMatrix);


		//set renderstate
		dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		dev->SetRenderState(D3DRS_ZWRITEENABLE, false);


		//draw background
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);
		sprite->SetTransform(&mat);
		D3DXVECTOR3 bgc(-36.0f, -36.0f, 0.0f);
		sprite->Draw(texes[Background], NULL, NULL, &bgc, D3DCOLOR_XRGB(255,255,255));


		//draw player
		if (player->IsActive())
			player->Draw();
		else if (player->IsExp())
			player->DrawExp();


		//draw enemies
		for (auto enemy : enemies)
			if (enemy->IsActive())
				enemy->Draw();
			else if (enemy->IsExp())
				enemy->DrawExp();


		//draw parL
		for (auto line : parL)
			if (line.IsActive())
				line.Draw();


		sprite->End();

	pGD->End();

	pGD->Present();
}

void Game::Update()
{
	//update delta timer //todo
	static DWORD start = GetTickCount(), count;
	count = GetTickCount() - start;
	start = GetTickCount(); //reset for next update
	float deltatime = count / 1000.0f;

	// update line ps
	std::vector<Line>::iterator i;
	for (i = parL.begin(); i != parL.end(); i++)
	{
		Line* line = &(*i);
		if (line->IsActive())
		{
			line->Update(deltatime);
		}
	}
		
	// 플레이어가 움직엿으면 라인 하나 리셋.
	if (player->GetSpd() != 0)
	{
		std::vector<Line>::iterator i;

		for (i = parL.begin(); i != parL.end(); i++)
		{
			Line* line = &(*i);
			if (!line->IsActive())
			{
				line->SetPos(player->GetPos().x , player->GetPos().y );
				line->SetSize(15.0f);
				line->ls = texes[GOLSegment];
				line->hc = texes[GOLHalfCircle];
				line->SetSprite(sprite);
				line->color = D3DCOLOR_XRGB(255, 128, 0);
				//line.span = 1.0f;
				line->SetAng(player->GetAng());
				line->SetActive(true);
				break;
			}
		}
	}

	//update player
	player->Update(deltatime);//
	if (player->IsExp())
		player->UpdateExp(deltatime);

	//update enemies
	for (auto enemy : enemies)
	{
		if (enemy->IsActive())
		{
			enemy->Update(deltatime);

			//플레이어 충돌.
			if (player->IsActive() && enemy->CheckCollision(&player->GetPos(), player->GetSize()))
			{
				PlayerHit();
				continue;
			}

			//총알 충돌. 구조..
			bool hit = false;
			std::vector<Weapon>::iterator i;
			for (i = player->weapons.begin(); i != player->weapons.end(); i++)
			{
				Weapon* weapon = &(*i);
				if (hit) break;
				for (auto bullet : weapon->bullets)
					if (enemy->CheckCollision(&bullet->GetPos(), bullet->GetSize()))
					{
						enemy->Hit();
						bullet->SetActive(false);
						hit = true;
						break;
					}
			}
		}
		else if (enemy->IsExp())
			enemy->UpdateExp(deltatime);
	}

	//spawn enemies
	if (start > EnemiesSpawnTick && player->IsActive())
	{
		EnemiesSpawnTick = start + 2000;

		Enemy *enemy = GetEnemy<DirectionalEnemy>();

		float x, y, s;
		s = 15.0f;
		x = GetRandomFloat(s*3, gameboxWidth - s*3);
		y = GetRandomFloat(s*3, gameboxHeight - s*3);
		D3DXVECTOR3 v(x, y, 0.0f);
		if (player->CheckCollision(&v, s))
		{
			x += s*1.5;
			y += s*1.5;
		}
		v = D3DXVECTOR3(x, y, 0.0f);
		enemy->SetSize(s);
		enemy->SetPos(x, y);

		D3DXVECTOR3 dis = player->GetPos() - v;
		enemy->SetAng(atan2(dis.y, dis.x));

		enemy->SetSprite(sprite);
		enemy->SetTexure(texes[ETriangle]);

		enemy->SetSpd(150.0f);
	}


	//update viewMatrix
	float cameraX, cameraY;

	cameraX = player->GetPos().x;
	cameraY = player->GetPos().y;

	viewMatrix = D3DXMATRIX(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-cameraX, -cameraY, 0, 1
	);

	//check game over

	//test

}

LRESULT WINAPI Game::WndProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		Draw();
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool Game::LoadTexture(LPCWSTR texStr, GameTextureID texID) 
{
	if (FAILED(
		D3DXCreateTextureFromFile(pGD->GetDevice(), texStr, &texes[texID])
	)
		) return false;
	return true;
}

void Game::PlayerHit()
{
	for (auto enemy : enemies)
		if (enemy->IsActive())
			enemy->Hit();

	player->Hit();
}



