
#ifndef SCENE_CONTROLLER_H
#define SCENE_CONTROLLER_H

enum Scene
{
    OPENING,
    FESTIVAL_DAY,
    DAY_TO_NIGHT,
    FESTIVAL_NIGHT,
    KIDS_PLAYZONE,
    FIREWORKS,
    FINALE,
    SCENE_COUNT
};

void initializeSceneController();
void displayScene();
void updateScene(int value);
void handleKeyboard(unsigned char key, int x, int y);
void nextScene();
void restartScene();

#endif
