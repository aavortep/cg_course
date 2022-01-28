#include "canvas.h"

using namespace std;

inline QRgb iColor(const QRgb& a, const float& i)
{
    QColor c(a);
    int r, g, b;

    c.getRgb(&r, &g, &b);

    r *= i; g *= i; b *= i;

    if (r > 255)
        r = 255;
    else if (r < 0)
        r = 0;

    if (g > 255)
        g = 255;
    else if (g < 0)
        g = 0;

    if (b > 255)
        b = 255;
    else if (b < 0)
        b = 0;

    c.setRgb(r, g, b);

    return c.rgba();
}

Canvas::Canvas(const int& w, const int& h, QObject *parent)
    : QGraphicsScene(parent), width(w), height(h), wPerm(w + FAULT), hPerm(h + FAULT)
{
    init();
    init_color_cache();
    init_zbuffer();

    scene.set_camera(Point(0, 0, 4), Point(0, 0, 0));
    scene.set_light(Point(100, 1000, 100), 1300);

    update();
    update_screen();
}

void Canvas::init()
{
    image = QImage(width + 1, height + 1, QImage::Format_RGB32);
}

void Canvas::clear()
{
    image.fill(bg_color);
}

void Canvas::update()
{
    for (int i = 0; i < width + 1; i++)
        for (int j = 0; j < height + 1; j++)
            image.setPixelColor(i, j, color_cache[i][j]);
}

void Canvas::init_zbuffer()
{
    zbuffer = ZBuffer(width + 1, height + 1);
}

void Canvas::clear_zbuffer()
{
    zbuffer.clear();
}

void Canvas::init_color_cache()
{
    color_cache = std::vector<std::vector<QColor>>(width + 1, std::vector<QColor>(height + 1, bg_color));
}

void Canvas::clear_color_cache()
{
    for (int i = 0; i < w + 1; i++)
        for (int j = 0; j < h + 1; j++)
            color_cache[i][j] = bg_color;
}

void Canvas::update_screen()
{
    this->addPixmap(QPixmap::fromImage(image));
}

void Canvas::clear_screen()
{
    clear();
    clear_color_cache();
    clear_zbuffer();
}

void Canvas::draw_body()
{
    clear_screen();

    Point cam_pos = scene.get_camera_pos();
    Point cam_dir = scene.get_camera_view();

    process_body(scene.get_body(), cam_pos, cam_dir);

    update();
    update_screen();
}

void Canvas::draw_pendulum()
{
    clear_screen();

    Point cam_pos = scene.get_camera_pos();
    Point cam_dir = scene.get_camera_view();

    process_pend(scene.get_pendulum(), cam_pos, cam_dir);

    update();
    update_screen();
}

void Canvas::add_body(Point &center, QString &filename, QColor &color)
{
    scene.set_body(Body(center, filename, color));
}

void Canvas::add_pendulum(Point &center, QString &filename, QColor &color)
{
    scene.set_pendulum(Pendulum(center, filename, color));
}

void Canvas::scale_model(const double kx, const double ky, const double kz)
{
    scene.scale_model(kx, ky, kz);
}

void Canvas::set_light(const Point &pos, const float power)
{
    scene.set_light(pos, power);
}

void Canvas::move_camera(const int dx, const int dy, const int dz)
{
    scene.move_camera(dx, dy, dz);
}

void Canvas::rotate_camera(const double fi_x, const double fi_y, const double fi_z)
{
    scene.rotate_camera(fi_x, fi_y, fi_z);
}

void Canvas::process_body(Body &body, Point &cam_pos, Point &cam_dir)
{
    size_t i, j;
    bool skip;
    float camZInc = fabs(cam_pos.get_z) + 1;
    float camZDec = fabs(cam_pos.get_z) - 1;

    Point center = body.get_center();
    size_t faces = body.get_faces_cnt();
    QColor color = body.get_color();

    Matrix viewPort = Camera::viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    Matrix projection = Matrix::identity(4);
    Matrix modelView = Camera::look_at(cam_pos, cam_dir);

    projection[3][2] = - 1.f / (cam_pos - cam_dir).norm();

    Matrix mvp = viewPort * projection * modelView;

    for (i = 0; i < faces; i++)
    {
        skip = false;
        std::vector<int> face = body.face(i);

        Point screenCoords[3];
        float intensity[3] = { BG_LIGHT, BG_LIGHT, BG_LIGHT };

        for (j = 0; j < 3; j++)
        {
            Point v = center + body.vert(face[j]);

            if (v.get_z > camZDec && v.get_z > camZInc)
            {
                skip = true;
                break;
            }

            screenCoords[j] = Point(mvp * Matrix(v));
            intensity[j] = process_light(v, body.norm(i, j));
        }

        if (skip || !is_visible(screenCoords[0]) ||
                    !is_visible(screenCoords[1]) ||
                    !is_visible(screenCoords[2])) continue;

        process_triangle(screenCoords[0], screenCoords[1], screenCoords[2],
                         color, intensity[0], intensity[1], intensity[2]);
    }
}

void Canvas::process_pend(Pendulum &pend, Point &cam_pos, Point &cam_dir)
{
    size_t i, j;
    bool skip;
    float camZInc = fabs(cam_pos.get_z) + 1;
    float camZDec = fabs(cam_pos.get_z) - 1;

    Point center = pend.get_center();
    size_t faces = pend.get_faces_cnt();
    QColor color = pend.get_color();

    Matrix viewPort = Camera::viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    Matrix projection = Matrix::identity(4);
    Matrix modelView = Camera::look_at(cam_pos, cam_dir);

    projection[3][2] = -1.f / (cam_pos - cam_dir).norm();

    Matrix mvp = viewPort * projection * modelView;

    for (i = 0; i < faces; i++)
    {
        skip = false;
        std::vector<int> face = pend.face(i);

        Point screenCoords[3];
        float intensity[3] = { BG_LIGHT, BG_LIGHT, BG_LIGHT };

        for (j = 0; j < 3; j++)
        {
            Point v = center + pend.vert(face[j]);

            if (v.get_z > camZDec && v.get_z > camZInc)
            {
                skip = true;
                break;
            }

            screenCoords[j] = Point(mvp * Matrix(v));
            intensity[j] = process_light(v, pend.norm(i, j));
        }

        if (skip || !is_visible(screenCoords[0]) ||
                    !is_visible(screenCoords[1]) ||
                    !is_visible(screenCoords[2])) continue;

        process_triangle(screenCoords[0], screenCoords[1], screenCoords[2],
                         color, intensity[0], intensity[1], intensity[2]);
    }
}

float Canvas::process_light(const Point &vert, const Point &norm)
{
    float intensity = 0;

    Light lsp = scene.get_light();

    Point lightDir = vert - lsp.get_pos();

    intensity += lightDir * norm / pow(lightDir.norm(), 2.0);
    intensity *= lsp.get_intensity() * LIGHT_REFLECT;

    intensity = fmax(0.0, intensity);
    intensity = fmin(1.0, intensity);

    intensity = BG_LIGHT + intensity * (1 - BG_LIGHT);

    if (intensity == 0)
        intensity = BG_LIGHT;

    return intensity;
}

void Canvas::process_triangle(Point &v1, Point &v2, Point &v3, const QColor &color, float &i1, float &i2, float &i3)
{
    if (v1.get_y == v2.get_y && v1.get_y == v3.get_y)
            return;

    if (v1.get_y > v2.get_y)
    {
        std::swap(v1, v2);
        std::swap(i1, i2);
    }
    if (v1.get_y > v3.get_y)
    {
        std::swap(v1, v3);
        std::swap(i1, i3);
    }
    if (v2.get_y > v3.get_y)
    {
        std::swap(v2, v3);
        std::swap(i2, i3);
    }

    int total_height = v3.get_y - v1.get_y;

    for (int i = 0; i < total_height; i++)
    {
        bool second_half = i > v2.get_y - v1.get_y || v2.get_y == v1.get_y;
        int segment_height = second_half ? v3.get_y - v2.get_y : v2.get_y - v1.get_y;

        float alpha = (float)i / total_height;
        float betta = (float)(i - (second_half ? v2.get_y - v1.get_y : 0)) / segment_height;

        Point A = v1 + Point(v3 - v1) * alpha;
        Point B = second_half ? v2 + Point(v3 - v2) * betta : v1 + Point(v2 - v1) * betta;

        float iA = i1 + (i3 - i1) * alpha;
        float iB = second_half ? i2 + (i3 - i2) * betta : i1 + (i2 - i1) * betta;

        if (A.get_x > B.get_x)
        {
            std::swap(A, B);
            std::swap(iA, iB);
        }

        A.get_x = std::max(A.get_x, 0);
        B.get_x = std::min(B.get_x, w);

        for (int j = A.get_x; j <= B.get_x; j++)
        {
            float phi = B.get_x == A.get_x ? 1. : (float)(j - A.get_x) / (float)(B.get_x - A.get_x);

            Point P = Point(A) + Point(B - A) * phi;
            float iP = iA + (iB - iA) * phi;

            if (P.get_x >= w || P.get_y >= h || P.get_x < 0 || P.get_y < 0) continue;

            if (zbuffer.get_depth(P.get_x, P.get_y) < P.get_z)
            {
                zbuffer.set_depth(P.get_x, P.get_y, P.get_z);
                color_cache[P.get_x][P.get_y] = QColor(iColor(color.rgba(), iP));
            }
        }
    }
}

bool Canvas::is_visible(const Point &pt)
{
    if (pt.get_x < -FAULT || pt.get_x > wPerm || pt.get_y < -FAULT || pt.get_y > hPerm)
        return false;
    return true;
}


// SCENE

Scene::Scene()
    : cam(Camera()), body(Body()), pend(Pendulum()), light(Light())
{

}

Body& Scene::get_body()
{
    return body;
}

Pendulum& Scene::get_pendulum()
{
    return pend;
}

void Scene::set_body(const Point center)
{
    body.set_center(center);
}

void Scene::set_pendulum(const Point center)
{
    pend.set_center(center);
}

void Scene::scale_model(const double kx, const double ky, const double kz)
{
    body.scale(kx, ky, kz);
    pend.scale(kx, ky, kz);
}

Light& Scene::get_light()
{
    return light;
}

void Scene::set_light(const Point& pos, const float power)
{
    light.set_pos(pos);
    light.set_intensity(power);
}

Camera& Scene::get_camera()
{
    return cam;
}

void Scene::set_camera(const Point& pos, const Point& view)
{
    cam = Camera(pos, view);
}

Point& Scene::get_camera_pos()
{
    return cam.get_pos();
}

Point& Scene::get_camera_view()
{
    return cam.get_view();
}

void Scene::rotate_camera(const double fi_x, const double fi_y, const double fi_z)
{
    cam.rot(fi_x, fi_y, fi_z);
}

void Scene::move_camera(const int dx, const int dy, const int dz)
{
    cam.move(dx, dy, dz);
}


// ZBUFFER

ZBuffer::ZBuffer() : w(0), h(0), data(std::vector<std::vector<int>>())
{

}

ZBuffer::ZBuffer(const int& w, const int& h)
    : w(w), h(h), data(std::vector<std::vector<int>>(w, std::vector<int>(h, background)))
{

}

int ZBuffer::width()
{
    return w;
}

int ZBuffer::height()
{
    return h;
}

void ZBuffer::clear()
{
    for (int i = 0; i < w; i++)
        for (int j = 0; j < h; j++)
             data[i][j] = background;
}

void ZBuffer::set_depth(const int& x, const int& y, const int& value)
{
    data[x][y] = value;
}

int  ZBuffer::get_depth(const int& x, const int& y)
{
    return data[x][y];
}
