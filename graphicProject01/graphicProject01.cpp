#include<GL/glut.h>
#include<cmath>

void init() {
	glClearColor(0.4039f, 0.7059f, 0.8314f, 1.0f); // Set background color to sky blue
	glMatrixMode(GL_PROJECTION); // Switch to projection matrix to see the 2D with orthographic projection
	glLoadIdentity(); // Load identity matrix
	gluOrtho2D(0, 1280, 0, 720); // Set the orthographic projection to match the window size
}

void drawCircle(float cx, float cy, float r) {
	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i <= 360; i++) {
		float angle = i * 3.14159f / 180.0f;
		float x = cx + r * cos(angle);
		float y = cy + r * sin(angle);
		glVertex2f(x, y);
	}

	glEnd();
}

void cloude(float cx, float cy, float r) {
	glColor3ub(255,255,255);
	drawCircle(cx, cy, r);
	drawCircle(cx + 50, cy, r);
	drawCircle(cx + 100, cy, r);
}
void sun(float cx, float cy, float r) {
	glColor3ub(255, 229, 0);
	drawCircle(cx, cy, r);
}

void wall() {
	glBegin(GL_LINES);
	glColor3ub(153, 102, 51);
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
void ad(float baseX, float baseY) {

	glColor3ub(255, 255, 255);

	int endX = baseX * 1.3759;
	int endY = baseY * 1.28;

	glBegin(GL_QUADS);

	glVertex2f(baseX, baseY);
	glVertex2f(endX, baseY);
	glVertex2f(endX, endY);
	glVertex2f(baseX, endY);

	glEnd();

	int space = 0;

	for (int i = 0; i < 11; i++)
	{
		glColor3ub(138, 0, 0);
		glBegin(GL_QUADS);

		glVertex2f((baseX + 5) + space, baseY + 30);
		glVertex2f((baseX + 11) + space, baseY + 30);
		glVertex2f((baseX + 11) + space, endY - 30);
		glVertex2f((baseX + 5) + space, endY - 30);

		glEnd();

		space += 15;
	}

	glColor3ub(128, 128, 128);
	glBegin(GL_QUADS);

	glVertex2f(baseX + 83, endY);
	glVertex2f(baseX + 85, endY);
	glVertex2f(baseX + 85, endY + 40);
	glVertex2f(baseX + 83, endY + 40);

	glEnd();
}

void tower(float base_x, float base_y, float base_width, float layer_height, int num_layers, float reduction_per_side) {
	glColor4ub(178, 178, 51, 128);

	float current_x = base_x;
	float current_y = base_y;
	float current_width = base_width;

	for (int i = 0; i < num_layers; i++) {
		float end_x = current_x + current_width;

		glBegin(GL_QUADS); // Tower base 1st

		glVertex2f(current_x, current_y);
		glVertex2f(end_x, current_y);
		glVertex2f(end_x, current_y + layer_height);
		glVertex2f(current_x, current_y + layer_height);

		glEnd();

		current_y += layer_height;
		current_x += reduction_per_side;
		current_width -= (reduction_per_side * 2.0);

		if (current_width <= 0) break;
	}

	ad(current_x - 80, current_y);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	/*cloude(100, 600, 35);
	cloude(600, 650, 35)*/;
	//wall();
	//sun(0, 540, 50);
	//tower(500, 20, 50, 20, 17, 1.176);
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