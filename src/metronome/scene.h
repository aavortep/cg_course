#ifndef SCENE_H
#define SCENE_H

#include "./SceneObjects/model.h"
#include "./SceneObjects/sprite.h"
#include "./SceneObjects/camera.h"
#include "./SceneObjects/lightsourcepoint.h"

class Scene
{
private:
    Camera mainCamera;
    std::vector<Model> models;
    std::vector<Model> inits;
    std::vector<Sprite> sprites;
    std::vector<LightSourcePoint> lightSources;

public:
    Scene();

    // Models
    int      countModels();
    void     addModel(const Model&, const Vector3f& scale = Vector3f(1, 1, 1));
    Model&   getModel(const int&);
    void     setModel(const int idx, const Model& new_model);
    void     editModel(const int&, Vector3f&, Vector3f&, Vector3f&);
    void     rotPend(const int&, Vector3f&);
    void     runModel(const int tempo);
    void     stopModel();
    float    getLen(const int idx);
    Vector3f computeFace();
    Vector3f getBase(const int idx);
    void     setBase(const int idx, const Vector3f& new_base);
    Model&   getInitState(const int idx);
    void     setInitState(const int idx, const Model& init_state);
    float    computeTan();
    int      getVertsCnt();


    // Sprites
    int     countSprites();
    void    addSprite(const Sprite&, const Vector3f&, const Vector3f&, const float&);
    Sprite& getSprite(const int&);
    void    editSprite(const int&, Vector3f&, Vector3f&, Vector3f&, Vector3f&, const float&);
    void    editSprite(const int&, Vector3f&, Vector3f&, Vector3f&);
    void    updateSpriteCenter(const int&);

    // Lights
    void              addLightSource(const Vector3f&, const float&);
    LightSourcePoint& getLightSource(const int&);
    int               getLightSourceCount();
    void              editLight(const int&, const Vector3f&, const float&);

    // Camera
    Camera&   getCamera();
    void      setCamera(const Vector3f&, const Vector3f&, const Vector3f&);

    Vector3f& getCameraPos();
    Vector3f& getCameraView();
    Vector3f& getCameraUp();

    void      upDownCamera(const float&);
    void      rotateCamera(const float&);
    void      strafeCamera(const float&);
    void      movingCamera(const float&);
};

#endif // SCENE_H
