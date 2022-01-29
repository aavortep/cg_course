#include <vector>
#include "model.h"
#include "point.h"

using namespace std;

Body::Body(QString filename, QColor color, Point center) : center(center), color(color)
{
    ifstream in;  // файл

    in.open(filename, ifstream::in);
    if (in.fail())
        return;

    string line;  // строка файла

    while (!in.eof())
    {
        getline(in, line);  // считывание строки файла

        istringstream iss(line.c_str());
        char trash;  // для приставок строк

        if (!line.compare(0, 2, "v "))  // если текущая строка файла - это описание вершины
        {
            iss >> trash;  // обрезка строки до нужной информации (удаление приставки "v")
            Point v;  // вершина
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            points.push_back(v);
        }

        else if (!line.compare(0, 3, "vn "))  // если текущая строка файла - это описание нормали
        {
            iss >> trash >> trash;
            Point n;  // нормаль
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            norms.push_back(n);
        }

        else if (!line.compare(0, 2, "f "))  // если текущая строка файла - это описание поверхности
        {
            vector<Point> f;  // поверхность
            Point tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                    tmp[i]--;
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }
}

Body::~Body()
{
    this->points.clear();
    this->norms.clear();
    this->faces.clear();
}

void Body::set_center(const Point &center)
{
    this->center = center;
}

Point& Body::get_center()
{
    return this->center;
}

int Body::get_verts_cnt()
{
    return points.size();
}

Point& Body::vert(const int& idx)
{
    return points[idx];
}

int Body::get_faces_cnt()
{
    return this->faces.size();
}

vector<int> Body::face(const int &idx)
{
    vector<int> face;
    size_t size = faces[idx].size();

    for (size_t i = 0; i < size; i++)
        face.push_back(faces[idx][i][0]);

    return face;
}

int Body::get_norms_cnt()
{
    return norms.size();
}

void Body::set_norm(const int &iface, const int &nvert, const Point &n)
{
    int idx = faces[iface][nvert][2];
    norms[idx] = n;
}

Point& Body::norm(const int &iface, const int &nvert)
{
    int idx = faces[iface][nvert][2];
    return norms[idx].normalize();
}

Point Body::norm_calc(const Point& a, const Point& b, const Point& c)
{
    Point n = (c - a) ^ (b - a);
    return n;
}

void Body::norms_processing()
{
    size_t nface = faces.size();

    for (size_t i = 0; i < nface; i++)
    {
        std::vector<int> f = face(i);

        set_norm(i, 0, norm_calc(vert(f[0]), vert(f[1]), vert(f[2])));
        set_norm(i, 1, norm_calc(vert(f[1]), vert(f[2]), vert(f[0])));
        set_norm(i, 2, norm_calc(vert(f[2]), vert(f[0]), vert(f[1])));
    }
}

QColor& Body::get_color()
{
    return this->color;
}

void Body::set_color(const QColor& color)
{
    this->color = color;
}

void Body::scale(const double kx, const double ky, const double kz)
{
    for (size_t i = 0; i < this->points.size(); ++i)
        this->points[i].scale(kx, ky, kz);
}

Body& Body::operator = (const Body &body)
{
    this->points = body.points;
    this->norms = body.norms;
    this->faces = body.faces;
    this->center = body.center;
    this->color = body.color;
    return *this;
}

Pendulum::Pendulum(QString filename, QColor color, Point center) : center(center), color(color)
{
    ifstream in;  // файл

    in.open(filename, ifstream::in);
    if (in.fail())
        return;

    string line;  // строка файла

    while (!in.eof())
    {
        getline(in, line);  // считывание строки файла

        istringstream iss(line.c_str());
        char trash;  // для приставок строк

        if (!line.compare(0, 2, "v "))  // если текущая строка файла - это описание вершины
        {
            iss >> trash;  // обрезка строки до нужной информации (удаление приставки "v")
            Point v;  // вершина
            for (int i = 0; i < 3; i++)
                iss >> v[i];
            points.push_back(v);
        }

        else if (!line.compare(0, 3, "vn "))  // если текущая строка файла - это описание нормали
        {
            iss >> trash >> trash;
            Point n;  // нормаль
            for (int i = 0; i < 3; i++)
                iss >> n[i];
            norms.push_back(n);
        }

        else if (!line.compare(0, 2, "f "))  // если текущая строка файла - это описание поверхности
        {
            vector<Point> f;  // поверхность
            Point tmp;
            iss >> trash;
            while (iss >> tmp[0] >> trash >> tmp[1] >> trash >> tmp[2])
            {
                for (int i = 0; i < 3; i++)
                    tmp[i]--;
                f.push_back(tmp);
            }
            faces.push_back(f);
        }
    }

    QObject::connect(this, SIGNAL(run(const int tempo)), this, SLOT(running(const int tempo)));
    QObject::connect(this, SIGNAL(stop()), this, SLOT(staying()));
}

Pendulum::~Pendulum()
{
    this->points.clear();
    this->norms.clear();
    this->faces.clear();
}

void Pendulum::set_center(const Point &center)
{
    this->center = center;
}

Point& Pendulum::get_center()
{
    return this->center;
}

int Pendulum::get_verts_cnt()
{
    return points.size();
}

Point& Pendulum::vert(const int& idx)
{
    return points[idx];
}

int Pendulum::get_faces_cnt()
{
    return this->faces.size();
}

vector<int> Pendulum::face(const int &idx)
{
    vector<int> face;
    size_t size = faces[idx].size();

    for (size_t i = 0; i < size; i++)
        face.push_back(faces[idx][i][0]);

    return face;
}

int Pendulum::get_norms_cnt()
{
    return norms.size();
}

void Pendulum::set_norm(const int &iface, const int &nvert, const Point &n)
{
    int idx = faces[iface][nvert][2];
    norms[idx] = n;
}

Point& Pendulum::norm(const int &iface, const int &nvert)
{
    int idx = faces[iface][nvert][2];
    return norms[idx].normalize();
}

Point Pendulum::norm_calc(const Point& a, const Point& b, const Point& c)
{
    Point n = (c - a) ^ (b - a);
    return n;
}

void Pendulum::norms_processing()
{
    size_t nface = faces.size();

    for (size_t i = 0; i < nface; i++)
    {
        std::vector<int> f = face(i);

        set_norm(i, 0, norm_calc(vert(f[0]), vert(f[1]), vert(f[2])));
        set_norm(i, 1, norm_calc(vert(f[1]), vert(f[2]), vert(f[0])));
        set_norm(i, 2, norm_calc(vert(f[2]), vert(f[0]), vert(f[1])));
    }
}

QColor& Pendulum::get_color()
{
    return this->color;
}

void Pendulum::set_color(const QColor& color)
{
    this->color = color;
}

Pendulum& Pendulum::operator = (const Pendulum &pend)
{
    this->points = pend.points;
    this->norms = pend.norms;
    this->faces = pend.faces;
    this->center = pend.center;
    this->color = pend.color;
    return *this;
}

bool Pendulum::is_running()
{
    if (this->state == STAY)
        return 0;
    else
        return 1;
}

void Pendulum::running(const int tempo)
{
    // колебания

    if (this->state == STAY)
        this->state = RUN;
    emit run(tempo);
}

void Pendulum::staying()
{
    // остановка метронома

    if (this->state == RUN)
        this->state = STAY;
}

void Pendulum::scale(const double kx, const double ky, const double kz)
{
    for (size_t i = 0; i < this->points.size(); ++i)
        this->points[i].scale(kx, ky, kz);
}
