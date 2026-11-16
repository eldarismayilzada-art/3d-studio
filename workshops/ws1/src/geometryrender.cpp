/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include <stdexcept>
#include "geometryrender.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "transformations.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


// Store shaderDir and call parent constructor with the rest
GeometryRender::GeometryRender(const GlfwContext &ctx,
                               std::filesystem::path shaderDir,
                               std::string title, int width, int height,
                               bool verbose)
    : OpenGLWindow(ctx, std::move(title), width, height, verbose),
      m_shaderDir(std::move(shaderDir))
{
}

GeometryRender::~GeometryRender()   
{
    makeCurrent();                       // delete in the right context
    glDeleteVertexArrays(1, &m_vao);       // deleting 0 is silently ignored
    glDeleteBuffers(1, &m_vBuffer);
    glDeleteBuffers(1, &m_iBuffer);
}

// Initialize OpenGL
void GeometryRender::initialize()
{
    makeCurrent();
    
    // Enable depth test. Needed for 3D data only.
    glEnable(GL_DEPTH_TEST);

    // Create a new ShaderProgram and store it in the shader member.
    // Destroys any old ShaderProgram object.
    m_shader.reset(new ShaderProgram(*this, m_shaderDir, "vshader.glsl", "fshader.glsl",
	    m_verbose));

    // Load the source code for the shaders. Will throw an exception
    // if either file does not exist.
    m_shader->loadSources();//Allocate storage to the VBO and EBO in the GPU driver and copy
    // data from the CPU. The driver is responsible for how and when
    // to transfer the data to the GPU. Flags 0 means the content can
    // never be modified from the CPU.
    //
    // Since the data is copied to the;
    // Compile and link the shaders. Will throw an exception for failure.
    m_shader->compileShaders();

    // Load the vertex data
    loadGeometry();
    m_modelMat =Transform::translate(0.1f, 0.0f, 0.0f) * m_modelMat;
    m_shader->setUniform("M", m_modelMat, GL_FALSE);
}

void GeometryRender::loadGeometry()
{
    // 3d data; 4 corners of a tetrahedron. Stored in system RAM. The
    // coordinates are in normalized device coordinates [-1,1].
    m_vertices = { Vec3(0,0,0), Vec3(0.5,0,0), Vec3(0,0.5,0) , Vec3(0,0,-0.5) };
    // Vertex numbers (Indices into vertices). Will be used to
    // describe how to connect the points when drawn. Stored in system
    // RAM.
    m_indices = { 0, 1, 2 ,3,2,1};

    // Create OpenGL buffer objects. Each object is initialized into
    // an empty, valid state with no GPU memory connected.
    // Each object is known via an object handle (an integer).

    // m_vBuffer will be used as a VBO (Vertex Buffer Object) and
    // store the vertex data.
    glCreateBuffers(1, &m_vBuffer);
    // m_iBuffer will be used as an EBO (Element Buffer Object) and
    // store indices into the VBO.
    glCreateBuffers(1, &m_iBuffer);
    
    // Allocate storage to the VBO and EBO in the GPU driver and copy
    // data from the CPU. The driver is responsible for how and when
    // to transfer the data to the GPU. Flags 0 means the content can
    // never be modified from the CPU.
    //
    // Since the data is copied to the driver, vertices and indices
    // may be freed after these calls.
    glNamedBufferStorage(m_vBuffer, m_vertices.size() * sizeof(Vec3),
	m_vertices.data(), 0);
    glNamedBufferStorage(m_iBuffer, m_indices.size() * sizeof(unsigned int),
	m_indices.data(), 0);

    // Create an empty VAO (Vertex Array Object) that will store the
    // "plumbing diagram" of how to get the vertex data to the vertex
    // processor on the GPU.
    glCreateVertexArrays(1, &m_vao);

    // This example only has one data buffer; use index 0 for it. If
    // we, e.g., had vertices stored in one buffer and color in
    // another, we would use different binding sites (indices) for
    // each. The bindingIndex is the "input" to the VAO.
    const GLuint bindingIndex = 0;
    // Specify the data layout for binding site 0 of this VAO; where
    // does the raw data come from? The buffer (m_vBuffer), the start
    // offset (0), and the stride (sizeof(Vec2)), i.e., the number of
    // bytes between consecutive vertices.
    glVertexArrayVertexBuffer(m_vao, bindingIndex, m_vBuffer, 0, sizeof(Vec3));

    // The attribIndex is the "output" of the VAO and is the input to
    // the vertex shader
    //
    // This example only has one attribute; vPosition that will
    // receive the position data. Other examples may have color,
    // normals, etc.
    GLint attribIndex = m_shader->attributeLocation("vPosition");

    if (attribIndex < 0) {
        throw std::runtime_error("Could not get attribute index of vPosition");
    }
    
    // The attribute named vPosition of this VAO should get its data
    // from the buffer attached to binding site 0.
    glVertexArrayAttribBinding(m_vao, attribIndex, bindingIndex);

    // Attribute 0 has 2 components each of type GL_FLOAT, not to be
    // normalized (GL_FALSE). It starts at byte offset 0 within each
    // vertex.
    glVertexArrayAttribFormat(m_vao, attribIndex, 3, GL_FLOAT, GL_FALSE, 0);

    // Turn on the use of this attribute. Otherwise, the vertex shader
    // would never see it.
    glEnableVertexArrayAttrib(m_vao, attribIndex);
    
    // Specify the element buffer for the VAO. It supplies the vertex
    // numbers for indexed draws. The grouping, usage, and type of the
    // elements in m_iBuffer is specified when calling glDrawElements().
    glVertexArrayElementBuffer(m_vao, m_iBuffer);
}
//Here I have added the onKey method to handle key presses and update the model matrix accordingly.
void GeometryRender::onKey(int key)
{
    if (key == GLFW_KEY_J) {
        m_modelMat =
            Transform::translate(-0.1f, 0.0f, 0.0f) * m_modelMat;

    } else if (key == GLFW_KEY_L) {
        m_modelMat =
            Transform::translate(0.1f, 0.0f, 0.0f) * m_modelMat;

    } else if (key == GLFW_KEY_I) {
        m_modelMat =
            Transform::translate(0.0f, 0.1f, 0.0f) * m_modelMat;

    } else if (key == GLFW_KEY_K) {
        m_modelMat =
            Transform::translate(0.0f, -0.1f, 0.0f) * m_modelMat;

    }else if(key==GLFW_KEY_LEFT){
        m_modelMat = Transform::rotateY(-10.0f) * m_modelMat;
    }else if(key==GLFW_KEY_RIGHT){  
        m_modelMat = Transform::rotateY(10.0f) * m_modelMat;
    }else if(key==GLFW_KEY_UP){
        m_modelMat = Transform::rotateX(-10.0f) * m_modelMat;
    }else if(key==GLFW_KEY_DOWN){
        m_modelMat = Transform::rotateX(10.0f) * m_modelMat;
    } else if(key==GLFW_KEY_O){
        std::cout<<"Enter OBJ filename to load: ";
        std::string filename;
        std::cin>>filename;
        try{
            loadObj(filename);
        }catch(const std::exception& e){
            std::cerr<<"Error loading OBJ file: "<<e.what()<<std::endl;
            return;
        }
    } else {
        return;
    }

    displayNow();
}
// Render object
void GeometryRender::display()
{
    if (!m_shader)
        throw std::logic_error("GeometryRender::initialize() must "
		"be called before display()");

    // Make the correct context current.
    makeCurrent();
    // Fill color and depth buffers with the "clear" values.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Select shader program and plumbing (VAO) to use.
    m_shader->use();
    m_shader->setUniform("M", m_modelMat, GL_FALSE);
    glBindVertexArray(m_vao);

    // Draw primitives as triangles (GL_TRIANGLES). Read
    // indices.size() indices in total, i.e., indices.size()/3
    // triangles. The indices are of type GL_UNSIGNED_INT and are read
    // from the element buffer of the currently bound VAO, starting at
    // byte offset 0.
    const size_t byteOffset = 0 * sizeof(unsigned int);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()),
	GL_UNSIGNED_INT, BUFFER_OFFSET(byteOffset));

    // Unbind the VAO and program to make failures loud.
    glBindVertexArray(0);
    glUseProgram(0);

}
  void GeometryRender::updateGeometry(const std::vector<Vec3>& vertices, const std::vector<unsigned int>& indices) {
        if(vertices.empty() || indices.empty()) {
            throw std::invalid_argument("Vertices and indices cannot be empty.");
        }
        makeCurrent();
        //here i creat buffers
        GLuint newVBuffer, newIBuffer;
        glCreateBuffers(1, &newVBuffer);
        glCreateBuffers(1, &newIBuffer);
        glNamedBufferStorage(newVBuffer, vertices.size() * sizeof(Vec3), vertices.data(), 0);
        glNamedBufferStorage(newIBuffer, indices.size() * sizeof(unsigned int), indices.data(), 0);
        //Delete old buffers
        glVertexArrayVertexBuffer(m_vao, 0, newVBuffer, 0, sizeof(Vec3));
        glVertexArrayElementBuffer(m_vao, newIBuffer);
        glDeleteBuffers(1, &m_vBuffer);
        glDeleteBuffers(1, &m_iBuffer);
        //Remember the new buffers 
        m_vBuffer = newVBuffer;
        m_iBuffer = newIBuffer;
        m_vertices = vertices;
        m_indices = indices;
    }
void GeometryRender::loadObj(const std::string&filename){
    tinyobj::ObjReaderConfig config;
    config.triangulate = true; // Ensure the model is triangulated
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename, config)) {
        throw std::runtime_error("Failed to load OBJ file: " + filename);
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;
    //Read vertex coordinates
    for(size_t v = 0; v < attrib.vertices.size() / 3; v++) {
        vertices.push_back(Vec3(
            attrib.vertices[3 * v + 0], //The obj reader stores positions as one long sequence.every three numbers form a vertex.
            attrib.vertices[3 * v + 1],
            attrib.vertices[3 * v + 2]
        ));
    }
    //Read indicies from shapes.
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }
    }

    //Normalization
    float minX = std::numeric_limits<float>::max();
    float minY= minX;
    float minZ= minX;
    float maxX = std::numeric_limits<float>::lowest();
    float maxY= maxX;
    float maxZ= maxX;
    for (const auto& vertex : vertices) {
        minX = std::min(minX, vertex.values[0]);
        minY = std::min(minY, vertex.values[1]);
        minZ = std::min(minZ, vertex.values[2]);
        maxX = std::max(maxX, vertex.values[0]);
        maxY = std::max(maxY, vertex.values[1]);
        maxZ = std::max(maxZ, vertex.values[2]);
    }

    //Find the center
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;
    //Find the largest dimension
    float width = maxX - minX;
    float height = maxY - minY;
    float depth = maxZ - minZ;
    float maxDimension = std::max({width, height, depth});
    if (maxDimension<=0.0f) {
        throw std::runtime_error("Invalid model dimensions in OBJ file: " + filename);
    }
    //Center and normalize each vertex
    for (auto& vertex : vertices) {
        vertex.values[0] = (vertex.values[0] - centerX) / maxDimension;
        vertex.values[1] = (vertex.values[1] - centerY) / maxDimension;
        vertex.values[2] = (vertex.values[2] - centerZ)/maxDimension;
    }

    updateGeometry(vertices, indices);
    m_modelMat=glm::mat4(1.0f); //Reset model matrix after loading new geometry
}

  