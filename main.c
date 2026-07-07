#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define size 800

int grid[size][size];
const int X_MIN = -3, X_MAX = 3, Y_MIN = 0, Y_MAX = 10;

typedef struct {
    double a, b, c, d, e, f, p;
} Transformation;

typedef struct {
    double x, y;
} Point;

typedef struct {
    int x, y;
} Pixel;

int chooseTf(const double r, const Transformation *tfs, const int length) {
    double sum = 0;
    for (int i = 0; i < length; i++) {
        sum += tfs[i].p;
        if (r <= sum) {
            return i;
        }
    }
    return -1;
}

void transform(Point* pt, Transformation tf) {
    double x_new = tf.a * pt->x + tf.b * pt->y + tf.e;
    double y_new = tf.c * pt->x + tf.d * pt->y + tf.f;
    pt->x = x_new;
    pt->y = y_new;
}

void save_ppm() {
    char value[15];
    char filename[20];
    sprintf(filename, "../fern.ppm");
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("No file found.");
        return;
    }
    fprintf(f, "P3\n%d %d\n255\n", size, size);
    char line[9610];

    for (int i = 0; i < size; i++) {
        line[0] = '\0';
        for (int j = 0; j < size; j++) {
            switch (grid[i][j]) {
                case 1:
                    sprintf(value, "33 21 2 ");
                    break;
                case 2:
                    sprintf(value, "36 77 31 ");
                    break;
                case 3:
                    sprintf(value, "26 82 43 ");
                    break;
                case 4:
                    sprintf(value, "4 41 37 ");
                    break;
                default:
                    sprintf(value, "200 200 200 ");
            }
            strcat(line, value);
        }
        fprintf(f, "%s\n", line);
    }
    fclose(f);
}

void draw(const Point* pt, const int *tfIndex) {
    double relative_x = (pt->x - X_MIN) / (X_MAX - X_MIN);
    double relative_y = (pt->y - Y_MIN) / (Y_MAX - Y_MIN);
    Pixel px = {
        (int) (relative_x * (size - 1)),
        (int) ((1 - relative_y) * (size - 1))
    };
    if (px.y >= 0 && px.y < size &&
        px.x >= 0 && px.x < size) {
        grid[px.y][px.x] = *tfIndex + 1;
    }
}

int main(void) {
    srand(time(NULL));
    printf("Barnsley fern is starting...\n");

    const Transformation tfs[] = {
        {0, 0, 0, 0.16, 0, 0, 0.01},
        {0.85, 0.04, -0.04, 0.85, 0, 1.6, 0.85},
        {0.2, -0.26, 0.23, 0.22, 0, 1.6, 0.07},
        {-0.15, 0.28, 0.26, 0.24, 0, 0.44, 0.07}
    };

    Point pt = {0, 0};

    for (int i = 0; i < 500000; i++) {
        double r = rand() / (double)RAND_MAX;
        int tfIndex = chooseTf(r, tfs, sizeof(tfs) / sizeof(tfs[0]));
        transform(&pt, tfs[tfIndex]);
        draw(&pt, &tfIndex);
    }
    save_ppm();

    return 0;
}
