#include "model.h"
#include <cmath>

Model::Model(const char *filename, const QColor& color, const Vector3f& center)
    : center(center), color(color)
{
    std::ifstream in;

    in.open(filename, std::ifstream::in);

    if (in.fail())
        return;

    std::string line;

    while (!in.eof())
    {
        std::getline(in, line);

        std::istringstream iss(line.c_str());
        char trash;

        if (!line.compare(0, 2, "v "))
        {
            iss >> trash;
            Vector3f v;

            for (int i = 0; i < 3; i++)
                iss >> v[i];

            verts.push_back(v);
        }

        else if (!line.compare(0, 3, "vn "))
        {
            iss >> trash >> trash;
            Vector3f n;

            for (int i = 0; i < 3; i++)
                iss >> n[i];

            norms.push_back(n);
        }

        else if (!line.compare(0, 2, "f "))
        {
            std::vector<Vector3i> f;
            Vector3i tmp;

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

    base = Vector3f(0, 0.805989, 0.362673);
    length = 1.027258;
}



// Center
Vector3f& Model::getCenter()
{
    return center;
}

void Model::setCenter(const Vector3f& newCenter)
{
    center = newCenter;
}



// Vertes
int Model::getVertsCount()
{
    return verts.size();
}

Vector3f& Model::vert(const int& idx)
{
    return verts[idx];
}

void Model::setVert(const int &idx, const Vector3f &v)
{
    this->verts[idx] = v;
}



// Faces
int Model::getFacesCount()
{
    return faces.size();
}

std::vector<int> Model::face(const int& idx)
{
    std::vector<int> face;
    size_t size = faces[idx].size();

    for (size_t i = 0; i < size; i++)
        face.push_back(faces[idx][i][0]);

    return face;
}

Vector3f Model::computeFace()
{
    Vector3f p1 = verts[5], p2 = verts[6], p3 = verts[8], res;
    float a, b, c;
    a = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
    b = (p3.x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (p3.z - p1.z);
    c = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);
    res = Vector3f(a, b, c);
    return res;
}



// Normals
int Model::getNormsCount()
{
    return norms.size();
}

void Model::setNorm(const int& iface, const int& nvert, const Vector3f& n)
{
    int idx = faces[iface][nvert][2];
    norms[idx] = n;
}

Vector3f& Model::norm(const int& iface, const int& nvert)
{
    int idx = faces[iface][nvert][2];
    return norms[idx].normalize();
}

Vector3f Model::normalCalculate(const Vector3f& a, const Vector3f& b, const Vector3f& c)
{
    Vector3f n = (c - a) ^ (b - a);
    return n;
}

void Model::normalsProcessing()
{
    size_t nface = faces.size();

    for (size_t i = 0; i < nface; i++)
    {
        std::vector<int> f = face(i);

        setNorm(i, 0, normalCalculate(vert(f[0]), vert(f[1]), vert(f[2])));
        setNorm(i, 1, normalCalculate(vert(f[1]), vert(f[2]), vert(f[0])));
        setNorm(i, 2, normalCalculate(vert(f[2]), vert(f[0]), vert(f[1])));
    }
}



// Color
QColor& Model::getColor()
{
    return color;
}

void Model::setColor(const QColor& newColor)
{
    color = newColor;
}



float Model::getLen()
{
    return length;
}

Vector3f Model::getBase()
{
    return base;
}

void Model::setBase(const Vector3f &new_base)
{
    base = new_base;
}



void Model::scale(const Vector3f& k)
{
    int nverts = verts.size();

    if (k.x == 1 && k.z == 1)
        length *= k.y;

    for (int i = 0; i < nverts; i++)
    {
        verts[i].x *= k.x;
        verts[i].y *= k.y;
        verts[i].z *= k.z;
    }

    normalsProcessing();
}


void Model::rotate(const Vector3f& angle)
{
    std::shared_ptr<Matrix> m1(new MoveMatrix(-center.x, -center.y, -center.z));
    std::shared_ptr<Matrix> m2(new MoveMatrix(center.x, center.y, center.z));

    std::shared_ptr<Matrix> rx(new RotateOxMatrix(angle.x * M_PI / 180));
    std::shared_ptr<Matrix> ry(new RotateOyMatrix(angle.y * M_PI / 180));
    std::shared_ptr<Matrix> rz(new RotateOzMatrix(angle.z * M_PI / 180));

    size_t nverts = verts.size();

    for (size_t i = 0; i < nverts; i++)
    {
        verts[i].transform(m1);

        verts[i].transform(rx);
        verts[i].transform(ry);
        verts[i].transform(rz);

        verts[i].transform(m2);
    }
}


void Model::rot_pend(const Vector3f& angle)
{
    float tmp_x, tmp_y;
    Vector3f base = Vector3f(0, 0.805989, 0.362673);;
    for (int i = 0; i < verts.size(); ++i)
    {
        tmp_x = (verts[i].x - base.x) * cos(angle.z * M_PI / 180) -
                (verts[i].y - base.y) * sin(angle.z * M_PI / 180);
        tmp_y = (verts[i].x - base.x) * sin(angle.z * M_PI / 180) +
                (verts[i].y - base.y) * cos(angle.z * M_PI / 180);
        verts[i].x = tmp_x;
        verts[i].y = tmp_y;
    }
}


void Model::run(const int tempo)
{
    isSprite = true;

    //calculate trajectories
    const float g = 9.81, ampl = M_PI / 4;
    float herz = tempo / 60.0, t = 1 / herz;
    float r, step, min_y = 0.805989;
    float x, y, start_x, start_y, end_x, end_y;
    std::vector<Vector3f> trajectory;
    for (int i = 0; i < verts.size(); ++i)
    {
        r = t * t / (4 * M_PI * M_PI) * g;
        if (verts[i].y == min_y)
            trajectory.push_back(Vector3f(verts[i].x, verts[i].y, verts[i].z));
        else
        {
            step = 1;
            start_x = r * cos(M_PI / 2 - ampl);
            start_y = r * sin(M_PI / 2 - ampl);
            end_x = r * cos(M_PI / 2 + ampl);
            end_y = start_y;
            x = start_x;
            y = start_y;
            float tmp_x, tmp_y;
            float angle = -2 * M_PI / 180;
            while (x >= end_x && y >= end_y)
            {
                trajectory.push_back(Vector3f(x, y, verts[i].z));
                tmp_x = x * cos(angle) + y * sin(angle);
                tmp_y = -x * sin(angle) + y * cos(angle);
                x = tmp_x;
                y = tmp_y;
            }
        }
        trajs.push_back(trajectory);
        trajectory.clear();
    }
}

void Model::stop()
{
    isSprite = false;
}

bool Model::isRunning()
{
    return isSprite;
}
