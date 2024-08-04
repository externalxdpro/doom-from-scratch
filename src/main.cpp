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

struct Frames {
    int fr1, fr2;
};
Frames frames;

struct Keys {
    int w, s, a, d;
    int sl, sr;
    int m;
};
Keys keys;

struct Math {
    std::array<float, 360> sin, cos;
};
Math math;

struct Player {
    int x, y, z;
    int a;
    int l;
};
Player player;

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
    if (keys.a == 1 && keys.m == 0) {
        player.a -= 4;
        player.a += player.a < 0 ? 360 : 0;
    }
    if (keys.d == 1 && keys.m == 0) {
        player.a += 4;
        player.a -= player.a > 359 ? 360 : 0;
    }

    int dx = math.sin[player.a] * 10.0, dy = math.cos[player.a] * 10.0;
    if (keys.w == 1 && keys.m == 0) {
        player.x += dx;
        player.y += dy;
    }
    if (keys.s == 1 && keys.m == 0) {
        player.x -= dx;
        player.y -= dy;
    }

    if (keys.sl == 1) {
        player.x -= dy;
        player.y -= dx;
    }
    if (keys.sr == 1) {
        player.x += dy;
        player.y += dx;
    }

    if (keys.a == 1 && keys.m == 1) {
        player.l -= 1;
    }
    if (keys.d == 1 && keys.m == 1) {
        player.l += 1;
    }
    if (keys.w == 1 && keys.m == 1) {
        player.z -= 4;
    }
    if (keys.s == 1 && keys.m == 1) {
        player.z += 4;
    }
}

void clearBackground() {
    for (int y = 0; y < SH; y++) {
        for (int x = 0; x < SW; x++) {
            pixel(x, y, 8);
        }
    }
}

void clipBehindPlayer(std::pair<int &, int> x, std::pair<int &, int> y,
                      std::pair<int &, int> z) {
    float da = y.first, db = y.second;
    float d = (da - db == 0) ? 1 : da - db;
    float s = da / d;
    x.first = x.first + s * (x.second - x.first);
    y.first = y.first + s * (y.second - y.first);
    if (y.first == 0) {
        y.first = 1;
    }
    z.first = z.first + s * (z.second - z.first);
}

void drawWall(std::pair<int, int> x, std::pair<int, int> b,
              std::pair<int, int> t) {
    int currX, currY;
    int dyb = b.second - b.first;
    int dyt = t.second - t.first;
    int dx  = (x.second - x.first == 0) ? 1 : x.second - x.first;
    int xi  = x.first;

    if (x.first < 1) {
        x.first = 1;
    }
    if (x.second < 1) {
        x.second = 1;
    }
    if (x.first > SW - 1) {
        x.first = SW - 1;
    }
    if (x.second > SW - 1) {
        x.second = SW - 1;
    }

    for (currX = x.first; currX < x.second; currX++) {
        std::pair<int, int> y = {dyb * (currX - xi + 0.5) / dx + b.first,
                                 dyt * (currX - xi + 0.5) / dx + t.first};

        if (y.first < 1) {
            y.first = 1;
        }
        if (y.second < 1) {
            y.second = 1;
        }
        if (y.first > SH - 1) {
            y.first = SH - 1;
        }
        if (y.second > SH - 1) {
            y.second = SH - 1;
        }

        for (currY = y.first; currY < y.second; currY++) {
            pixel(currX, currY, 0);
        }
    }
}

void draw3D() {
    std::array<int, 4> wx, wy, wz;

    float cos = math.cos[player.a], sin = math.sin[player.a];
    int   x1 = 40 - player.x, y1 = 10 - player.y;
    int   x2 = 40 - player.x, y2 = 290 - player.y;

    wx[0] = x1 * cos - y1 * sin;
    wx[1] = x2 * cos - y2 * sin;
    wx[2] = wx[0];
    wx[3] = wx[1];

    wy[0] = y1 * cos + x1 * sin;
    wy[1] = y2 * cos + x2 * sin;
    wy[2] = wy[0];
    wy[3] = wy[1];

    wz[0] = -player.z + ((player.l * wy[0]) / 32.0);
    wz[1] = -player.z + ((player.l * wy[1]) / 32.0);
    wz[2] = wz[0] + 40;
    wz[3] = wz[1] + 40;

    if (wy[0] < 1 && wy[1] < 1) {
        return;
    } else if (wy[0] < 1) {
        clipBehindPlayer({wx[0], wx[1]}, {wy[0], wy[1]}, {wz[0], wz[1]});
        clipBehindPlayer({wx[2], wx[3]}, {wy[2], wy[3]}, {wz[3], wz[2]});
    } else if (wy[1] < 1) {
        clipBehindPlayer({wx[1], wx[0]}, {wy[1], wy[0]}, {wz[1], wz[0]});
        clipBehindPlayer({wx[3], wx[2]}, {wy[3], wy[2]}, {wz[3], wz[2]});
    }

    wx[0] = wx[0] * 200 / wy[0] + SW2;
    wy[0] = wz[0] * 200 / wy[0] + SH2;
    wx[1] = wx[1] * 200 / wy[1] + SW2;
    wy[1] = wz[1] * 200 / wy[1] + SH2;
    wx[2] = wx[2] * 200 / wy[2] + SW2;
    wy[2] = wz[2] * 200 / wy[2] + SH2;
    wx[3] = wx[3] * 200 / wy[3] + SW2;
    wy[3] = wz[3] * 200 / wy[3] + SH2;

    // if (wx[0] > 0 && wx[0] < SW && wy[0] > 0 && wy[0] < SH) {
    //     pixel(wx[0], wy[0], 0);
    // }
    // if (wx[1] > 0 && wx[1] < SW && wy[1] > 0 && wy[1] < SH) {
    //     pixel(wx[1], wy[1], 0);
    // }
    drawWall({wx[0], wx[1]}, {wy[0], wy[1]}, {wy[2], wy[3]});
}

void display() {
    if (frames.fr1 - frames.fr2 >= 50) {
        clearBackground();
        movePlayer();
        draw3D();

        frames.fr2 = frames.fr1;
        glutSwapBuffers();
        glutReshapeWindow(GLSW, GLSH);
    }

    frames.fr1 = glutGet(GLUT_ELAPSED_TIME);
    glutPostRedisplay();
}

void keysDown(unsigned char key, int x, int y) {
    if (key == 'w') {
        keys.w = 1;
    }
    if (key == 's') {
        keys.s = 1;
    }
    if (key == 'a') {
        keys.a = 1;
    }
    if (key == 'd') {
        keys.d = 1;
    }
    if (key == 'm') {
        keys.m = 1;
    }
    if (key == ',') {
        keys.sl = 1;
    }
    if (key == '.') {
        keys.sr = 1;
    }
}

void keysUp(unsigned char key, int x, int y) {
    if (key == 'w') {
        keys.w = 0;
    }
    if (key == 's') {
        keys.s = 0;
    }
    if (key == 'a') {
        keys.a = 0;
    }
    if (key == 'd') {
        keys.d = 0;
    }
    if (key == 'm') {
        keys.m = 0;
    }
    if (key == ',') {
        keys.sl = 0;
    }
    if (key == '.') {
        keys.sr = 0;
    }
}

void init() {
    for (int i = 0; i < 360; i++) {
        math.sin[i] = sin(i / 180.0 * M_PI);
        math.cos[i] = cos(i / 180.0 * M_PI);
    }

    player = {.x = 70, .y = -110, .z = 20, .a = 0, .l = 0};
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
