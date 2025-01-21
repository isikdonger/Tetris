# Tetris

To set up the glfw3 library follow these commands:

1-Open project properties.

2-Go to C/C++ General settings and click additional include directories and click edit. Click the folder icon (new line), click three dots at the end of the line and select the include directory inside the glfw3 folder.

3-Go to Linker General settings and click additional include directories and click edit. Click the folder icon (new line), click three dots at the end of the line and select the lib folder according to your Visual Studio version under the glfw3 folder.

4-Go to Linker input settings and click additional dependencies and click edit. And enter these lines:
glfw3.lib
opengl32.lib
user32.lib
gdi32.lib
shell32.lib