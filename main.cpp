/*Tetris Project*/
#define _CRT_SECURE_NO_WARNINGS

#include <GLFW/glfw3.h>
#include "TetrisObjects.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <list>
using namespace std;

//Global Variables
double upBorder = 0.9, leftBorder = -0.45, downBorder = -0.9 + SQUARE_SIDE * MULTIPLIER, rightBorder = 0.45 - SQUARE_SIDE * MULTIPLIER;
bool grid[20][10], up, down, left, right, canDouble;
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

bool isActive(object o)
{
    for (int i = 0; i < o.size; i++)
    {
        if (!o.squares[i].isActive)
        {
            return false;
        }
    }
    return true;
}

void deleteObjects() {
    for (auto it = objects.begin(); it != objects.end();) {
        if (!isActive(*it)) 
        {
            // Erase returns an iterator to the next element after the erased one
            it = objects.erase(it);
        }

        else 
        {
            ++it; // Move to the next element
        }
    }
}

void findGridPosition(square s, int* gridX, int* gridY)
{
    double posY = s.x - leftBorder, posX = -s.y + upBorder;
    *gridX = round(posX / (SQUARE_SIDE * MULTIPLIER));
    *gridY = round(posY / (SQUARE_SIDE * MULTIPLIER));
}

square* findSquare(int row, int column)
{
    for (object o : objects)
    {
        for (int i = 0; i < o.size; i++)
        {
            square s = o.squares[i];
            if (s.isActive)
            {
                int gridX, gridY;
                findGridPosition(s, &gridY, &gridX);
                if (gridX == column && gridY == row)
                {
                    return &o.squares[i];
                }
            }
        }
    }
    return NULL;
}

void sortSquares(object* o)
{
    if (o->squares == NULL || o->size <= 0) 
    {
        return;
    }

    for (int i = 0; i < o->size - 1; i++) 
    {
        int minIndex = i;

        for (int j = i + 1; j < o->size; j++) 
        {
            // Primary criterion: isBottom
            if (!o->squares[minIndex].isBottom && o->squares[j].isBottom) {
                minIndex = j;
            }
            // Secondary criterion: y value (only if both are isBottom or neither isBottom)
            else if ((o->squares[minIndex].isBottom == o->squares[j].isBottom) &&
                (o->squares[j].y < o->squares[minIndex].y)) {
                minIndex = j;
            }
        }

        // Swap the current square with the minimum square found
        if (minIndex != i) 
        {
            square temp = o->squares[i];
            o->squares[i] = o->squares[minIndex];
            o->squares[minIndex] = temp;
        }
    }
}

void findBottom(object* o)
{
    square* bottom = &o->squares[0];

    for (int i = 1; i < o->size; i++)
    {
        square* s = &o->squares[i];

        if (s->isActive && s->y < bottom->y)
        {
            bottom = s;
        }
    }

    bottom->isBottom = true;
}

void destroyRow(int row)
{
    for (int i = 0; i < 10; i++)
    {
        square* s = findSquare(row, i);

        if (s != NULL)
        {
            s->isActive = false;
            s->isPlaced = false;
            s->isBottom = false;
            grid[row][i] = false;
        }
    }

    for (object o : objects)
    {
        for (int i = 0; i < o.size; i++)
        {
            square* s = &o.squares[i];

            if (s->isActive)
            {
                int y, x;
                findGridPosition(*s, &y, &x);
                
                if (y < row)
                {
                    grid[y][x] = false;
                    s->y -= SQUARE_SIDE * MULTIPLIER;
                    grid[y + 1][x] = true;
                }
            }
        }

        findBottom(&o);
        sortSquares(&o);
    }
}

bool isFull(int i)
{
    for (int j = 0; j < 10; j++)
    {
        if (!grid[i][j])
        {
            return false;
        }
    }
    return true;
}

void checkRows()
{
    int i = 19;
    while (i >= 0)
    {
        if (isFull(i))
        {
            destroyRow(i);
        }

        i -= isFull(i) ? 0 : 1;
    }

    deleteObjects();
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
                canDouble = !grid[gridY + 2][gridX];
                if (grid[gridY + 1][gridX])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool canRight(object o)
{
    for (int i = 0; i < o.size; i++)
    {
        square s = o.squares[i];
        int gridX, gridY;
        findGridPosition(s, &gridY, &gridX);

        if (gridX >= 9 || grid[gridY + 1][gridX + 1])
        {
            return false;
        }
    }
    return true;
}

bool canLeft(object o)
{
    for (int i = 0; i < o.size; i++)
    {
        square s = o.squares[i];
        int gridX, gridY;
        findGridPosition(s, &gridY, &gridX);

        if (gridX <= 0 || grid[gridY + 1][gridX - 1])
        {
            return false;
        }
    }
    return true;
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

    for (object& o : objects)
    {
        glColor3f(o.r / 255.0, o.g / 255.0, o.b / 255.0);

        for (int i = 0; i < o.size; i++)
        {
            square s = o.squares[i];
            
            if (s.isActive)
            {
                glRectf(s.x, s.y - SQUARE_SIDE * MULTIPLIER, s.x + SQUARE_SIDE * MULTIPLIER, s.y);
            }
        }
    }
}

void display()
{
    displayObjects();
    displayBoard();
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

bool gameOver()
{
    for (int i = 0; i < 10; i++)
    {
        if (grid[0][i])
        {
            return true;
        }
    }
    return false;
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

    currenObject = createRandomObject(objects, 0, upBorder);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window) && !gameOver())
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
                bool cr = canRight(currenObject), cl = canLeft(currenObject);

                for (int i = 0; i < currenObject.size; i++)
                {
                    square* s = &(currenObject.squares[i]);

                    if (s->isActive && !s->isPlaced)
                    {
                        if (s->isBottom)
                        {
                            int gridX, gridY;
                            findGridPosition(currenObject.squares[i], &gridY, &gridX);
                            canDouble = gridY < 18 && canDouble ? true : false;

                            if (gridY < 19)
                            {
                                if (down && canDouble)
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER * 2;
                                }

                                else
                                {
                                    s->y -= SQUARE_SIDE * MULTIPLIER;
                                }

                                if (right && cr)
                                {
                                    s->x += SQUARE_SIDE * MULTIPLIER;
                                }

                                else if (left && cl)
                                {
                                    s->x -= SQUARE_SIDE * MULTIPLIER;
                                }
                            }
                        }

                        else 
                        {
                            int gridX, gridY;
                            findGridPosition(currenObject.squares[i], &gridY, &gridX);

                            if (down && canDouble)
                            {
                                s->y -= SQUARE_SIDE * MULTIPLIER * 2;
                            }

                            else
                            {
                                s->y -= SQUARE_SIDE * MULTIPLIER;
                            }

                            if (right && cr)
                            {
                                s->x += SQUARE_SIDE * MULTIPLIER;
                            }

                            else if (left && cl)
                            {
                                s->x -= SQUARE_SIDE * MULTIPLIER;
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
                checkRows();
                currenObject = createRandomObject(objects, 0, upBorder);
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