#include <GL/glut.h>

void init() {
	glClearColor(0.4039f, 0.7059f, 0.8314f, 1.0f); // Set background color to sky blue
	glMatrixMode(GL_PROJECTION); // Switch to projection matrix to see the 2D with orthographic projection
	glLoadIdentity(); // Load identity matrix
	gluOrtho2D(0, 1280, 0, 720); // Set the orthographic projection to match the window size
}

void wall() {
	glColor3ub(153, 102, 51);
	glBegin(GL_LINES);
	glVertex2f(0, 100);
	glVertex2f(1280, 100);
	glVertex2f(1, 100);
	glVertex2f(1281, 100);
	glVertex2f(2, 100);
	glVertex2f(1282, 100);
	glVertex2f(3, 100);
	glVertex2f(1283, 100);
	glVertex2f(4, 100);
	glVertex2f(1284, 100);
	glEnd();

	for (int i = 0; i <= 1280; i += 25) {
		glColor3ub(153, 102, 51);
		glBegin(GL_QUADS);
		glVertex2f(10 + i, 80);
		glVertex2f(15 + i, 80);
		glVertex2f(15 + i, 107);
		glVertex2f(10 + i, 107);
		glEnd();
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	//wall();

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