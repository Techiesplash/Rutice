#include <Rutice/Generic>
#include <Rutice/Internal/scene.hpp>

GameObject::GameObject(string texture, C3D_RenderTarget *rendererObject, fvect3 pos, string name, int index)
{
    target = rendererObject;
    texturePath = texture;

    
    textures = C2D_SpriteSheetLoad((const char *)texture.c_str());
    
    if (!textures)
    {
        printf("object.cpp [ GameObject::GameObject() ] :\n Error loading texturesheet %s\n", texture.c_str());
    }

    position = pos;

    C2D_SpriteFromSheet(&activeTexture, textures, index);

    name = name;

    textureIndex = index;
}

GameObject::~GameObject()
{
    C2D_SpriteSheetFree(textures);
}

// Events

void GameObject::Event_Start()
{
}

bool GameObject::hasTag(string tag)
{
    for (auto &t : tags)
    {
        if (t == tag)
        {
            return true;
        }
    }
    return false;
}

bool GameObject::addTag(string tag)
{
    for (auto &t : tags)
    {
        if (t == tag)
        {
            return true;
        }
    }
    tags.push_back(tag);
    return false;
}

bool GameObject::removeTag(string tag)
{
    bool hadTag = false;
    for (int i = 0; i < (int)tags.size(); i++)
    {
        if (tags[i] == tag)
        {
            hadTag = true;
            tags.erase((tags.begin() + i));
        }
    }
    return hadTag;
}

void GameObject::Event_Update()
{

    for (auto &c : components)
        c->update();
}

void GameObject::Event_Destroy()
{
    if (state == DEAD)
    {
        for (auto &c : components)
            c->onDelete();
    }
}

void GameObject::Event_Timers()
{
    for (auto &c : components)
    {
        for (u32 i = 0; i < sizeof(c->timer) / sizeof(int); i++)
        {
            // printf( "Proc t %i \n", i );
            if (c->timer[i] >= 0)
            {
                c->timer[i]--;
            }
            if (c->timer[i] == 0)
            {
                c->timers(i);
            }
        }
    }
}

void GameObject::Event_Touch()
{
    if (target == Render_bottom && touchable == true && touch.px != 0 && touch.py != 0)
    {

        if (touchBoundary[0].x + position.x < touch.px && touchBoundary[1].x + position.x > touch.px)
        {

            if (touchBoundary[0].y + position.y < touch.py && touchBoundary[1].y + position.y > touch.py)
            {

                for (auto &c : components)
                {

                    c->onTouch();
                }
            }
        }
    }
}

void GameObject::Event_Draw()
{
    for (auto &c : components)
    {
        c->draw();
    }
}

void GameObject::Handle_Anim()
{
    if (!animation.ids.empty())
    {
        if (animation.frameCount > animation.frameTimer)
        {
            animation.frameCount = 0;
        }
        else if (animation.frameCount == animation.frameTimer)
        {
            animation.index++;
            if (animation.index == animation.ids.size())
            {
                animation.index = 0;
            }
            C2D_SpriteFromSheet(&activeTexture, textures, animation.ids[animation.index]);
            animation.frameCount++;
        }
        else
        {
            animation.frameCount++;
        }
    }
    
    // Move the texture to the correct position
    C2D_SpriteSetPos(&activeTexture, position.x + Render_cameraPosition.x, position.y + Render_cameraPosition.y);
}

ENTITY_STATE GameObject::GetState()
{
    return state;
}

void GameObject::reloadTextures()
{

    C2D_SpriteSheetFree(textures);

    textures = C2D_SpriteSheetLoad((const char *)texturePath.c_str());
    if (!textures)
    {
        printf("object.cpp [ GameObject::reloadTextures() ] :\n Error loading texturesheet %s\n", texturePath.c_str());
    }

    C2D_SpriteFromSheet(&activeTexture, textures, textureIndex);
}

void GameObject::Kill()
{
    state = DEAD;
}