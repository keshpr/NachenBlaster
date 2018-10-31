#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class StudentWorld;
const int LEFT = 0;
const int UP_LEFT = LEFT + 1;
const int DOWN_LEFT = UP_LEFT + 1;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
	Actor(int ImageID, int startX, int startY, StudentWorld* world, 
		int startDirection, double size, int depth);	
		
	bool isAlive() const;
	StudentWorld* getWorld() const;		
	virtual ~Actor();
	virtual bool doSomething();
	virtual bool canCollide() const = 0;
	virtual bool canBeAttacked() const = 0;
	virtual bool isEnemy() const = 0;	
	virtual void collision(Actor* other);
	virtual void sufferDamage(int damage);
	
protected:
	virtual int getMoveDirectionX() const = 0;
	virtual bool isOutOfBounds(double x, double y) const;
	virtual void setDead();
private:
	StudentWorld* m_world;
	bool m_alive;
	
};

class Ship : public Actor
{
public:
	Ship(int ImageID, int startX, int startY, StudentWorld* world, 
		double size, int depth, int shootOffset);
	virtual ~Ship();
	virtual bool canCollide() const;
	virtual bool canBeAttacked() const;
	virtual void sufferDamage(int damage);
	double getHitPoints() const;
protected:
	virtual void shoot(int projetile);
	void setHitPoints(double hitPoints);
	
private:
	double m_hitPoints;
	int m_shootOffset;

	
};
class Explosion : public Actor
{
public:
	Explosion(int startX, int startY, StudentWorld* world);
	~Explosion();
	virtual bool doSomething();
	virtual bool canCollide() const;
	virtual bool canBeAttacked() const;
	virtual bool isEnemy() const;
protected:
	virtual int getMoveDirectionX() const;
private:
	int m_timeLeft;
};

class NachenBlaster : public Ship
{
public:
	NachenBlaster(StudentWorld* world);
	virtual ~NachenBlaster();
	virtual bool doSomething();	
	virtual bool isEnemy() const;
	void repair(double amt);
	void incTorpedoes(int amt);
	void setStats(int& hitPercent, int& cabbagePercent, int& numTorp) const;
protected:
	virtual bool isOutOfBounds(double x, double y) const;
	virtual int getMoveDirectionX() const;
private:
	int m_cabbagePoints;
	int m_maxCabbage;
	int m_numTorpedos;
	int m_maxHitPoints;
	//virtual void shoot(int projectile);

};
class FlyingActor : public Actor
{
public:
	FlyingActor(int ImageID, int startX, int startY, StudentWorld* world,
		int startDirection, double size, int moveDirectionX, int moveDirectionY, 
		double moveAmt,	int depth, bool _canCollide);
	virtual ~FlyingActor();
	virtual bool doSomething();
	virtual bool canCollide() const;
	virtual bool canBeAttacked() const;
	virtual bool isEnemy() const;	
	double getMoveAmt() const;
protected:
	virtual int getMoveDirectionX() const;
	virtual void move();
private:
	int m_moveDirectionX;
	int m_moveDirectionY;
	bool m_canCollide;
	double m_moveAmt;
};
class Star : public FlyingActor
{
public:
	Star(int startX, int startY, StudentWorld* world, double size);
	virtual ~Star();
	virtual bool isEnemy() const;
private:
};

class Projectile : public FlyingActor
{
public:
	Projectile(int ImageID, int startX, int startY, StudentWorld* world,
		int startDirection, double size, int moveDirection, double moveAmt, int depth, int damage);
	virtual ~Projectile();
	virtual void collision(Actor* other);
protected:
	virtual void move();
private:
	int m_damage;
};

class Cabbage : public Projectile
{
public:
	Cabbage(int startX, int startY, StudentWorld* world);
	virtual ~Cabbage();
protected:
private:
};
class Turnip : public Projectile
{
public:
	Turnip(int startX, int startY, StudentWorld* world);
	virtual ~Turnip();
protected:
private:
};
class Torpedo : public Projectile
{
public:
	Torpedo(int startX, int startY, StudentWorld* world, int moveDirection);
	virtual ~Torpedo();
protected:
	virtual void move();
private:
};
class EnemyShip : public Ship
{
public:
	EnemyShip(int ImageID, int startX, int startY, StudentWorld* world, int damage, int points);
	virtual ~EnemyShip();
	virtual bool isEnemy() const;	
	virtual bool doSomething();
	virtual void collision(Actor* other);
protected:
	virtual int getMoveDirectionX() const;
	virtual int getMoveDirectionY() const;
	virtual void move();
	void correctDirection();
	virtual void correctLength();
	virtual void setDead();
	virtual bool handleNearPlayerY() = 0;
	virtual bool shouldShoot() const;
	void setFlightPlan(int dir, int len, double speed);
	virtual void dropGoodie() = 0;

private:
	struct FlightPlan
	{
		int direction;
		int length;
		double speed;
	};
	FlightPlan m_plan;	
	int m_damage;
	int m_points;
};

class Smallgon : public EnemyShip
{
public:
	Smallgon(int startX, int startY,  StudentWorld* world);
	virtual ~Smallgon();
protected:
	virtual bool handleNearPlayerY();
	virtual void dropGoodie();
};
class Smoregon : public EnemyShip
{
public:
	Smoregon(int startX, int startY, StudentWorld* world);
	virtual ~Smoregon();
protected:
	virtual bool handleNearPlayerY();
	virtual void dropGoodie();
};
class Snagglegon : public EnemyShip
{
public:
	Snagglegon(int startX, int startY, StudentWorld* world);
	virtual ~Snagglegon();
protected:
	virtual bool handleNearPlayerY();
	virtual void dropGoodie();
	virtual bool shouldShoot() const;
	virtual void correctLength();
};

class Goodie : public FlyingActor
{
public:
	Goodie(int ImageID, int startX, int startY, StudentWorld* world);
	virtual ~Goodie();
	virtual void collision(Actor* other);
	virtual bool isEnemy() const;
protected:
	virtual bool isOutOfBounds(double x, double y) const;
	virtual void handlePickUp() = 0;
};
class RepairGoodie : public Goodie
{
public:
	RepairGoodie(int startX, int startY, StudentWorld* world);
	virtual ~RepairGoodie();
protected:
	virtual void handlePickUp();
};
class ExtraLifeGoodie : public Goodie
{
public:
	ExtraLifeGoodie(int startX, int startY, StudentWorld* world);
	virtual ~ExtraLifeGoodie();
protected:
	virtual void handlePickUp();
};
class TorpedoGoodie : public Goodie
{
public:
	TorpedoGoodie(int startX, int startY, StudentWorld* world);
	virtual ~TorpedoGoodie();
protected:
	virtual void handlePickUp();
};
#endif // ACTOR_H_
