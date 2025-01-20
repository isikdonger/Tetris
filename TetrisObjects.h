#pragma once
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define MULTIPLIER 0.002
#define SQUARE_SIDE 45
#define UPDATE_INTERVAL 0.1

#include <cstdlib>
#include <ctime>
#include <list>
using namespace std;

typedef struct {
    double x, y;
    bool isActive;
    bool isPlaced;
    bool isBottom;
}square;

typedef struct {
    int size, r, g, b;
    square* squares;
}object;

// Functions for creating specific Tetris shapes
object createLineObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    for (int i = 0; i < o.size; i++) {
        o.squares[i] = { startX, startY - i * SQUARE_SIDE * MULTIPLIER, true, false, i == o.size - 1 };
    }
    o.r = r;
    o.g = g;
    o.b = b;
    return o;
}

object createSquareObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { startX, startY, true, false, true };
    o.squares[1] = { startX + SQUARE_SIDE * MULTIPLIER, startY, true, false, true };
    o.squares[2] = { startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { startX + SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    return o;
}

object createTShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { startX, startY, true, false, true };
    o.squares[1] = { startX - SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { startX + SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    return o;
}

object createLShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { startX, startY, true, false, true };
    o.squares[1] = { startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { startX, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { startX + SQUARE_SIDE * MULTIPLIER, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    return o;
}

object createZShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { startX, startY, true, false, true };
    o.squares[1] = { startX - SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, false };
    o.squares[3] = { startX + SQUARE_SIDE * MULTIPLIER, startY, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    return o;
}

// Function to create a random object
object createRandomObject(list<object>& objects, double startX, double startY) {
    srand(time(0));
    int randomType = rand() % 5; // Generate a random number between 0 and 4
    int r = rand() % 150 + 50, g = rand() % 150 + 50, b = rand() % 150 + 50; // Generate random colors between 50 and 200
    int colors[3] = { r,g,b };

    object newObject;

    switch (randomType) {
        case 0: newObject = createLineObject(startX, startY, r, g, b); break;
        case 1: newObject = createSquareObject(startX, startY, r, g, b); break;
        case 2: newObject = createTShapeObject(startX, startY, r, g, b); break;
        case 3: newObject = createLShapeObject(startX, startY, r, g, b); break;
        case 4: newObject = createZShapeObject(startX, startY, r, g, b); break;
    }

    objects.push_back(newObject);
    return newObject;
}