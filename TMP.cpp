#include <GL/glut.h> //keyboard and mouse imput library
#include <math.h> 

#define FPS 60 //anti-sonic
#define TO_RADIANS 3.14/180.0

//using for testing
const int width = 16 * 50;
const int height = 9 * 50;

float pitch = 0.0, yaw = 0.0;
float camX = 0.0, camZ = 0.0;

void display();
void reshape(int w, int h);
void timer(int);
void passive_motion(int, int);
void camera();
void keyboard(unsigned char key, int x, int y);
void keyboard_up(unsigned char key, int x, int y);

struct Motion
{
    bool Forward, Backward, Left, Right;
};

Motion motion = { false,false,false,false };

void init()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glutWarpPointer(width / 2, height / 2);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("Projectile Motion - 3D Simulation");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive_motion);
    glutTimerFunc(0, timer, 0);    
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboard_up);

    glutMainLoop();
    return 0;
}

/* floor for testing */

void draw()
{
    glEnable(GL_TEXTURE_2D);
    GLuint texture;
    glGenTextures(1, &texture);

    unsigned char texture_data[2][2][4] =
    {
        0,0,0,255,  255,255,255,255,
        255,255,255,255,    0,0,0,255
    };

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);

    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 0.0);  glVertex3f(-50.0, -5.0, -50.0);
    glTexCoord2f(25.0, 0.0);  glVertex3f(50.0, -5.0, -50.0);
    glTexCoord2f(25.0, 25.0);  glVertex3f(50.0, -5.0, 50.0);
    glTexCoord2f(0.0, 25.0);  glVertex3f(-50.0, -5.0, 50.0);

    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera();
    draw();

    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 16.0 / 9.0, 1, 75);
    glMatrixMode(GL_MODELVIEW);

}


/*
* Anti sonic mode
* Sets constant movement 
*/
void timer(int)
{
    glutPostRedisplay();
    glutWarpPointer(width / 2, height / 2);
    glutTimerFunc(1000 / FPS, timer, 0);
}

void passive_motion(int x, int y)
{
    /* two variables to store X and Y coordinates, as observed from the center
      of the window
    */
    int dev_x, dev_y;
    dev_x = (width / 2) - x;
    dev_y = (height / 2) - y;

    /* apply the changes to pitch and yaw*/
    yaw += (float)dev_x / 10.0;
    pitch += (float)dev_y / 10.0;
}
/*
* Motion which direction the camera should "change" to
* Adding better comments later, soz 
*/
void camera()
{

    if (motion.Forward)
    {
        camX += cos((yaw + 90) * TO_RADIANS) / 5.0;
        camZ -= sin((yaw + 90) * TO_RADIANS) / 5.0;
    }
    if (motion.Backward)
    {
        camX += cos((yaw + 90 + 180) * TO_RADIANS) / 5.0;
        camZ -= sin((yaw + 90 + 180) * TO_RADIANS) / 5.0;
    }
    if (motion.Left)
    {
        camX += cos((yaw + 90 + 90) * TO_RADIANS) / 5.0;
        camZ -= sin((yaw + 90 + 90) * TO_RADIANS) / 5.0;
    }
    if (motion.Right)
    {
        camX += cos((yaw + 90 - 90) * TO_RADIANS) / 5.0;
        camZ -= sin((yaw + 90 - 90) * TO_RADIANS) / 5.0;
    }

    /*limit the values of pitch
      between -60 and 70
    */
    if (pitch >= 70)
        pitch = 70;
    if (pitch <= -60)
        pitch = -60;

    glRotatef(-pitch, 1.0, 0.0, 0.0); // Along X axis
    glRotatef(-yaw, 0.0, 1.0, 0.0);    //Along Y axis

    glTranslatef(-camX, 0.0, -camZ);
}
/*
* Keyboard imput stuff for camera rotation, maybe add to pacman movement later
*/
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'W':
    case 'w':
        motion.Forward = true;
        break;
    case 'A':
    case 'a':
        motion.Left = true;
        break;
    case 'S':
    case 's':
        motion.Backward = true;
        break;
    case 'D':
    case 'd':
        motion.Right = true;
        break;
    }
}
void keyboard_up(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'W':
    case 'w':
        motion.Forward = false;
        break;
    case 'A':
    case 'a':
        motion.Left = false;
        break;
    case 'S':
    case 's':
        motion.Backward = false;
        break;
    case 'D':
    case 'd':
        motion.Right = false;
        break;
    }
}