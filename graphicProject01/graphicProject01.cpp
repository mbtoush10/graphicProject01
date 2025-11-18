#include <GL/glut.h>

void init() {
	glClearColor(0.4039f, 0.7059f, 0.8314f, 1.0f); // Set background color to sky blue
	glMatrixMode(GL_PROJECTION); // Switch to projection matrix to see the 2D with orthographic projection
	glLoadIdentity(); // Load identity matrix
	gluOrtho2D(0, 1280, 0, 720); // Set the orthographic projection to match the window size
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	//////////////////////////////
	// Your drawing functions here  //
	//////////////////////////////

	glFlush();

}

int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutCreateWindow("The GOAT Project ^_-");

	init();
	glutDisplayFunc(display);

	glutMainLoop();
    return 0;
}