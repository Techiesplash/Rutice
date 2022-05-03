#pragma once

#include <Rutice/Internal/scene.hpp>
//#include <Rutice/Internal/sprites.hpp>
#include <Rutice/Generic>

Scene::Scene()
{
}

Scene::~Scene()
{
    for (auto &o : entities)
    {
        o->Event_Destroy();
    }
}

void Scene::Tick()
{

    secondsUpdateRate = updateRate / 60.0;
    if (updateRate > 1)
    {
        if (frameCount >= updateRate)
        {
            frameCount = 1;
        }
        else
        {
            frameCount++;
        }
    }

    // Before deletion of entities
    for (auto &o : entities)
    {
        // Call events
        o->Event_Destroy();
    }

    entities.erase(std::remove_if(std::begin(entities), std::end(entities),
                                  [](const std::unique_ptr<GameObject> &mObject)
                                  {
                                      return mObject->GetState() == DEAD;
                                  }),
                   std::end(entities));

    if (frameCount >= updateRate || updateRate == 1)
    {
        for (auto &o : entities)
        {

            o->Event_Update();
            o->Event_Timers();
            o->Event_Touch();
        }
    }
}

void Scene::Render(C3D_RenderTarget *target)
{
    for (auto &o : entities)
    {
        o->Event_Draw();
        if (o->target == target && o->visible)
        {
            o->Handle_Anim();
            o->DrawSelf();
        }
    }
}

GameObject &Scene::Create(string texture, C3D_RenderTarget *rendererObject, fvect3 pos, string name, int index)
{

    GameObject *object = new GameObject(texture, rendererObject, pos, name, index);

    std::unique_ptr<GameObject> uPtr{object};
    entities.emplace_back(std::move(uPtr));
    return *object;
}
