#include <GL/glut.h>

// Function to plot 8 points using symmetry
void draw8Points(int xc, int yc, int x, int y) {
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

void bresenhamCircle(int xc, int yc, int r) {
  int x = 0;
  int y = r;
  int d = 3 - 2 * r; // Initial decision parameter

  draw8Points(xc, yc, x, y);

  while (y >= x) {
    // Increment x for every step
    x++;

    if (d > 0) {
      // Choose the diagonal pixel (South-East)
      y--;
      d = d + 4 * (x - y) + 10;
    } else {
      // Choose the horizontal pixel (East)
      d = d + 4 * x + 6;
    }

    draw8Points(xc, yc, x, y);
  }
}

// Draws a horizontal line between two x-coordinates at a specific y-level
void drawScanline(int x1, int x2, int y) {
  glBegin(GL_LINES);
  glVertex2i(x1, y);
  glVertex2i(x2, y);
  glEnd();
}

void fillCircle(int xc, int yc, int r) {
  int x = 0;
  int y = r;
  int p = 1 - r;

  while (x <= y) {
    // Draw horizontal lines between the symmetrical points
    // These four lines cover the entire interior as the algorithm progresses
    drawScanline(xc - x, xc + x, yc + y); // Top section
    drawScanline(xc - x, xc + x, yc - y); // Bottom section
    drawScanline(xc - y, xc + y, yc + x); // Upper-middle section
    drawScanline(xc - y, xc + y, yc - x); // Lower-middle section

    x++;
    if (p < 0) {
      p += 2 * x + 1;
    } else {
      y--;
      p += 2 * (x - y) + 1;
    }
  }
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 0.0, 0.0); // Red circle
  glPointSize(2.0);
  glBegin(GL_POINTS);
  glVertex2i(250, 250); // Center point
  glEnd();

  glPointSize(1.0);
  glColor3f(0.0, 1.0, 0.0); // Green circle

  // Center (250, 250), Radius 100
  bresenhamCircle(250, 250, 100);

  glColor3f(0.0, 0.0, 1.0); // Blue filled circle
  fillCircle(250, 250, 100);

  glFlush();
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Standard 2D projection
  gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Bresenham Circle Drawing");

  init();
  glutDisplayFunc(display);
  glutMainLoop();

  return 0;
}