
#include "festival_street.h"
#include <GL/freeglut.h>
#include <cmath>

// Draw rectangle
void fsRectangle(float x1, float y1, float x2, float y2,
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

// Draw circle
void fsCircle(float cx, float cy, float radius,
              float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);

        for (int i = 0; i <= 100; i++)
        {
            float angle = 2.0f * 3.1415926f * i / 100;

            glVertex2f(
                cx + cos(angle) * radius,
                cy + sin(angle) * radius
            );
        }
    glEnd();
}

// Draw text
void fsText(float x, float y, const char* text)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

// Draw cloud
void drawCloud(float x, float y)
{
    fsCircle(x, y, 22, 1.0f, 1.0f, 1.0f);
    fsCircle(x + 25, y + 10, 28, 1.0f, 1.0f, 1.0f);
    fsCircle(x + 52, y, 22, 1.0f, 1.0f, 1.0f);

    fsRectangle(x - 5, y - 18, x + 58, y + 5,
                1.0f, 1.0f, 1.0f);
}

// Draw tree
void drawTree(float x, float y)
{
    fsRectangle(x - 7, y, x + 7, y + 65,
                0.42f, 0.22f, 0.10f);

    fsCircle(x, y + 85, 32, 0.12f, 0.48f, 0.20f);
    fsCircle(x - 22, y + 65, 24, 0.18f, 0.58f, 0.25f);
    fsCircle(x + 22, y + 65, 24, 0.18f, 0.58f, 0.25f);
}

// Draw lantern
void drawLantern(float x, float y)
{
    // Hanging string
    glColor3f(0.2f, 0.2f, 0.2f);
    glLineWidth(2);

    glBegin(GL_LINES);
        glVertex2f(x, y + 35);
        glVertex2f(x, y + 12);
    glEnd();

    // Lantern body
    fsRectangle(x - 13, y - 18, x + 13, y + 15,
                0.9f, 0.12f, 0.10f);

    // Top and bottom
    fsRectangle(x - 16, y + 13, x + 16, y + 18,
                0.2f, 0.1f, 0.08f);

    fsRectangle(x - 16, y - 21, x + 16, y - 17,
                0.2f, 0.1f, 0.08f);

    // Decoration
    glColor3f(1.0f, 0.85f, 0.2f);

    glBegin(GL_LINES);
        glVertex2f(x, y - 15);
        glVertex2f(x, y + 12);
    glEnd();
}

// Draw festival banner
void drawBanner()
{
    glColor3f(0.25f, 0.15f, 0.1f);
    glLineWidth(3);

    glBegin(GL_LINES);
        glVertex2f(170, 530);
        glVertex2f(830, 530);
    glEnd();

    float colors[5][3] =
    {
        {1.0f, 0.2f, 0.2f},
        {1.0f, 0.8f, 0.1f},
        {0.2f, 0.7f, 1.0f},
        {0.3f, 0.8f, 0.3f},
        {0.9f, 0.3f, 0.7f}
    };

    for (int i = 0; i < 10; i++)
    {
        int c = i % 5;
        float x = 190 + i * 65;

        glColor3f(colors[c][0], colors[c][1], colors[c][2]);

        glBegin(GL_TRIANGLES);
            glVertex2f(x, 530);
            glVertex2f(x + 45, 530);
            glVertex2f(x + 22, 485);
        glEnd();
    }
}

// Draw Japanese food stall
void drawFoodStall(float x, float y,
                   float r, float g, float b,
                   const char* name)
{
    // Stall body
    fsRectangle(x, y, x + 145, y + 100,
                0.78f, 0.45f, 0.22f);

    // Counter
    fsRectangle(x - 5, y + 20, x + 150, y + 42,
                0.42f, 0.20f, 0.08f);

    // Roof
    glColor3f(r, g, b);

    glBegin(GL_QUADS);
        glVertex2f(x - 12, y + 100);
        glVertex2f(x + 157, y + 100);
        glVertex2f(x + 135, y + 150);
        glVertex2f(x + 10, y + 150);
    glEnd();

    // Roof decoration
    fsRectangle(x - 12, y + 95, x + 157, y + 104,
                0.95f, 0.85f, 0.55f);

    // Stall opening
    fsRectangle(x + 20, y + 42, x + 125, y + 95,
                0.30f, 0.18f, 0.12f);

    // Stall sign
    fsRectangle(x + 20, y + 118, x + 125, y + 142,
                0.12f, 0.10f, 0.12f);

    fsText(x + 28, y + 125, name);

    // Food display
    fsCircle(x + 45, y + 53, 9, 1.0f, 0.75f, 0.2f);
    fsCircle(x + 72, y + 53, 9, 1.0f, 0.55f, 0.2f);
    fsCircle(x + 99, y + 53, 9, 1.0f, 0.75f, 0.2f);
}

// Main daytime festival scene
void drawFestivalStreetDay()
{
    // Sky
    fsRectangle(0, 150, 1000, 600,
                0.45f, 0.78f, 0.98f);

    // Sun
    fsCircle(850, 465, 48, 1.0f, 0.88f, 0.20f);

    // Clouds
    drawCloud(130, 455);
    drawCloud(550, 490);

    // Distant hills
    fsCircle(50, 155, 110, 0.25f, 0.65f, 0.38f);
    fsCircle(330, 155, 125, 0.30f, 0.70f, 0.40f);
    fsCircle(700, 155, 115, 0.25f, 0.65f, 0.38f);
    fsCircle(950, 155, 110, 0.30f, 0.70f, 0.40f);

    // Grass
    fsRectangle(0, 0, 1000, 175,
                0.30f, 0.62f, 0.30f);

    // Festival road
    fsRectangle(0, 0, 1000, 95,
                0.48f, 0.45f, 0.40f);

    // Road markings
    for (int i = 0; i < 1000; i += 120)
    {
        fsRectangle(i, 43, i + 60, 48,
                    0.95f, 0.90f, 0.75f);
    }

    // Trees
    drawTree(55, 165);
    drawTree(930, 165);

    // Festival banner
    drawBanner();

    // Lanterns
    drawLantern(250, 445);
    drawLantern(750, 445);

    // Food stalls
    drawFoodStall(100, 175,
                  0.85f, 0.12f, 0.12f, "TAKOYAKI");

    drawFoodStall(425, 175,
                  0.15f, 0.35f, 0.85f, "YAKISOBA");

    drawFoodStall(750, 175,
                  0.90f, 0.45f, 0.10f, "KAKIGORI");

    // Festival decorations
    fsCircle(340, 185, 8, 1.0f, 0.25f, 0.25f);
    fsCircle(670, 185, 8, 1.0f, 0.25f, 0.25f);

    // Title
    glColor3f(0.12f, 0.12f, 0.18f);
    glRasterPos2f(350, 565);

    const char* title = "JAPANESE SUMMER FESTIVAL";

    for (int i = 0; title[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, title[i]);
    }
}
