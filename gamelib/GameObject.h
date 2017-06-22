#pragma once
#include "GraphicsDevice.h"
#include <vector>
#include <typeinfo>

#define GB_WIDTH 800
#define GB_HEIGHT 600

enum GameTextureID { PShip, ETriangle, ERectangle, ECircle, GOBullet, GOLSegment, GOLHalfCircle, Background, TEX_ID_MAX };
extern LPDIRECT3DTEXTURE9 texes[TEX_ID_MAX];

float GetRandomFloat(float lowBound, float highBound);

void GetRandomVector(
	D3DXVECTOR2* out,
	D3DXVECTOR2* min,
	D3DXVECTOR2* max)
	;

DWORD FtoDw(float f);


class GameObject
{
protected:

	LPDIRECT3DTEXTURE9 texture;
	LPD3DXSPRITE sprite;//


	bool isActive;

	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	D3DXVECTOR3 accel;//
	float		angle;
	float		angularSpeed;
	float		speed;
	float		size;

public:
	//Reset()?.
	GameObject(float x = 0.0f, float y = 0.0f, float s = 0.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL, float z = 0.0f);
	virtual ~GameObject();

	virtual void Draw(); //
	virtual void Update(float deltaTime); //using spd and ang
	virtual void Reset() = 0; //

	//
	void SetTexure(LPDIRECT3DTEXTURE9 tex) { texture = tex; } //
	void SetSprite(LPD3DXSPRITE spt) { sprite = spt; }//
	void SetActive(bool b) { isActive = b; }//
	void SetSize(float s) { size = s; }//
	void SetPos(float x, float y) { position.x = x; position.y = y; }
	void SetAng(float r) { angle = r; }
	void SetVel(float x, float y) { velocity.x = x; velocity.y = y; }
	void SetAccel(float x, float y) { accel.x = x; accel.y = y; }
	void SetSpd(float s) { speed = s; }

	bool IsActive() const { return isActive; }
	D3DXVECTOR3 GetPos() const { return position; }
	float GetAng() const { return angle; }
	float GetAngSpd() const { return angularSpeed; }
	float GetSpd() const { return speed; }
	float GetSize() const { return size; }
	D3DXVECTOR3 GetVel() const { return velocity; }
	D3DXVECTOR3 GetAcc() const { return accel; }
};



//template<class T, int N> //
//class GameObjectPool
//{
//friend class Game;
//
//	std::vector<GameObject *> objs;
//
//	void New()
//	{
//		GameObject *temp = new T;
//		objs.push_back(temp);
//	}
//
//public:
//	GameObjectPool()
//	{
//		for (int i = 0; i < N; i++)
//		{
//			New();
//		}
//	}
//
//	~GameObjectPool()
//	{
//		for (auto obj : objs)
//			delete obj;
//	}
//
//	T *GetObj()
//	{
//		for (auto obj : objs)
//		{
//			if (!obj->IsActive())
//			{
//
//				return static_cast<T *> (obj);
//			}
//		}
//
//		New();
//		return static_cast<T *> (objs.back()); //
//	}
//
//	void Draw()
//	{
//		for (auto obj : objs)
//			if (obj->IsActive())
//				obj->Draw();
//	}
//
//	void Update(float deltatime)
//	{
//		for (auto obj : objs)
//			if (obj->IsActive())
//				obj->Update(deltatime);
//	}
//};

class Line : public GameObject
{
public:
	float thickness;
	D3DXCOLOR color;
	//D3DXVECTOR2 src;
	//D3DXVECTOR2 des;
	float life;
	float span;

	//test
	LPDIRECT3DTEXTURE9 ls;
	LPDIRECT3DTEXTURE9 hc;

	Line(float thickness = 1.0f, D3DXCOLOR color = D3DCOLOR_XRGB(255, 255, 255), float span = 0.3f) : GameObject::GameObject(), thickness(thickness), color(color), span(span), life(0.0f) {}
	~Line() 
	{
	}

	void Reset()
	{
	}

	void Update(float deltatime)
	{
		life += deltatime;
		
		if (life > span)
		{
			SetActive(false);
			life = 0.0f;
		}
		
		GameObject::Update(deltatime);
	}

	void Draw();
};

struct Bullet_Att
{
	float speed;
};


class Bullet : public GameObject
{
public:
	Bullet_Att att;

	Bullet(float x = 0.0f, float y = 0.0f, float s = 6.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL) : GameObject::GameObject(x, y, s, ang, spd, spt) 
	{ SetTexure(texes[GOBullet]); }
	//virtual void Update(float deltaTime); //
	void Reset() {}

};

// Pattern Directional, Aiming, Missile
class DirectionalBullet : public Bullet
{
public:
	void Update(float deltaTime);
	void Draw();
	//void Reset();

};

class Weapon : public GameObject
{
public:
	std::vector<Bullet *> bullets;

	Weapon(float x = 0.0f, float y = 0.0f, float s = 0.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL) : GameObject::GameObject(x, y, s, ang, spd, spt)
	{
			//
	}
	~Weapon();

	void Reset() {}
	
	void Shoot();
	void Update(float deltaTime); //
	void Draw();

	template<class T>
	void InitBullets(Bullet_Att *a) //
	{
		for (int i = 0; i < 100; i++)
		{
			Bullet *temp = new T; //
			temp->att = *a;
			temp->SetSprite(sprite);
			bullets.push_back(temp);
		}
	}
};


class Shooter : public GameObject
{
protected:
	std::vector<Line> expL; //구조..
	bool isExplosing;

public:
	std::vector<Weapon> weapons;
	float shootRate;


	Shooter(float x = 0.0f, float y = 0.0f, float s = 0.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL) : GameObject::GameObject(x, y, s, ang, spd, spt), isExplosing(false)
	{
		for (int i = 0; i < 100; i++)
		{
			Line l;
			expL.push_back(l);
		}
	}
	virtual ~Shooter()
	{
	}
	virtual void Hit(); // 충돌이 확인됐을 때. explosion.
	virtual void Update(float deltatime);

	void UpdateExp(float deltatime); //update all expl, 끝났을 때 isExplosing false.
	void DrawExp();

//
	virtual void Draw();
	void Attack(); //Shoot() all active weapons
	void Reset() {	} //todo

	bool CheckCollision(const D3DXVECTOR3 *desP, float desS);
	bool IsExp() { return isExplosing; }

};

class Player : public Shooter
{
	int hp;
	float respawntime;
	//float revivingtime;

public:
	Player(float x = 0.0f, float y = 0.0f, float s = 0.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL) : Shooter::Shooter(x, y, s, ang, spd, spt), hp(5), respawntime(0.0f)
	{
		Weapon weapon;
		
		weapons.push_back(weapon);

		Bullet_Att a;
		a.speed = 450.0f;

		weapons[0].SetActive(true);
		weapons[0].SetSprite(sprite);
		weapons[0].InitBullets<DirectionalBullet>(&a);
		
	}
	

	void Draw();
	void Update(float deltaTime);

	void Input();

	void ChangeShooter(const Shooter& src);
	
	void Hit() 
	{ 
		hp--;
		if (hp > 0)
		{
			respawntime = 2.0f;
		}

		Shooter::Hit();
	}
};



class Enemy : public Shooter
{

public:
	Enemy(float x = 0.0f, float y = 0.0f, float s = 0.0f, float ang = 0.0f, float spd = 0.0f, LPD3DXSPRITE spt = NULL) : Shooter::Shooter(x, y, s, ang, spd, spt)
	{
		
	}
	~Enemy()
	{
	}
	void Draw();

	//virtual void Update(float deltatime); // out of window
};

// EnemyMovePattern Directional(tri), Distancing(rect), Turning(circle)
class DirectionalEnemy : public Enemy
{
	void Update(float deltatime);
};