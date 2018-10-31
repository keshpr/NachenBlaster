#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <cmath>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
	m_player = nullptr;
	m_shipsDestroyed = 0;
	m_AliensOnScreen = 0;
}

int StudentWorld::init()
{
	m_player = new NachenBlaster(this);
	m_actors.push_back(m_player);
	m_shipsDestroyed = 0;
	m_AliensOnScreen = 0;
	for (int i = 0; i < 30; i++)
	{
		int x = randInt(0, VIEW_WIDTH - 1);
		int y = randInt(0, VIEW_HEIGHT - 1);
		double size = static_cast<double>(randInt(5, 50)) / 100;
		m_actors.push_back(new Star(x,y,this,size));
	}

	setStatText();
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{	
	//m_player->doSomething();
	setStatText();
	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		(*it)->doSomething();
	}
	for (it = m_actors.begin(); it != m_actors.end();)
	{
		if (!(*it)->isAlive())
		{
			if ((*it)->canBeAttacked() && !(*it)->isEnemy())
			{
				decLives();
				return GWSTATUS_PLAYER_DIED;
			}
			else if ((*it)->canBeAttacked())
			{
				shipLeftScreen();
			}
			delete (*it);
			it = m_actors.erase(it);
		}
		else
			it++;
	}
	//delete m_player;
	//m_player = nullptr;
	addNewFlyingObjects();
	if (m_shipsDestroyed == getMaxAliens())
	{
		playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
	//delete m_player;
	//m_player = nullptr;
	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end();)
	{
		delete (*it);
		it = m_actors.erase(it);
	}
	m_player = nullptr;
}

void StudentWorld::addNewFlyingObjects()
{
	addStar();
	addEnemyShip();
}
void StudentWorld::addNewActor(Actor* actor)
{
	m_actors.push_back(actor);
}
StudentWorld::~StudentWorld()
{
	cleanUp();
}
bool StudentWorld::checkForCollision(Actor* actor)
{
	int flag = 0;
	list<Actor*>::iterator it;
	for (it = m_actors.begin(); it != m_actors.end(); it++)
	{
		double sq = pow(actor->getX() - (*it)->getX(), 2) + pow(actor->getY() - (*it)->getY(), 2);
		double dist = sqrt(sq);
		if (actor != (*it) && abs(dist) < 0.75 * (actor->getRadius() + (*it)->getRadius()))
		{
			flag = 1;
			actor->collision(*it);
			(*it)->collision(actor);
		}
	}
	return flag == 1;
}

const NachenBlaster* StudentWorld::getNacheBlaster() const
{
	return m_player;
}

void StudentWorld::shipDestroyed()
{
	m_shipsDestroyed++;
}
void StudentWorld::shipLeftScreen()
{
	m_AliensOnScreen--;
}
int StudentWorld::getMaxAliens()
{
	return 6 + 4 * getLevel();
	//return 1;
}
int StudentWorld::getMaxAliensOnScreen()
{
	return 4 + (0.5 * getLevel());
	//return 1;
}
void StudentWorld::addStar()
{
	int c = randInt(1, 15);
	if (c == 1)
	{
		int y = randInt(0, VIEW_HEIGHT - 1);
		double size = static_cast<double>(randInt(5, 50)) / 100;
		m_actors.push_back(new Star(VIEW_WIDTH - 1, y, this, size));
	}
}
void StudentWorld::addEnemyShip()
{
	int r = getMaxAliens() - m_shipsDestroyed;
	int m = getMaxAliensOnScreen();
	int min = m < r ? m : r;
	if (m_AliensOnScreen < min)
	{
 		int s1 = 60;
		int s2 = 20 + getLevel() * 5;
 		int s3 = 5 + getLevel() * 10;
		int s = s1 + s2 + s3;
		int ran = randInt(1,s);		
		int y = randInt(0, VIEW_HEIGHT - 1);
		if (ran <= s1)
			m_actors.push_back(new Smallgon(VIEW_WIDTH - 1, y, this));
		else if (ran <= s1 + s2)
			m_actors.push_back(new Smoregon(VIEW_WIDTH - 1, y, this));
		else
			m_actors.push_back(new Snagglegon(VIEW_WIDTH - 1, y, this));
		//m_actors.push_back(new ExtraLifeGoodie(VIEW_WIDTH/2,y,this));
		m_AliensOnScreen++;
	}
}
void StudentWorld::repairPlayer(double amt)
{
	m_player->repair(amt);
}
void StudentWorld::addTorpedoes(int amt)
{
	m_player->incTorpedoes(amt);
}
void StudentWorld::setStatText()
{
	ostringstream statStream;
	statStream << "Lives: " << getLives() << "  ";
	int hitPercent, cabbagePercent, numTorp;
	m_player->setStats(hitPercent, cabbagePercent, numTorp);
	statStream << "Health: " << hitPercent << "%" << "  ";
	statStream << "Score: " << getScore() << "  ";
	statStream << "Level: " << getLevel() << "  ";
	statStream << "Cabbages: " << cabbagePercent << "%" << "  ";
	statStream << "Torpedoes: " << numTorp << "  ";
	setGameStatText(statStream.str());
}