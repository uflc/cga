#include "GameObject.h"
#include <cmath>
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)


float GetRandomFloat(float lowBound, float highBound)
{
	if (lowBound >= highBound) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f;

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound;
}

void GetRandomVector(D3DXVECTOR2 * out, D3DXVECTOR2 * min, D3DXVECTOR2 * max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
}

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

GameObject::GameObject(float x, float y, float s, float ang, float spd, LPD3DXSPRITE spt, float z)
:	texture(NULL), position(x,y,z), size(s), velocity(spd * cos(ang), spd * sin(ang), 0.0f), angle(ang), speed(spd), isActive(false), sprite(spt), angularSpeed(0.0f)
{
}

GameObject::~GameObject()
{
}

void GameObject::Draw()
{
	//if (!isActive) return;
	
}

void GameObject::Update(float deltaTime)
{
	//if (!isActive) return;//

	//todo ang accel

	angle += angularSpeed * deltaTime;
	
	velocity += accel * deltaTime;
	position += velocity * deltaTime;
}


//disasterrrrrrrrrrrrrrr
//void Line::Draw()
//{
//	D3DXMATRIX	mat, mat1;
//	D3DXVECTOR2 tan,
//				scale,
//				center;
//	D3DXVECTOR3 sptc;
//
//	//D3DXVECTOR3 pos;
//
//	tan = des - src;
//
//	const float imgThickness = 8.0f;
//	float		thicknessScale = thickness / imgThickness;
//	float		rotation = atan2(tan.y, tan.x);//
//	float		len = D3DXVec2Length(&tan);
//	//1 pixel of line
//	scale = D3DXVECTOR2(len, thicknessScale);
//	center = D3DXVECTOR2(0.0f, 64.0f);
//	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, rotation, NULL);
//	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &src);
//	mat *= mat1;
//	sprite->SetTransform(&mat);
//	sptc = D3DXVECTOR3(center.x, center.y, 0.0f);
//
//	sprite->Draw(ls, NULL, &sptc, NULL, color);
//
//	//half circle left
//	scale = D3DXVECTOR2(thicknessScale, thicknessScale);
//	center = D3DXVECTOR2(64.0f, 64.0f);
//	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, rotation, NULL);
//	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &src);
//	mat *= mat1;
//	sprite->SetTransform(&mat);
//	sptc = D3DXVECTOR3(center.x, center.y, 0.0f);
//	sprite->Draw(hc, NULL, &sptc, NULL, color);
//
//	//half circle right
//	//scale = D3DXVECTOR2(thicknessScale, thicknessScale);
//	//center = D3DXVECTOR2(64.0f, 64.0f);
//	//sptc = D3DXVECTOR3(center.x, center.y, 0.0f);
//	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, rotation + D3DX_PI, NULL);
//	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &des);
//	mat *= mat1;
//	sprite->SetTransform(&mat);
//	sprite->Draw(hc, NULL, &sptc, NULL, color);
//
//}
void Line::Draw()
{
	D3DXMATRIX	mat, mat1;
	D3DXVECTOR2 scale, center, trans;
	D3DXVECTOR3 sptc;
	D3DXCOLOR   acolor = color;

	acolor *= 1 - life / span;

	//D3DXVECTOR3 pos;

	const float imgThickness = 8.0f;
	float		thicknessScale = thickness / imgThickness;//
														  //float		len = D3DXVec2Length(&tan);
														  //1 pixel of line
	scale = D3DXVECTOR2(size, thicknessScale);
	center = D3DXVECTOR2(0.0f, 64.0f);
	trans = D3DXVECTOR2(position.x, position.y);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, angle, NULL);
	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &trans);
	mat *= mat1;
	sprite->SetTransform(&mat);
	sptc = D3DXVECTOR3(center.x, center.y, 0.0f);

	sprite->Draw(ls, NULL, &sptc, NULL, acolor);

	//half circle left
	scale = D3DXVECTOR2(thicknessScale, thicknessScale);
	center = D3DXVECTOR2(64.0f, 64.0f);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, angle, NULL);
	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &trans);
	mat *= mat1;
	sprite->SetTransform(&mat);
	sptc = D3DXVECTOR3(center.x, center.y, 0.0f);
	sprite->Draw(hc, NULL, &sptc, NULL, acolor);

	//half circle right
	//scale = D3DXVECTOR2(thicknessScale, thicknessScale);
	//center = D3DXVECTOR2(64.0f, 64.0f);
	//sptc = D3DXVECTOR3(center.x, center.y, 0.0f);
	trans = D3DXVECTOR2(position.x + size * cos(angle), position.y + size * sin(angle));
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, angle + D3DX_PI, NULL);
	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &trans);
	mat *= mat1;
	sprite->SetTransform(&mat);
	sprite->Draw(hc, NULL, &sptc, NULL, acolor);

}

Weapon::~Weapon()
{
	for (auto bullet : bullets)
		delete bullet;
}

void Weapon::Shoot()
{
	for (auto bullet : bullets)
		if (!bullet->IsActive())
		{
			bullet->SetPos(position.x, position.y);
			bullet->SetAng(angle);
			bullet->SetActive(true);
			return;
		}
}

void Weapon::Update(float deltaTime)
{
	for (auto bullet : bullets)
		if (bullet->IsActive())
			bullet->Update(deltaTime);
}

void Weapon::Draw()
{
	//GameObject::Draw();
	for (auto bullet : bullets)
		if (bullet->IsActive())
			bullet->Draw();
}

void Shooter::Draw()
{
	std::vector<Weapon>::iterator i;
	for (i = weapons.begin(); i != weapons.end(); i++)
	{
		Weapon* weapon = &(*i);
		if (weapon->IsActive())
			weapon->Draw();
	}
}

void Shooter::Update(float deltatime)
{
	std::vector<Weapon>::iterator i;
	for (i = weapons.begin(); i != weapons.end(); i++)
	{
		Weapon* weapon = &(*i);
		if (weapon->IsActive())
		{
			weapon->SetPos(position.x, position.y);
			weapon->SetAng(angle);
			weapon->Update(deltatime);
		}
	}
}

bool Shooter::CheckCollision(const D3DXVECTOR3 *desP, float desS)
{
	D3DXVECTOR3 dis = *desP - position;
	if (D3DXVec3Length(&dis) < desS + size)
		return true;
	return false;
}

void Shooter::Attack()
{
	std::vector<Weapon>::iterator i;
	for (i = weapons.begin(); i != weapons.end(); i++)
	{
		Weapon* weapon = &(*i);
		if (weapon->IsActive())
			weapon->Shoot();
	}
}

void Shooter::UpdateExp(float deltatime)
{
	std::vector<Line>::iterator i;
	for (i = expL.begin(); i != expL.end(); i++)
	{
		Line* line = &(*i);
		if (line->IsActive())//
		{
			line->Update(deltatime);

			/*float X = line->GetPos().x;
			float Y = line->GetPos().y;
*/
			/*if (X > GB_WIDTH)
			{
				line->SetAng(D3DX_PI - line->GetAng());
				line->SetVel(-line->GetVel().x, -line->GetVel().y);
				line->SetAccel(-line->GetAcc().x, -line->GetAcc().y);
			}
			else if (X < 0)
				position.x = 0.0f;
			

			if (Y > GB_HEIGHT)
				position.y = GB_HEIGHT;
			else if (Y < 0)
				position.y = 0.0f;*/
			
		}
	}
	if (!expL[0].IsActive())
		isExplosing = false;
}

void Shooter::DrawExp()
{
	std::vector<Line>::iterator i;
	for (i = expL.begin(); i != expL.end(); i++)
	{
		Line* line = &(*i);
		if (line->IsActive())//
			line->Draw();
	}
}

void Shooter::Hit()
{
	SetActive(false);
	isExplosing = true;

	//expL
	std::vector<Line>::iterator i;
	for (i = expL.begin(); i != expL.end(); i++)
	{
		Line* line = &(*i);

		line->SetActive(true);
		line->SetPos(position.x, position.y);
		line->span = 1.0f;
		//line->life = 0.0f;

		//텍스쳐나 스프라이트를 어떻게 저장하는게 가장 좋은 것인지를 모르겠다.
		line->ls = texes[GOLSegment];
		line->hc = texes[GOLHalfCircle];
		line->SetSprite(sprite);

		D3DXVECTOR2 vel;
		D3DXVECTOR2 min = D3DXVECTOR2(-1.0f, -1.0f);
		D3DXVECTOR2 max = D3DXVECTOR2(1.0f, 1.0f);

		GetRandomVector(&vel, &min, &max);
		vel *= 500.0f;
		//D3DXVec2Normalize(&vel, &vel);
		line->SetAng(atan2(vel.y, vel.x));
		line->SetVel(vel.x, vel.y);
		line->SetAccel(-vel.x, -vel.y); //dt is 1.0f sec.
		line->SetSize(GetRandomFloat(7.0f, 10.0f));
		line->color = D3DXCOLOR(GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), GetRandomFloat(0.0f, 1.0f), 1.0f);
	}
}

void Player::Draw()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 spriteCentre(position.x, position.y);
	D3DXVECTOR3 spriteCentrev3(32.0f, 32.0f, 0.0f);
	sprite->SetTransform(D3DXMatrixTransformation2D(&mat, NULL, 0.0, NULL, &spriteCentre, angle, NULL));
	sprite->Draw(texture, NULL, &spriteCentrev3, &position, D3DCOLOR_XRGB(255, 255, 255));

	Shooter::Draw();
}

void Player::Update(float deltaTime)//
{
	//movement
	angle += angularSpeed * deltaTime;

	velocity.x = speed * cos(angle);
	velocity.y = speed * sin(angle);
	
	float X = position.x + velocity.x * deltaTime;
	float Y = position.y + velocity.y * deltaTime;

	if (X > GB_WIDTH)
		position.x = GB_WIDTH;
	else if (X < 0)
		position.x = 0.0f;
	else
		position.x = X;

	if (Y > GB_HEIGHT)
		position.y = GB_HEIGHT;
	else if (Y < 0)
		position.y = 0.0f;
	else
		position.y = Y;
	
	angularSpeed = speed = 0.0f;

	//update weapons
	Shooter::Update(deltaTime);

	//respawn check
	if (respawntime > 0)
	{
		respawntime -= deltaTime;
		if (respawntime <= 0)
			SetActive(true);
	}

	shootRate += deltaTime;
}

void Player::Input() //test
{
	//if dead
	if (!IsActive()) return;

	if (KEY_DOWN(0x57)) //w: 속도 설정.
		speed = 300.0f;
	

	if (KEY_DOWN(0x41)) //a: 왼쪽으로 각속도 설정.
		angularSpeed = -D3DX_PI * 1.7;
	

	if (KEY_DOWN(0x44)) //d: 오른쪽으로 각속도 설정.
		angularSpeed = (angularSpeed == 0.0f ? D3DX_PI * 1.7 : 0.0f); //a가 눌려 각속도가 설정됬는데 d도 눌려있는 경우 0으로.

	
	if (KEY_DOWN(VK_SPACE) && shootRate > 0.05f)
	{
		Shooter::Attack();
		shootRate = 0.0f;
	}

}

void Enemy::Draw()
{
	D3DXMATRIX mat;
	D3DXVECTOR2 spriteCentre(position.x, position.y);
	D3DXVECTOR3 spriteCentrev3(32.0f, 32.0f, 0.0f);
	sprite->SetTransform(D3DXMatrixTransformation2D(&mat, NULL, 0.0, NULL, &spriteCentre, angle, NULL));
	sprite->Draw(texture, NULL, &spriteCentrev3, &position, D3DCOLOR_XRGB(255, 255, 255));

	Shooter::Draw();
}

//todo rand
void DirectionalEnemy::Update(float deltatime)
{
	velocity.x = speed * cos(angle);
	velocity.y = speed * sin(angle);

	float X = position.x + velocity.x * deltatime;
	float Y = position.y + velocity.y * deltatime;

	//
	if (X > GB_WIDTH || X < 0 || Y > GB_HEIGHT || Y < 0)
		Hit();
	else
	{
		position.x = X;
		position.y = Y;
	}
}


void DirectionalBullet::Update(float deltatime)
{
	speed = att.speed;

	velocity.x = speed * cos(angle);
	velocity.y = speed * sin(angle);

	float X = position.x + velocity.x * deltatime;
	float Y = position.y + velocity.y * deltatime;

	//
	if (X > GB_WIDTH || X < 0 || Y > GB_HEIGHT || Y < 0)
		SetActive(false);
	else
	{
		position.x = X;
		position.y = Y;
	}
}

void DirectionalBullet::Draw()
{
	D3DXMATRIX	mat, mat1;
	D3DXVECTOR2 scale, center, trans;
	D3DXVECTOR3 sptc;
	center = D3DXVECTOR2(16.0f, 16.0f);
	scale  = D3DXVECTOR2(0.5f, 0.5f);
	trans  = D3DXVECTOR2(position.x, position.y);
	D3DXMatrixTransformation2D(&mat, NULL, 0.0f, &scale, NULL, angle, NULL);
	D3DXMatrixTransformation2D(&mat1, NULL, 0.0f, NULL, &center, 0.0f, &trans);
	mat *= mat1;
	sprite->SetTransform(&mat);
	sptc = D3DXVECTOR3(center.x, center.y, 0.0f);

	sprite->Draw(texture, NULL, &sptc, NULL, D3DCOLOR_XRGB(224, 192, 32));
}
