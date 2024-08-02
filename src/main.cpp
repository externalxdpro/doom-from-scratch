#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <array>
#include <cmath>
#include <iostream>

#define res 1
#define SW 160 * res
#define SH 120 * res
#define SW2 (SW / 2)
#define SH2 (SH / 2)
#define pixelScale 4 / res
#define GLSW (SW * pixelScale)
#define GLSH (SH * pixelScale)

struct {
    int fr1, fr2;
} Time;

struct {
    int w, s, a, d;
    int sl, sr;
    int m;
} Keys;

struct {
    std::array<float, 360> sin, cos;
} Math;

struct {
    int x, y, z;
    int a;
    int l;
} Player;

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
    if (Keys.a == 1 && Keys.m == 0) {
        Player.a -= 4;
        Player.a += Player.a < 0 ? 360 : 0;
    }
    if (Keys.d == 1 && Keys.m == 0) {
        Player.a += 4;
        Player.a -= Player.a > 359 ? 360 : 0;
    }

    int dx = Math.sin[Player.a] * 10.0, dy = Math.cos[Player.a] * 10.0;
    if (Keys.w == 1 && Keys.m == 0) {
        Player.x += dx;
        Player.y += dy;
    }
    if (Keys.s == 1 && Keys.m == 0) {
        Player.x -= dx;
        Player.y -= dy;
    }

    if (Keys.sl == 1) {
        Player.x += dy;
        Player.y += dx;
    }
    if (Keys.sr == 1) {
        Player.x -= dy;
        Player.y -= dx;
    }

    if (Keys.a == 1 && Keys.m == 1) {
        Player.l -= 1;
    }
    if (Keys.d == 1 && Keys.m == 1) {
        Player.l += 1;
    }
    if (Keys.w == 1 && Keys.m == 1) {
        Player.z -= 4;
    }
    if (Keys.s == 1 && Keys.m == 1) {
        Player.z += 4;
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
    if (Time.fr1 - Time.fr2 >= 50) {
        clearBackground();
        movePlayer();
        draw3D();

        Time.fr2 = Time.fr1;
        glutSwapBuffers();
        glutReshapeWindow(GLSW, GLSH);
    }

    Time.fr1 = glutGet(GLUT_ELAPSED_TIME);
    glutPostRedisplay();
}

void keysDown(unsigned char key, int x, int y) {
    if (key == 'w') {
        Keys.w = 1;
    }
    if (key == 's') {
        Keys.s = 1;
    }
    if (key == 'a') {
        Keys.a = 1;
    }
    if (key == 'd') {
        Keys.d = 1;
    }
    if (key == 'm') {
        Keys.m = 1;
    }
    if (key == ',') {
        Keys.sl = 1;
    }
    if (key == '.') {
        Keys.sr = 1;
    }
}

void keysUp(unsigned char key, int x, int y) {
    if (key == 'w') {
        Keys.w = 0;
    }
    if (key == 's') {
        Keys.s = 0;
    }
    if (key == 'a') {
        Keys.a = 0;
    }
    if (key == 'd') {
        Keys.d = 0;
    }
    if (key == 'm') {
        Keys.m = 0;
    }
    if (key == ',') {
        Keys.sl = 0;
    }
    if (key == '.') {
        Keys.sr = 0;
    }
}

void init() {
    for (int i = 0; i < 360; i++) {
        Math.sin[i] = sin(i / 180.0 * M_PI);
        Math.cos[i] = cos(i / 180.0 * M_PI);
    }

    Player = {.x = 70, .y = -110, .z = 20, .a = 0, .l = 0};
}

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
