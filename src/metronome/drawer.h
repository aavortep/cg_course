#ifndef DRAWER_H
#define DRAWER_H

#include <QImage>
#include <QGraphicsScene>

#include <cmath>
#include <string>

#include "zbuffer.h"
#include "scene.h"

#define FAULT 500

class Drawer : public QGraphicsScene
{
    Q_OBJECT

private:
    int w, h;

    // Scene
    Scene scene;

    QColor bgColor = QColor(102, 204, 255);

    // Screen
    QImage canvas;

    void initCanvas();
    void clearCanvas();
    void updateCanvas();

    // Z-Buffer
    ZBuffer zBuffer;

    void initZBuffer();
    void clearZBuffer();

    // Color cache
    std::vector<std::vector<QColor>> colorCache;

    void initColorCache();
    void clearColorCache();

    // Screen
    void updateScreen();
    void clearScreen();


    // Draw processing
    void  objectProcessing(Model&, Vector3f&, Vector3f&, Vector3f&);
    float lightProcessing(const Vector3f&, const Vector3f&);
    void  triangleProcessing(Vector3i&, Vector3i&, Vector3i&,
                            const QColor&, float&, float&, float&);

    int  wPerm, hPerm;
    bool checkIsVisible(const Vector3i&);

public:
    explicit Drawer(const int&, const int&, QObject *parent = nullptr);
    ~Drawer();

    void draw();

    // Model
    void addModel(Vector3f&, Vector3f&, QString, QColor);
    void setModel(const int idx, const Model& new_model);
    void editModel(const int&, Vector3f&, Vector3f&, Vector3f&);
    void rotPend(const int&, Vector3f&);
    std::vector<int> runModel(std::vector<int> cur_pos);
    void stopModel();
    float getLen(const int idx);
    Vector3f computeFace();
    Vector3f getBase(const int idx);
    void setBase(const int idx, const Vector3f& new_base);
    Model& getInitState(const int idx);
    void setInitState(const int idx, const Model& init_state);
    void toInitState();
    float computeTan();
    int getVertsCnt();

    // Sprite
    void addSprite(Vector3f&, Vector3f&, QString&, QColor&, Vector3f&, float&);
    void editSprite(const int&, Vector3f&, Vector3f&, Vector3f&, Vector3f&, const float&);
    void editSprite(const int&, Vector3f&, Vector3f&, Vector3f&);   // Without movement

    // Light
    void addLight(const Vector3f&, const float&);
    void editLight(const int&, const Vector3f&, const float& power = 500);

    // Camera
    void upDownCamera(const float&);
    void rotateCamera(const float&);
    void strafeCamera(const float&);
    void movingCamera(const float&);

    Scene& getScene();
};

inline QRgb iColor(const QRgb& a, const float& i);

#endif // DRAWER_H
