#include <GL/glut.h>
#include <iostream>
using namespace std;

// Window size
int WIDTH = 600;
int HEIGHT = 600;

// Clipping window
float xmin = -100, xmax = 100, ymin = -100, ymax = 100;

bool clipEnabled = false;

struct Point {
  float x, y;
};

Point points[] = {Point{-150, 50}, Point{150, 50},    Point{-50, -150},
                  Point{50, 150},  Point{-200, -200}, Point{-50, -50},
                  Point{0, 0},     Point{80, 80}};

const int INSIDE = 0;
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4;
const int TOP = 8;

int computeCode(Point p) {
  int code = INSIDE;

  if (p.x < xmin)
    code |= LEFT;
  else if (p.x > xmax)
    code |= RIGHT;

  if (p.y < ymin)
    code |= BOTTOM;
  else if (p.y > ymax)
    code |= TOP;

  return code;
}

bool cohenSutherland(Point &p1, Point &p2) {
  int code1 = computeCode(p1);
  int code2 = computeCode(p2);

  while (true) {
    if (!(code1 | code2)) {
      return true; // Accept
    } else if (code1 & code2) {
      return false; // Reject
    } else {
      float x, y;
      int codeOut = code1 ? code1 : code2;

      if (codeOut & TOP) {
        x = p1.x + (p2.x - p1.x) * (ymax - p1.y) / (p2.y - p1.y);
        y = ymax;
      } else if (codeOut & BOTTOM) {
        x = p1.x + (p2.x - p1.x) * (ymin - p1.y) / (p2.y - p1.y);
        y = ymin;
      } else if (codeOut & RIGHT) {
        y = p1.y + (p2.y - p1.y) * (xmax - p1.x) / (p2.x - p1.x);
        x = xmax;
      } else if (codeOut & LEFT) {
        y = p1.y + (p2.y - p1.y) * (xmin - p1.x) / (p2.x - p1.x);
        x = xmin;
      }

      if (codeOut == code1) {
        p1.x = x;
        p1.y = y;
        code1 = computeCode(p1);
      } else {
        p2.x = x;
        p2.y = y;
        code2 = computeCode(p2);
      }
    }
  }
}

void drawWindow() {
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_LOOP);
  glVertex2f(xmin, ymin);
  glVertex2f(xmax, ymin);
  glVertex2f(xmax, ymax);
  glVertex2f(xmin, ymax);
  glEnd();
}

void drawLine(Point p1, Point p2) {
  glBegin(GL_LINES);
  glVertex2f(p1.x, p1.y);
  glVertex2f(p2.x, p2.y);
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawWindow();

  for (int i = 0; i < 4; i++) {
    Point p1 = points[2 * i];
    Point p2 = points[2 * i + 1];
    if (!clipEnabled) {
      glColor3f(1, 0, 0); // red
      drawLine(p1, p2);
    } else {
      if (cohenSutherland(p1, p2)) {
        glColor3f(0, 1, 0); // green
        drawLine(p1, p2);
      }
    }
  }

  glFlush();
}

void keyboard(unsigned char key, int x, int y) {
  if (key == 'c') {
    clipEnabled = true;
    glutPostRedisplay();
  }
  if (key == 'r') {
    clipEnabled = false;
    glutPostRedisplay();
  }
}

void init() {
  glClearColor(1, 1, 1, 1);
  glMatrixMode(GL_PROJECTION);
  gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Cohen-Sutherland Line Clipping");

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}