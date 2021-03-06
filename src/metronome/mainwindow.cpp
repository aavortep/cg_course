#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <ctime>

int frames = 0;
auto frameTime = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Metronome");
    this->setWindowIcon(QIcon("icon2.png"));

    ui->centralwidget->setStyleSheet("QWidget {background: rgba(50, 50, 50, 255);}");

    initDrawer();
    initLables();
    initButton();

    // Combobox
    connect(ui->comboBox_model, SIGNAL(currentIndexChanged(QString)), SLOT(changeModel()));
    connect(ui->comboBox_light, SIGNAL(currentIndexChanged(QString)), SLOT(changeLight()));

    modelCnt  = 2;
    spriteCnt = 0;
    lightCnt  = 0;

    lightPos.push_back(Vector3f(100, 1000, 100));
    drawer->addLight(Vector3f(100, 1000, 100), 1300);
    ui->comboBox_light->addItem("Main light");

    Vector3f center(0, 0, 0);
    Vector3f scaleK(1, -1, 1);
    drawer->addModel(center, scaleK, QString("../metronome/SourceObjects/model1.obj"), QColor(255, 255, 255));
    drawer->addModel(center, scaleK, QString("../metronome/SourceObjects/pend1.obj"), QColor(253, 124, 110));
    centersM.push_back(center);
    pend_base = computeBase();
    ui->comboBox_model->addItem("metronome");
    //unsigned int start_time, end_time;
    //int n = 10;
    //start_time = clock();
    //for (int i = 0; i < n; ++i)
        drawer->draw();
    //end_time = clock();
    //auto elapsed_time = (end_time - start_time) / n;
    //qDebug() << drawer->getVertsCnt() << elapsed_time;
}

MainWindow::~MainWindow()
{
    if (addModelWindow->isVisible())
        addModelWindow->destroyed();

    if (addLightWindow->isVisible())
        addLightWindow->destroyed();

    delete ui;
}



void MainWindow::initDrawer()
{
    size_t w = ui->graphicsView->width();
    size_t h = ui->graphicsView->height();

    drawer = new Drawer(w, h, this);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setAlignment(Qt::AlignCenter);

    ui->graphicsView->setScene(drawer);
}

void MainWindow::initButton()
{
    //connect(ui->pushButton_addModel, SIGNAL(released()), this, SLOT(openAddModelWindow()));
    connect(ui->pushButton_addLight, SIGNAL(released()), this, SLOT(openAddLightWindow()));

    connect(ui->pushButton_mapply, SIGNAL(released()), this, SLOT(applyModelChange()));
    connect(ui->pushButton_mcancel, SIGNAL(released()), this, SLOT(cancelLineEditsModel()));

    connect(ui->pushButton_run, SIGNAL(released()), this, SLOT(runModel()));
    connect(ui->pushButton_stop, SIGNAL(released()), this, SLOT(stopModel()));

    connect(ui->pushButton_lapply, SIGNAL(released()), this, SLOT(applyLightChange()));
    connect(ui->pushButton_lcancel, SIGNAL(released()), this, SLOT(cancelLineEditsLight()));

    connect(ui->pushButton_init, SIGNAL(released()), this, SLOT(modelToInit()));
}

void MainWindow::initLables()
{
    // Models
    ui->lbl_models->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_model_move->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mmove_x->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mmove_y->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mmove_z->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");

    ui->lbl_model_scale->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mscale_x->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mscale_y->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mscale_z->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");

    ui->lbl_mrotate->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mrotate_x->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mrotate_y->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_mrotate_z->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");


    // Motion
    ui->lbl_metr->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_metr_2->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_model_tempo->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_model_bpm->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");


    // Lights
    ui->lbl_lights->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_lmove->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_lmove_x->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_lmove_y->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
    ui->lbl_lmove_z->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");

    ui->lbl_power->setStyleSheet("QLabel {color: rgba(200, 200, 200, 255);}");
}



// Model
void MainWindow::changeModel()
{
    ui->le_mmove_x->setText("0");
    ui->le_mmove_y->setText("0");
    ui->le_mmove_z->setText("0");

    ui->le_mscale_x->setText("1");
    ui->le_mscale_y->setText("1");
    ui->le_mscale_z->setText("1");

    ui->le_mrotate_x->setText("0");
    ui->le_mrotate_y->setText("0");
    ui->le_mrotate_z->setText("0");
}

void MainWindow::applyModelChange()
{
    if (centersM.size() == 0)
        return;

    if (running)
        return;

    Vector3f center, scale, rotate;

    for (int idx = 0; idx < modelCnt; ++idx)
    {
        // For move
        if (ui->le_mmove_x->text().isEmpty() ||
            ui->le_mmove_y->text().isEmpty() ||
            ui->le_mmove_z->text().isEmpty())
        {
            center = Vector3f(centersM[0]);
        }
        else
        {
            center = Vector3f(ui->le_mmove_x->text().toFloat(),
                              ui->le_mmove_y->text().toFloat(),
                              ui->le_mmove_z->text().toFloat());
        }

        // For scale
        if (ui->le_mscale_x->text().isEmpty() ||
            ui->le_mscale_y->text().isEmpty() ||
            ui->le_mscale_z->text().isEmpty())
        {
            scale = Vector3f(1, 1, 1);
        }
        else
        {
            scale = Vector3f(ui->le_mscale_x->text().toFloat(),
                             ui->le_mscale_y->text().toFloat(),
                             ui->le_mscale_z->text().toFloat());
        }

        // For rotate
        if (ui->le_mrotate_x->text().isEmpty() ||
            ui->le_mrotate_y->text().isEmpty() ||
            ui->le_mrotate_z->text().isEmpty())
        {
            rotate = Vector3f(0, 0, 0);
        }
        else
        {
            rotate = Vector3f(ui->le_mrotate_x->text().toFloat(),
                              ui->le_mrotate_y->text().toFloat(),
                              ui->le_mrotate_z->text().toFloat());
        }

        if (rotate != Vector3f(0, 0, 0))
            initState = false;

        drawer->editModel(idx, center, scale, rotate);
        drawer->draw();
    }
}

void MainWindow::cancelLineEditsModel()
{
    ui->le_lmove_x->setText("0");
    ui->le_lmove_y->setText("0");
    ui->le_lmove_z->setText("0");

    ui->le_mscale_x->setText("1");
    ui->le_mscale_y->setText("1");
    ui->le_mscale_z->setText("1");

    ui->le_mrotate_x->setText("0");
    ui->le_mrotate_y->setText("0");
    ui->le_mrotate_z->setText("0");
}

Vector3f MainWindow::computeBase()
{
    int idx = 0;
    Vector3f base = drawer->getBase(idx), center = Vector3f(0, 0, 0);
    size_t w = ui->graphicsView->width();
    size_t h = ui->graphicsView->height();
    Scene scene = drawer->getScene();
    Vector3f camPos = scene.getCameraPos();
    Vector3f camDir = scene.getCameraView();
    Vector3f camUp  = scene.getCameraUp();

    Matrix viewPort   = Camera::viewport(w/8, h/8, w*3/4, h*3/4);
    Matrix projection = Matrix::identity(4);
    Matrix modelView  = Camera::lookAt(camPos, camDir, camUp);
    projection[3][2]  = - 1.f / (camPos - camDir).norm();
    Matrix mvp = viewPort * projection * modelView;
    Vector3f v = center + base;
    base = Vector3f(mvp * Matrix(v));
    return base;
}

Model& MainWindow::getInitState(const int idx)
{
    return drawer->getInitState(idx);
}

void MainWindow::setInitState(const int idx, const Model &init_state)
{
    drawer->setInitState(idx, init_state);
}

void MainWindow::toInitState()
{
    drawer->toInitState();
}

void MainWindow::updateAnimation()
{
    int idx = modelCnt - 1;
    Vector3f center, scale, rotate;
    float coef = tempo / 20;

    center = Vector3f(0, 0, 0);
    scale = Vector3f(1, 1, 1);

    float tg = drawer->computeTan();
    float ampl = 70.0 * M_PI / 180.0;
    if (tg <= tan(ampl) && tg >= 0)
    {
        left = true;
        right = false;
    }
    else if (tg >= -tan(ampl) && tg <= 0)
    {
        right = true;
        left = false;
    }

    if (right)
        rotate = Vector3f(0, 0, 2 * coef);
    if (left)
        rotate = Vector3f(0, 0, -2 * coef);

    drawer->rotPend(idx, rotate);
    drawer->draw();
}

void MainWindow::runModel()
{
    if (!initState || running)
        return;

    tempo = ui->spinBox_bpm->value();
    running = true;

    _timer = new QTimer();
    _timer->setInterval(1);
    connect(_timer, SIGNAL(timeout()), this, SLOT(updateAnimation()));
    _timer->start();
}

void MainWindow::stopModel()
{
    if (!running)
        return;

    _timer->stop();
    delete _timer;

    running = false;
    toInitState();
}

void MainWindow::modelToInit()
{
    drawer->toInitState();
    initState = true;
}



// Light
void MainWindow::changeLight()
{
    int idx = ui->comboBox_light->currentIndex();
    Vector3f currentCenter = lightPos[idx];

    ui->le_lmove_x->setText(std::to_string(currentCenter.x).c_str());
    ui->le_lmove_y->setText(std::to_string(currentCenter.y).c_str());
    ui->le_lmove_z->setText(std::to_string(currentCenter.z).c_str());

    ui->le_power->setText("500");
}

void MainWindow::applyLightChange()
{
    if (lightPos.size() == 0)
        return;

    int idx = ui->comboBox_light->currentIndex();
    Vector3f pos;

    if (ui->le_lmove_x->text().isEmpty() ||
        ui->le_lmove_y->text().isEmpty() ||
        ui->le_lmove_z->text().isEmpty())
    {
        pos = lightPos[idx];
    }
    else
    {
        pos = Vector3f(ui->le_lmove_x->text().toFloat(),
                       ui->le_lmove_y->text().toFloat(),
                       ui->le_lmove_z->text().toFloat());
    }

    if (ui->le_power->text().isEmpty())
        drawer->editLight(idx, pos);
    else
        drawer->editLight(idx, pos, ui->le_power->text().toFloat());

    drawer->draw();
}

void MainWindow::cancelLineEditsLight()
{
    int idx = ui->comboBox_light->currentIndex();
    Vector3f currentCenter = lightPos[idx];

    ui->le_lmove_x->setText(std::to_string(currentCenter.x).c_str());
    ui->le_lmove_y->setText(std::to_string(currentCenter.y).c_str());
    ui->le_lmove_z->setText(std::to_string(currentCenter.z).c_str());

    ui->le_power->setText("200");
}



void MainWindow::constructorMode(bool value)
{
    if (value)
    {
        this->resize(990, 800);
    }
    else
    {
        this->resize(800, 800);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();

    switch (key)
    {
    case Qt::Key_W:
        drawer->movingCamera(1);
        break;
    case Qt::Key_S:
        drawer->movingCamera(-1);
        break;
    case Qt::Key_A:
        drawer->strafeCamera(-0.5);
        break;
    case Qt::Key_D:
        drawer->strafeCamera(0.5);
        break;

    case Qt::Key_I:
        drawer->upDownCamera(0.25);
        break;
    case Qt::Key_J:
        drawer->rotateCamera(-0.05);
        break;
    case Qt::Key_K:
        drawer->upDownCamera(-0.25);
        break;
    case Qt::Key_L:
        drawer->rotateCamera(0.05);
        break;
    }

    drawer->draw();
}



// Add new model
void MainWindow::openAddModelWindow()
{
    addModelWindow = new AddModelWindow(modelCnt);

    connect(addModelWindow, SIGNAL(saveModelParams(AddModelParameters&)),
            this, SLOT(setAddModelParams(AddModelParameters&)));

    addModelWindow->show();
}

void MainWindow::setAddModelParams(AddModelParameters& newParams)
{
    Vector3f center(newParams.moveX, newParams.moveY, newParams.moveZ);
    Vector3f scaleK(newParams.scaleX, newParams.scaleY, newParams.scaleZ);

    if (newParams.isSprite)
    {
        Vector3f end(newParams.enX, newParams.enY, newParams.enZ);
        drawer->addSprite(center, scaleK, newParams.filename, newParams.color, end, newParams.speed);

        centersS.push_back(center);

        spriteCnt++;
    }
    else
    {
        drawer->addModel(center, scaleK, newParams.filename, newParams.color);

        centersM.push_back(center);
        ui->comboBox_model->addItem(newParams.modelName);

        modelCnt++;
    }

    frames = 0;
    frameTime = 0;
    drawer->draw();
}


// Add new light
void MainWindow::openAddLightWindow()
{
    addLightWindow = new AddLightWindow(lightCnt);

    connect(addLightWindow, SIGNAL(saveLightParams(AddLightParameters&)),
            this, SLOT(setLightParams(AddLightParameters&)));

    addLightWindow->show();
}

void MainWindow::setLightParams(AddLightParameters& newParams)
{
    Vector3f pos(newParams.x, newParams.y, newParams.z);
    drawer->addLight(pos, newParams.power);

    lightPos.push_back(pos);
    ui->comboBox_light->addItem(newParams.name);

    drawer->draw();
}
