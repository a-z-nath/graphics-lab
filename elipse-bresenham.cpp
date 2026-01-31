#include <GL/glut.h>

/**
 * 1. Initialize x = 0, y = ry.
 * 2. d1 = ry² - rx²ry + ¼rx²
 * 3. Plot points using 4-way symmetry.
 * 4. Region 1 (slope < 1):
 *    If d1 < 0: d1 = d1 + 2ry²x + 3ry²
 *    Else: d1 = d1 + 2ry²x - 2rx²y + 2rx² + 3ry², y--
 *    x++
 * 5. Region 2 (slope > 1):
 *    If d2 < 0: d2 = d2 + 2ry²x - 2rx²y + 2rx² + 3ry², x++
 *    Else: d2 = d2 - 2rx²y + 3rx²
 *    y--
 */
void bresenhamEllipse(int xc, int yc, int rx, int ry) {
  long rx2 = rx * rx;
  long ry2 = ry * ry;
  long twoRx2 = 2 * rx2;
  long twoRy2 = 2 * ry2;
  int x = 0;
  int y = ry;
  long px = 0;
  long py = twoRx2 * y;

  auto plot = [&](int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(xc + x, yc + y);
    glVertex2i(xc - x, yc + y);
    glVertex2i(xc + x, yc - y);
    glVertex2i(xc - x, yc - y);
    glEnd();
  };

  // Region 1
  long p = (long)(ry2 - (rx2 * ry) + (0.25 * rx2) + 0.5);
  while (px < py) {
    plot(x, y);
    x++;
    px += twoRy2;
    if (p < 0)
      p += ry2 + px;
    else {
      y--;
      py -= twoRx2;
      p += ry2 + px - py;
    }
  }

  // Region 2
  p = (long)(ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2 +
             0.5);
  while (y >= 0) {
    plot(x, y);
    y--;
    py -= twoRx2;
    if (p > 0)
      p += rx2 - py;
    else {
      x++;
      px += twoRy2;
      p += rx2 - py + px;
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0, 1.0, 1.0); // Cyan
  bresenhamEllipse(250, 250, 200, 100);
  glFlush();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Bresenham Ellipse");
  gluOrtho2D(0, 500, 0, 500);
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}