#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ImageID, int startX, int startY, StudentWorld* world, int startDirection, double size ,
	int depth):GraphObject(ImageID,startX,startY,startDirection,size,depth)
{
	m_world = world;
	m_alive = true;
}

Actor::~Actor()
{
	
}
bool Actor::isAlive() const
{
	return m_alive;
}
bool Actor::doSomething()
{
	if (!isAlive())
		return false;
	if (isOutOfBounds(getX(),getY()))
	{
		m_alive = false;
		return false;
	}
	return true;
}

bool Actor::isOutOfBounds(double x, double y) const
{
	return (x < 0 || x >= VIEW_WIDTH);
}

StudentWorld* Actor::getWorld() const
{
	return m_world;
}
void Actor::sufferDamage(int damage)
{
	if (!canCollide())
		return;
	if (!canBeAttacked())
		setDead();
}
void Actor::collision(Actor* other)
{
	if (!canCollide())
		return;
	
}
void Actor::setDead()
{
	m_alive = false;
}
Ship::Ship(int ImageID, int startX, int startY,  StudentWorld* world,
	double size, int depth, int shootOffset): Actor(ImageID, startX, startY, world,
		0, size, depth)
{
	m_shootOffset = shootOffset;
}

bool Ship::canCollide() const
{
	return true;
}
bool Ship::canBeAttacked() const
{
	return true;
}
void Ship::sufferDamage(int damage)
{
	m_hitPoints -= damage;
	if (m_hitPoints <= 0)
		setDead();
}
Ship::~Ship()
{

}
void Ship::shoot(int projectile)
{
	Projectile* p;
	int x, y;
	x = getX() + getMoveDirectionX() * m_shootOffset;
	y = getY();
	if (projectile == IID_CABBAGE)
	{
		p = new Cabbage(x, y,getWorld());
	}
	else if (projectile == IID_TURNIP)
	{
		p = new Turnip(x,y,getWorld() );
	}
	else if (projectile == IID_TORPEDO)
	{
		p = new Torpedo(x, y, getWorld(), getMoveDirectionX());
	}
	getWorld()->addNewActor(p);
	if (projectile == IID_TURNIP)
		getWorld()->playSound(SOUND_ALIEN_SHOOT);
	else if (projectile == IID_CABBAGE)
		getWorld()->playSound(SOUND_PLAYER_SHOOT);
	else if (projectile == IID_TORPEDO)
		getWorld()->playSound(SOUND_TORPEDO);

}
void Ship::setHitPoints(double hitPoints)
{
	m_hitPoints = hitPoints;
}
double Ship::getHitPoints() const
{
	return m_hitPoints;
}
NachenBlaster::NachenBlaster(StudentWorld* world)
	: Ship( IID_NACHENBLASTER, 0, 128, world,1, 0,12)
{
	m_cabbagePoints = m_maxCabbage = 30;
	m_numTorpedos = 0;
	setHitPoints(50);
	m_maxHitPoints = getHitPoints();
}
int NachenBlaster::getMoveDirectionX() const
{
	return 1;
}

bool NachenBlaster::doSomething()
{
	if (!Actor::doSomething())
		return false;
	int keyVal;
	if (getWorld()->getKey(keyVal))
	{
		double x = getX(), y = getY();
		if (keyVal == KEY_PRESS_LEFT && !isOutOfBounds(x - 6, y))
			x -= 6;
		else if (keyVal == KEY_PRESS_RIGHT && !isOutOfBounds(x + 6, y))
			x += 6;
		else if (keyVal == KEY_PRESS_DOWN && !isOutOfBounds(x, y - 6))
			y -= 6;
		else if (keyVal == KEY_PRESS_UP && !isOutOfBounds(x, y + 6))
			y += 6;
		else if (keyVal == KEY_PRESS_SPACE && m_cabbagePoints >= 5)
		{
			shoot(IID_CABBAGE);
			m_cabbagePoints -= 5;
		}
		else if (keyVal == KEY_PRESS_TAB)
		{
			if (m_numTorpedos > 0)
			{
				shoot(IID_TORPEDO);
				m_numTorpedos--;
			}
		}
		moveTo(x, y);
	}
	if (m_cabbagePoints < m_maxCabbage)
		m_cabbagePoints++;
	return true;
}


bool NachenBlaster::isEnemy() const
{
	return false;
}
void NachenBlaster::repair(double amt)
{
	int hitPoints = amt + getHitPoints();
	if (hitPoints <= 50)
		setHitPoints(hitPoints);
	else
		setHitPoints(50);
}
void NachenBlaster::incTorpedoes(int amt)
{
	m_numTorpedos += amt;
}
void NachenBlaster::setStats(int& hitPercent, int& cabbagePercent, int& numTorp) const
{
	hitPercent = getHitPoints() / m_maxHitPoints * 100;
	cabbagePercent = m_cabbagePoints * 100 / m_maxCabbage ;
	//cabbagePercent = m_maxCabbage;
	numTorp = m_numTorpedos;
}
NachenBlaster::~NachenBlaster()
{}

bool NachenBlaster::isOutOfBounds(double x, double y) const
{
	return (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT);
}

FlyingActor::FlyingActor(int ImageID, int startX, int startY, StudentWorld* world,
	int startDirection, double size, int moveDirectionX, int moveDirectionY, double moveAmt,
	int depth, bool _canCollide)
	: Actor(ImageID, startX, startY, world, startDirection, size, depth)
{
	m_moveDirectionX = moveDirectionX;
	m_moveDirectionY = moveDirectionY;
	m_canCollide = _canCollide;
	m_moveAmt = moveAmt;
}

bool FlyingActor::doSomething()
{	
	if (!Actor::doSomething())
		return false;

	getWorld()->checkForCollision(this);

	if (!isAlive())
		return false;

	move();

	getWorld()->checkForCollision(this);
	if (!isAlive())
		return false;
	return true;
}
void FlyingActor::move()
{
	moveTo(getX() + m_moveDirectionX * m_moveAmt, getY() + m_moveDirectionY * m_moveAmt);
}
bool FlyingActor::canBeAttacked() const
{
	return false;
}

bool FlyingActor::canCollide() const
{
	return m_canCollide;
}
bool FlyingActor::isEnemy() const
{
	return m_moveDirectionX == -1;
}
int FlyingActor::getMoveDirectionX() const
{
	return m_moveDirectionX;
}
double FlyingActor::getMoveAmt() const
{
	return m_moveAmt;
}
FlyingActor::~FlyingActor()
{}


Star::Star(int startX, int startY, StudentWorld* world, double size)
	: FlyingActor(IID_STAR,startX,startY,world,0,size,-1, 0, 1, 3,false)
{}

bool Star::isEnemy() const
{
	return false;
}

Star::~Star()
{}

Projectile::Projectile(int ImageID, int startX, int startY, StudentWorld* world,
	int startDirection, double size, int moveDirection, double moveAmt, int depth, int damage)
	: FlyingActor(ImageID, startX, startY, world, startDirection, size, moveDirection, 0, moveAmt, depth, true)
{
	m_damage = damage;
}
Projectile::~Projectile()
{}

void Projectile::move()
{
	FlyingActor::move();
	int dir = getDirection() < 340 ? getDirection() : getDirection() - 360;
	setDirection(dir + 20);
}

void Projectile::collision(Actor* other)
{
	if (other->canBeAttacked() && other->isEnemy() != isEnemy())
	{
		other->sufferDamage(m_damage);
		if (other->isAlive())
		{
			getWorld()->playSound(SOUND_BLAST);
		}
		setDead();
	}
}



Cabbage::Cabbage(int startX, int startY, StudentWorld* world)
	:Projectile(IID_CABBAGE,startX,startY,world,0,0.5,1,8,1, 2)
{}

Cabbage::~Cabbage()
{}

Turnip::Turnip(int startX, int startY, StudentWorld* world)
	:Projectile(IID_TURNIP,startX,startY,world,0,0.5,-1,6,1, 2)
{}

Turnip::~Turnip()
{}

Torpedo::Torpedo(int startX, int startY, StudentWorld* world, int moveDirection)
	:Projectile(IID_TORPEDO, startX, startY, world, 
		moveDirection == 1 ? 0 : 180, 
		0.5, moveDirection, 8, 1, 8)
{}

Torpedo::~Torpedo()
{}

void Torpedo::move()
{
	FlyingActor::move();
}

EnemyShip::EnemyShip(int ImageID, int startX, int startY, StudentWorld* world, 
	int damage, int points)
	: Ship(ImageID,startX,startY,world,1.5,1,14)
{
	m_plan.length = 0;
	m_plan.speed = 2.0;
	m_plan.direction = -1;
	m_damage = damage;
	m_points = points;
}

EnemyShip::~EnemyShip()
{
}

bool EnemyShip::isEnemy() const
{
	return true;
}
int EnemyShip::getMoveDirectionX() const
{
	return -1;
}
int EnemyShip::getMoveDirectionY() const
{
	if (m_plan.direction == UP_LEFT)
		return 1;
	else if (m_plan.direction == DOWN_LEFT)
		return -1;
	else
		return 0;
}
void EnemyShip::move()
{
	moveTo(getX() + getMoveDirectionX() * m_plan.speed, getY() + getMoveDirectionY() * m_plan.speed);
	m_plan.length--;
}
void EnemyShip::correctDirection()
{
	if (getY() >= VIEW_HEIGHT - 1)
		m_plan.direction = DOWN_LEFT;
	else if (getY() <= 0)
		m_plan.direction = UP_LEFT;
}
void EnemyShip::correctLength()
{
	if (m_plan.length <= 0)
	{
		m_plan.length = randInt(1, 32);
		m_plan.direction = randInt(LEFT, LEFT + 2);
	}	
}
void EnemyShip::setFlightPlan(int dir, int len, double speed)
{
	m_plan.direction = dir;
	m_plan.length = len;
	m_plan.speed = speed;
}
bool EnemyShip::doSomething()
{
	if (!Actor::doSomething())
		return false;
	getWorld()->checkForCollision(this);
	if (!isAlive())
		return false;
	correctDirection();
	correctLength();
	const NachenBlaster* player = getWorld()->getNacheBlaster();
	if (abs(getY() - player->getY()) <= 4 && player->getX() < getX())
	{
		if (handleNearPlayerY())
			return true;
	}
	move();
	getWorld()->checkForCollision(this);
	return isAlive();
}
bool EnemyShip::shouldShoot() const
{
	int n = randInt(1,20/getWorld()->getLevel() + 5);
	return n == 1;
}
void EnemyShip::collision(Actor* other)
{
	if (other->isEnemy() != isEnemy() && other->canCollide())
	{
		other->sufferDamage(m_damage);
		if (other->canBeAttacked())
		{
			setDead();
		}
	}
}
void EnemyShip::setDead()
{
	Actor::setDead();
	getWorld()->shipDestroyed();
	getWorld()->increaseScore(m_points);
	getWorld()->playSound(SOUND_DEATH);
	getWorld()->addNewActor(new Explosion(getX(),getY(),getWorld()));
	dropGoodie();
}

Smallgon::Smallgon(int startX, int startY, StudentWorld* world)
	: EnemyShip(IID_SMALLGON, startX, startY, world, 5, 250)
{
	setHitPoints(5 * (1 + (getWorld()->getLevel() - 1) * 0.1));
}
Smallgon::~Smallgon()
{}

bool Smallgon::handleNearPlayerY()
{
	if (shouldShoot())
	{
		shoot(IID_TURNIP);
		return true;
	}
	return false;
}
void Smallgon::dropGoodie()
{
	return;
}
Smoregon::Smoregon(int startX, int startY, StudentWorld* world)
	: EnemyShip(IID_SMOREGON, startX, startY, world,5, 250)
{
	setHitPoints(5 * (1 + (getWorld()->getLevel() - 1) * 0.1));
}
bool Smoregon::handleNearPlayerY()
{
	if (shouldShoot())
	{
		shoot(IID_TURNIP);
		return true;
	}
	if (shouldShoot())
	{
		setFlightPlan(LEFT,VIEW_WIDTH,5.0);
	}
	return false;
}
void Smoregon::dropGoodie()
{
	int chance = randInt(1, 6);
	if (chance <= 2)
	{
		if (chance == 1)
			getWorld()->addNewActor(new RepairGoodie(getX(), getY(), getWorld()));
		else
			getWorld()->addNewActor(new TorpedoGoodie(getX(), getY(), getWorld()));
	}
}
Smoregon::~Smoregon()
{}
Snagglegon::Snagglegon(int startX, int startY, StudentWorld* world)
	: EnemyShip(IID_SNAGGLEGON, startX, startY, world, 15, 1000)
{
	setHitPoints(10 * (1 + (getWorld()->getLevel() - 1) * 0.1));
	setFlightPlan(DOWN_LEFT, VIEW_WIDTH, 1.75);
}
bool Snagglegon::handleNearPlayerY()
{
	if (shouldShoot())
	{
		shoot(IID_TORPEDO);
		return true;
	}
	return false;
}
void Snagglegon::dropGoodie()
{
	int chance = randInt(1, 6);
	if (chance == 1)
	{
		getWorld()->addNewActor(new ExtraLifeGoodie(getX(),getY(),getWorld()));
	}
}
bool Snagglegon::shouldShoot() const
{
	int n = randInt(1, 15 / getWorld()->getLevel() + 10);
	return n == 1;
}
void Snagglegon::correctLength()
{
	return;
}
Snagglegon::~Snagglegon()
{}

Goodie::Goodie(int ImageID, int startX, int startY, StudentWorld* world)
	: FlyingActor(ImageID,startX,startY,world,0,0.5,-1,-1,0.75,1,false)
{	
}
bool Goodie::isOutOfBounds(double x, double y) const
{
	return (x < 0 || x >= VIEW_WIDTH || y < 0 || y >= VIEW_HEIGHT);
}
bool Goodie::isEnemy() const
{
	return false;
}
void Goodie::collision(Actor* other)
{
	if (other->canBeAttacked() && !other->isEnemy())
	{
		getWorld()->increaseScore(100);
		handlePickUp();
		getWorld()->playSound(SOUND_GOODIE);
		setDead();		
	}
}

Goodie::~Goodie()
{
}

RepairGoodie::RepairGoodie(int startX, int startY, StudentWorld* world)
	: Goodie(IID_REPAIR_GOODIE,startX,startY,world)
{}
void RepairGoodie::handlePickUp()
{
	getWorld()->repairPlayer(10);
}
RepairGoodie::~RepairGoodie()
{}

ExtraLifeGoodie::ExtraLifeGoodie(int startX, int startY, StudentWorld* world)
	: Goodie(IID_LIFE_GOODIE, startX, startY, world)
{}
void ExtraLifeGoodie::handlePickUp()
{
	getWorld()->incLives();
}
ExtraLifeGoodie::~ExtraLifeGoodie()
{}

TorpedoGoodie::TorpedoGoodie(int startX, int startY, StudentWorld* world)
	: Goodie(IID_TORPEDO_GOODIE, startX, startY, world)
{}
void TorpedoGoodie::handlePickUp()
{
	getWorld()->addTorpedoes(5);
}
TorpedoGoodie::~TorpedoGoodie()
{}
Explosion::Explosion(int startX, int startY, StudentWorld* world)
	: Actor(IID_EXPLOSION, startX, startY, world, 0, 1, 0)
{
	m_timeLeft = 4;
}
Explosion::~Explosion()
{}
bool Explosion::doSomething()
{
	if (!Actor::doSomething())
		return false;
	if (m_timeLeft == 0)
	{
		setDead();
		return false;
	}
	m_timeLeft--;
	setSize(1.5 * getSize());
	return true;
}
bool Explosion::canCollide() const
{
	return false;
}
bool Explosion::canBeAttacked() const
{
	return false;
}
bool Explosion::isEnemy() const
{
	return false;
}
int Explosion::getMoveDirectionX() const
{
	return 0;
}
