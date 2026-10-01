#include <iostream>
#include <numbers>
#include <cmath>

#include <GL/freeglut.h>

import vect3d;

constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

vect3d::vect3d x(40.0, 0.0, 0.0);
vect3d::vect3d y(0.0, 10.0, 0.0);

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 1.0, 0.0, 0.0, 1.0 };
GLfloat blue[] = { 0.0, 0.0, 1.0, 1.0 };

GLfloat lightpos[] = { 200.0, 200.0, 0.0, 1.0 };

long double sign(long double A) {
    return A >= 0 ? 1.0 : -1.0;
}

void cylinder(long double radius, long double height, int sides)
{
    
    glNormal3d(0.0, 1.0, 0.0);
    glBegin(GL_POLYGON);
    for (long double i = 0; i < sides; i++) {
        long double t = std::numbers::pi * 2 / sides * (double)i;
        glVertex3d(radius * cos(t), height, radius * sin(t));
    }
    glEnd();
    
    glBegin(GL_QUAD_STRIP);
    for (long double i = 0; i <= sides; i = i + 1) {
        long double t = i * 2 * std::numbers::pi / sides;
        glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
        glVertex3f((GLfloat)(radius * cos(t)), -height, (GLfloat)(radius * sin(t)));
        glVertex3f((GLfloat)(radius * cos(t)), height, (GLfloat)(radius * sin(t)));
    }
    glEnd();
    
    glNormal3d(0.0, -1.0, 0.0);
    glBegin(GL_POLYGON);
    for (long double i = sides; i >= 0; --i) {
        long double t = std::numbers::pi * 2 / sides * (double)i;
        glVertex3d(radius * cos(t), -height, radius * sin(t));
    }
    glEnd();
}

static void DrawWireCircle(vect3d::vect3d cen, long double rad, int n)
{
    long double t = 0.0;
    long double dt = 2.0 * std::numbers::pi / (double)n;

    glPushMatrix();

    glTranslated(cen.x(), cen.y(), cen.z());
    glBegin(GL_LINE_LOOP);
    do {
        glVertex3d(rad * cos(t), rad * sin(t), 0.0);
        t += dt;
    } while (t < 2.0 * std::numbers::pi);
    glEnd();

    glPopMatrix();
}

void arrow(long double x,long double y, long double z) {
    long double length = sqrt(x * x + y * y + z * z);
    long double angle = acos(y / length)*(180.0/std::numbers::pi);
    glPushMatrix();
    glRotated(angle, z, 0, -x);
    cylinder(1.0, length*0.7, 8);
    glPushMatrix();
    glTranslated(0.0, length*0.7, 0.0);
    glRotated(-90.0, 1.0, 0.0, 0.0);
    glutSolidCone(2.0, length*0.3, 8, 8);
    glPopMatrix();
    glPopMatrix();
}

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, WIDTH, HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (double)WIDTH / (double)HEIGHT, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(150.0, 150.0, 0.0, 
        0.0, 0.0, 0.0, 
        0.0, 1.0, 0.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
    glPushMatrix();
    glTranslated(x.x(), x.y(), x.z());
    arrow(y.x(), y.y(), y.z());
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    glPushMatrix();
    glTranslated(40.0, 0.0, 0.0);
    arrow(0.0, 10.0, 0.0);
    glPopMatrix();

    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    glutSolidSphere(40.0, 256, 256);
    
    glLineWidth(5.0);
    glPushMatrix();
    glRotated(45.0, 1.0, 0.0, 0.0);
    DrawWireCircle(vect3d::vect3d(0.0, 0.0, 0.0), 40.0, 256);
    glPopMatrix();

    glPushMatrix();
    glRotated(-45.0, 1.0, 0.0, 0.0);
    DrawWireCircle(vect3d::vect3d(0.0, 0.0, 0.0), 40.0, 256);
    glPopMatrix();

    glPushMatrix();
    glRotated(90.0, 0.0, 1.0, 0.0);
    DrawWireCircle(vect3d::vect3d(0.0, 0.0, 0.0), 40.0, 256);
    glPopMatrix();



    glutSwapBuffers();

}

long double cotan(long double theta) {
    return abs(theta - std::numbers::pi / 2.0) >= 0.0001 ? 1.0 / tan(theta) : 0.0;
}

void idle(void)
{   
    const long double R = 40.0;
    long double theta = acos(x.z() / R);
    long double phi = sign(x.y()) * acos(x.x() / sqrt(x.x() * x.x() + x.y() * x.y()));
    long double theta2 = theta;
    long double phi2 = phi;
    const long double dx = 0.001;
    long double dtheta = dx;
    long double dphi = dx;
    vect3d::vect3d e1(cos(theta) * cos(phi), cos(theta) * sin(phi), -sin(theta));
    vect3d::vect3d e2(-sin(theta) * sin(phi), sin(theta) * cos(phi), 0.0);

    long double v[2] = { 0.0 };
    e1.normalize();
    e2.normalize();

    static int i = 0,j=0,k=0;
    /*
    //3
    const int N = int((std::numbers::pi * 2.0) / dx);
    if (i < N) {
        dphi = dx;
        phi2 += dphi;
        theta2 = (std::numbers::pi / 2.0) - atan(-cotan(-std::numbers::pi / 2.0) * sin(phi2));
        dtheta = theta2 - theta;
        ++i;
    }
    */

    /*
    //2
    const int N = int((std::numbers::pi/2.0) / dx);
    const int M = int((std::numbers::pi / 4.0) / dx);
    if (i < N) {
        dphi = dx;
        phi2 += dphi;
        theta2 = (std::numbers::pi / 2.0) - atan(-cotan(-std::numbers::pi / 2.0 + std::numbers::pi / 8.0) * sin(phi2));
        dtheta = theta2 - theta;
        ++i;
    }
    else if (j < M) {
        dphi = 0.0;
        dtheta = dx;
        theta2 += dtheta;
        ++j;
    }
    else if (k < N) {
        dphi = -dx;
        phi2 += dphi;
        theta2 = (std::numbers::pi / 2.0) - atan(-cotan(std::numbers::pi / 2.0 - std::numbers::pi / 8.0) * sin(phi2));
        dtheta = theta2 - theta;
        ++k;
    }
    */
    
    
    //1
    const int N = int((std::numbers::pi/2.0) / dx);
    if (i < N) {
        dphi = dx;
        phi2 += dphi;
        theta2 = (std::numbers::pi / 2.0) - atan(-cotan(-std::numbers::pi / 4.0) * sin(phi2));
        dtheta = theta2 - theta;
        ++i;
    }
    else if (j < N) {
        dphi = 0.0;
        dtheta = dx;
        theta2 += dtheta;
        ++j;
    }
    else if (k < N) {
        dphi = -dx;
        phi2 += dphi;
        theta2 = (std::numbers::pi / 2.0) - atan(-cotan(std::numbers::pi / 4.0) * sin(phi2));
        dtheta = theta2 - theta;
        ++k;
    }
    
    x.setv(R * sin(theta2) * cos(phi2), R * sin(theta2) * sin(phi2), R * cos(theta2));

    //long double tmplen = y.length();
    v[0] = (y * e1) + sin(theta) * cos(theta) * (y * e2) * dphi;
    v[1] = (y * e2) - cotan(theta) * ((y * e2) * dtheta + (y * e1) * dphi);
    std::cout << v[0] << ' ' << v[1] << ' ' << sqrt(v[1]*v[1]+v[0]*v[0]) << ' ' << theta << ' ' << phi << std::endl;
    //std::cout << theta << ' ' << cotan(theta) << ' ' << (y * e2) * dtheta + (y * e1) * dphi << std::endl;

    e1.setv(cos(theta2) * cos(phi2), cos(theta2) * sin(phi2), -sin(theta2));
    e2.setv(-sin(theta2) * sin(phi2), sin(theta2) * cos(phi2), 0.0);
    e1.normalize();
    e2.normalize();
    //v[1] /= e2.length();
    y.setv(v[0] * e1.x() + v[1] * e2.x(), v[0] * e1.y() + v[1] * e2.y(), v[0] * e1.z() + v[1] * e2.z());
    //y.normalize();
    //y = y * tmplen;
    
    Sleep(1);
    glutPostRedisplay();
}

void Init() {
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

}

int main(int argc, char* argv[])
{
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("Parallel translation on sphere");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    Init();
    glutMainLoop();
    return 0;
}
