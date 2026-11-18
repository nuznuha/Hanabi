
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

const int WIDTH = 1000;
const int HEIGHT = 650;

void drawCircle(float x, float y, float radius,
                float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);

    for (int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * 3.14159f * i / 100;
        glVertex2f(x + radius * std::cos(angle),
                   y + radius * std::sin(angle));
    }

    glEnd();
}

void drawRectangle(float x1, float y1, float x2, float y2,
                   float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // Night sky
    drawRectangle(0, 0, WIDTH, HEIGHT, 0.02f, 0.02f, 0.15f);

    // Moon
    drawCircle(800, 520, 55, 1.0f, 0.9f, 0.55f);

    // Ground
    drawRectangle(0, 0, WIDTH, 150, 0.15f, 0.12f, 0.10f);

    // Festival stall
    drawRectangle(300, 180, 700, 350, 0.65f, 0.15f, 0.15f);

    // Stall roof
    glColor3f(0.9f, 0.1f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(250, 350);
    glVertex2f(750, 350);
    glVertex2f(500, 470);
    glEnd();

    // Lanterns
    drawCircle(350, 430, 25, 1.0f, 0.4f, 0.1f);
    drawCircle(650, 430, 25, 1.0f, 0.4f, 0.1f);

    // Title
    glColor3f(1, 1, 1);
    glRasterPos2f(430, 570);

    const char* title = "HANABI";

    for (int i = 0; title[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

void init()
{
    glClearColor(0, 0, 0, 1);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("HANABI - Japanese Summer Festival");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}
