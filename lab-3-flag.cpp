#include <GL/glut.h>
#include <cmath>
/**
 * Lab 3: Drawing the Bangladesh Flag using Flood Fill Algorithm
 * Author: Avizith Debnath
 *
 * Problem Statement:
 * Your are given four co-ordinates (-20, -15), (30, -15), (30, 15) and
 * (-20, 15) which form a rectangle. You are also given a circle with radius 8
 *  and center at the origin (0, 0). With these information, you've to draw the
 * outlines of the flag. Then, when the user clicks inside the rectangle, it
 * should be filled with green color. When the user clicks inside the circle,
 * it should be filled with red color. You've to use the flood fill algorithm
 * to fill the shapes.
 */

const int WINDOW_SIZE = 500;
bool visited[WINDOW_SIZE][WINDOW_SIZE];

// Flag dimensions (large & centered)
const int LEFT = -200;
const int RIGHT = 200;
const int TOP = 120;
const int BOTTOM = -120;

const int RADIUS = 80;

void resetVisited() {
  for (int i = 0; i < WINDOW_SIZE; i++)
    for (int j = 0; j < WINDOW_SIZE; j++)
      visited[i][j] = false;
}

void drawPoint(int x, int y) {
  glBegin(GL_POINTS);
  glVertex2i(x, y);
  glEnd();
}

bool insideRectangle(int x, int y) {
  return (x > LEFT && x < RIGHT && y > BOTTOM && y < TOP);
}

bool insideCircle(int x, int y) { return (x * x + y * y <= RADIUS * RADIUS); }

void floodFill(int x, int y, const char *shape) {
  switch (shape[0]) {
  case 'c':
    if (!insideCircle(x, y))
      return;
    break;
  case 'r':
    if (!insideRectangle(x, y))
      return;
    break;
  }

  int sx = x + WINDOW_SIZE / 2; // screen coordinates - x
  int sy = y + WINDOW_SIZE / 2; // screen coordinates - y

  if (sx < 0 || sx >= WINDOW_SIZE || sy < 0 || sy >= WINDOW_SIZE)
    return;
  if (visited[sx][sy])
    return;

  visited[sx][sy] = true;

  drawPoint(x, y);

  floodFill(x + 1, y, shape);
  floodFill(x - 1, y, shape);
  floodFill(x, y + 1, shape);
  floodFill(x, y - 1, shape);
}

void drawRectangleOutline() {
  glBegin(GL_LINE_LOOP);
  glVertex2i(LEFT, BOTTOM);
  glVertex2i(RIGHT, BOTTOM);
  glVertex2i(RIGHT, TOP);
  glVertex2i(LEFT, TOP);
  glEnd();
}

void drawCircleOutline() {
  glBegin(GL_POINTS);
  for (int x = -RADIUS; x <= RADIUS; x++) {
    for (int y = -RADIUS; y <= RADIUS; y++) {
      if (abs(x * x + y * y - RADIUS * RADIUS) <= 50)
        glVertex2i(x, y);
    }
  }
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(0, 0, 0); // Black outlines
  drawRectangleOutline();
  drawCircleOutline();

  glFlush();
}

void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

    // Correct mouse conversion
    int wx = x - WINDOW_SIZE / 2;
    int wy = WINDOW_SIZE / 2 - y;

    if (insideCircle(wx, wy)) {
      resetVisited();
      glColor3f(1, 0, 0);
      floodFill(wx, wy, "circle");
    } else if (insideRectangle(wx, wy)) {
      resetVisited();
      glColor3f(0, 0.6, 0);
      floodFill(wx, wy, "rectangle");
    }

    glFlush();
  }
}

void init() {
  glClearColor(1, 1, 1, 1); // White background
  glPointSize(2);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Perfect match with window
  gluOrtho2D(-WINDOW_SIZE / 2, WINDOW_SIZE / 2, -WINDOW_SIZE / 2,
             WINDOW_SIZE / 2);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
  glutCreateWindow("Bangladesh Flag - Flood Fill");

  init();

  glutDisplayFunc(display);
  glutMouseFunc(mouse);

  glutMainLoop();
  return 0;
}
