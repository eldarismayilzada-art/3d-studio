#include "shaderprogram.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// Constructor only initializes the window and file names.
ShaderProgram::ShaderProgram(const OpenGLWindow& w, const std::filesystem::path& shaderDir,
	const std::string& vertexFile, const std::string& fragmentFile, bool verbose)
    : m_window(w),
      m_shaderDir(shaderDir),
      m_vertexFilename(vertexFile),
      m_fragmentFilename(fragmentFile),
      m_verbose(verbose)
{
    if (m_verbose) 
	std::cout << "ShaderProgram: " 
		  << " Shader dir: " << shaderDir
		  << ", win: \"" << m_window.getTitle() << "\" created" << std::endl;
}
GLint ShaderProgram::uniformLocation(const std::string& name) const
{
GLint loc = glGetUniformLocation(program(), name.c_str());
if (loc == -1)
std::cerr << "Warning: uniform '" << name
<< "' not found (misspelled or optimized away?)" << std::endl;
return loc;
}
// Delete the program ID. Works with multiple windows.
ShaderProgram::~ShaderProgram()
{
    if (m_verbose)
	std::cout << "Destroying ShaderProgram: " 
		  << " Shader dir: " << m_shaderDir
		  << ", win: \"" << m_window.getTitle() << "\" ..." << std::endl;

    // Remember the current context
    GLFWwindow* prev = glfwGetCurrentContext();
    // Make the window context current before calling delete
    m_window.makeCurrent();
    glDeleteProgram(m_programId);
    if (m_verbose) {
	// Recover a pointer to the OpenGLWindow object from the glfwWindow
	OpenGLWindow *winObj = nullptr;
	if (prev) {
	    winObj = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(prev));
	}
	const char* t = (winObj ? winObj->getTitle().c_str() : nullptr);
	std::cout << "Destroying ShaderProgram: " 
		  << " Shader dir: " << m_shaderDir
		  << ": restoring window \""
		  << (t ? t : "(none)") << "\" context...";
    }
    // Restore the previously active context
    glfwMakeContextCurrent(prev);    
    if (m_verbose)
	std::cout << "done." << std::endl;
}

// Read a text file. Throws std::runtime_error if error. Uses type in
// error message.
std::string ShaderProgram::readFile(const char *type, const std::string& file)
{
    std::ifstream in(file);
    if (!in)
        throw std::runtime_error("Cannot open " + std::string(type) + " " + file);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Read the source code for the vertex or fragment shader.
void ShaderProgram::loadSource(GLenum type)
{
    switch (type) {
    case GL_VERTEX_SHADER:
	m_vertexCode = readFile("vertex shader", m_shaderDir / m_vertexFilename);
	break;
    case GL_FRAGMENT_SHADER:
	m_fragmentCode = readFile("fragment shader", m_shaderDir / m_fragmentFilename);
	break;
    default:
	std::cerr << "Bad shader type " + type;
	throw std::logic_error("loadSource: Bad shader type " + type);
    }
}

void ShaderProgram::loadSources(void)
{
    // Load each shader's source code.
    if (m_verbose)
	std::cout << "ShaderProgram " << m_vertexFilename << ", " << m_fragmentFilename
	     << ": loading vertex shader...";

    loadSource(GL_VERTEX_SHADER);
    if (m_verbose)
	std::cout << "done." << std::endl;

    if (m_verbose)
	std::cout << "ShaderProgram " << m_vertexFilename << ", " << m_fragmentFilename
	     << ": loading fragment shader...";

    loadSource(GL_FRAGMENT_SHADER);
    if (m_verbose)
	std::cout << "done." << std::endl;
}    

// Compile one of the shaders. NOTE: Must have the correct window context.
GLuint ShaderProgram::compileShader(GLenum type)
{

    const char *shaderType;
    std::string shaderSrc;
    std::string shaderFilename;

    // Set type string for error messages
    switch (type) {
    case GL_VERTEX_SHADER:
	shaderType = "vertex";
	shaderSrc = m_vertexCode;
	shaderFilename = m_vertexFilename;
	break;
    case GL_FRAGMENT_SHADER:
	shaderType = "fragment";
	shaderSrc = m_fragmentCode;
	shaderFilename = m_fragmentFilename;
	break;
    default:
	std::cerr << "ShaderProgram::loadSource: Bad shader type " << type;
	throw std::logic_error("ShaderProgram::loadSource: Bad shader type " + type);
    }

    if (shaderSrc.empty()) {
	std::cerr << shaderType << " shader code empty";
	throw std::runtime_error(std::string("ShaderProgram::compile: ") + shaderType
		+ " shader code empty");
    }
    
    GLuint shader = glCreateShader(type);
    // Convert source string to a c pointer.
    const char *src = shaderSrc.c_str();
    // Give the source to the driver.
    glShaderSource(shader, 1, &src, nullptr);
    // Compile the shader code.
    glCompileShader(shader);

    // Check if compile was successful.
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
	// Get length of error message from compile.
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	// Allocate a buffer to hold the error message
	std::vector<char> log(len > 0 ? len : 1);
	// Copy the error message to the buffer
        glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr,
		log.data());
	// Delete the failed shader object
        glDeleteShader(shader);
	// Throw an error with the compile error message
	std::cerr << "Failed to compile " << shaderType << " shader file "
	     << shaderFilename << ":" << std::endl << log.data();
	throw std::runtime_error(std::string("Failed to compile ") + shaderType
		+ " shader file " + shaderFilename + ":\n" + log.data());
    }
    // Compile successful, return the handle
    return shader;
}

// Compile all shaders and link the shader to form an OpenGL program object.
void ShaderProgram::compileShaders(void)
{
    // Remember the current context
    GLFWwindow* prev = glfwGetCurrentContext();

    if (m_verbose)
	std::cout << "ShaderProgram " << m_vertexFilename << ", "
	     << m_fragmentFilename << ": setting window \""
	     << m_window.getTitle() << "\" context" << std::endl;
    
    // Make this window's context current before compiling
    m_window.makeCurrent();

    GLuint vs;
    try {
	// Compile the vertex shader.
	if (m_verbose)
	    std::cout << "Compiling the vertex shader...";
	vs = compileShader(GL_VERTEX_SHADER);
	if (m_verbose)
	    std::cout << "done (vs = " << vs << ")" << std::endl;
    } catch (...) {
	// Clean up the shader and context before re-throwing
	std::cerr << "Caught exception when compiling the vertex shader" << std::endl;
	glfwMakeContextCurrent(prev);    
        throw;
    }
    
    GLuint fs;
    try {
	if (m_verbose)
	    std::cout << "Compiling the fragment shader...";
        fs = compileShader(GL_FRAGMENT_SHADER);
	if (m_verbose)
	    std::cout << "done (fs = " << fs << ")" << std::endl;
    } catch (...) {
	// Clean up the shader and context before re-throwing
	std::cerr << "Caught exception when compiling the fragment shader" << std::endl;
        glDeleteShader(vs);
	glfwMakeContextCurrent(prev);    
        throw;
    }

    // Create the program
    if (m_verbose)
	std::cout << "Creating a GL program object...";
    m_programId = glCreateProgram();
    if (m_programId) {
	if (m_verbose)
	    std::cout << "done (id = " << m_programId << ")" << std::endl;
    } else {
        glDeleteShader(vs);
        glDeleteShader(fs);
	glfwMakeContextCurrent(prev);    
	throw std::runtime_error("Failed to create program");
    }
    glAttachShader(m_programId, vs);
    glAttachShader(m_programId, fs);
    glLinkProgram(m_programId);

    // The program keeps the compiled code; the shader objects can go
    glDetachShader(m_programId, vs);
    glDetachShader(m_programId, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // Check link status and get error message
    GLint ok = GL_FALSE;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &len);
	std::vector<char> log(len > 0 ? len : 1);
        glGetProgramInfoLog(m_programId, static_cast<GLsizei>(log.size()), nullptr, log.data());
        glDeleteProgram(m_programId);
	glfwMakeContextCurrent(prev);    
        throw std::runtime_error(std::string("Failed to link program:\n") + log.data());
    }
    if (m_verbose) {
	// Recover a pointer to the OpenGLWindow object from the glfwWindow
	OpenGLWindow *winObj = nullptr;
	if (prev) {
	    winObj = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(prev));
	}
	const char* t = (winObj ? winObj->getTitle().c_str() : nullptr);
	std::cout << "ShaderProgram " << m_vertexFilename << ", "
	     << m_fragmentFilename << ": restoring window \""
	     << (t ? t : "(none)") << "\" context...";
    }
    // Restore the previously active context
    glfwMakeContextCurrent(prev);    
    if (m_verbose)
	std::cout << "done." << std::endl;
}


// Return the location of the VAO attribute within the current program
GLint ShaderProgram::attributeLocation(const std::string& name) const
{
    GLint loc = glGetAttribLocation(m_programId, name.c_str());
    if (loc == -1) {
	std::cerr << "Warning: attribute '" << name
		  << "' not found (misspelled or optimized away?)" << std::endl;
    }
    return loc;
}

void ShaderProgram::setUniform(const std::string& name, const glm::mat4& m,
GLboolean transpose) const
{
glProgramUniformMatrix4fv(program(), uniformLocation(name), 1, transpose, glm::value_ptr(m));
}
    
