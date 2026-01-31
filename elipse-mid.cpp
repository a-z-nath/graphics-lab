#include <GL/glut.h>

void plot4Points(int xc, int yc, int x, int y) {
  glBegin(GL_POINTS);
  glVertex2i(xc + x, yc + y);
  glVertex2i(xc - x, yc + y);
  glVertex2i(xc + x, yc - y);
  glVertex2i(xc - x, yc - y);
  glEnd();
}

/**
 * 1. Input rx, ry and center (xc, yc).
 * 2. Start Region 1 at (x, y) = (0, ry).
 * 3. Initial decision parameter p1 = ry² - (rx² * ry) + (0.25 * rx²).
 * 4. While (2 * ry² * x) <= (2 * rx² * y):
 *     a. Plot (xc+x, yc+y), (xc-x, yc+y), (xc+x, yc-y), (xc-x, yc-y).
 *     b. If p1 < 0:
 *        x++, p1 = p1 + 2*ry²*x + ry²
 *     c. Else:
 *        x++, y--, p1 = p1 + 2*ry²*x - 2*rx²*y + ry²
 * 5. Start Region 2 at the last (x, y) from Region 1.
 * 6. Initial decision parameter p2 = ry²(x+0.5)² + rx²(y-1)² - rx²ry².
 * 7. While y > 0:
 *    a. Plot (xc+x, yc+y), (xc-x, yc+y), (xc+x, yc-y), (xc-x, yc-y).
 *    b. If p2 > 0:
 *        y--, p2 = p2 - 2*rx²*y + rx²
 *    c. Else:
 *        x++, y--, p2 = p2 + 2*ry²*x - 2*rx²*y + rx²
 */
void midpointEllipse(int xc, int yc, int rx, int ry) {
  float dx, dy, d1, d2, x, y;
  x = 0;
  y = ry;

  // Region 1
  d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
  dx = 2 * ry * ry * x;
  dy = 2 * rx * rx * y;

  while (dx < dy) {
    plot4Points(xc, yc, x, y);
    if (d1 < 0) {
      x++;
      dx = dx + (2 * ry * ry);
      d1 = d1 + dx + (ry * ry);
    } else {
      x++;
      y--;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d1 = d1 + dx - dy + (ry * ry);
    }
  }

  // Region 2
  d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) +
       ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);

  while (y >= 0) {
    plot4Points(xc, yc, x, y);
    if (d2 > 0) {
      y--;
      dy = dy - (2 * rx * rx);
      d2 = d2 + (rx * rx) - dy;
    } else {
      y--;
      x++;
      dx = dx + (2 * ry * ry);
      dy = dy - (2 * rx * rx);
      d2 = d2 + dx - dy + (rx * rx);
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.5, 0.0); // Orange
  midpointEllipse(250, 250, 150, 100);
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Mid-point Ellipse");
  gluOrtho2D(0, 500, 0, 500);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}