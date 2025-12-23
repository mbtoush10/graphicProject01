#include<GL/glut.h>
#include<cmath>
#include<vector>
#include<cstdlib>
#include<ctime>

#define BUILDING_MIN_Y 200

float car01_x = 400.0f;
float car02_x = 600.0f;
float car01_rotation = 0.0f;
float car02_rotation = 0.0f;

float cloud01_x = 740.0f;
float cloud02_x = 50.0f;

// === متغيرات دورة اليوم والليل المعدلة ===
float day_angle = 0.0f;    // العداد الرئيسي (0 لـ 360)
float day_cycle_speed = 0.2f; // سرعة دوران الشمس والقمر

bool force_time = false;

float helicopter_x = -100.0f;
float helicopter_oscillation = 0.0f;
float oscillation_speed = 0.05f;
float oscillation_amount = 5.0f;

float sun_x = 0.0f;
float sun_y = 0.0f;
float moon_x = 0.0f;
float moon_y = 0.0f;
float day_time = 0.0f;
float day_speed = 0.0004f;
bool is_day = true;
bool moon_visible = false;

float car01_speed = 2.0f;
float car02_speed = 1.5f;

struct BuildingBox {
    float x1, y1, x2, y2;
};

std::vector<BuildingBox> buildings;

float bicycle_x = 800.0f;
float bicycle_wheel_rotation = 0.0f;
float rider_leg_angle = 0.0f;
float leg_speed = 5.0f;

bool is_raining = false;
float rain_speed = 5.0f;
int active_rain_drops = 200;
struct RainDrop {
    float x, y;
    float speed;
};
std::vector<RainDrop> rain_drops;
const int NUM_RAIN_DROPS = 500;

bool streetlights_on = false;
float streetlight_flicker_timer = 0.0f;
float streetlight_intensity = 1.0f;

enum TrafficLightState { RED, YELLOW, GREEN };
TrafficLightState traffic_light_state = RED;
float traffic_light_timer = 0.0f;
const float RED_TIME = 3.0f;
const float YELLOW_TIME = 1.0f;
const float GREEN_TIME = 3.0f;

bool animation_paused = false;
bool helicopter_visible = true;

bool zoom_mode = false;
float zoom_x = 640.0f;
float zoom_y = 360.0f;
float zoom_level = 1.0f;

// دالة مساعدة للدمج بين لونين (Interpolation)
float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

void init() {
    glClearColor(0.196f, 0.290f, 0.467f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 720);

    srand(time(NULL));
    rain_drops.resize(NUM_RAIN_DROPS);
    for (int i = 0; i < NUM_RAIN_DROPS; i++) {
        rain_drops[i].x = rand() % 1280;
        rain_drops[i].y = rand() % 720;
        rain_drops[i].speed = 3.0f + (rand() % 100) / 100.0f * 2.0f;
    }
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

void drawRain() {
    if (!is_raining) return;
    glColor3ub(200, 200, 255);
    glBegin(GL_LINES);
    for (int i = 0; i < active_rain_drops; i++) {
        const auto& drop = rain_drops[i];
        glVertex2f(drop.x, drop.y);
        glVertex2f(drop.x - 3.0f, drop.y - 15.0f);
    }
    glEnd();
    glLineWidth(1.0f);
}

void updateRain() {
    if (!is_raining) return;
    for (auto& drop : rain_drops) {
        drop.y -= drop.speed * rain_speed * 0.1f;
        if (drop.y < 0) {
            drop.y = 720;
            drop.x = rand() % 1280;
        }
    }
}

void drawEmptyCircle(float cx, float cy, float radius, int num_segments, float thickness) {
    float innerRadius = radius;
    float outerRadius = radius + thickness;
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i <= num_segments; i++) {
        float angle = 2.0f * 3.1415926f * i / num_segments;
        float x_inner = cx + innerRadius * cos(angle);
        float y_inner = cy + innerRadius * sin(angle);
        float x_outer = cx + outerRadius * cos(angle);
        float y_outer = cy + outerRadius * sin(angle);
        glVertex2f(x_outer, y_outer);
        glVertex2f(x_inner, y_inner);
    }
    glEnd();
}

void cloude(float cx, float cy, float r) {
    glColor3ub(255, 255, 255);
    drawCircle(cx, cy, r);
    drawCircle(cx + 40, cy, r);
    drawCircle(cx + 80, cy, r);
}

void sun(float cx, float cy, float r) {
    glColor3ub(255, 229, 0);
    drawCircle(cx, cy, r);
}

void setWindowColorNightAware() {
    if (is_day) glColor3ub(153, 204, 255);
    else glColor3ub(255, 220, 140);
}

void wall() {
    glLineWidth(1.5f);
    glBegin(GL_LINES);
    glColor3ub(153, 102, 51);
    glVertex2f(0, 210);
    glVertex2f(1280, 210);
    glEnd();
    for (int i = 0; i <= 1280; i += 25) {
        glColor3ub(153, 102, 51);
        glBegin(GL_QUADS);
        glVertex2f(10 + i, 200);
        glVertex2f(14 + i, 200);
        glVertex2f(14 + i, 217);
        glVertex2f(10 + i, 217);
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
    glColor3ub(0, 204, 76);
    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1280, 0);
    glVertex2f(1280, 50);
    glVertex2f(0, 50);
    glEnd();
}

void setWindowColorBuilding34() {
    if (is_day) glColor3ub(204, 204, 153);
    else glColor3ub(255, 220, 140);
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
    glBegin(GL_QUADS);
    glColor3ub(204, 204, 229);
    glVertex2d(x, y);
    glVertex2d(buildwidth, y);
    glVertex2d(buildwidth, buildheight);
    glVertex2d(x, buildheight);
    glEnd();
    int glass_x = x + 6;
    int glass_y = (buildheight / 2) + 110;
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2d(glass_x, glass_y);
    glVertex2d(buildwidth - 6, glass_y);
    glVertex2d(buildwidth - 6, buildheight - 9);
    glVertex2d(glass_x, buildheight - 9);
    glEnd();
    glBegin(GL_QUADS);
    setWindowColorNightAware();
    glVertex2d(glass_x + 3, glass_y + 3);
    glVertex2d(buildwidth - 9, glass_y + 3);
    glVertex2d(buildwidth - 9, buildheight - 12);
    glVertex2d(glass_x + 3, buildheight - 12);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(102, 102, 128);
    glVertex2d(x - 4, buildheight - 4);
    glVertex2d(buildwidth + 4, buildheight - 4);
    glVertex2d(buildwidth + 4, buildheight + 9);
    glVertex2d(x - 4, buildheight + 9);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(51, 25, 0);
    glVertex2d(midX - 10, y);
    glVertex2d(midX + 10, y);
    glVertex2d(midX + 10, y + 60);
    glVertex2d(midX - 10, y + 60);
    glEnd(); // Added missing glEnd here
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
        glColor3ub(128, 76, 25);
        glVertex2d(baseX + xOffSet, height);
        glVertex2d(baseX + xOffSet + boxWidth, height);
        glVertex2d(baseX + xOffSet + boxWidth, height + 18);
        glVertex2d(baseX + xOffSet, height + 18);
        glColor3ub(25, 25, 25);
        glVertex2d(baseX + xOffSet + 2, height);
        glVertex2d(baseX + xOffSet + boxWidth - 2, height);
        glVertex2d(baseX + xOffSet + boxWidth - 2, height + 18);
        glVertex2d(baseX + xOffSet + 2, height + 18);
        setWindowColorNightAware();
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
        glColor3ub(128, 76, 25);
        glVertex2d(baseX + xOffSet, height);
        glVertex2d(baseX + xOffSet + boxWidth, height);
        glVertex2d(baseX + xOffSet + boxWidth, height + 18);
        glVertex2d(baseX + xOffSet, height + 18);
        glColor3ub(25, 25, 25);
        glVertex2d(baseX + xOffSet + 2, height);
        glVertex2d(baseX + xOffSet + boxWidth - 2, height);
        glVertex2d(baseX + xOffSet + boxWidth - 2, height + 18);
        glVertex2d(baseX + xOffSet + 2, height + 18);
        setWindowColorNightAware();
        glVertex2d(baseX + xOffSet + 5, height + 3);
        glVertex2d(baseX + xOffSet + boxWidth - 5, height + 3);
        glVertex2d(baseX + xOffSet + boxWidth - 5, height + 15);
        glVertex2d(baseX + xOffSet + 5, height + 15);
        glEnd();
        xOffSet += (boxWidth + 10);
    }
    glBegin(GL_TRIANGLES);
    glColor3ub(102, 51, 51);
    glVertex2d(baseX - 10, buldHeight);
    glVertex2d(buldWidth + 10, buldHeight);
    glVertex2d(midX, buldHeight + 70);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(51, 25, 0);
    glVertex2d(midX - 10, baseY);
    glVertex2d(midX + 10, baseY);
    glVertex2d(midX + 10, baseY + 60);
    glVertex2d(midX - 10, baseY + 60);
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
    setWindowColorBuilding34();
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

void build04(float baseX, float baseY) {
    const int buldWidth = baseX + 100;
    const int buldHeight = baseY + 190;
    const int midX = baseX + (buldWidth - baseX) / 2;
    glBegin(GL_QUADS);
    glColor3ub(144, 128, 136);
    glVertex2d(baseX, baseY);
    glVertex2d(buldWidth, baseY);
    glVertex2d(buldWidth, buldHeight);
    glVertex2d(baseX, buldHeight);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3ub(110, 110, 119);
    glVertex2d(baseX, buldHeight);
    glVertex2d(buldWidth, buldHeight);
    glVertex2d(midX, buldHeight + 35);
    glEnd();
    float startX = baseX + 17;
    float startY = baseY + 15;
    float spaceX = 22;
    float spaceY = 25;
    setWindowColorBuilding34();
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
    for (int i = 0; i < 11; i++) {
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
    glColor3ub(178, 178, 51);
    float current_x = base_x;
    float current_y = base_y;
    float current_width = base_width;
    for (int i = 0; i < num_layers; i++) {
        float end_x = current_x + current_width;
        glBegin(GL_QUADS);
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

void trafficLight(int baseX, int baseY) {
    if (is_day) glColor3ub(60, 60, 60);
    else glColor3ub(40, 40, 40);
    glColor3ub(40, 40, 40);
    glBegin(GL_QUADS);
    glVertex2f(baseX - 5, baseY);
    glVertex2f(baseX + 15, baseY);
    glVertex2f(baseX + 15, baseY + 50);
    glVertex2f(baseX - 5, baseY + 50);
    glEnd();
    switch (traffic_light_state) {
    case RED:
        glColor3ub(255, 0, 0); drawCircle(baseX + 5, baseY + 37, 5);
        glColor3ub(100, 100, 0); drawCircle(baseX + 5, baseY + 22, 5);
        glColor3ub(0, 100, 0); drawCircle(baseX + 5, baseY + 7, 5);
        break;
    case YELLOW:
        glColor3ub(100, 0, 0); drawCircle(baseX + 5, baseY + 37, 5);
        glColor3ub(255, 255, 0); drawCircle(baseX + 5, baseY + 22, 5);
        glColor3ub(0, 100, 0); drawCircle(baseX + 5, baseY + 7, 5);
        break;
    case GREEN:
        glColor3ub(100, 0, 0); drawCircle(baseX + 5, baseY + 37, 5);
        glColor3ub(100, 100, 0); drawCircle(baseX + 5, baseY + 22, 5);
        glColor3ub(0, 255, 0); drawCircle(baseX + 5, baseY + 7, 5);
        break;
    }
}

void tree(float baseX, float baseY, float colorTree[2][3]) {
    glColor3ub(colorTree[0][0], colorTree[0][1], colorTree[0][2]);
    glBegin(GL_QUADS);
    glVertex2d(baseX, baseY);
    glVertex2d(baseX + 7.2, baseY);
    glVertex2d(baseX + 7.2, baseY + 18);
    glVertex2d(baseX, baseY + 18);
    glEnd();
    glColor3ub(colorTree[1][0], colorTree[1][1], colorTree[1][2]);
    glBegin(GL_TRIANGLES);
    glVertex2d(baseX - 10.8, baseY + 18);
    glVertex2d(baseX + 3.6, baseY + 36);
    glVertex2d(baseX + 18, baseY + 18);
    glVertex2d(baseX - 7.2, baseY + 28.8);
    glVertex2d(baseX + 3.6, baseY + 46.8);
    glVertex2d(baseX + 14.4, baseY + 28.8);
    glVertex2d(baseX - 3.6, baseY + 39.6);
    glVertex2d(baseX + 3.6, baseY + 51);
    glVertex2d(baseX + 10.8, baseY + 39.6);
    glEnd();
}

void car(float baseX, float baseY, int colorBody[], float rotation) {
    glBegin(GL_POLYGON);
    glColor3ub(colorBody[0], colorBody[1], colorBody[2]);
    glVertex2d(baseX, baseY);
    glVertex2d(baseX + 85, baseY);
    glVertex2d(baseX + 90, baseY + 9);
    glVertex2d(baseX + 79, baseY + 18);
    glVertex2d(baseX + 4, baseY + 18);
    glVertex2d(baseX - 4, baseY + 9);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(153, 204, 255);
    glVertex2d(baseX + 15, baseY + 13);
    glVertex2d(baseX + 70, baseY + 13);
    glVertex2d(baseX + 63, baseY + 21);
    glVertex2d(baseX + 22, baseY + 21);
    glEnd();
    glPushMatrix();
    glTranslatef(baseX + 15, baseY + 1, 0);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(-(baseX + 15), -(baseY + 1), 0);
    glColor3ub(25, 25, 25);
    drawCircle(baseX + 15, baseY + 1, 6.5);
    glColor3ub(200, 200, 200);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(baseX + 15, baseY + 1 + 6.5);
    glVertex2f(baseX + 15, baseY + 1 - 6.5);
    glVertex2f(baseX + 15 + 6.5, baseY + 1);
    glVertex2f(baseX + 15 - 6.5, baseY + 1);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(baseX + 70, baseY + 1, 0);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(-(baseX + 70), -(baseY + 1), 0);
    glColor3ub(25, 25, 25);
    drawCircle(baseX + 70, baseY + 1, 6.5);
    glColor3ub(200, 200, 200);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(baseX + 70, baseY + 1 + 6.5);
    glVertex2f(baseX + 70, baseY + 1 - 6.5);
    glVertex2f(baseX + 70 + 6.5, baseY + 1);
    glVertex2f(baseX + 70 - 6.5, baseY + 1);
    glEnd();
    glPopMatrix();
}

// ============================================
// دالة الحركة المعدلة (Animation Loop)
// ============================================
void animate(int value) {
    if (animation_paused) {
        glutTimerFunc(16, animate, 0);
        return;
    }

    // 1. تحديث زاوية اليوم
    if (!force_time) {
        day_angle += day_cycle_speed;
        if (day_angle >= 360.0f) day_angle -= 360.0f;
    }

    // 2. حساب موقع الشمس والقمر
    float rad = day_angle * 3.14159f / 180.0f;
    sun_x = 640.0f - 600.0f * cos(rad);
    sun_y = 100.0f + 500.0f * sin(rad);

    float moon_rad = rad + 3.14159f;
    moon_x = 640.0f - 600.0f * cos(moon_rad);
    moon_y = 100.0f + 500.0f * sin(moon_rad);

    // 3. تحديد الحالات
    is_day = (day_angle >= 0 && day_angle < 180);
    moon_visible = !is_day;
    streetlights_on = !is_day || is_raining;

    // 4. تلوين السماء (بين اللونين المحددين فقط)

    // اللون الصباحي (RGB: 103, 180, 212)
    float col_day[3] = { 0.404f, 0.706f, 0.831f };

    // اللون الليلي (RGB: 50, 74, 119)
    float col_night[3] = { 0.196f, 0.290f, 0.467f };

    // حساب نسبة الخلط (t) بناءً على موقع الشمس
    // هذه المعادلة تجعل القيمة تتأرجح بنعومة بين 0 (ليل دامس) و 1 (ظهر مشمس)
    // sin(rad) يعطي من -1 إلى 1. المعادلة تحولها لتصبح من 0 إلى 1.
    float t = (sin(rad) + 1.0f) / 2.0f;

    // دمج اللونين بناءً على النسبة t
    float r = lerp(col_night[0], col_day[0], t);
    float g = lerp(col_night[1], col_day[1], t);
    float b = lerp(col_night[2], col_day[2], t);

    // تغميق بسيط جداً عند المطر (للحفاظ على جمالية المشهد)
    if (is_raining) {
        r *= 0.8f; g *= 0.8f; b *= 0.85f;
    }

    glClearColor(r, g, b, 1.0f);

    // --- تحديث بقية العناصر ---
    traffic_light_timer += 0.016f;
    switch (traffic_light_state) {
    case RED:
        if (traffic_light_timer >= RED_TIME) { traffic_light_state = GREEN; traffic_light_timer = 0.0f; } break;
    case YELLOW:
        if (traffic_light_timer >= YELLOW_TIME) { traffic_light_state = RED; traffic_light_timer = 0.0f; } break;
    case GREEN:
        if (traffic_light_timer >= GREEN_TIME) { traffic_light_state = YELLOW; traffic_light_timer = 0.0f; } break;
    }

    streetlight_flicker_timer += 0.016f;
    updateRain();

    float speedFactor = (traffic_light_state == GREEN) ? 1.0f : ((traffic_light_state == YELLOW) ? 0.4f : 0.0f);
    car01_x += car01_speed * speedFactor;
    car01_rotation -= 10.0f * speedFactor;
    car02_x -= car02_speed * speedFactor;
    car02_rotation += 10.0f * speedFactor;

    if (car01_x > 1280) car01_x = -100.0f;
    if (car02_x < -100) car02_x = 1280.0f;

    cloud01_x += 0.5f; cloud02_x += 0.5f;
    if (cloud01_x > 1280) cloud01_x = -200.0f;
    if (cloud02_x > 1280) cloud02_x = -200.0f;

    if (helicopter_visible) {
        helicopter_x += 2.0f;
        if (helicopter_x > 1300) helicopter_x = -100.0f;
        helicopter_oscillation += oscillation_speed;
    }

    bicycle_x += 1.0f;
    if (bicycle_x > 1300) bicycle_x = -100.0f;
    bicycle_wheel_rotation -= 8.0f;
    rider_leg_angle += leg_speed;

    glutPostRedisplay();
    glutTimerFunc(16, animate, 0);
}

// ============================================
// دالة التحكم بالكيبورد المعدلة
// ============================================
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'd': // Day Start
        // نضبط الزاوية لبداية الصباح فوراً
        day_angle = 10.0f; // شروق الشمس
        is_day = true;
        break;

    case 'n': // Night Start
        // نضبط الزاوية لبداية الليل فوراً
        day_angle = 190.0f; // بعد الغروب بقليل
        is_day = false;
        break;

    case 'r': is_raining = !is_raining; break;
    case '+': rain_speed += 0.5f; if (rain_speed > 10.0f) rain_speed = 10.0f; break;
    case '-': rain_speed -= 0.5f; if (rain_speed < 0.5f) rain_speed = 0.5f; break;
    case 't': if (is_raining) active_rain_drops = (active_rain_drops + 50) % NUM_RAIN_DROPS; break;
    case 'p': animation_paused = !animation_paused; break;
    case 'h': helicopter_visible = !helicopter_visible; break;
    case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        float oglY = 720 - y;
        for (auto& b : buildings) {
            if (x >= b.x1 && x <= b.x2 && oglY >= b.y1 && oglY <= b.y2) {
                zoom_mode = !zoom_mode;
                zoom_x = x;
                zoom_y = oglY;
                zoom_level = zoom_mode ? 2.0f : 1.0f;
                break;
            }
        }
    }
}

void streetLight(float baseX) {
    glBegin(GL_QUADS);
    glColor3ub(128, 128, 128);
    glVertex2d(baseX, 200); glVertex2d(baseX + 10, 200); glVertex2d(baseX + 10, 217); glVertex2d(baseX, 217);
    glVertex2d(baseX + 1, 217); glVertex2d(baseX + 9, 217); glVertex2d(baseX + 9, 260); glVertex2d(baseX + 1, 260);
    glVertex2d(baseX + 2, 260); glVertex2d(baseX + 8, 260); glVertex2d(baseX + 8, 300); glVertex2d(baseX + 2, 300);
    glEnd();
    float flicker = streetlights_on ? streetlight_intensity : 0.0f;
    if (streetlights_on && is_raining) flicker *= (0.9f + 0.1f * sin(streetlight_flicker_timer * 10.0f));
    glColor3ub(200, 200, 180);
    drawCircle(baseX + 5, 310, 12);
    if (streetlights_on) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4ub(219, 219, 204, 60 * flicker);
        drawCircle(baseX + 5, 310, 20 + 10 * flicker);
        glDisable(GL_BLEND);
    }
}

void helicopter(int x, int y) {
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2d(x + 16.8, y - 17.5);
    glVertex2d(x + 18.55, y - 17.5);
    glVertex2d(x + 18.55, y + 35);
    glVertex2d(x + 16.8, y + 35);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(0, 0, 0);
    glVertex2d(x - 10.5, y + 8.75);
    glVertex2d(x - 10.5, y + 7);
    glVertex2d(x + 45.5, y + 7);
    glVertex2d(x + 45.5, y + 8.75);
    glEnd();
    glBegin(GL_LINES);
    glColor3ub(128, 128, 128);
    glVertex2d(x, y + 10.5);
    glVertex2d(x - 35, y + 10.5);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128, 128, 128);
    glVertex2d(x - 35, y + 10.15);
    glVertex2d(x - 35.7, y + 10.15);
    glVertex2d(x - 35.7, y + 11.9);
    glVertex2d(x - 35, y + 11.9);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(128, 128, 128);
    glVertex2d(x - 33.95, y + 11.9);
    glVertex2d(x - 36.75, y + 11.9);
    glVertex2d(x - 36.75, y + 14.7);
    glVertex2d(x - 33.95, y + 14.7);
    glVertex2d(x - 32.9, y + 14.7);
    glVertex2d(x - 37.45, y + 14.7);
    glVertex2d(x - 37.45, y + 16.1);
    glVertex2d(x - 32.9, y + 16.1);
    glColor3ub(0, 0, 0);
    glVertex2d(x - 32.9, y + 15.4);
    glVertex2d(x - 38.15, y + 15.4);
    glVertex2d(x - 38.15, y + 17.5);
    glVertex2d(x - 32.9, y + 17.5);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3ub(204, 0, 0);
    glVertex2d(x, y);
    glVertex2d(x, y + 17.5);
    glVertex2d(x + 31.5, y + 17.5);
    glVertex2d(x + 38.5, y + 8.75);
    glVertex2d(x + 38.5, y + 5.25);
    glVertex2d(x + 34.3, y);
    glEnd();
    glBegin(GL_QUADS);
    glColor3ub(178, 204, 229);
    glVertex2d(x + 17.5, y + 10.5);
    glVertex2d(x + 17.5, y + 15.75);
    glVertex2d(x + 23.1, y + 15.75);
    glVertex2d(x + 23.1, y + 10.5);
    glEnd();
}

void bicycle(float baseX, float baseY, float wheelRotation, float legAngle) {
    glColor3ub(25, 25, 128);
    glPushMatrix();
    glTranslatef(baseX, baseY, 0);
    glRotatef(wheelRotation, 0, 0, 1);
    glTranslatef(-baseX, -baseY, 0);
    drawEmptyCircle(baseX, baseY, 12.0f, 100, 3.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(baseX, baseY + 12); glVertex2f(baseX, baseY - 12);
    glVertex2f(baseX + 12, baseY); glVertex2f(baseX - 12, baseY);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(baseX + 40, baseY, 0);
    glRotatef(wheelRotation, 0, 0, 1);
    glTranslatef(-(baseX + 40), -baseY, 0);
    drawEmptyCircle(baseX + 40, baseY, 12.0f, 100, 3.0f);
    glBegin(GL_LINES);
    glVertex2f(baseX + 40, baseY + 12); glVertex2f(baseX + 40, baseY - 12);
    glVertex2f(baseX + 40 + 12, baseY); glVertex2f(baseX + 40 - 12, baseY);
    glEnd();
    glPopMatrix();
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex2f(baseX, baseY); glVertex2f(baseX + 40, baseY);
    glVertex2f(baseX, baseY); glVertex2f(baseX + 20, baseY + 20);
    glVertex2f(baseX + 20, baseY + 20); glVertex2f(baseX + 20, baseY + 25);
    glVertex2f(baseX + 40, baseY); glVertex2f(baseX + 20, baseY + 20);
    glVertex2f(baseX + 40, baseY); glVertex2f(baseX + 60, baseY + 15);
    glEnd();
    glColor3ub(255, 204, 153);
    drawCircle(baseX + 25, baseY + 25, 5);
    glColor3ub(0, 0, 0);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2d(baseX + 23, baseY + 27); glVertex2d(baseX + 27, baseY + 27);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3ub(200, 0, 0);
    glVertex2d(baseX + 24, baseY + 30); glVertex2d(baseX + 24, baseY + 15);
    glVertex2d(baseX + 24, baseY + 20); glVertex2f(baseX + 55, baseY + 15);
    glEnd();
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glColor3ub(200, 0, 0);
    float pedal1_angle = legAngle * 3.14159f / 180.0f;
    float pedal1_x = baseX + 20 + 8 * cos(pedal1_angle);
    float pedal1_y = baseY + 10 + 8 * sin(pedal1_angle);
    glVertex2d(baseX + 24, baseY + 15); glVertex2f(pedal1_x, pedal1_y);
    float pedal2_angle = (legAngle + 180) * 3.14159f / 180.0f;
    float pedal2_x = baseX + 20 + 8 * cos(pedal2_angle);
    float pedal2_y = baseY + 10 + 8 * sin(pedal2_angle);
    glVertex2d(baseX + 24, baseY + 15); glVertex2f(pedal2_x, pedal2_y);
    glEnd();
    glColor3ub(51, 25, 0);
    drawCircle(pedal1_x, pedal1_y, 2);
    drawCircle(pedal2_x, pedal2_y, 2);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (zoom_mode) {
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        gluOrtho2D(zoom_x - 640 / zoom_level, zoom_x + 640 / zoom_level, zoom_y - 360 / zoom_level, zoom_y + 360 / zoom_level);
        glMatrixMode(GL_MODELVIEW);
    }
    else {
        glMatrixMode(GL_PROJECTION); glLoadIdentity();
        gluOrtho2D(0, 1280, 0, 720);
        glMatrixMode(GL_MODELVIEW);
    }

    

    // رسم الشمس والقمر فقط إذا كانوا فوق الأفق (-50 للتأكد من الاختفاء السلس)
    if (sun_y > -50) {
        glColor3ub(255, 220, 100);
        drawCircle(sun_x, sun_y, 50);
    }

    if (moon_y > -50) {
        glColor3ub(240, 240, 255);
        drawCircle(moon_x, moon_y, 40);
        glColor3ub(200, 200, 220);
        drawCircle(moon_x + 10, moon_y + 10, 8);
        drawCircle(moon_x - 15, moon_y - 5, 12);
    }

    cloude(cloud01_x, 520, 25);
    cloude(cloud02_x, 570, 25);

    float colorTree01[2][3] = { {110, 105, 76}, {38, 158, 76} };
    tree(450, 200, colorTree01);
    float colorTree02[2][3] = { {102, 51, 0}, {0, 128, 0} };
    tree(775, 200, colorTree02);
    float colorTree03[2][3] = { {115, 140, 128}, {64, 178, 128} };
    tree(1025, 200, colorTree03);

    bicycle(bicycle_x, 203, bicycle_wheel_rotation, rider_leg_angle);
    grass();
    street();
    sidewalk();
    wall();

    tower(575, 150, 50, 20, 17, 1.176);
    trafficLight(60, 275);
    build01(100, 200);
    build02(300, 200);
    build03(875, 200);
    build04(1080, 200);

    streetLight(30);
    streetLight(345);
    streetLight(700);
    streetLight(850);
    streetLight(1200);

    int colorCar01[] = { 255, 178, 0 };
    car(car01_x, 104, colorCar01, car01_rotation);
    int colorCar02[] = { 0, 0, 255 };
    car(car02_x, 60, colorCar02, car02_rotation);

    if (helicopter_visible) {
        float helicopter_y = 600 + sin(helicopter_oscillation) * oscillation_amount;
        helicopter(helicopter_x, helicopter_y);
    }

    buildings.clear();
    buildings.push_back({ 100, 200, 220, 330 });
    buildings.push_back({ 300, 200, 420, 360 });
    buildings.push_back({ 875, 200, 995, 400 });
    buildings.push_back({ 1080, 200, 1180, 390 });

    drawRain();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("The GOAT Project ^_-");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutTimerFunc(0, animate, 0);

    printf("Controls:\n");
    printf("d - Jump to Morning\n");
    printf("n - Jump to Night\n");
    printf("r - Toggle Rain\n");
    printf("+/- - Rain Speed\n");
    printf("p - Pause\n");
    printf("h - Helicopter\n");
    printf("Left Click - Zoom\n");

    glutMainLoop();
    return 0;
}