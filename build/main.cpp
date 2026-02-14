#include <iostream>
//#include <cassert>
//#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
//#include <chrono>
using namespace std;

extern "C"
{
    #include "../lua_548/lua.h"
    #include "../lua_548/lualib.h"
    #include "../lua_548/lauxlib.h"
}

#include <cstdint>
#include <array>
#include <bitset>

typedef uint32_t entity_id;
using ComponentTypeID = uint32_t;
constexpr uint32_t MAX_COMPONENTS = 32;
struct Component { virtual ~Component() = default; };

inline ComponentTypeID getUniqueComponentID()
{
    static ComponentTypeID lastID = 0;
    return lastID++;
}

template<typename T> ComponentTypeID getComponentTypeID()
{
    static_assert(is_base_of<Component, T>::value, "T must inherit from Component");
    static ComponentTypeID typeID = getUniqueComponentID();
    return typeID;
}

class Entity
{
    entity_id id;
    bitset<MAX_COMPONENTS> componentBitset;
    array<Component*, MAX_COMPONENTS> componentArray;
    
public :
    explicit Entity(entity_id eid) : id(eid) { componentArray.fill(nullptr); }
    
    entity_id getID() const { return id; }
    
    template<typename T> bool hasComponent() const { return componentBitset[getComponentTypeID<T>()]; }
    
    template<typename T, typename... TArgs> T& addComponent(TArgs&&... args)
    {
        T* component = new T(forward<TArgs>(args)...);
        
        ComponentTypeID componentID = getComponentTypeID<T>();
        
        if (hasComponent<T>()) { delete componentArray[componentID]; }
        
        componentArray[componentID] = component;
        componentBitset[componentID] = true;
        
        return *component;
    }
    
    template<typename T> T& getComponent() const
    {
        ComponentTypeID componentID = getComponentTypeID<T>();
        return *static_cast<T*>(componentArray[componentID]);
    }
    
    template<typename T> void removeComponent()
    {
        ComponentTypeID componentID = getComponentTypeID<T>();
        delete componentArray[componentID];
        componentArray[componentID] = nullptr;
        componentBitset[componentID] = false;
    }
    
    ~Entity() { for (auto& component : componentArray) { delete component; }}
};

class System
{
protected :
    vector<Entity*> entities;
    
public :
    virtual ~System() = default;
    
    void addEntity(Entity* entity)    { entities.push_back(entity); }
    void removeEntity(Entity* entity) { entities.erase(remove(entities.begin(), entities.end(), entity),  entities.end()); }
    virtual void update(float deltaTime) = 0;
};

class ECSManager
{
    vector<unique_ptr<Entity>> entities;
    unordered_map<ComponentTypeID, vector<Entity*>> componentEntities;
    vector<unique_ptr<System>> systems;
    entity_id nextentity_id = 0;
    
public:
    Entity* createEntity()
    {
        entity_id id = nextentity_id++;
        entities.emplace_back(make_unique<Entity>(id));
        return entities.back().get();
    }
    
    void destroyEntity(Entity* entity)
    {
        auto it = find_if(entities.begin(), entities.end(), [entity](const unique_ptr<Entity>& e) { return e.get() == entity; });
        
        if (it != entities.end())
        {
            for (auto& system : systems) { system->removeEntity(entity); }
            
            for (auto& pair : componentEntities)
            {
                auto& vec = pair.second;
                vec.erase(remove(vec.begin(), vec.end(), entity), vec.end());
            }
            
            entities.erase(it);
        }
    }
    
    template<typename T> void registerComponent()
    {
        ComponentTypeID typeID = getComponentTypeID<T>();
        componentEntities[typeID] = vector<Entity*>();
    }
    
    template<typename T> void addComponentToEntity(Entity* entity, T* component)
    {
        entity->addComponent<T>(*component);
        ComponentTypeID typeID = getComponentTypeID<T>();
        componentEntities[typeID].push_back(entity);
    }
    
    template<typename T> vector<Entity*> getEntitiesWithComponent()
    {
        ComponentTypeID typeID = getComponentTypeID<T>();
        return componentEntities[typeID];
    }
    
    template<typename T, typename... TArgs> System* registerSystem(TArgs&&... args)
    {
        systems.emplace_back(make_unique<T>(forward<TArgs>(args)...));
        return systems.back().get();
    }
    
    void update(float deltaTime) { for (auto& system : systems) { system->update(deltaTime); }}
};

struct Vec2     { float x, y; };
struct Colour   { float r, g, b, a; };
struct Position : Component { Vec2 pos; };
struct Velocity : Component { Vec2 vel; };
struct Sprite   : Component { string name; };

class MovementSystem
{
    vector<Entity*> entities;

public :

    void add_entity(Entity* e) { entities.push_back(e); }
    
    void update(float dt)
    {
        for (auto* e : entities)
        {
            auto& pos = e->getComponent<Position>();
            auto& vel = e->getComponent<Velocity>();
            pos.pos.x += vel.vel.x * dt;
            pos.pos.y += vel.vel.y * dt;
            cout << "Moved to: (" << pos.pos.x << ", " << pos.pos.y << ")\n";
        }
    }
};

int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    luaL_dostring(L, "local demo = 6 + 9 + 6 * 9 ; io.write(demo, '\\n')");

    char literal[] = "a = 4 ; b = 3.0 ; c = a * b";
    luaL_dostring(L, literal);
    lua_getglobal(L, "a");
    int var_L = (int)lua_tonumber(L, -1);
    lua_getglobal(L, "b");
    double var_R = (double)lua_tonumber(L, -1);
    lua_getglobal(L, "c");
    int vars = lua_tonumber(L, -1);
    printf("%d * %lf = %d\n", var_L, var_R, vars);

    lua_close(L);

    return 0;
}