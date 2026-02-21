#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
  int yMax;
  float xCur;
  float invSlope;

  // Sort by current X for the Active Edge Table
  bool operator<(const Edge &other) const { return xCur < other.xCur; }
};

struct Point {
  int x, y;
};

// Example polygon vertices (anticlockwise pentagon)
vector<Point> vertices = {
    {250, 400}, // top
    {130, 320}, // upper-left
    {180, 150}, // lower-left
    {320, 150}, // lower-right
    {370, 320}  // upper-right
};

/**
 * 1. Initialize the Edge Table (ET) with all polygon edges.
 * 2. Set y = minimum y-coordinate in the polygon.
 * 3. Repeat until ET and Active Edge Table (AET) are empty:
 *     a. Move edges from ET to AET where edge.y_min == y.
 *     b. Remove edges from AET where edge.y_max == y.
 *     c. Sort AET by x-coordinate of intersection.
 *     d. For each pair of x-coordinates (x1, x2) in AET:
 *         - Draw a horizontal line from (x1, y) to (x2, y).
 *     e. Increment y (move to next scanline).
 *     f. For each edge in AET, update x: x = x + (1/slope).
 */
void drawScanline() {
  int n = vertices.size();
  int yMin = 1000, yMax = 0;

  // Find min and max Y to define the loop range
  for (const auto &p : vertices) {
    if (p.y < yMin)
      yMin = p.y;
    if (p.y > yMax)
      yMax = p.y;
  }

  // Edge Table (ET) - using a vector of vectors for each scanline
  vector<vector<Edge>> ET(yMax + 1);

  for (int i = 0; i < n; i++) {
    Point p1 = vertices[i];
    Point p2 = vertices[(i + 1) % n];

    if (p1.y == p2.y)
      continue; // Ignore horizontal edges

    if (p1.y > p2.y)
      swap(p1, p2);

    Edge e;
    e.yMax = p2.y;
    e.xCur = p1.x;
    e.invSlope = (float)(p2.x - p1.x) / (p2.y - p1.y);
    ET[p1.y].push_back(e);
  }

  vector<Edge> AET;
  glBegin(GL_POINTS);
  glColor3f(0.0, 0.8, 0.8); // Teal color

  for (int y = yMin; y <= yMax; y++) {
    // 1. Move edges from ET to AET
    for (const auto &e : ET[y]) {
      AET.push_back(e);
    }

    // 2. Remove edges where y == yMax
    AET.erase(
        remove_if(AET.begin(), AET.end(), [y](Edge e) { return e.yMax <= y; }),
        AET.end());

    // 3. Sort AET by xCur
    sort(AET.begin(), AET.end());

    // 4. Fill between pairs
    for (size_t i = 0; i < AET.size(); i += 2) {
      for (int x = (int)AET[i].xCur; x < (int)AET[i + 1].xCur; x++) {
        glVertex2i(x, y);
      }
    }

    // 5. Update xCur for next scanline
    for (auto &e : AET) {
      e.xCur += e.invSlope;
    }
  }
  glEnd();
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawScanline();
  glFlush();
}

void init() {
  glClearColor(0.1, 0.1, 0.1, 1.0);
  gluOrtho2D(0, 500, 0, 500);
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Scanline Polygon Fill");
  init();
  glutDisplayFunc(display);
  glutMainLoop();
  return 0;
}