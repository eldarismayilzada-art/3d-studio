/*
 * glcheckpoint.cpp
 *
 * Setup checkpoint for the graphics course.
 *
 * Purpose:
 *   Verifies that your compile environment can build and run a program
 *   that uses GLFW (window and context creation) and glad (OpenGL
 *   function loading). It opens a window, creates an OpenGL context,
 *   loads the OpenGL functions, and prints the driver information.
 *   It does not render anything.
 *
 *   The GLFW error callback is installed before glfwInit(), so any
 *   problem is reported with GLFW's own error code and description.
 *
 * Usage:
 *   ./glcheckpoint                Normal run. Should print the OpenGL
 *                                 version, renderer and vendor, then
 *                                 "All checks passed." and exit with
 *                                 status 0.
 *   ./glcheckpoint --bad-version  Deliberate failure demo: requests
 *                                 OpenGL 4.9, which does not exist, so
 *                                 context creation fails and the error
 *                                 callback fires.
 *   ./glcheckpoint --help         Show usage and exit.
 *
 * Expected failures:
 *   GLFW_PLATFORM_ERROR  No display available (e.g. $DISPLAY is unset
 *                        on Linux, or you are connected over plain SSH).
 *   GLFW_VERSION_UNAVAILABLE
 *                        The requested OpenGL version is not supported.
 *                        This is what you get with --bad-version.
 *
 * Exit status: 0 on success, 1 on any check failure, 2 on bad
 *              command line arguments.
 *
 * Build: needs C++11, GLFW (3.2 or later) and glad.
 *
 * Author: Niclas Borlin, niclas.borlin@cs.umu.se with help from Claude AI.
 *
 * Version history:
 * - v1.0 2026-10-17 First published version.
 */

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// The OpenGL version a normal run asks for. Change these if your course
// needs a different version (macOS, for example, only goes up to 4.1).
static const int GL_VERSION_MAJOR_REQUESTED = 4;
static const int GL_VERSION_MINOR_REQUESTED = 5;

// The deliberately impossible version used by --bad-version.
static const int GL_VERSION_MAJOR_BAD = 4;
static const int GL_VERSION_MINOR_BAD = 9;

static void usage(const std::string& prog)
{
    std::cout
        << "Usage: " << prog << " [--bad-version | --help]\n"
        << "\n"
        << "Checks that GLFW and glad work on this computer.\n"
        << "\n"
        << "  (no arguments)  Open a window, load OpenGL and print the version.\n"
        << "  --bad-version   Request OpenGL " << GL_VERSION_MAJOR_BAD << "."
        << GL_VERSION_MINOR_BAD << " on purpose, to show what a\n"
        << "                  GLFW error looks like. This is expected to fail.\n"
        << "  --help          Show this message.\n"
        << "\n"
        << "If you see GLFW_PLATFORM_ERROR, check that a display is available\n"
        << "(on Linux, make sure $DISPLAY is set).\n"
        << "If you see a version error, your driver does not support the\n"
        << "requested OpenGL version.\n";
}

#include <iostream>
#include <string>

static std::string compilerInfo()
{
#if defined(__clang__)
    return "Clang " + std::to_string(__clang_major__) + "." +
           std::to_string(__clang_minor__) + "." +
           std::to_string(__clang_patchlevel__);
#elif defined(__GNUC__)
    // MinGW also defines __GNUC__
    return "GCC " + std::to_string(__GNUC__) + "." +
           std::to_string(__GNUC_MINOR__) + "." +
           std::to_string(__GNUC_PATCHLEVEL__);
#elif defined(_MSC_VER)
    return "MSVC " + std::to_string(_MSC_VER);
#else
    return "unknown compiler";
#endif
}

static std::string osInfo()
{
#if defined(__MINGW32__) || defined(__MINGW64__)
    return "Windows (MinGW)";
#elif defined(_WIN32)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "unknown OS";
#endif
}

static std::string archInfo()
{
    return std::to_string(sizeof(void *) * 8) + "-bit";
}

static std::string cppStandard()
{
    // __cplusplus: 201103L = C++11, 201402L = C++14, 201703L = C++17, ...
    switch (__cplusplus) {
    case 201103L: return "C++11";
    case 201402L: return "C++14";
    case 201703L: return "C++17";
    case 202002L: return "C++20";
    default:      return "C++ (__cplusplus = " + std::to_string(__cplusplus) + ")";
    }
}

// Convert an unsigned value to hexadecimal, including the 0x prefix
// and zero-padded to the full width of the type.
template <typename T>
static std::string int_to_hex(T i)
{
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2)
           << std::hex << i;
    return stream.str();
}

// Translate a GLFW error code to its symbolic name. Codes added in
// newer GLFW versions are guarded with #ifdef so the file also compiles
// with older GLFW releases.
static std::string GLFWErrorString(int error)
{
    switch (error) {
    // Use 0 instead of GLFW_NO_ERROR to deal with pre-v3.3 GLFW
    case 0:                          return "GLFW_NO_ERROR";
    case GLFW_NOT_INITIALIZED:       return "GLFW_NOT_INITIALIZED";
    case GLFW_NO_CURRENT_CONTEXT:    return "GLFW_NO_CURRENT_CONTEXT";
    case GLFW_INVALID_ENUM:          return "GLFW_INVALID_ENUM";
    case GLFW_INVALID_VALUE:         return "GLFW_INVALID_VALUE";
    case GLFW_OUT_OF_MEMORY:         return "GLFW_OUT_OF_MEMORY";
    case GLFW_API_UNAVAILABLE:       return "GLFW_API_UNAVAILABLE";
    case GLFW_VERSION_UNAVAILABLE:   return "GLFW_VERSION_UNAVAILABLE";
    case GLFW_PLATFORM_ERROR:        return "GLFW_PLATFORM_ERROR";
    case GLFW_FORMAT_UNAVAILABLE:    return "GLFW_FORMAT_UNAVAILABLE";
#ifdef GLFW_NO_WINDOW_CONTEXT
    case GLFW_NO_WINDOW_CONTEXT:     return "GLFW_NO_WINDOW_CONTEXT";
#endif
#ifdef GLFW_CURSOR_UNAVAILABLE
    case GLFW_CURSOR_UNAVAILABLE:    return "GLFW_CURSOR_UNAVAILABLE";
#endif
#ifdef GLFW_FEATURE_UNAVAILABLE
    case GLFW_FEATURE_UNAVAILABLE:   return "GLFW_FEATURE_UNAVAILABLE";
#endif
#ifdef GLFW_FEATURE_UNIMPLEMENTED
    case GLFW_FEATURE_UNIMPLEMENTED: return "GLFW_FEATURE_UNIMPLEMENTED";
#endif
#ifdef GLFW_PLATFORM_UNAVAILABLE
    case GLFW_PLATFORM_UNAVAILABLE:  return "GLFW_PLATFORM_UNAVAILABLE";
#endif
    default:
        return "Error " + int_to_hex(static_cast<unsigned int>(error));
    }
}

static void GLFW_error_callback(int error, const char* description)
{
    // Start with a newline, since a progress message ("Creating
    // window... ") is probably still on the current line.
    std::cerr << "\nGLFW Error " << int_to_hex(static_cast<unsigned int>(error))
              << " (" << GLFWErrorString(error) << "): " << description
              << std::endl;
}

static std::string glfwVersionInfo()
{
    // Return the run-time GLFW version numbers and the compile-time
    // version string
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    return "GLFW version: " + std::to_string(major) + "."
        + std::to_string(minor) + "." + std::to_string(rev)
        + " (" + std::string(glfwGetVersionString()) + ")";
}

int main(int argc, char* argv[])
{
    const std::string prog = (argc > 0) ? argv[0] : "glcheckpoint";
    bool bad_version = false;

    std::cout << "OS:            " << osInfo() << " " << archInfo() << std::endl
	      << "Compiler:      " << compilerInfo() << std::endl
	      << "Language:      " << cppStandard() << std::endl << std::endl;
    
    // Check command line arguments
    if (argc > 1) {
        const std::string arg = argv[1];
        if (arg == "--help") {
            usage(prog);
            return 0;
        } else if (arg == "--bad-version") {
            bad_version = true;
        } else {
            std::cout << "Unknown argument: " << arg << "\n\n";
            usage(prog);
            return 2;
        }
    }

    // Set the GLFW error callback first, so we get any GLFW errors
    // through it
    glfwSetErrorCallback(GLFW_error_callback);

    // Print the GLFW version
    std::cout << glfwVersionInfo() << std::endl;

    std::cout << "Initializing GLFW... " << std::flush;
    if (!glfwInit()) {
        std::cerr << "FAILED: could not initialize GLFW." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "done." << std::endl;

    if (bad_version) {
        // Ask for a too high OpenGL version to force an error
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR_BAD);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR_BAD);
    } else {
        // This version should be available
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR_REQUESTED);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR_REQUESTED);
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // Create a window
    std::cout << "Creating window... " << std::flush;
    GLFWwindow* window = glfwCreateWindow(800, 600, "glcheckpoint",
                                          nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "FAILED: could not create a window." << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    std::cout << "done." << std::endl;

    // glad needs a current OpenGL context
    glfwMakeContextCurrent(window);

    std::cout << "Setting up glad... " << std::flush;
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "FAILED: could not initialize glad." << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    std::cout << "done." << std::endl;

    // Print driver information. Students: please include these three
    // lines if you ask for help.
    std::cout
        << "OpenGL version:  "
        << reinterpret_cast<const char*>(glGetString(GL_VERSION)) << std::endl
        << "OpenGL renderer: "
        << reinterpret_cast<const char*>(glGetString(GL_RENDERER)) << std::endl
        << "OpenGL vendor:   "
        << reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << std::endl;

    // Clean up
    std::cout << "Closing GLFW window... " << std::flush;
    glfwDestroyWindow(window);
    std::cout << "done." << std::endl;

    std::cout << "Terminating GLFW... " << std::flush;
    glfwTerminate();
    std::cout << "done." << std::endl;

    std::cout << "All checks passed." << std::endl;

    return 0;
}
