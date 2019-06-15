#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double maxlen = 60;
double curlen;

struct point
{
    double x,y,z;
};

struct point pos, u, r, l;

struct point rotation3D(struct point p, struct point constant, double angle) //angle in degree
{
    struct point result, temp;

    temp.x = constant.y*p.z - constant.z*p.y;
    temp.y = constant.z*p.x - constant.x*p.z;
    temp.z = constant.x*p.y - constant.y*p.x;

    result.x = p.x*cos(angle*pi/180.0) + temp.x*sin(angle*pi/180.0);
    result.y = p.y*cos(angle*pi/180.0) + temp.y*sin(angle*pi/180.0);
    result.z = p.z*cos(angle*pi/180.0) + temp.z*sin(angle*pi/180.0);

    return result;
}

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}

void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    glColor3f(1, 0, 0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a, 0);
        glVertex3f( a,-a, 0);
        glVertex3f(-a,-a, 0);
        glVertex3f(-a, a, 0);
    }
    glEnd();
}

void drawSphere(double radius,int slices,int stacks)
{
    glColor3f(0, 1, 0);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
			points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}

void drawCylinder(double radius, double height, int slices, int stacks)
{
    glColor3f(0,0,1);
    struct point points[100][100];
    int i, j;
    double h, r;
    //generate points
    for (i = 0; i <= stacks; i++)
    {
        h = height * sin(((double)i / (double)stacks)*(pi/ 2.0));
        //r=radius*cos(((double)i/(double)stacks)*(pi/2));
        //h = height;
        r = radius;
        for (j = 0; j <= slices; j++)
        {
            points[i][j].x = r * cos(((double)j / (double)slices)/2.0*pi);
            points[i][j].y = r * sin(((double)j / (double)slices)/2.0*pi);
            points[i][j].z = h;
        }
    }
    //draw quads using generated points
    for (i = 0; i<stacks; i++)
    {
        //glColor3f(0, 1, 0);
        for (j = 0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawSpheres8()
{
    glPushMatrix();
    {
        glTranslated(curlen, curlen, curlen);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(curlen, -curlen, curlen);
        glRotated(-90, 0, 0, 1);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-curlen, curlen, curlen);
        glRotated(90, 0, 0, 1);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-curlen, -curlen, curlen);
        glRotated(180, 0, 0, 1);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(curlen, curlen, -curlen);
        glRotated(90, 0, 1, 0);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(curlen, -curlen, -curlen);
        //glRotated(180, 1, 1, 0);
        glRotated(180, 1, 0, 0);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-curlen, curlen, -curlen);
        glRotated(-180, 0, 1, 0);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-curlen, -curlen, -curlen);
        glRotated(180, 1, 1, 0);
        glRotated(180, 0, 0, 1);
        drawSphere(maxlen - curlen, 50, 50);
    }
    glPopMatrix();
}

void drawCylinders12()
{
    glPushMatrix(); //ok
    {
        glTranslated(curlen, curlen, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(curlen, -curlen, 0);
        glRotated(-90, 0, 0, 1);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(-curlen, curlen, 0);
        glRotated(90, 0, 0, 1);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(-curlen, -curlen, 0);
        glRotated(180, 0, 0, 1);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(curlen, 0, curlen);
        glRotated(90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //pk
    {
        glTranslated(-curlen, 0, curlen);
         glRotated(90, 0, 0, 1);
        glRotated(-90, 0, 1, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(curlen, 0, -curlen);
        glRotated(-90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(-curlen, 0, -curlen);
        glRotated(90, 0, 0, 1);
        glRotated(90, 0, 1, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0, curlen, curlen); //ok
         glRotated(90, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(0, curlen, -curlen);
        glRotated(90, 0, 0, 1);
        glRotated(-90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(0, -curlen, curlen);
         glRotated(-90, 0, 0, 1);
        glRotated(90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();

    glPushMatrix(); //ok
    {
        glTranslated(0, -curlen, -curlen);
        glRotated(-90, 0, 0, 1);
        glRotated(-90, 1, 0, 0);
        drawCylinder(maxlen - curlen, curlen, 50, 50);
    }
    glPopMatrix();
}

void drawSquares6()
{
    glPushMatrix();
    {
        glTranslated(0, 0, maxlen);
        drawSquare(curlen);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0, 0, -maxlen);
        drawSquare(curlen);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0, maxlen, 0);
        glRotated(90, 1, 0, 0);
        drawSquare(curlen);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(0, -maxlen, 0);
        glRotated(90, 1, 0, 0);
        drawSquare(curlen);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(maxlen, 0, 0);
        glRotated(90, 0, 1, 0);
        drawSquare(curlen);
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(-maxlen, 0, 0);
        glRotated(90, 0, 1, 0);
        drawSquare(curlen);
    }
    glPopMatrix();
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1': //rotate left
        l = rotation3D(l, u, 0.5);
        r = rotation3D(r, u, 0.5);
        break;
    case '2': //rotate right
        l = rotation3D(l, u, -0.5);
        r = rotation3D(r, u, -0.5);
        break;
    case '3': //look up
        u = rotation3D(u, r, 0.5);
        l = rotation3D(l, r, 0.5);
        break;
    case '4': //look down
        u = rotation3D(u, r, -0.5);
        l = rotation3D(l, r, -0.5);
        break;
    case '5': //tilt clockwise
        u = rotation3D(u, l, -0.5);
        r = rotation3D(r, l, -0.5);
        break;
    case '6': //tilt anticlockwise
        u = rotation3D(u, l, 0.5);
        r = rotation3D(r, l, 0.5);
        break;

    default:
        break;
    }
}

void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        pos.x -= l.x;
        pos.y -= l.y;
        pos.z -= l.z;
        break;
    case GLUT_KEY_UP:		// up arrow key
        pos.x += l.x;
        pos.y += l.y;
        pos.z += l.z;
        break;

    case GLUT_KEY_RIGHT:
        pos.x += r.x;
        pos.y += r.y;
        pos.z += r.z;
        break;
    case GLUT_KEY_LEFT:
        pos.x -= r.x;
        pos.y -= r.y;
        pos.z -= r.z;
        break;

    case GLUT_KEY_PAGE_UP:
        pos.z += 1.0;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos.z -= 1.0;
        break;
    case GLUT_KEY_HOME:
        if(curlen < 60) curlen++;
        break;
    case GLUT_KEY_END:
        if(curlen > 0) curlen--;
        break;
    default:
        break;
    }
}

void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    default:
        break;
    }
}

void display()
{
    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(pos.x,pos.y,pos.z,  l.x+pos.x,l.y+pos.y,l.z+pos.z,  u.x,u.y,u.z);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();
    drawSpheres8();
    drawCylinders12();
    drawSquares6();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;

    u.x = 0;
    u.y = 0;
    u.z = 1;
    r.x = -1/sqrt(2);
    r.y = 1/sqrt(2);
    r.z = 0;
    l.x = -1/sqrt(2);
    l.y = -1/sqrt(2);
    l.z = 0;
    pos.x = 200;
    pos.y = 200;
    pos.z = 75;

    curlen = 40;

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("1505102_Camera");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
