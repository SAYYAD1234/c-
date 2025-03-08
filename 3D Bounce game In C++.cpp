// 3D Bounce game In C++
// Install OpenGL and GLUT libraries in your system
// Run the following command in your terminal to install OpenGL and GLUT libraries
// sudo apt-get install freeglut3-dev
// Compile the code using the following command
#include <GL/glut.h>
#include <cmath>

const float BOUNCE_DAMPING = 0.8f;
const float GRAVITY = -0.001f;
const float BALL_RADIUS = 0.5f;
const float ROOM_SIZE = 10.0f;

float ballYVel = 0.0f;
float ballXVel = 0.1f;
float ballZVel = 0.1f;

float ballX = 0.0f, ballY = BALL_RADIUS, ballZ = 0.0f;

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glClearColor(0.0, 0.0, 0.0, 1.0);

    GLfloat lightPos[] = {5.0f, 10.0f, 5.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    GLfloat matSpecular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat matShininess[] = {50.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
}

void drawRoom() {
    glColor3f(0.5, 0.5, 0.5);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f); 
    glutWireCube(ROOM_SIZE * 2);
    glPopMatrix();
}

void drawBall() {
    glPushMatrix();
    glTranslatef(ballX, ballY, ballZ);
    glutSolidSphere(BALL_RADIUS, 32, 32);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(ROOM_SIZE, ROOM_SIZE, ROOM_SIZE, 0, 0, 0, 0, 1, 0);
    drawRoom();
    drawBall();
    glutSwapBuffers();
}

void update(int value) {
    // Apply gravity to Y velocity
    ballYVel += GRAVITY;

    // Update positions
    ballX += ballXVel;
    ballY += ballYVel;
    ballZ += ballZVel;

    // Collision detection with walls
    if (ballX + BALL_RADIUS > ROOM_SIZE) {
        ballX = ROOM_SIZE - BALL_RADIUS;
        ballXVel *= -BOUNCE_DAMPING;
    } else if (ballX - BALL_RADIUS < -ROOM_SIZE) {
        ballX = -ROOM_SIZE + BALL_RADIUS;
        ballXVel *= -BOUNCE_DAMPING;
    }

    if (ballY + BALL_RADIUS > ROOM_SIZE) {
        ballY = ROOM_SIZE - BALL_RADIUS;
        ballYVel *= -BOUNCE_DAMPING;
    } else if (ballY - BALL_RADIUS < -ROOM_SIZE) {
        ballY = -ROOM_SIZE + BALL_RADIUS;
        ballYVel *= -BOUNCE_DAMPING;
        
        // Prevent infinite small bouncing (stop ball if it has almost no velocity)
        if (fabs(ballYVel) < 0.002f) {
            ballYVel = 0.0f;
        }
    }

    if (ballZ + BALL_RADIUS > ROOM_SIZE) {
        ballZ = ROOM_SIZE - BALL_RADIUS;
        ballZVel *= -BOUNCE_DAMPING;
    } else if (ballZ - BALL_RADIUS < -ROOM_SIZE) {
        ballZ = -ROOM_SIZE + BALL_RADIUS;
        ballZVel *= -BOUNCE_DAMPING;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); // 60 FPS approx
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Bounce");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
