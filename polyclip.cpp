#include <GL/glut.h>
#include <bits/stdc++.h>
using namespace std;

int WIDTH = 600;
int HEIGHT = 600;

// Clipping window
float xmin = -100, xmax = 100, ymin = -100, ymax = 100;

bool clipEnabled = false;

struct Point {
  float x, y;
};

// Triangle
vector<Point> polygon = {{100, 150}, {200, 100}, {-150, -100}};

// Inside Test
bool inside(Point p, int edge) {
  switch (edge) {
  case 0:
    return p.x >= xmin; // LEFT
  case 1:
    return p.x <= xmax; // RIGHT
  case 2:
    return p.y >= ymin; // BOTTOM
  case 3:
    return p.y <= ymax; // TOP
  }
  return false;
}

// Intersection
Point intersection(Point s, Point p, int edge) {
  Point i;
  float m;

  if (p.x != s.x)
    m = (p.y - s.y) / (p.x - s.x);
  else
    m = 1e9;

  switch (edge) {

  case 0: // LEFT
    i.x = xmin;
    i.y = s.y + m * (xmin - s.x);
    break;

  case 1: // RIGHT
    i.x = xmax;
    i.y = s.y + m * (xmax - s.x);
    break;

  case 2: // BOTTOM
    i.y = ymin;
    if (p.x != s.x)
      i.x = s.x + (ymin - s.y) / m;
    else
      i.x = s.x;
    break;

  case 3: // TOP
    i.y = ymax;
    if (p.x != s.x)
      i.x = s.x + (ymax - s.y) / m;
    else
      i.x = s.x;
    break;
  }
  return i;
}

// ------------------------------------------------
// Sutherland–Hodgman
// ------------------------------------------------
vector<Point> clipPolygon(vector<Point> input) {

  vector<Point> output = input;

  for (int edge = 0; edge < 4;
       edge++) { // LEFT, RIGHT, BOTTOM, TOP windown edges

    vector<Point> temp = output;
    output.clear();

    for (int i = 0; i < temp.size(); i++) {
      // for SP polygon edge
      Point S = temp[i];
      Point P = temp[(i + 1) % temp.size()];

      bool S_inside = inside(S, edge);
      bool P_inside = inside(P, edge);

      if (S_inside && P_inside) {
        output.push_back(P);
      } else if (S_inside && !P_inside) {
        output.push_back(intersection(S, P, edge));
      } else if (!S_inside && P_inside) {
        output.push_back(intersection(S, P, edge));
        output.push_back(P);
      }
    }
  }

  return output;
}

// ------------------------------------------------
// Draw Window
// ------------------------------------------------
void drawWindow() {
  glColor3f(0, 0, 1);
  glBegin(GL_LINE_LOOP);
  glVertex2f(xmin, ymin);
  glVertex2f(xmax, ymin);
  glVertex2f(xmax, ymax);
  glVertex2f(xmin, ymax);
  glEnd();
}

// ------------------------------------------------
// Draw Polygon
// ------------------------------------------------
void drawPolygon(vector<Point> poly) {
  glBegin(GL_LINE_LOOP);
  for (auto p : poly)
    glVertex2f(p.x, p.y);
  glEnd();
}

// ------------------------------------------------
// Display
// ------------------------------------------------
void display() {

  glClear(GL_COLOR_BUFFER_BIT);

  drawWindow();

  if (!clipEnabled) {
    glColor3f(1, 0, 0);
    drawPolygon(polygon);
  } else {
    vector<Point> clipped = clipPolygon(polygon);
    glColor3f(0, 1, 0);
    drawPolygon(clipped);
  }

  glFlush();
}

// ------------------------------------------------
// Keyboard
// ------------------------------------------------
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

// ------------------------------------------------
// Init
// ------------------------------------------------
void init() {
  glClearColor(0, 0, 0, 1);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-WIDTH / 2, WIDTH / 2, -HEIGHT / 2, HEIGHT / 2);
}

// ------------------------------------------------
// Main
// ------------------------------------------------
int main(int argc, char **argv) {

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Polygon Clipping - Sutherland Hodgman");

  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}