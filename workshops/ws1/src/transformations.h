#include <glm/glm.hpp>
#include<cmath>

namespace Transform{
    inline glm::mat4 translate(float x, float y, float z){
        glm::mat4 translationMatrix(1.0f);
        translationMatrix[3][0] = x;
        translationMatrix[3][1] = y;
        translationMatrix[3][2] = z;
        return translationMatrix;
    }

    inline glm::mat4 scale(float x,float y,float z){
        glm::mat4 scaleMatrix(1.0f);
        scaleMatrix[0][0] = x;
        scaleMatrix[1][1] = y;
        scaleMatrix[2][2] = z;
        return scaleMatrix;
    }

    inline glm::mat4 rotateX(float angle){
        glm::mat4 rotationMatrix(1.0f);
        float rad = glm::radians(angle);
        rotationMatrix[1][1] = cos(rad);
        rotationMatrix[1][2] = -sin(rad);
        rotationMatrix[2][1] = sin(rad);
        rotationMatrix[2][2] = cos(rad);
        return rotationMatrix;
    }

    inline glm::mat4 rotateY(float angle){
        glm::mat4 rotationMatrix(1.0f);
        float rad = glm::radians(angle);
        rotationMatrix[0][0] = cos(rad);
        rotationMatrix[0][2] = sin(rad);
        rotationMatrix[2][0] = -sin(rad);
        rotationMatrix[2][2] = cos(rad);
        return rotationMatrix;
    }

    inline glm::mat4 rotateZ(float angle){
        glm::mat4 rotationMatrix(1.0f);
        float rad = glm::radians(angle);
        rotationMatrix[0][0] = cos(rad);
        rotationMatrix[0][1] = -sin(rad);
        rotationMatrix[1][0] = sin(rad);
        rotationMatrix[1][1] = cos(rad);
        return rotationMatrix;
    }
}