#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <bitset>
#include <array>
#include <mutex>
#include <thread>
#include <exception>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

// Function to generate a new component type ID
inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

// Template function to get the component type ID
template <typename T>
inline ComponentID getComponentTypeID() noexcept
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
	Entity(Manager& mManager) : manager(mManager) {}

	// Update all components of the entity
	void update()
	{
		for (auto& c : components)
			c->update();
	}

	// Draw all components of the entity
	void draw()
	{
		for (auto& c : components)
			c->draw();
	}

	// Check if the entity is active
	bool isActive()
	{
		return active;
	}

	// Set the entity as inactive
	void destroy()
	{
		active = false;
	}

	// Check if the entity belongs to a group
	bool hasGroup(Group mGroup)
	{
		return groupBitset[mGroup];
	}

	// Add the entity to a group
	void addGroup(Group mGroup);

	// Remove the entity from a group
	void deleteGroup(Group mGroup)
	{
		groupBitset[mGroup] = false;
	}

	// Check if the entity has a specific component
	template <typename T>
	bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	// Add a new component to the entity
	template <typename T, typename... TArgs>
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

	// Get the reference to a specific component of the entity
	template <typename T>
	T& getComponent() const
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
	std::mutex mutex;

public:
	// Update a range of entities in parallel
	void updateEntities(std::size_t start, std::size_t end)
	{
		for (std::size_t i = start; i < end; ++i)
		{
			std::lock_guard<std::mutex> lock(mutex);
			entities[i]->update();
		}
	}

	// Update all entities in parallel using multiple threads
	void update()
	{
		std::size_t numEntities = entities.size();
		std::size_t numThreads = std::thread::hardware_concurrency();

		std::size_t entitiesPerThread = numEntities / numThreads;

		std::vector<std::thread> updateThreads;

		for (std::size_t i = 0; i < numThreads - 1; ++i)
		{
			std::size_t start = i * entitiesPerThread;
			std::size_t end = start + entitiesPerThread;

			// Create a thread for updating a range of entities
			updateThreads.emplace_back(&Manager::updateEntities, this, start, end);
		}

		std::size_t start = (numThreads - 1) * entitiesPerThread;
		std::size_t end = numEntities;

		// Update the remaining entities in the current thread
		updateEntities(start, end);

		// Wait for all update threads to finish
		for (auto& thread : updateThreads)
		{
			thread.join();
		}
	}

	// Draw all entities
	void draw()
	{
		for (auto& e : entities)
		{
			e->draw();
		}
	}

	// Remove inactive entities and entities not belonging to any group
	void refresh()
	{
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v), [i](Entity* mEntity)
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

	// Add an entity to a group
	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	// Get a vector of entities belonging to a specific group
	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	// Create a new entity and add it to the manager
	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}
};
