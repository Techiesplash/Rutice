#pragma once
#include <Rutice/Generic>
#include <Rutice/Internal/scene.hpp>
#include <Rutice/Internal/sprites.hpp>

GameObject::GameObject(string texture, C3D_RenderTarget *rendererObject, fvect3 pos, string name, int index)
{
    target = rendererObject;
    // texturePath = texture;

    // textures = C2D_SpriteSheetLoad((const char *)texture.c_str());

    if (!textures)
    {
        printf("object.cpp [ GameObject::GameObject() ] :\n Error loading texturesheet %s\n", texture.c_str());
    }

    position = pos;

    textureAsset.loadSprite(texture);

    name = name;
}

GameObject::~GameObject()
{
}

void GameObject::DrawSelf()
{
    graphics::drawSprite(textureAsset, -1, position.x, position.y);
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
        {
            c->onDelete();
            delete &c;
        }
        delete this;
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
}

ENTITY_STATE GameObject::GetState()
{
    return state;
}

void GameObject::reloadTextures()
{
}

void GameObject::Kill()
{
    state = DEAD;
}