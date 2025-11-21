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

void trafficLight(int baseX, int baseY)
{
	glColor3ub(25, 25, 25);
	glBegin(GL_QUADS);

	glVertex2f(baseX, baseY);
	glVertex2f(baseX + 10, baseY);
	glVertex2f(baseX + 10, baseY + 35);
	glVertex2f(baseX, baseY + 35);

	glEnd();

	glColor3ub(254, 0, 0);
	drawCircle(baseX + 5, baseY + 27, 3);

	glColor3ub(51, 51, 0);
	drawCircle(baseX + 5, baseY + 17, 3);

	glColor3ub(0, 51, 0);
	drawCircle(baseX + 5, baseY + 7, 3);
}

void tree(float baseX, float baseY, float colorTree[2][3]) { // colorTree[0] = trunk color, colorTree[1] = leaf color

	glColor3ub(colorTree[0][0], colorTree[0][1], colorTree[0][2]);

	glBegin(GL_QUADS);
	glVertex2f(baseX, baseY);
	glVertex2f(baseX + 12, baseY);
	glVertex2f(baseX + 12, baseY + 30);
	glVertex2f(baseX, baseY + 30);
	glEnd();

	glColor3ub(colorTree[1][0], colorTree[1][1], colorTree[1][2]);

	glBegin(GL_TRIANGLES);
	glVertex2d(baseX - 18, baseY + 30);
	glVertex2d(baseX + 6, baseY + 60);
	glVertex2d(baseX + 30, baseY + 30);

	glVertex2d(baseX - 12, baseY + 48);
	glVertex2d(baseX + 6, baseY + 78);
	glVertex2d(baseX + 24, baseY + 48);

	glVertex2d(baseX - 6, baseY + 66);
	glVertex2d(baseX + 6, baseY + 85);
	glVertex2d(baseX + 18, baseY + 66);

	glEnd();

}
void build02(float baseX, float baseY) {

	const int buldWidth = baseX + 120;
	const int buldHeight = baseY + 160;
	const int midX = baseX + (buldWidth - baseX) / 2;

	glBegin(GL_QUADS);

	glColor3ub(178, 128, 76);
	glVertex2d(baseX, baseY);
	glVertex2d(buldWidth, baseY);
	glVertex2d(buldWidth, buldHeight);
	glVertex2d(baseX, buldHeight);

	glEnd();

	int xOffSet = 10;
	const int boxWidth = 18;
	int height = baseY + 110;

	for (int i = 0; i < 3; i++) {
		glBegin(GL_QUADS);

		// Outer Box
		glColor3ub(128, 76, 25);
		glVertex2d(baseX + xOffSet, height);
		glVertex2d(baseX + xOffSet + boxWidth, height);
		glVertex2d(baseX + xOffSet + boxWidth, height + 18);
		glVertex2d(baseX + xOffSet, height + 18);

		// Frame
		glColor3ub(25, 25, 25);
		glVertex2d(baseX + xOffSet + 2, height);
		glVertex2d(baseX + xOffSet + boxWidth - 2, height);
		glVertex2d(baseX + xOffSet + boxWidth - 2, height + 18);
		glVertex2d(baseX + xOffSet + 2, height + 18);

		// Glass
		glColor3ub(204, 229, 255);
		glVertex2d(baseX + xOffSet + 5, height + 3);
		glVertex2d(baseX + xOffSet + boxWidth - 5, height + 3);
		glVertex2d(baseX + xOffSet + boxWidth - 5, height + 15);
		glVertex2d(baseX + xOffSet + 5, height + 15);

		glEnd();

		xOffSet += (boxWidth + 10);
	}
	xOffSet = 10;
	height -= 30;
	for (int i = 0; i < 3; i++) {
		glBegin(GL_QUADS);

		// Outer Box
		glColor3ub(128, 76, 25);
		glVertex2d(baseX + xOffSet, height);
		glVertex2d(baseX + xOffSet + boxWidth, height);
		glVertex2d(baseX + xOffSet + boxWidth, height + 18);
		glVertex2d(baseX + xOffSet, height + 18);

		// Frame
		glColor3ub(25, 25, 25);
		glVertex2d(baseX + xOffSet + 2, height);
		glVertex2d(baseX + xOffSet + boxWidth - 2, height);
		glVertex2d(baseX + xOffSet + boxWidth - 2, height + 18);
		glVertex2d(baseX + xOffSet + 2, height + 18);

		// Glass
		glColor3ub(204, 229, 255);
		glVertex2d(baseX + xOffSet + 5, height + 3);
		glVertex2d(baseX + xOffSet + boxWidth - 5, height + 3);
		glVertex2d(baseX + xOffSet + boxWidth - 5, height + 15);
		glVertex2d(baseX + xOffSet + 5, height + 15);
		glEnd();


		xOffSet += (boxWidth + 10);
	}

	// Roof
	glBegin(GL_TRIANGLES);
	glColor3ub(102, 51, 51);
	glVertex2d(baseX - 10, buldHeight);
	glVertex2d(buldWidth + 10, buldHeight);
	glVertex2d(midX, buldHeight + 70);

	glEnd();

	// Door
	glBegin(GL_QUADS);
	glColor3ub(51, 25, 0);
	glVertex2d(midX - 10, baseY);
	glVertex2d(midX + 10, baseY);
	glVertex2d(midX + 10, baseY + 60);
	glVertex2d(midX - 10, baseY + 60);

	// Door Handle
	glColor3ub(204, 153, 51);
	glVertex2d(midX + 6, baseY + 25);
	glVertex2d(midX + 9, baseY + 25);
	glVertex2d(midX + 9, baseY + 30);
	glVertex2d(midX + 6, baseY + 30);

	glEnd();

}

void build04(float baseX, float baseY)
{
	const int buldWidth = baseX + 100;
	const int buldHeight = baseY + 175;
	const int midX = baseX + (buldWidth - baseX) / 2;
	// Building Base
	glBegin(GL_QUADS);
	glColor3ub(144, 128, 136);
	glVertex2d(baseX, baseY);
	glVertex2d(buldWidth, baseY);
	glVertex2d(buldWidth, buldHeight);
	glVertex2d(baseX, buldHeight);
	glEnd();

	// Roof
	glBegin(GL_TRIANGLES);
	glColor3ub(110, 110, 119);
	glVertex2d(baseX, buldHeight);
	glVertex2d(buldWidth, buldHeight);
	glVertex2d(midX, buldHeight + 40);
	glEnd();

	// Windows
	float startX = baseX + 17;
	float startY = baseY + 20;

	float spaceX = 22;
	float spaceY = 25;

	glColor3ub(204, 204, 153);
	for (int row = 0; row < 6; row++)
		for (int col = 0; col < 4; col++) {
			float winX = startX + col * spaceX;
			float winY = startY + row * spaceY;

			drawCircle(winX, winY, 6);
		}

}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	/*cloude(100, 600, 35);
	cloude(600, 650, 35)*/;
	//wall();
	//sun(0, 540, 50);
	//tower(500, 20, 50, 20, 17, 1.176);
	//trafficLight(200, 100);

	//float colorTree01[2][3] =
	//{
	//	{110, 105, 76},
	//	{38, 158, 76}
	//};
	//tree(1000, 100, colorTree01);

	//float colorTree02[2][3] =
	//{
	//	{102, 51, 0},
	//	{0, 128, 0}
	//};
	//tree(800, 100, colorTree02);

	//float colorTree03[2][3] =
	//{
	//	{115, 140, 128},
	//	{64, 178, 128}
	//};
	//tree(700, 300, colorTree03);

	//build02(300, 100);
	build04(300, 100);

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