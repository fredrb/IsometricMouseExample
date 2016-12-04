#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

GLfloat mouse_x = 2.0f;
GLfloat mouse_y = 2.0f;

GLfloat th = 2.0f / 20;
GLfloat tw = 2 * th;

void translate_mouse(GLFWwindow* window, double xpos, double ypos) {
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  GLfloat x_percent = xpos / width;
  GLfloat y_percent = ypos / height;

  mouse_x = ((x_percent)*2.0f) - 1.0f;
  mouse_y = (((y_percent)*2.0f) - 1.0f) * -1;
}

GLfloat area(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
  return std::abs(((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1))/2.0);
}

bool mouse_in_triangle(GLfloat* p1, GLfloat* p2, GLfloat* p3) {
  GLfloat a = area(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
  GLfloat t1 = area(mouse_x, mouse_y, p2[0], p2[1], p3[0], p3[1]);
  GLfloat t2 = area(p1[0], p1[1], mouse_x, mouse_y, p3[0], p3[1]);
  GLfloat t3 = area(p1[0], p1[1], p2[0], p2[1], mouse_x, mouse_y);

  std::cout << "Triangle area: " << a << std::endl;
  std::cout << "Points: " << t1 << ", " << t2 << ", "<< t3 << std::endl;
  std::cout << "Mouse: " << mouse_x << ":" << mouse_y << std::endl;
  return (a == (t1 + t2 + t3));
}

bool mouse_in(GLfloat* a, GLfloat* b, GLfloat* c, GLfloat* d) {
  if (mouse_x > a[0] && mouse_x <= c[0]) {
    if (mouse_y > d[1] && mouse_y <= b[1]) {
      return (mouse_in_triangle(a, b, d) || mouse_in_triangle(b, c, d));
    }
  }

  return false;
}

void draw_tile(GLfloat x, GLfloat y, bool odd) {
  glBegin(GL_POLYGON);
  if (odd) {
    glColor3f(0.0f, 0.0f, 1.0f);
  } else {
    glColor3f(1.0f, 1.0f, 0.0f);
  }

  GLfloat a[2] = {x, y + (th/2)};
  GLfloat b[2] = {x + (tw/2), y + th};
  GLfloat c[2] = {x + tw, y + (th/2)};
  GLfloat d[2] = {x + (tw/2), y};

  if (mouse_in(a, b, c, d)) {
    glColor3f(0.5f, 0.7f, 0.7f);
  }

  glVertex2fv(a);
  glVertex2fv(b);
  glVertex2fv(c);
  glVertex2fv(d);
  glEnd();
}

void draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GLfloat initial_x = -1.0f;
  GLfloat initial_y = -1.0f;

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      bool odd = (j%2 == 0);
      draw_tile((i * tw + j * (tw/2) + initial_x),
                (j * (th/2) + initial_y), odd);
    }
  }

}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
  translate_mouse(window, xpos, ypos);
}

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit()) {
    std::cout << "GLFW initialization failed";
    return -1;
  }

  window = glfwCreateWindow(600, 600, "Tutorial", NULL, NULL);
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glOrtho(0, width, height, 0, 0, 1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (window == NULL) {
    std::cout << "Error opening window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetCursorPosCallback(window, cursor_position_callback);

  do {
    glfwSwapBuffers(window);
    glfwPollEvents();
    draw();
    glfwSwapBuffers(window);
  } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0 );

  glfwTerminate();
  std::cout << "Exit pressed!" << std::endl;
  return 0;
}


