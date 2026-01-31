#include <GL/glut.h>
#include <iostream>

// Helper to plot the 8 symmetrical points
void plotPoints(int xc, int yc, int x, int y) {
  glBegin(GL_POINTS);
  glVertex2i(xc + x, yc + y);
  glVertex2i(xc - x, yc + y);
  glVertex2i(xc + x, yc - y);
  glVertex2i(xc - x, yc - y);
  glVertex2i(xc + y, yc + x);
  glVertex2i(xc - y, yc + x);
  glVertex2i(xc + y, yc - x);
  glVertex2i(xc - y, yc - x);
  glEnd();
}

/**
 * 1. Input radius 'r' and center (xc, yc).
 * 2. Start at (x, y) = (0, r).
 * 3. Calculate initial decision parameter p = 1 - r.
 * 4. While x <= y:
 *    a. Plot points at:
 *       (xc+x, yc+y), (xc-x, yc+y), (xc+x, yc-y), (xc-x, yc-y)
 *       (xc+y, yc+x), (xc-y, yc+x), (xc+y, yc-x), (xc-y, yc-x)
 *    b. Increment x: x = x + 1.
 *    c. If p < 0:
 *       p = p + 2x + 1.
 *    d. Else:
 *       y = y - 1.
 *       p = p + 2(x - y) + 1.
 */
void drawMidpointCircle(int xc, int yc, int r) {
  int x = 0;
  int y = r;
  int p = 1 - r;

  plotPoints(xc, yc, x, y);

  while (x < y) {
    x++;
    if (p < 0) {
      // Midpoint is inside, move East
      p += 2 * x + 1;
    } else {
      // Midpoint is outside, move South-East
      y--;
      p += 2 * (x - y) + 1;
    }
    plotPoints(xc, yc, x, y);
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 1.0, 1.0); // White circle

  // Draw a circle with center (250, 250) and radius 150
  drawMidpointCircle(250, 250, 150);

  glFlush();
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0); // Black background
  // Set up a 2D coordinate system (500x500)
  gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Mid-point Circle Algorithm");

  init();
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}