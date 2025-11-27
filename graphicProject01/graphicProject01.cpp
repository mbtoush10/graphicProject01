#include<GL/glut.h>
#include<cmath>

void init() {// 50 74 119
	glClearColor(0.196f, 0.290f, 0.467f, 1.0f); // Set background color to sky blue
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
	drawCircle(cx + 40, cy, r);
	drawCircle(cx + 80, cy, r);
}

void sun(float cx, float cy, float r) {
	glColor3ub(255, 229, 0);
	drawCircle(cx, cy, r);
}

void wall() {
	glLineWidth(1.5f);
	glBegin(GL_LINES);

	glColor3ub(153, 102, 51);
	glVertex2f(0, 215); // +20
	glVertex2f(1280, 215);

	glEnd();

	for (int i = 0; i <= 1280; i += 25) {
		glColor3ub(153, 102, 51);
		glBegin(GL_QUADS);
		glVertex2f(10 + i, 200); // 200
		glVertex2f(14 + i, 200);
		glVertex2f(14 + i, 222); //227
		glVertex2f(10 + i, 222);
		glEnd();
	}
}

void street() {
	glColor3ub(51, 51, 51);

	glBegin(GL_QUADS);
	glVertex2f(0, 50);
	glVertex2f(1280, 50);
	glVertex2f(1280, 150);
	glVertex2f(0, 150);
	glEnd();

	for (int i = 0; i <= 1280; i += 60) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2f(i, 95);
		glVertex2f(i + 30, 95);
		glVertex2f(i + 30, 105);
		glVertex2f(i, 105);
		glEnd();
	}
}

void grass() {
	glColor3ub(0 ,204, 76);

	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(1280, 0);
	glVertex2f(1280, 50);
	glVertex2f(0, 50);
	glEnd();
}

void sidewalk() {

	glBegin(GL_QUADS);
	glColor3ub(128, 128, 128);
	glVertex2f(0, 150);
	glVertex2f(1280, 150);
	glVertex2f(1280, 200);
	glVertex2f(0, 200);
	glEnd();


}

void build01(int x, int y) {

	int buildwidth = x + 120;
	int buildheight = y + 130;
	int midX = x + (buildwidth - x) / 2;

	// Building Base
	glBegin(GL_QUADS);

	glColor3ub(204, 204, 229);
	glVertex2d(x, y);
	glVertex2d(buildwidth, y);
	glVertex2d(buildwidth, buildheight);
	glVertex2d(x, buildheight);

	glEnd();

	int glass_x = x + 6;
	int glass_y = (buildheight / 2) + 110;

	// Frame
	glBegin(GL_QUADS);

	glColor3ub(0, 0, 0);
	glVertex2d(glass_x, glass_y);
	glVertex2d(buildwidth - 6, glass_y);
	glVertex2d(buildwidth - 6, buildheight - 9);
	glVertex2d(glass_x, buildheight - 9);

	glEnd();

	// Glass
	glBegin(GL_QUADS);
	glColor3ub(153, 204, 255);

	glVertex2d(glass_x + 3, glass_y + 3);
	glVertex2d(buildwidth - 9, glass_y + 3);
	glVertex2d(buildwidth - 9, buildheight - 12);
	glVertex2d(glass_x + 3, buildheight - 12);

	glEnd();

	// Roof
	glBegin(GL_QUADS);
	glColor3ub(102, 102, 128);

	glVertex2d(x - 4, buildheight - 4);
	glVertex2d(buildwidth + 4, buildheight - 4);
	glVertex2d(buildwidth + 4, buildheight + 9);
	glVertex2d(x - 4, buildheight + 9);

	glEnd();

	// Door
	glBegin(GL_QUADS);
	glColor3ub(51, 25, 0);
	glVertex2d(midX - 10, y);
	glVertex2d(midX + 10, y);
	glVertex2d(midX + 10, y + 60);
	glVertex2d(midX - 10, y + 60);

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

void build03(int x, int y) {
	int buildwidth = x + 120;
	int buildheight = y + 200;


	glBegin(GL_QUADS);
	glColor3ub(128, 161, 204);
	glVertex2d(x, y);
	glVertex2d(buildwidth, y);
	glVertex2d(buildwidth, buildheight);
	glVertex2d(x, buildheight);
	glEnd();

	glBegin(GL_QUADS);
	glColor3ub(93, 128, 153);
	glVertex2d(x - 6, buildheight);
	glVertex2d(buildwidth + 6, buildheight);
	glVertex2d(buildwidth + 6, buildheight + 5);
	glVertex2d(x - 6, buildheight + 5);
	glEnd();

	int win_w = 21;
	int win_h = 20;
	int gap_x = 9;
	int gap_y = 13;
	int start_x = x + 9;
	int start_y = y + 14;
	glColor3ub(204, 204, 153);
	for (int row = 0; row < 6; row++) {
		for (int col = 0; col < 4; col++) {
			int cx = start_x + col * (win_w + gap_x);
			int cy = start_y + row * (win_h + gap_y);

			glBegin(GL_QUADS);
			glVertex2f(cx, cy);
			glVertex2f(cx + win_w, cy);
			glVertex2f(cx + win_w, cy + win_h);
			glVertex2f(cx, cy + win_h);
			glEnd();
		}
	}
}

void build04(float baseX, float baseY)
{
	const int buldWidth = baseX + 100;
	const int buldHeight = baseY + 190;
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
	glVertex2d(midX, buldHeight + 35);
	glEnd();

	// Windows
	float startX = baseX + 17;
	float startY = baseY + 15;

	float spaceX = 22;
	float spaceY = 25;

	glColor3ub(204, 204, 153);
	for (int row = 0; row <= 6; row++)
		for (int col = 0; col < 4; col++) {
			float winX = startX + col * spaceX;
			float winY = startY + row * spaceY;

			drawCircle(winX, winY, 6);
		}
}

void ad(float baseX, float baseY) {

	glColor3ub(255, 255, 255);

	int endX = baseX * 1.325;
	int endY = baseY * 1.223;

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

	ad(current_x - 80, current_y - 55);
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

void car(float baseX, float baseY, int colorBody[]) {

	// Body
	glBegin(GL_POLYGON);
	glColor3ub(colorBody[0], colorBody[1], colorBody[2]);
	glVertex2d(baseX, baseY);
	glVertex2d(baseX + 85, baseY);
	glVertex2d(baseX + 90, baseY + 9);
	glVertex2d(baseX + 79, baseY + 18);
	glVertex2d(baseX + 4, baseY + 18);
	glVertex2d(baseX - 4, baseY + 9);
	glEnd();

	// Window
	glBegin(GL_QUADS);
	glColor3ub(153, 204, 255);
	glVertex2d(baseX + 15, baseY + 13);
	glVertex2d(baseX + 70, baseY + 13);
	glVertex2d(baseX + 63, baseY + 21);
	glVertex2d(baseX + 22, baseY + 21);
	glEnd();

	// Wheels
	glColor3ub(25, 25, 25);
	drawCircle(baseX + 15, baseY + 1, 6);
	drawCircle(baseX + 70, baseY + 1, 6);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	sun(-20, 650, 50);

	cloude(740, 520, 25);
	cloude(50, 570, 25);

	grass();

	street();

	sidewalk();

	float colorTree01[2][3] = {
		{110, 105, 76},
		{38, 158, 76}
	};
	tree(450, 200, colorTree01);
	float colorTree02[2][3] = {
		{102, 51, 0},
		{0, 128, 0}
	};
	tree(700, 200, colorTree02);
	float colorTree03[2][3] = {
		{115, 140, 128},
		{64, 178, 128}
	};
	tree(1025, 200, colorTree03);

	wall();

	tower(575, 150, 50, 20, 17, 1.176);

	trafficLight(50, 275);

	build01(100, 200);
	build02(300, 200);
	build03(875, 200);
	build04(1080, 200);

	int colorCar01[] = { 255, 178, 0 };
	car(400, 104, colorCar01);

	int colorCar02[] = { 0, 0, 255 };
	car(600, 60, colorCar02);

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