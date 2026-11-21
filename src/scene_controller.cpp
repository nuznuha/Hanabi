
#include "scene_controller.h"
#include "festival_street.h"
#include <GL/freeglut.h>
#include <cstdlib>

Scene currentScene = OPENING;

bool isPaused = false;
bool soundEnabled = true;

const char* sceneNames[SCENE_COUNT] =
{
    "HANABI - JAPANESE SUMMER FESTIVAL",
    "FESTIVAL STREET - DAY MODE",
    "DAY TO NIGHT TRANSITION",
    "FESTIVAL STREET - NIGHT MODE",
    "KIDS' PLAY ZONE",
    "FIREWORKS SHOW",
    "GRAND FINALE"
};

void drawText(float x, float y, const char* text)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(x, y);

    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

void drawRectangle(float x1, float y1,
                   float x2, float y2,
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

void drawSceneBackground()
{
    switch (currentScene)
    {
        case OPENING:
        case FESTIVAL_DAY:
            glClearColor(0.45f, 0.75f, 0.95f, 1.0f);
            break;

        case DAY_TO_NIGHT:
            glClearColor(0.75f, 0.40f, 0.55f, 1.0f);
            break;

        default:
            glClearColor(0.03f, 0.03f, 0.18f, 1.0f);
            break;
    }

    glClear(GL_COLOR_BUFFER_BIT);
}

void displayScene()
{
    drawSceneBackground();

    if (currentScene == FESTIVAL_DAY)
    {
        drawFestivalStreetDay();
    }
    else
    {
        drawText(300, 420, sceneNames[currentScene]);

        if (currentScene == OPENING)
        {
            drawText(350, 320, "WELCOME TO HANABI!");
            drawText(325, 280, "A JAPANESE SUMMER FESTIVAL");
        }
        else if (currentScene == DAY_TO_NIGHT)
        {
            drawText(350, 320, "ENJOY THE EVENING!");
        }
        else if (currentScene == FESTIVAL_NIGHT)
        {
            drawText(350, 320, "FESTIVAL NIGHT");
        }
        else if (currentScene == KIDS_PLAYZONE)
        {
            drawText(350, 330, "GOLDFISH SCOOPING");
            drawText(350, 290, "RING TOSS GAME");
            drawText(350, 250, "TARGET GAME");
        }
        else if (currentScene == FIREWORKS)
        {
            drawText(370, 330, "THE FIREWORKS SHOW");
            drawText(350, 290, "ENJOY THE NIGHT SKY!");
        }
        else if (currentScene == FINALE)
        {
            drawText(370, 330, "THANK YOU!");
            drawText(330, 290, "SEE YOU NEXT SUMMER!");
        }
    }

    if (isPaused)
    {
        glColor3f(1.0f, 0.9f, 0.1f);
        drawText(450, 200, "PAUSED");
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    if (soundEnabled)
        drawText(20, 30, "Sound: ON");
    else
        drawText(20, 30, "Sound: OFF");

    drawText(20, 10,
        "SPACE: Pause | R: Restart | N: Next Scene | M: Sound | ESC: Exit");

    glutSwapBuffers();
}

void nextScene()
{
    if (currentScene < FINALE)
        currentScene = static_cast<Scene>(currentScene + 1);
    else
        currentScene = OPENING;

    glutPostRedisplay();
}

void restartScene()
{
    currentScene = OPENING;
    isPaused = false;

    glutPostRedisplay();
}

void updateScene(int value)
{
    // Automatic scene changing is disabled.
    // Press N to change scenes manually.

    glutPostRedisplay();
    glutTimerFunc(16, updateScene, 0);
}

void handleKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:
            exit(0);
            break;

        case ' ':
            isPaused = !isPaused;
            break;

        case 'r':
        case 'R':
            restartScene();
            break;

        case 'n':
        case 'N':
            if (!isPaused)
                nextScene();
            break;

        case 'm':
        case 'M':
            soundEnabled = !soundEnabled;
            break;
    }

    glutPostRedisplay();
}

void initializeSceneController()
{
    currentScene = OPENING;
    isPaused = false;
    soundEnabled = true;
}
