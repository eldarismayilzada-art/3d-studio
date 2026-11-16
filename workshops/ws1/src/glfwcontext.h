/*
 * glfwcontext.h
 *
 * Workshop 1
 * Computer Graphics course
 * Dept of Computing Science, Umea University
 * Niclas Borlin, niclas.borlin@cs.umu.se
 */

#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Class to handle all global GLFW settings
class GlfwContext {
public:
    GlfwContext(bool verbose = false);
    ~GlfwContext();

    // The GLFW context is solitary; disable copying
    GlfwContext(const GlfwContext&) = delete;
    GlfwContext& operator=(const GlfwContext&) = delete;

private:
    // Remember verbosity level for shutdown
    bool m_verbose = false;
};

// Return a string with the GLFW version
std::string GLFW_version_string(void);

// Return a GLFW_ string for known GLFW error codes Currently
// known values span GLFW_NOT_INITIALIZED - GLFW_PLATFORM_UNAVAILABLE
std::string GLFW_error_string(int error);
