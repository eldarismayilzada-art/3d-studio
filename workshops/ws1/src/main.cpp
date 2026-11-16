/*
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Adapted from code by Stefan Johansson, stefanj@cs.umu.se
 */

#include <iostream>
#include <string>
#include <filesystem>
#include "openglwindow.h"
#include "shaderprogram.h"
#include "geometryrender.h"

namespace fs = std::filesystem;

int main(int argc, const char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <shader-directory>\n"
                  << "Example: " << argv[0] << " workshop1/shaders\n";
        return 1;
    }

    const fs::path shaderDir = argv[1];

    std::error_code ec;   // the ec overloads return false instead of throwing
    if (!fs::is_directory(shaderDir, ec)) {
        std::cerr << "Error: '" << shaderDir.string()
                  << "' does not exist or is not a directory.\n";
        return 1;
    }
    
    // Create the GLFW global context
    GlfwContext GLFW(true);

    std::cout << GLFW_version_string() << std::endl;

    GeometryRender win(GLFW, shaderDir, "Test", 800, 600, true);
    try {
	win.initialize();
	// Set the background color to a light gray to see any
	// drawings of black primitives
	win.setBackgroundColor(Vec3(0.2,0.2,0.2));
	win.start();
    } catch (const std::exception& e) {
	// Well-behaved exception
        std::cerr << "Exception: what(): " << e.what() << std::endl;
	std::cout << "Abnormal exit." << std::endl;
	return EXIT_FAILURE;
    } catch (...) {
	// Ill-behaved exception
        std::cerr << "Caught unknown exception" << std::endl;
	std::cout << "Abnormal exit." << std::endl;
	return EXIT_FAILURE;
    }

    std::cout << "Normal exit." << std::endl;
    return EXIT_SUCCESS;
}
