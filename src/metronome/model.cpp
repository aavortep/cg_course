#include <vector>
#include "model.h"
#include "point.h"

using namespace std;

Body::Body(Point& center, QString& filename, QColor& color) : center(center), color(color)
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

void Body::scale(const double kx, const double ky, const double kz)
{
    for (size_t i = 0; i < this->points.size(); ++i)
        this->points[i].scale(kx, ky, kz);
}

Pendulum::Pendulum(Point& center, QString& filename, QColor& color) : center(center), color(color)
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
