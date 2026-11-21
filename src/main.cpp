
#include <GL/freeglut.h>
#include "scene_controller.h"

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1000, 600);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("HANABI - Japanese Summer Festival");

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1000, 0, 600);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initializeSceneController();

    glutDisplayFunc(displayScene);
    glutKeyboardFunc(handleKeyboard);
    glutTimerFunc(16, updateScene, 0);

    glutMainLoop();
    return 0;
}
