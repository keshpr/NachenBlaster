#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>
class Actor;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	virtual ~StudentWorld();

	bool checkForCollision(Actor* actor);
	void addNewActor(Actor* actor);
	void shipDestroyed();	
	void repairPlayer(double amt);
	void addTorpedoes(int amt);
	const NachenBlaster* getNacheBlaster() const;

	
private:
	std::list<Actor*> m_actors;
	NachenBlaster* m_player;
	int m_shipsDestroyed;
	int m_AliensOnScreen;
	std::string m_gameStats;
	

	void addNewFlyingObjects();
	int getMaxAliens();
	int getMaxAliensOnScreen();
	void addEnemyShip();
	void addStar();
	void setStatText();
	void shipLeftScreen();
};

#endif // STUDENTWORLD_H_
