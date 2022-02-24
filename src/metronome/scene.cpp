#include "scene.h"

#include <QDebug>

// Constructors
Scene::Scene()
    : mainCamera(Camera()), models(std::vector<Model>()), lightSources(std::vector<LightSourcePoint>())
{

}



// Models
int Scene::countModels()
{
    return models.size();
}

void Scene::addModel(const Model& newModel, const Vector3f& scale)
{
    models.push_back(newModel);
    models.back().scale(scale);

    inits.push_back(newModel);
    inits.back().scale(scale);
}

Model& Scene::getModel(const int& idx)
{
    return models[idx];
}

void Scene::setModel(const int idx, const Model &new_model)
{
    models[idx] = new_model;
}

void Scene::editModel(const int& idx, Vector3f& center, Vector3f& scale, Vector3f& rotate)
{
    if (!models[models.size() - 1].isRunning())
    {
        models[idx].setCenter(center);
        models[idx].scale(scale);
        models[idx].rotate(rotate);
    }
}

void Scene::rotPend(const int& idx, Vector3f& rotate)
{
    models[idx].rot_pend(rotate);
}

void Scene::runModel(const int tempo)
{
    models[1].run(tempo);
}

void Scene::stopModel()
{
    models[1].stop();
}

float Scene::getLen(const int idx)
{
    return models[idx].getLen();
}

Vector3f Scene::computeFace()
{
    return models[0].computeFace();
}

Vector3f Scene::getBase(const int idx)
{
    return models[idx].getBase();
}

void Scene::setBase(const int idx, const Vector3f &new_base)
{
    models[idx].setBase(new_base);
}

Model& Scene::getInitState(const int idx)
{
    return inits[idx];
}

void Scene::setInitState(const int idx, const Model &init_state)
{
    inits[idx] = init_state;
}

float Scene::computeTan()
{
    return models[1].computeTan();
}

int Scene::getVertsCnt()
{
    return models[0].getVertsCount();
}


// Sprites
int     Scene::countSprites()
{
    return sprites.size();
}

void    Scene::addSprite(const Sprite& sprite, const Vector3f& scale,
                         const Vector3f& end, const float& speed)
{
    sprites.push_back(sprite);
    sprites.back().scale(scale);
    sprites.back().setPath(end, speed);
}

Sprite& Scene::getSprite(const int& idx)
{
    return sprites[idx];
}

void    Scene::editSprite(const int& idx, Vector3f& cntr, Vector3f& scl, Vector3f& rt, Vector3f& end, const float& sp)
{
    sprites[idx].setCenter(cntr);
    sprites[idx].scale(scl);
    sprites[idx].rotate(rt);
    sprites[idx].setPath(end, sp);
}

void    Scene::editSprite(const int& idx, Vector3f& cntr, Vector3f& scl, Vector3f& rt)
{
    sprites[idx].setCenter(cntr);
    sprites[idx].scale(scl);
    sprites[idx].rotate(rt);
}

void    Scene::updateSpriteCenter(const int& idx)
{
    if (!sprites[idx].isReachedEnd())
        sprites[idx].updatePath();
}



// Lights
void Scene::addLightSource(const Vector3f& lPos, const float& lPower)
{
    lightSources.push_back(LightSourcePoint(lPos, lPower));
}

LightSourcePoint& Scene::getLightSource(const int& idx)
{
    return lightSources[idx];
}

int Scene::getLightSourceCount()
{
    return lightSources.size();
}

void Scene::editLight(const int& idx, const Vector3f& pos, const float& power)
{
    lightSources[idx].setPosition(pos);
    lightSources[idx].setIntensity(power);
}



// Camera methods
Camera&   Scene::getCamera()
{
    return mainCamera;
}

void      Scene::setCamera(const Vector3f& pos, const Vector3f& view, const Vector3f& up)
{
    mainCamera = Camera(pos, view, up);
}



Vector3f& Scene::getCameraPos()
{
    return mainCamera.getPosition();
}

Vector3f& Scene::getCameraView()
{
    return mainCamera.getView();
}

Vector3f& Scene::getCameraUp()
{
    return mainCamera.getUp();
}



void    Scene::upDownCamera(const float& speed)
{
    Vector3f pos = mainCamera.getPosition();

    if (fabs(pos.y + speed) < 1.25)
        mainCamera.upDown(speed);
}

void    Scene::rotateCamera(const float& speed)
{
    mainCamera.rotateView(speed);
}

void    Scene::strafeCamera(const float& speed)
{
    mainCamera.update();
    mainCamera.strafe(speed);
}

void    Scene::movingCamera(const float& speed)
{
    mainCamera.move(speed);
}
