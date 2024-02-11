// Include necessary libraries
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>
#include <thread>

// Global variables
GLfloat angle = 0.0; // Angle for rotor rotation
int speedSetting = 5; // Default speed setting

// Initialization function for OpenGL
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set white background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-200.0, 200.0, -200.0, 200.0); // Set orthographic viewing frustum
    glMatrixMode(GL_MODELVIEW);
}

// Function to draw the tower
void drawTower() {
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -100.0);
    glVertex2f(10.0, -100.0);
    glVertex2f(10.0, -200.0);
    glVertex2f(-10.0, -200.0);
    glEnd();
}

// Function to draw the nacelle
void drawNacelle() {
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glBegin(GL_QUADS);
    glVertex2f(-25.0, 0.0);
    glVertex2f(25.0, 0.0);
    glVertex2f(20.0, -50.0);
    glVertex2f(-20.0, -50.0);
    glEnd();
}

// Function to draw a blade
void drawBlade() {
    glColor3f(0.0, 0.0, 0.0); // Set color to black
    glBegin(GL_QUADS);
    glVertex2f(-5.0, 0.0);
    glVertex2f(5.0, 0.0);
    glVertex2f(3.0, -120.0);
    glVertex2f(-3.0, -120.0);
    glEnd();
}

// Function to draw the wind turbine
void windTurbine() {
    glPushMatrix();

    // Draw tower
    drawTower();

    // Draw nacelle
    glTranslatef(0.0, -100.0, 0.0);
    drawNacelle();

    // Draw rotor blades
    glTranslatef(0.0, 25.0, 0.0);
    glRotatef(angle, 0.0, 0.0, 1.0);
    drawBlade();

    glRotatef(120, 0.0, 0.0, 1.0);
    drawBlade();

    glRotatef(120, 0.0, 0.0, 1.0);
    drawBlade();

    glPopMatrix();
}

// Display function for OpenGL
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    windTurbine();

    glutSwapBuffers();
}

// Function to read speed setting from file
int readSpeedSetting() {
    int speed = 5; // Default speed setting

    std::ifstream inputFile("speed_setting.txt");
    if (inputFile.is_open()) {
        inputFile >> speed;
        inputFile.close();
    }

    // Clamp speed to range [1, 10]
    speed = std::max(1, std::min(10, speed));

    return speed;
}

// Function to update rotation speed based on speed setting
void updateRotationSpeed() {
    speedSetting = readSpeedSetting();
}

// Function to update angle and redisplay
void update(int value) {
    angle += 0.1f * speedSetting; // Adjust speed based on setting

    if (angle > 360) {
        angle -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(25, update, 0);
}

// Function to periodically update speed setting
void updateSpeedSetting() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5 + (rand() % 6))); // Random sleep between 5-10 seconds
        updateRotationSpeed();
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Wind Turbine");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(25, update, 0);

    // Start thread to periodically update speed setting
    std::thread speedThread(updateSpeedSetting);
    speedThread.detach();

    glutMainLoop();
    return 0;
}
