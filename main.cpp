#include <GL/freeglut.h>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int WIDTH = 1000;
const int HEIGHT = 650;

enum Scene { MENU, FESTIVAL, FIREWORKS, ABOUT };
Scene currentScene = MENU;

struct Particle {
    float x, y, vx, vy;
    float r, g, b;
    float life;
};

std::vector<Particle> particles;
float smokeOffset = 0.0f;
float visitorX = 0.0f;
float walkPhase = 0.0f;
float glow = 0.0f;
bool paused = false;
int fireworkPattern = 0;

void color(float r, float g, float b) { glColor3f(r, g, b); }

void rect(float x, float y, float w, float h, float r, float g, float b) {
    color(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y); glVertex2f(x+w, y);
    glVertex2f(x+w, y+h); glVertex2f(x, y+h);
    glEnd();
}

void circle(float x, float y, float radius, float r, float g, float b) {
    color(r, g, b);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= 40; ++i) {
        float a = 2.0f * 3.1415926f * i / 40.0f;
        glVertex2f(x + std::cos(a)*radius, y + std::sin(a)*radius);
    }
    glEnd();
}

void line(float x1, float y1, float x2, float y2,
          float r, float g, float b, float width = 2.0f) {
    color(r, g, b);
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(x1, y1); glVertex2f(x2, y2);
    glEnd();
}

void text(float x, float y, const std::string& value,
          void* font = GLUT_BITMAP_HELVETICA_18,
          float r = 1.0f, float g = 1.0f, float b = 1.0f) {
    color(r, g, b);
    glRasterPos2f(x, y);
    for (unsigned char c : value) glutBitmapCharacter(font, c);
}

void drawLantern(float x, float y, float scale = 1.0f) {
    // Soft glow behind the lantern
    circle(x, y, 25.0f*scale, 0.65f, 0.22f, 0.08f);
    rect(x-11*scale, y-17*scale, 22*scale, 34*scale,
         0.88f, 0.20f + 0.08f*std::sin(glow), 0.12f);
    rect(x-7*scale, y-13*scale, 14*scale, 26*scale, 1.0f, 0.65f, 0.25f);
    line(x, y+17*scale, x, y+28*scale, 0.95f, 0.75f, 0.35f, 2);
    line(x-8*scale, y-18*scale, x+8*scale, y-18*scale, 0.2f, 0.1f, 0.08f, 2);
}

void drawTree(float x, float y, float s = 1.0f) {
    rect(x-8*s, y, 16*s, 65*s, 0.32f, 0.16f, 0.08f);
    circle(x, y+78*s, 34*s, 0.08f, 0.34f, 0.20f);
    circle(x-22*s, y+60*s, 23*s, 0.10f, 0.42f, 0.22f);
    circle(x+22*s, y+61*s, 23*s, 0.08f, 0.30f, 0.18f);
}

void drawStall(float x, float y, const std::string& name,
               float roofR, float roofG, float roofB) {
    // Stall body and counter
    rect(x, y, 175, 105, 0.48f, 0.22f, 0.12f);
    rect(x-7, y+100, 189, 14, 0.92f, 0.78f, 0.55f);
    // Roof
    color(roofR, roofG, roofB);
    glBegin(GL_TRIANGLES);
    glVertex2f(x-15, y+114); glVertex2f(x+87, y+174); glVertex2f(x+190, y+114);
    glEnd();
    rect(x+10, y+45, 155, 12, 0.88f, 0.68f, 0.42f);
    text(x+22, y+125, name, GLUT_BITMAP_HELVETICA_12, 1, 1, 0.9f);
    // Counter display
    circle(x+45, y+28, 12, 0.95f, 0.55f, 0.18f);
    circle(x+88, y+28, 12, 0.95f, 0.55f, 0.18f);
    circle(x+132, y+28, 12, 0.95f, 0.55f, 0.18f);
}

void drawPerson(float x, float y, float scale, float phase, bool moving) {
    float swing = moving ? std::sin(phase)*10.0f : 0.0f;
    // Yukata-inspired body
    circle(x, y+66*scale, 12*scale, 0.95f, 0.72f, 0.54f);
    rect(x-12*scale, y+25*scale, 24*scale, 38*scale,
         0.35f, 0.28f, 0.65f);
    // Arms
    line(x-10*scale, y+55*scale, x-20*scale, y+38*scale+swing,
         0.95f, 0.72f, 0.54f, 4);
    line(x+10*scale, y+55*scale, x+20*scale, y+38*scale-swing,
         0.95f, 0.72f, 0.54f, 4);
    // Legs
    line(x-5*scale, y+25*scale, x-8*scale+swing*0.4f, y,
         0.12f, 0.12f, 0.18f, 4);
    line(x+5*scale, y+25*scale, x+8*scale-swing*0.4f, y,
         0.12f, 0.12f, 0.18f, 4);
}

void drawMenu() {
    rect(0, 0, WIDTH, HEIGHT, 0.035f, 0.045f, 0.12f);
    // Decorative stars
    for (int i=0; i<55; ++i) {
        float x = (i*173 % 970) + 15;
        float y = (i*97 % 560) + 45;
        circle(x, y, 1.8f, 0.95f, 0.85f, 0.58f);
    }
    // Moon and lantern accents
    circle(830, 500, 48, 1.0f, 0.83f, 0.48f);
    circle(850, 515, 43, 0.035f, 0.045f, 0.12f);
    drawLantern(150, 470, 1.1f);
    drawLantern(850, 300, 1.0f);

    text(300, 535, "HANABI", GLUT_BITMAP_TIMES_ROMAN_24, 1.0f, 0.72f, 0.35f);
    text(270, 495, "JAPANESE SUMMER FESTIVAL", GLUT_BITMAP_HELVETICA_18);
    text(365, 405, "1. ENTER FESTIVAL", GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.65f);
    text(365, 355, "2. WATCH FIREWORKS", GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.65f);
    text(365, 305, "3. ABOUT HANABI", GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.65f);
    text(365, 255, "4. EXIT", GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.65f);
    text(300, 125, "Click an option or press keys 1, 2, 3, 4",
         GLUT_BITMAP_HELVETICA_12, 0.75f, 0.78f, 0.85f);
}

void drawFestival() {
    rect(0, 0, WIDTH, HEIGHT, 0.035f, 0.055f, 0.15f);
    // Stars
    for (int i=0; i<45; ++i) {
        float x = (i*211 % 990) + 5;
        float y = 390 + (i*71 % 230);
        circle(x, y, 1.5f, 0.95f, 0.91f, 0.72f);
    }
    // Distant buildings
    rect(0, 245, WIDTH, 100, 0.12f, 0.13f, 0.23f);
    for (int x=20; x<1000; x+=95) {
        rect(x, 280, 45, 45, 0.92f, 0.57f, 0.22f);
    }
    // Ground
    rect(0, 0, WIDTH, 245, 0.18f, 0.12f, 0.12f);
    rect(0, 0, WIDTH, 28, 0.10f, 0.08f, 0.10f);

    drawTree(70, 240, 0.85f);
    drawTree(930, 240, 0.85f);

    drawStall(65, 110, "TAKOYAKI", 0.68f, 0.12f, 0.13f);
    drawStall(410, 110, "YAKISOBA", 0.12f, 0.42f, 0.30f);
    drawStall(755, 110, "KAKIGORI", 0.20f, 0.28f, 0.62f);

    // Hanging lantern string
    line(100, 575, 900, 575, 0.72f, 0.62f, 0.42f, 2);
    for (int x=150; x<=850; x+=140) drawLantern((float)x, 530, 0.75f);

    // Visitors
    drawPerson(320 + visitorX, 55, 0.85f, walkPhase, true);
    drawPerson(610, 55, 0.75f, 1.0f, false);
    drawPerson(690, 55, 0.75f, 2.0f, false);

    // Rising smoke from a stall
    for (int i=0; i<5; ++i) {
        float sy = 225 + std::fmod(smokeOffset + i*22.0f, 95.0f);
        circle(150 + std::sin(smokeOffset*0.025f+i)*9, sy, 8+i*1.5f,
               0.62f, 0.64f, 0.68f);
    }

    text(20, 625, "HANABI - FESTIVAL STREET  |  Press 0 for Menu",
         GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.60f);
}

void launchFirework(float x, float y) {
    const int count = 65;
    float baseHue = (std::rand()%100)/100.0f;
    for (int i=0; i<count; ++i) {
        float angle = 2.0f*3.1415926f*i/count;
        float speed = 1.5f + (std::rand()%100)/35.0f;
        Particle p;
        p.x=x; p.y=y;
        p.vx=std::cos(angle)*speed;
        p.vy=std::sin(angle)*speed;
        p.r=0.5f+0.5f*std::sin(baseHue*6.28f);
        p.g=0.5f+0.5f*std::sin(baseHue*6.28f+2.1f);
        p.b=0.5f+0.5f*std::sin(baseHue*6.28f+4.2f);
        p.life=1.0f;
        particles.push_back(p);
    }
}

void drawFireworks() {
    rect(0, 0, WIDTH, HEIGHT, 0.015f, 0.025f, 0.10f);
    // Stars
    for (int i=0; i<65; ++i) {
        float x=(i*149%990)+5;
        float y=(i*83%600)+25;
        circle(x,y,1.6f,0.92f,0.91f,0.80f);
    }
    // Moon
    circle(830, 535, 43, 1.0f, 0.88f, 0.55f);
    // Mountains
    color(0.12f,0.19f,0.25f);
    glBegin(GL_TRIANGLES);
    glVertex2f(0,210); glVertex2f(190,440); glVertex2f(380,210);
    glVertex2f(260,210); glVertex2f(520,400); glVertex2f(760,210);
    glVertex2f(620,210); glVertex2f(830,430); glVertex2f(1000,210);
    glEnd();
    // River and bank
    rect(0, 0, WIDTH, 150, 0.04f, 0.16f, 0.25f);
    rect(0, 145, WIDTH, 22, 0.20f, 0.22f, 0.18f);
    for (int i=0; i<12; ++i)
        line(30+i*82, 45+(i%3)*25, 90+i*82, 45+(i%3)*25,
             0.18f,0.42f,0.52f,1.5f);
    // Spectators as silhouettes
    for (int i=0; i<14; ++i) {
        float x=35+i*70;
        circle(x, 180, 9, 0.025f,0.025f,0.04f);
        rect(x-7, 145, 14, 28, 0.025f,0.025f,0.04f);
    }
    // Firework particles
    glPointSize(4.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        glColor4f(p.r, p.g, p.b, std::max(0.0f,p.life));
        glVertex2f(p.x,p.y);
    }
    glEnd();

    // Simple reflected sparkles
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    for (const auto& p : particles) {
        if (p.y > 170 && p.life > 0.25f) {
            glColor4f(p.r*0.45f,p.g*0.45f,p.b*0.45f,p.life*0.35f);
            glVertex2f(p.x, 145.0f-(p.y-170.0f)*0.12f);
        }
    }
    glEnd();

    text(20, 625, "HANABI - FIREWORKS SHOW  |  Click sky to launch | 0 = Menu",
         GLUT_BITMAP_HELVETICA_18, 1.0f, 0.85f, 0.60f);
    text(20, 595, "P = Change pattern (prototype uses circular particle burst)",
         GLUT_BITMAP_HELVETICA_12, 0.8f,0.85f,0.95f);
}

void drawAbout() {
    rect(0,0,WIDTH,HEIGHT,0.035f,0.045f,0.12f);
    text(390,520,"ABOUT HANABI",GLUT_BITMAP_TIMES_ROMAN_24,1.0f,0.75f,0.35f);
    text(210,430,"A 2D animated Japanese summer festival project.",GLUT_BITMAP_HELVETICA_18);
    text(210,390,"Developed using C++, OpenGL and FreeGLUT.",GLUT_BITMAP_HELVETICA_18);
    text(210,350,"Explore a festival street and an interactive fireworks scene.",GLUT_BITMAP_HELVETICA_18);
    text(210,250,"Press 0 to return to the main menu.",GLUT_BITMAP_HELVETICA_18,1.0f,0.8f,0.5f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (currentScene == MENU) drawMenu();
    else if (currentScene == FESTIVAL) drawFestival();
    else if (currentScene == FIREWORKS) drawFireworks();
    else drawAbout();

    glutSwapBuffers();
}

void update(int) {
    if (!paused) {
        smokeOffset += 1.4f;
        walkPhase += 0.16f;
        glow += 0.08f;

        for (auto& p : particles) {
            p.x += p.vx;
            p.y += p.vy;
            p.vy -= 0.045f;
            p.vx *= 0.995f;
            p.life -= 0.009f;
        }
        particles.erase(
            std::remove_if(particles.begin(), particles.end(),
                           [](const Particle& p){ return p.life <= 0.0f; }),
            particles.end()
        );

        // Launch occasional fireworks automatically in the fireworks scene.
        static int tick = 0;
        ++tick;
        if (currentScene == FIREWORKS && tick % 95 == 0) {
            launchFirework(180 + std::rand()%640, 280 + std::rand()%230);
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case '0': currentScene = MENU; break;
        case '1': currentScene = FESTIVAL; break;
        case '2': currentScene = FIREWORKS; break;
        case '3': currentScene = ABOUT; break;
        case '4': std::exit(0); break;
        case ' ': paused = !paused; break;
        case 'r': case 'R': particles.clear(); paused=false; break;
        case 'p': case 'P': fireworkPattern = (fireworkPattern+1)%4; break;
        case 27: std::exit(0); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    if (currentScene == FESTIVAL) {
        if (key == GLUT_KEY_LEFT) visitorX = std::max(-250.0f, visitorX-12.0f);
        if (key == GLUT_KEY_RIGHT) visitorX = std::min(250.0f, visitorX+12.0f);
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return;
    float mx = (float)x;
    float my = HEIGHT - (float)y;

    if (currentScene == MENU) {
        if (my > 380 && my < 435) currentScene = FESTIVAL;
        else if (my > 330 && my < 380) currentScene = FIREWORKS;
        else if (my > 280 && my < 330) currentScene = ABOUT;
        else if (my > 230 && my < 280) std::exit(0);
    } else if (currentScene == FIREWORKS && my > 170) {
        launchFirework(mx, my);
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    std::srand((unsigned int)std::time(nullptr));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(100, 60);
    glutCreateWindow("HANABI - Japanese Summer Festival");

    glClearColor(0.02f,0.03f,0.08f,1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouse);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}
