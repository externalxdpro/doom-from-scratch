#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <array>
#include <cmath>
#include <iostream>
#include <vector>

#define res 1
#define SW 160 * res
#define SH 120 * res
#define SW2 (SW / 2)
#define SH2 (SH / 2)
#define pixelScale 4 / res
#define GLSW (SW * pixelScale)
#define GLSH (SH * pixelScale)

struct Time {
    int fr1, fr2;
};
Time T;

struct Keys {
    int w, s, a, d;
    int sl, sr;
    int m;
};
Keys K;

void pixel(int x, int y, int c) {
    std::array<int, 3> rgb;
    if (c == 0) {
        rgb = {255, 255, 0}; // Yellow
    } else if (c == 1) {
        rgb = {160, 160, 0}; // Yellow darker
    } else if (c == 2) {
        rgb = {0, 255, 0}; // Green
    } else if (c == 3) {
        rgb = {0, 160, 0}; // Green darker
    } else if (c == 4) {
        rgb = {0, 255, 255}; // Cyan
    } else if (c == 5) {
        rgb = {0, 160, 160}; // Cyan darker
    } else if (c == 6) {
        rgb = {160, 100, 0}; // Brown
    } else if (c == 7) {
        rgb = {110, 50, 0}; // Brown darker
    } else if (c == 8) {
        rgb = {0, 60, 130}; // Background
    }
    glColor3ub(rgb[0], rgb[1], rgb[2]);
    glBegin(GL_POINTS);
    glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
    glEnd();
}

void movePlayer() {
    if (K.a == 1 && K.m == 0) {
        std::cout << "left\n";
    }
    if (K.d == 1 && K.m == 0) {
        std::cout << "right\n";
    }
    if (K.w == 1 && K.m == 0) {
        std::cout << "up\n";
    }
    if (K.s == 1 && K.m == 0) {
        std::cout << "down\n";
    }

    if (K.sl == 1) {
        std::cout << "strafe left\n";
    }
    if (K.sr == 1) {
        std::cout << "strafe right\n";
    }

    if (K.a == 1 && K.m == 1) {
        std::cout << "look up\n";
    }
    if (K.d == 1 && K.m == 1) {
        std::cout << "look down\n";
    }
    if (K.w == 1 && K.m == 1) {
        std::cout << "move up\n";
    }
    if (K.s == 1 && K.m == 1) {
        std::cout << "move up\n";
    }
}

void clearBackground() {
    for (int y = 0; y < SH; y++) {
        for (int x = 0; x < SW; x++) {
            pixel(x, y, 8);
        }
    }
}

int  tick;
void draw3D() {
    int c = 0;
    for (int y = 0; y < SH2; y++) {
        for (int x = 0; x < SW2; x++) {
            pixel(x, y, c);
            if (++c > 8) {
                c = 0;
            }
        }
    }
    if (++tick > 20) {
        tick = 0;
    }
    pixel(SW2, SH2 + tick, 0);
}

void display() {
    if (T.fr1 - T.fr2 >= 50) {
        clearBackground();
        movePlayer();
        draw3D();

        T.fr2 = T.fr1;
        glutSwapBuffers();
        glutReshapeWindow(GLSW, GLSH);
    }

    T.fr1 = glutGet(GLUT_ELAPSED_TIME);
    glutPostRedisplay();
}

void keysDown(unsigned char key, int x, int y) {
    if (key == 'w') {
        K.w = 1;
    }
    if (key == 's') {
        K.s = 1;
    }
    if (key == 'a') {
        K.a = 1;
    }
    if (key == 'd') {
        K.d = 1;
    }
    if (key == 'm') {
        K.m = 1;
    }
    if (key == ',') {
        K.sl = 1;
    }
    if (key == '.') {
        K.sr = 1;
    }
}

void keysUp(unsigned char key, int x, int y) {
    if (key == 'w') {
        K.w = 0;
    }
    if (key == 's') {
        K.s = 0;
    }
    if (key == 'a') {
        K.a = 0;
    }
    if (key == 'd') {
        K.d = 0;
    }
    if (key == 'm') {
        K.m = 0;
    }
    if (key == ',') {
        K.sl = 0;
    }
    if (key == '.') {
        K.sr = 0;
    }
}

void init() {}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(GLSW / 2, GLSH / 2);
    glutInitWindowSize(GLSW, GLSH);
    glutCreateWindow("");
    glPointSize(pixelScale);
    gluOrtho2D(0, GLSW, 0, GLSH);
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keysDown);
    glutKeyboardUpFunc(keysUp);
    glutMainLoop();
}
