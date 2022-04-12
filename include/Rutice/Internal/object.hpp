#ifndef _H_OBJECT_I_HPP_
#define _H_OBJECT_I_HPP_

#include "basics.hpp"
#include "types.hpp"

touchPosition touch;

enum ENTITY_STATE
{
    AWAKE,
    DEAD,
    SLEEP
};

class Component;
using ComponentID = std::size_t;
constexpr std::size_t maxComponents = 64;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component *, maxComponents>;

inline ComponentID getComponentTypeID()
{
    static ComponentID lastID = 0;
    return lastID++;
}

template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
    static ComponentID typeID = getComponentTypeID();
    return typeID;
}

class GameObject
{

public:
    GameObject(string texture, C3D_RenderTarget *rendererObject, fvect3 pos, string name, int index);
    ~GameObject();

    /// GameObject - Start() Event.
    /// Called when the GameObject first loads in the scene.
    void Event_Start();

    /// GameObject - Update() Event.
    /// Called during a scene Tick() - Every frame.
    void Event_Update();

    void Event_Timers();

    /// GameObject - Destroy() Event.
    /// Called when the GameObject is being removed from the scene.
    void Event_Destroy();

    void Event_Touch();

    void Event_Draw();

    void Handle_Anim();

    ENTITY_STATE GetState();

    /// Reload the spritesheet from texturePath
    void reloadTextures();

    /// GameObject - Kill() Function.
    /// Use to mark the GameObject for deletion on the next tick - will call Event_Destroy() before.
    void Kill();

    /// Check if the GameObject has this tag.
    bool hasTag(string tag);

    // Add a tag to the GameObject if it does not already have one.
    bool addTag(string tag);

    bool removeTag(string tag);

    // Variables used internally
    C2D_Sprite activeTexture;
    std::vector<string> tags;
    string name;

    /// The position of the GameObject, in three-dimensional space.
    fvect3 position;

    /// The rotation of the GameObject, in one-dimensional space along the z-axis.
    float rotation;

    /// the scale of the GameObject, in two-dimensional space.
    fvect2 scale;

    Anim animation;

    C3D_RenderTarget *target;

    ivect2 touchBoundary[2];

    bool visible = true;
    bool touchable = true;
    C2D_SpriteSheet textures;
    string texturePath;
    int textureIndex;

private:
    int prevIndex;
    // Variables used internally
    std::vector<std::unique_ptr<Component>> components;

    ComponentArray componentArray;
    ComponentBitSet componentBitSet;

    ENTITY_STATE state = AWAKE;

public:
    // ECS-Related

    template <typename T>
    bool hasComponent() const
    {
        return (bool)ComponentBitSet(getComponentTypeID<T>);
    }

    template <typename T, typename... TArgs>
    T &addComponent(TArgs &&...mArgs)
    {
        T *c(new T(std::forward<TArgs>(mArgs)...));
        c->object = this;
        std::unique_ptr<Component> uPtr{c};
        components.emplace_back(std::move(uPtr));

        componentArray[getComponentTypeID<T>()] = c;
        ComponentBitSet(getComponentTypeID<T>()) = true;

        c->init();
        return *c;
    }

    template <typename T>
    T &getComponent() const
    {
        auto ptr(componentArray[getComponentTypeID<T>()]);
        return *static_cast<T *>(ptr);
    }
};

class Component
{
public:
    int timer[16];
    GameObject *object;

    virtual void init() {}

    virtual void updateBegin() {}
    virtual void update() {}
    virtual void updateEnd() {}

    virtual void draw() {}
    virtual void timers(int timerNumber) {}
    virtual void onDelete() {}
    virtual void onTouch() {}

    virtual ~Component() {}
};

#endif