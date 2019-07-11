#include <bits/stdc++.h>

using namespace std;

#define pi (2*acos(0.0))
#define N 4

ofstream stage1;

struct point
{
    double x, y, z, w;
};

struct point eye, look, up;
double near, far, fovY, aspectRatio;

class Matrix
{
public:
    double arr[N][N];
    bool forStack;

    Matrix()
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                arr[i][j] = 0;
            }
        }

        forStack = false;
    }

    void identityM()
    {
        for(int i = 0; i < N; i++)
        {
            arr[i][i] = 1;
        }
    }

    void print()
    {
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                cout << fixed << setprecision(7) << arr[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

stack<Matrix> stck;

void init()
{
    stage1.open("stage1.txt");
    cin >> eye.x >> eye.y >> eye.z;
    cin >> look.x >> look.y >> look.z;
    cin >> up.x >> up.y >> up.z;
    cin >> fovY >> aspectRatio >> near >> far;
}

Matrix matMultiply(Matrix m1, Matrix m2)
{
    Matrix result;

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                result.arr[i][j] += m1.arr[i][k] * m2.arr[k][j];
            }
        }
    }

    return result;
}

point crossProduct(point p, point q)
{
    point result;

    result.x = p.y*q.z - p.z*q.y;
    result.y = p.z*q.x - p.x*q.z;
    result.z = p.x*q.y - p.y*q.x;

    return result;
}

double dotProduct(point p, point q)
{
    double result = 0.0;

    result += p.x*q.x;
    result += p.y*q.y;
    result += p.z*q.z;

    return result;
}

point rodrigues(point xVec, point aVec, double theta)
{
    point result;
    double angle = theta*pi/180.0;
    point aCROSSx = crossProduct(aVec, xVec);
    double xDOTa = dotProduct(xVec, aVec);

    result.x = cos(angle)*xVec.x + (1.0 - cos(angle))*xDOTa*aVec.x + sin(angle)*aCROSSx.x;
    result.y = cos(angle)*xVec.y + (1.0 - cos(angle))*xDOTa*aVec.y + sin(angle)*aCROSSx.y;
    result.z = cos(angle)*xVec.z + (1.0 - cos(angle))*xDOTa*aVec.z + sin(angle)*aCROSSx.z;

    return result;
}

void printMatrix(Matrix m)
{
    for(int i = 0; i < N - 1; i++)
    {
        for(int j = 0; j < N - 1; j++)
        {
            stage1 << fixed << setprecision(7) << m.arr[j][i];
            if(j < N-2) stage1 << " ";
        }
        stage1 << endl;
    }

    stage1 << endl;
}

void triangle()
{
    point a, b, c;
    cin >> a.x >> a.y >> a.z;
    cin >> b.x >> b.y >> b.z;
    cin >> c.x >> c.y >> c.z;

    Matrix triangleMatrix;

//    triangleMatrix.arr[0][0] = a.x; triangleMatrix.arr[0][1] = a.y; triangleMatrix.arr[0][2] = a.z; triangleMatrix.arr[0][3] = 1.0;
//    triangleMatrix.arr[1][0] = b.x; triangleMatrix.arr[1][1] = b.y; triangleMatrix.arr[1][2] = b.z; triangleMatrix.arr[1][3] = 1.0;
//    triangleMatrix.arr[2][0] = c.x; triangleMatrix.arr[2][1] = c.y; triangleMatrix.arr[2][2] = c.z; triangleMatrix.arr[2][3] = 1.0;
//    triangleMatrix.arr[3][0] = 1.0; triangleMatrix.arr[3][1] = 1.0; triangleMatrix.arr[3][2] = 1.0; triangleMatrix.arr[3][3] = 1.0;

    triangleMatrix.arr[0][0] = a.x; triangleMatrix.arr[1][0] = a.y; triangleMatrix.arr[2][0] = a.z; triangleMatrix.arr[3][0] = 1.0;
    triangleMatrix.arr[0][1] = b.x; triangleMatrix.arr[1][1] = b.y; triangleMatrix.arr[2][1] = b.z; triangleMatrix.arr[3][1] = 1.0;
    triangleMatrix.arr[0][2] = c.x; triangleMatrix.arr[1][2] = c.y; triangleMatrix.arr[2][2] = c.z; triangleMatrix.arr[3][2] = 1.0;
    triangleMatrix.arr[0][3] = 1.0; triangleMatrix.arr[1][3] = 1.0; triangleMatrix.arr[2][3] = 1.0; triangleMatrix.arr[3][3] = 1.0;

    Matrix result;
    result = matMultiply(stck.top(), triangleMatrix);

    //stck.top().print();
    //triangleMatrix.print();

    printMatrix(result);
}

void translate()
{
    double tx, ty, tz;
    cin >> tx >> ty >> tz;

    Matrix translationMatrix;
    translationMatrix.identityM();

    translationMatrix.arr[0][N-1] = tx;
    translationMatrix.arr[1][N-1] = ty;
    translationMatrix.arr[2][N-1] = tz;

    Matrix result;
    result = matMultiply(stck.top(), translationMatrix);

    //result.forStack = true;
    stck.push(result);
}

void scale()
{
    double sx, sy, sz;
    cin >> sx >> sy >> sz;

    Matrix scalingMatrix;
    scalingMatrix.identityM();

    scalingMatrix.arr[0][0] = sx;
    scalingMatrix.arr[1][1] = sy;
    scalingMatrix.arr[2][2] = sz;

    Matrix result;
    result = matMultiply(stck.top(), scalingMatrix);

    //result.forStack = true;
    stck.push(result);
}


void rotateM()
{
    double angle, ax, ay, az;
    cin >> angle >> ax >> ay >> az;

    Matrix rotationMatrix;
    rotationMatrix.identityM();
    point xAxis, yAxis, zAxis;

    xAxis.x = 1; xAxis.y = 0; xAxis.z = 0;
    yAxis.x = 0; yAxis.y = 1; yAxis.z = 0;
    zAxis.x = 0; zAxis.y = 0; zAxis.z = 1;

    double valueOfVector = sqrt(ax*ax + ay*ay + az*az);

    point a;
    a.x = ax/valueOfVector;
    a.y = ay/valueOfVector;
    a.z = az/valueOfVector;

    point c1 = rodrigues(xAxis, a, angle);
    point c2 = rodrigues(yAxis, a, angle);
    point c3 = rodrigues(zAxis, a, angle);

    rotationMatrix.arr[0][0] = c1.x; rotationMatrix.arr[0][1] = c2.x; rotationMatrix.arr[0][2] = c3.x;
    rotationMatrix.arr[1][0] = c1.y; rotationMatrix.arr[1][1] = c2.y; rotationMatrix.arr[1][2] = c3.y;
    rotationMatrix.arr[2][0] = c1.z; rotationMatrix.arr[2][1] = c2.z; rotationMatrix.arr[2][2] = c3.z;
    //rotationMatrix.arr[3][3] = 1.0;

    Matrix result;
    result = matMultiply(stck.top(), rotationMatrix);

    //stck.top().print();
    //rotationMatrix.print();
    //result.forStack = true;
    stck.push(result);
}

void push()
{
    stck.push(stck.top());
    stck.top().forStack = true;
}

void pop()
{
    while(!stck.top().forStack)
        stck.pop();

    stck.pop();
}

int main()
{
    freopen("scene.txt", "r", stdin);

    init();

    Matrix initial;
    initial.identityM();
    stck.push(initial);

    string command;

    while(true)
    {
        cin >> command;

        if(command == "triangle") triangle();
        else if(command == "translate") translate();
        else if(command == "scale") scale();
        else if(command == "rotate") rotateM();
        else if(command == "push") push();
        else if(command == "pop") pop();
        else if(command == "end") break;
        else continue;
    }

    return 0;
}
