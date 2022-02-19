#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <QKeyEvent>
#include <QTimer>

#include <string>
#include <chrono>

#include "addmodelwindow.h"
#include "addlightwindow.h"

#include "MathObjects/vector3.h"
#include "drawer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Drawer *drawer;
    QTimer *_timer;

    int modelCnt, spriteCnt, lightCnt;

    bool initState = true;
    bool running = false;

    std::vector<Vector3f> centersM;
    std::vector<Vector3f> centersS;
    std::vector<Vector3f> lightPos;
    Vector3f pend_base;

    void initDrawer();
    void initLables();
    void initButton();
    Vector3f computeBase();
    Model& getInitState(const int idx);
    void setInitState(const int idx, const Model& init_state);
    void toInitState();

    void keyPressEvent(QKeyEvent *event);

    AddModelWindow *addModelWindow;
    AddModelParameters addModelparams;

    AddLightWindow *addLightWindow;
    AddLightParameters addLightParams;

private slots:
    void constructorMode(bool);

    // Model
    void changeModel();
    void applyModelChange();
    void cancelLineEditsModel();
    void updateAnimation();
    void runModel();
    void stopModel();
    void modelToInit();

    // Light
    void changeLight();
    void applyLightChange();
    void cancelLineEditsLight();

    // Window
    void openAddModelWindow();
    void setAddModelParams(AddModelParameters&);

    void openAddLightWindow();
    void setLightParams(AddLightParameters&);
};
#endif // MAINWINDOW_H
