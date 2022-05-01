#ifndef _H_SCENE_I_HPP_
#define _H_SCENE_I_HPP_

#include "basics.hpp"
#include "types.hpp"

ivect2 Render_cameraPosition = {0, 0};
C3D_RenderTarget *Render_top;
C3D_RenderTarget *Render_bottom;

int updateRate = 0;
double secondsUpdateRate;



class Scene
{
    
public:
    string sceneName;

   // ivect2 cameraPosition = {0, 0};
    Scene();
    ~Scene();

    // Call once per frame ( Before Render ).
    void Tick();

    // Call once per frame.
    void Render(C3D_RenderTarget *target);

    /// Currently unimplemented, do not use.
    std::vector<GameObject &> FindGameObjectsWithTag(string tag);

    /// Currently unimplemented, do not use.
    std::vector<GameObject &> FindGameObjectsWithName(string name);

    /// Currently unimplemented, do not use.
    GameObject &FindGameObjectWithName(string name);

    /// Create a new GameObject in the scene.
    GameObject &Create(string texture, C3D_RenderTarget *rendererObject, fvect3 pos, string name, int index);

    /// Controls how many seconds pass between every Update() call - 0 for every frame.
    int frameCount;

    /// Controls whether or not events in the scene will be run.
    bool paused = false;

    /// Get the value of a scene global if it exists.
    /// Returns value if it exists. Returns 0 otherwise.
    double getGlobal(string name)
    {
        for (auto &v : sceneVariables)
        {
            if (v.name == name)
            {
                return v.value;
            }
        }
        return 0;
    }

    /// Set a scene global. One will be created if it does not exist.
    void setGlobal(string name, double value)
    {
        for (auto &v : sceneVariables)
        {
            if (v.name == name)
            {
                v.value = value;
                return;
            }
        }
        SceneVariable temp;
        temp.name = name;
        temp.value = value;

        sceneVariables.push_back(temp);
    }

    /// Remove global scene variable if it exists.
    /// Returns true if it was able to remove it.
    bool removeGlobal(string name)
    {
        for (int i = 0; i < (int)sceneVariables.size(); i++)
        {
            if (sceneVariables[i].name == name)
            {
                sceneVariables.erase(sceneVariables.begin() + i);
                return true;
            }
        }

        return false;
    }

private:
    typedef struct
    {
        string name;
        double value;
    } SceneVariable;

    std::vector<std::unique_ptr<GameObject>> entities;
    std::vector<SceneVariable> sceneVariables;
};

Scene *activeScene;



#endif