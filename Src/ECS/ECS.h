#pragma once

#include <iostream>
#include<vector>
#include<algorithm>
#include<memory>
#include<bitset>
#include<array>
#include <mutex>
#include <thread>
#include <exception>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
	
private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componenetArray;
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitset;

public:
	Entity(Manager& mManager) :manager(mManager)
	{

	}
	void update()
	{
		for (auto& c : components) c->update();
	}

	void draw()
	{
		for (auto& c : components) c->draw();
	}
	bool isActive() { return active; }
	void destroy() { active = false; }

	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	void addGroup(Group mGroup);

	void deleteGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}
	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template<typename T, typename ... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componenetArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;
		
		c->init();

		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componenetArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;

public:
	std::vector<std::thread> threads;
	mutable std::mutex mutex;

	void updateEntities(std::size_t start, std::size_t end)
	{
		for (std::size_t i = start; i < end; ++i)
		{
			entities[i]->update();
		}
	}

	void update()
	{
		std::size_t numEntities = entities.size();
		std::size_t numThreads = std::thread::hardware_concurrency();
		std::cout << numThreads<<std::endl;
		std::size_t entitiesPerThread = numEntities / numThreads;

		std::vector<std::thread> updateThreads;

		for (std::size_t i = 0; i < numThreads - 1; ++i)
		{
			std::size_t start = i * entitiesPerThread;
			std::size_t end = start + entitiesPerThread;

			updateThreads.emplace_back(&Manager::updateEntities, this, start, end);
		}

		// Ostatni w¹tek mo¿e obs³u¿yæ pozosta³e encje, które nie mieszcz¹ siê w równym podziale
		std::size_t start = (numThreads - 1) * entitiesPerThread;
		std::size_t end = numEntities;

		updateEntities(start, end);

		// Oczekiwanie na zakoñczenie wszystkich w¹tków
		for (auto& thread : updateThreads)
		{
			thread.join();
		}
	}
	void draw()
	{
		for (auto& e : entities)
		{
			e->draw();
		}
	}

	void refresh()
	{	
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),[i](Entity* mEntity)
					{
						return !mEntity->isActive() || !mEntity->hasGroup(i);
					}
				),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(entities));
	}

	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}
	
	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};