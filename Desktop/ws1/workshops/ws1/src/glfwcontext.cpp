/*
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 *
 * Code to handle all global GLFW settings
 */

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "glfwcontext.h"
#include "3dstudio.h"

using namespace std;

// Return a string version of known GLFW error codes, or just the
// numeric value
string GLFW_error_string(int error)
{
    // Known GLFW error codes
    const int errorCodeBase = 0x00010001;
    const char *glfwErrorStrings[] = {
	STR(GLFW_NOT_INITIALIZED), // 0x00010001
	STR(GLFW_NO_CURRENT_CONTEXT),
	STR(GLFW_INVALID_ENUM),
	STR(GLFW_INVALID_VALUE),
	STR(GLFW_OUT_OF_MEMORY),
	STR(GLFW_API_UNAVAILABLE),
	STR(GLFW_VERSION_UNAVAILABLE),
	STR(GLFW_PLATFORM_ERROR),
	STR(GLFW_FORMAT_UNAVAILABLE),
	STR(GLFW_NO_WINDOW_CONTEXT),
	STR(GLFW_CURSOR_UNAVAILABLE),
	STR(GLFW_FEATURE_UNAVAILABLE),
	STR(GLFW_FEATURE_UNIMPLEMENTED),
	STR(GLFW_PLATFORM_UNAVAILABLE) // 0x0001000e
    };

    // Deal with no error separately
    if (error == 0) {
	// Use 0 instead of GLFW_NO_ERROR to deal with pre-v3.3 GLFW
	return STR(GLFW_NO_ERROR);
    }
    // Next, check if it is a known error constant
    if (error >= errorCodeBase &&
	error < errorCodeBase + (int)ARRAY_SIZE(glfwErrorStrings)) {
	return glfwErrorStrings[error - errorCodeBase];
    }
    // Catch unknown error constants
    return "Error " + int_to_hex(error);
}

// Return the GLFW version numbers and the compile-time lib string
string GLFW_version_string(void)
{
    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);
    return string("GLFW version: ") + to_string(major) + "."
	+ to_string(minor) + "." + to_string(rev)
	+ " (" + string(glfwGetVersionString()) +")";
}

// The error callback that will be called whenever GLFW triggers an
// error
void GLFW_error_callback(int error, const char* description)
{
    cerr << endl << "GLFW ERROR: " << int_to_hex(error) << " ("
	 << GLFW_error_string(error) << "): " << description << endl;
}

// Initialize the global GLFW context. After initialization, GLFW
// windows may be created.
GlfwContext::GlfwContext(bool verbose)
{
    // Set error callback first to get more info on any errors
    glfwSetErrorCallback(GLFW_error_callback);

    // Store the verbose flag; if we were verbose at startup, we
    // should be verbose at shutdown.
    m_verbose = verbose;

    if (m_verbose)
	cout << "Initializing GLFW...";
    // Initialize GLFW. Any errors will trigger a call to
    // GLFW_error_callback.
    if (!glfwInit()) {
	cerr << "Failed to init GLFW, exiting." << endl;
	return;
    }
    if (m_verbose)
	cout << "done." << endl;
    
    // Require OpenGL version 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    // Remove deprecated features
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Disable any functionality marked for removal
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

// Shut down GLFW
GlfwContext::~GlfwContext()
{
    if (m_verbose)
	cout << "Terminating GLFW...";
    glfwTerminate();
    if (m_verbose)
	cout << "done." << endl;
}
