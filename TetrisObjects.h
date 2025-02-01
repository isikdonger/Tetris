#pragma once
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define MULTIPLIER 0.002
#define SQUARE_SIDE 45
#define PI 3.14159265359

#include <cstdlib>
#include <ctime>
#include <list>
using namespace std;

typedef struct object object;

typedef enum {
    LINE_SHAPE,    // I-Tetromino
    SQUARE_SHAPE,  // O-Tetromino
    T_SHAPE,       // T-Tetromino
    J_SHAPE,       // J-Tetromino
    L_SHAPE,       // L-Tetromino
    S_SHAPE,        // S-Tetromino
    Z_SHAPE       // Z-Tetromino
} Shapes;

typedef struct {
    object* parent;
    double x, y;
    bool isActive, isPlaced, isBottom;
}square;

struct object {
    int size, r, g, b;
    square* squares;
    square* pivotSquare;
    Shapes shape;
};

// Functions for creating specific Tetris shapes
object createLineObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    for (int i = 0; i < o.size; i++) {
        o.squares[i] = { &o, startX, startY - i * SQUARE_SIDE * MULTIPLIER, true, false, i == o.size - 1 };
    }
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = LINE_SHAPE;
    o.pivotSquare = &o.squares[2];
    return o;
}

object createSquareObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, false };
    o.squares[1] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY, true, false, false };
    o.squares[2] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = SQUARE_SHAPE;
    o.pivotSquare = &o.squares[3];
    return o;
}

object createTShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, true };
    o.squares[1] = { &o, startX - SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = T_SHAPE;
    o.pivotSquare = &o.squares[2];
    return o;
}

object createJShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, true };
    o.squares[1] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { &o, startX, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { &o, startX - SQUARE_SIDE * MULTIPLIER, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = J_SHAPE;
    o.pivotSquare = &o.squares[1];
    return o;
}

object createLShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, true };
    o.squares[1] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { &o, startX, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[3] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY - 2 * SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = L_SHAPE;
    o.pivotSquare = &o.squares[1];
    return o;
}

object createSShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, true };
    o.squares[1] = { &o, startX - SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, false };
    o.squares[3] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = S_SHAPE;
    o.pivotSquare = &o.squares[2];
    return o;
}

object createZShapeObject(double startX, double startY, int r, int g, int b) {
    object o;
    o.size = 4;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    o.squares[0] = { &o, startX, startY, true, false, true };
    o.squares[1] = { &o, startX + SQUARE_SIDE * MULTIPLIER, startY - SQUARE_SIDE * MULTIPLIER, true, false, true };
    o.squares[2] = { &o, startX, startY - SQUARE_SIDE * MULTIPLIER, true, false, false };
    o.squares[3] = { &o, startX - SQUARE_SIDE * MULTIPLIER, startY, true, false, true };
    o.r = r;
    o.g = g;
    o.b = b;
    o.shape = Z_SHAPE;
    o.pivotSquare = &o.squares[2];
    return o;
}

// Function to create a random object
object createRandomObject(list<object>& objects, double startX, double startY) {
    srand(time(0));
    int randomType = rand() % 7; // Generate a random number between 0 and 4
    int r = rand() % 150 + 50, g = rand() % 150 + 50, b = rand() % 150 + 50; // Generate random colors between 50 and 200

    object newObject;

    switch (randomType) {
        case 0: newObject = createLineObject(startX, startY, r, g, b); break;
        case 1: newObject = createSquareObject(startX, startY, r, g, b); break;
        case 2: newObject = createTShapeObject(startX, startY, r, g, b); break;
        case 3: newObject = createJShapeObject(startX, startY, r, g, b); break;
        case 4: newObject = createLShapeObject(startX, startY, r, g, b); break;
        case 5: newObject = createSShapeObject(startX, startY, r, g, b); break;
        case 6: newObject = createZShapeObject(startX, startY, r, g, b); break;
    }

    objects.push_back(newObject);
    return newObject;
}