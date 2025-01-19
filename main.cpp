/*Tetris Project*/

#define _CRT_SECURE_NO_WARNINGS
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000
#define MULTIPLIER 0.002
#define SQUARE_SIDE 45
#define UPDATE_INTERVAL 0.3

#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <list>
using namespace std;

typedef struct {
    double x, y;
    bool isActive;
    bool isPlaced;
    bool isBottom;
}square;

typedef struct {
    int size;
    square* squares;
}object;

//Global Variables
double upBorder = 0.9, leftBorder = -0.45, downBorder = -0.9 + SQUARE_SIDE * MULTIPLIER, rightBorder = 0.45 - SQUARE_SIDE * MULTIPLIER;
bool grid[20][10], up, down, left, right;
list<object> objects;
object currenObject;

void initGrid()
{
    for (int i = 0; i < 20; i++)
    {
        for (int k = 0; k < 10; k++)
        {
            grid[i][k] = false;
        }
    }
}

void findGridPosition(square s, int* gridX, int* gridY)
{
    double posY = s.x - leftBorder, posX = -s.y + upBorder;
    *gridY = round(posY / (SQUARE_SIDE * MULTIPLIER));
    *gridX = round(posX / (SQUARE_SIDE * MULTIPLIER));
}

bool canMove(object o)
{
    for (int i = 0; i < o.size; i++)
    {
        square cur = o.squares[i];
        if (cur.isBottom)
        {
            if (cur.y <= downBorder + SQUARE_SIDE * MULTIPLIER)
            {
                return false;
            }
            else
            {
                int gridX, gridY;
                findGridPosition(o.squares[i], &gridY, &gridX);
                if (grid[gridY + 1][gridX])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool isPlaced(object o)
{
    for (int i = 0; i < o.size; i++)
    {
        if (!o.squares[i].isPlaced)
        {
            return false;
        }
    }
    return true;
}

void sortSquares(object* o)
{
    if (o->squares == NULL || o->size <= 0) {
        return;
    }

    // Bubble sort implementation
    for (int i = 0; i < o->size - 1; i++) {
        for (int j = 0; j < o->size - i - 1; j++) {
            // Swap if the current square is not isBottom but the next one is
            if (!o->squares[j].isBottom && o->squares[j + 1].isBottom) {
                square temp = o->squares[j];
                o->squares[j] = o->squares[j + 1];
                o->squares[j + 1] = temp;
            }
        }
    }
}

void createObject()
{
    object o;
    o.size = 3;
    o.squares = (square*)malloc(sizeof(square) * o.size);
    for (int i = 0; i < o.size; i++)
    {
        square s;
        s.x = leftBorder;
        s.y = upBorder - i * SQUARE_SIDE * MULTIPLIER;
        s.isActive = true;
        s.isPlaced = false;
        s.isBottom = i == o.size - 1 ? true : false;
        o.squares[i] = s;
    }
    currenObject = o;
    objects.push_back(o);
}

void displayBoard()
{
    glColor3f(1, 1, 1);

    double times, per;

    per = SQUARE_SIDE * MULTIPLIER;
    times = 21 * SQUARE_SIDE * MULTIPLIER;

    for (double i = 0; i < times; i += per)
    {
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2f(leftBorder, upBorder - i);
        glVertex2f(rightBorder + SQUARE_SIDE * MULTIPLIER, upBorder - i);
        glEnd();
    }
    
    per = SQUARE_SIDE * MULTIPLIER;
    times = 10 * SQUARE_SIDE * MULTIPLIER;

    for (double i = 0; i < times; i += per)
    {
        glLineWidth(2);
        glBegin(GL_LINES);
        glVertex2f(leftBorder + i, upBorder);
        glVertex2f(leftBorder + i, downBorder - SQUARE_SIDE * MULTIPLIER);
        glEnd();
    }
}

void displayObjects()
{
    glColor3f(0.3, 0.8, 0.2);
    for (object o : objects)
    {
        for (int i = 0; i < o.size; i++)
        {
            glRectf(o.squares[i].x, o.squares[i].y - SQUARE_SIDE * MULTIPLIER, o.squares[i].x + SQUARE_SIDE * MULTIPLIER, o.squares[i].y);
        }
    }
}

void display()
{
    displayBoard();
    displayObjects();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key) {
            case GLFW_KEY_UP:
                up = true;
                break;
            case GLFW_KEY_DOWN:
                down = true;
                break;
            case GLFW_KEY_LEFT:
                left = true;
                break;
            case GLFW_KEY_RIGHT:
                right = true;
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }

    else
    {
        switch (key) {
            case GLFW_KEY_UP:
                up = false;
                break;
            case GLFW_KEY_DOWN:
                down = false;
                break;
            case GLFW_KEY_LEFT:
                left = false;
                break;
            case GLFW_KEY_RIGHT:
                right = false;
                break;
        }
    }
}

void displayGrid()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 10; j++)
            printf("%d ", grid[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Tetris", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Set the key callback
    glfwSetKeyCallback(window, keyCallback);

    // Timer
    double lastUpdateTime = glfwGetTime();

    createObject();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // Current Time
        double currentTime = glfwGetTime();

        if (currentTime - lastUpdateTime >= UPDATE_INTERVAL)
        {
            if (canMove(currenObject))
            {
                sortSquares(&currenObject);

                for (int i = 0; i < currenObject.size; i++)
                {
                    square* s = &(currenObject.squares[i]);

                    if (!s->isPlaced)
                    {
                        if (s->isBottom)
                        {
                            int gridX, gridY;
                            findGridPosition(currenObject.squares[i], &gridY, &gridX);

                            if (gridY < 19)
                            {
                                if (down && gridY < 18)
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER * 2;
                                }

                                else
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER;
                                }

                                if (right && gridX < 10)
                                {
                                    s->x += SQUARE_SIDE * MULTIPLIER;
                                }

                                else if (left && gridX > 0)
                                {
                                    s->x -= SQUARE_SIDE * MULTIPLIER;
                                }
                            }
                        }

                        else 
                        {
                            int gridX, gridY;
                            findGridPosition(currenObject.squares[i], &gridY, &gridX);

                            if (!currenObject.squares[i - 1].isPlaced && !grid[gridY + 1][gridX])
                            {
                                if (down && !grid[gridY + 2][gridX])
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER * 2;
                                }

                                else
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER;
                                }

                                if (right && s->x < rightBorder)
                                {
                                    s->x += SQUARE_SIDE * MULTIPLIER;
                                }

                                else if (left && s->x > leftBorder)
                                {
                                    s->x -= SQUARE_SIDE * MULTIPLIER;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < currenObject.size; i++)
                {
                    currenObject.squares[i].isPlaced = true;
                    int gridX, gridY;
                    findGridPosition(currenObject.squares[i], &gridY, &gridX);
                    grid[gridY][gridX] = true;
                }
                displayGrid();
                createObject();
            }

            lastUpdateTime = currentTime; // Reset the Timer
        }

        // Redisplay the board
        display();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}