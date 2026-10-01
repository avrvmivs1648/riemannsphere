module;

#include<cmath>
#include<numbers>>
#include<GL/freeglut.h>

export module grObj;

import vect3d;

export namespace grObj {
    class grObj {
        vect3d::vect3d _v;
    protected:
        grObj(vect3d::vect3d v):_v(v) {}
        virtual void drawObj() = 0;
    public:
        void translate(vect3d::vect3d v) {
            _v = v;
            glPushMatrix();
            glTranslated(v.x(), v.y(), v.z());
            draw();
            glPopMatrix();
        }
        void rotate(double angle, vect3d::vect3d rotAxis) {
            glPushMatrix();
            glRotated(angle, rotAxis.x(), rotAxis.y(), rotAxis.z());
            draw();
            glPopMatrix();
        }
        void draw() {
            drawObj();
        }
    };

    class circle :public grObj {
        double _radius;
        int _split;
    protected:
        void drawObj(){
            
        }
    public:
        circle(vect3d::vect3d v,double radius, int split) :grObj(v),_radius(radius), _split(split) {
        }
        
    };

    class cylinder:public grObj {
        double _radius;
        double _height;
        int _sides;
    protected:
        void drawObj() {

        }
    public:
        cylinder(vect3d::vect3d v, double radius, double height, int sides) :grObj(v), _radius(radius), _height(height), _sides(sides) {
        }
    };
    void cylinder(float radius, float height, int sides)
    {
        //è„ñ 
        glNormal3d(0.0, 1.0, 0.0);
        glBegin(GL_POLYGON);
        for (double i = 0; i < sides; i++) {
            double t = std::numbers::pi * 2 / sides * (double)i;
            glVertex3d(radius * cos(t), height, radius * sin(t));
        }
        glEnd();
        //ë§ñ 
        glBegin(GL_QUAD_STRIP);
        for (double i = 0; i <= sides; i = i + 1) {
            double t = i * 2 * std::numbers::pi / sides;
            glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
            glVertex3f((GLfloat)(radius * cos(t)), -height, (GLfloat)(radius * sin(t)));
            glVertex3f((GLfloat)(radius * cos(t)), height, (GLfloat)(radius * sin(t)));
        }
        glEnd();
        //â∫ñ 
        glNormal3d(0.0, -1.0, 0.0);
        glBegin(GL_POLYGON);
        for (double i = sides; i >= 0; --i) {
            double t = std::numbers::pi * 2 / sides * (double)i;
            glVertex3d(radius * cos(t), -height, radius * sin(t));
        }
        glEnd();
    }

    void arrow(double x, double y, double z) {
        double length = sqrt(x * x + y * y + z * z);
        double angle = acos(y / length) * (180.0 / std::numbers::pi);
        glPushMatrix();
        glRotated(angle, z, 0, -x);
        cylinder(1.0, length * 0.7, 8);
        glPushMatrix();
        glTranslated(0.0, length * 0.7, 0.0);
        glRotated(-90.0, 1.0, 0.0, 0.0);
        glutSolidCone(2.0, length * 0.3, 8, 8);
        glPopMatrix();
        glPopMatrix();
    }
}