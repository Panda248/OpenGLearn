#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Camera_Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// default values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float FOV = 45.0f;
const float SENS = 0.5f;

class Camera {

public:
    glm::vec3 Pos, Front, TrueFront, Right, Up, WorldUp;
    float Yaw, Pitch, Fov, Sens, Speed;


    // vector constructor
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float pitch = PITCH, float yaw = YAW) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), Fov(FOV), Speed(SPEED), Sens(SENS) {
        Pos = pos;
        WorldUp = up;
        Pitch = pitch;
        Yaw = yaw;

        updateCameraVectors();
    }

    // Movement
    void cameraMoveInput(Camera_Direction dir, float deltaTime) {
        float vel = deltaTime * Speed;
        if (dir == FORWARD) {
            Pos += TrueFront * vel;
            //std::cout << "Front" << std::endl;
        }
        if (dir == BACKWARD) {
            Pos -= TrueFront * vel;
            //std::cout << "Back" << std::endl;
        }
        if (dir == LEFT) {
            Pos -= Right * vel;
            //std::cout << "Left" << std::endl;
        }
        if (dir == RIGHT) {
            Pos += Right * vel;
            //std::cout << "Right" << std::endl;
        }
    }

    //Mouse movement
    void cameraMouseInput(float xOffset, float yOffset, GLboolean lockPitch = true) {

        xOffset *= Sens;
        yOffset *= Sens;

        Yaw += xOffset;
        Pitch += yOffset;

        std::cout << "Yaw " << Yaw << " Pitch " << Pitch << std::endl;

        if (lockPitch) {
            if (Pitch > 89.0f) {
                Pitch = 89.0f;
            }
            if (Pitch < -89.0f) {
                Pitch = -89.0f;
            }
        }

        updateCameraVectors();
    }


    glm::mat4 generateView() {
        //return glm::lookAt(Pos, Pos + Front, Up);
        return lookAt();
    }
private:

    // TODO recreate the lookat matrix that glm returns with their glm::lookAt function
    /*
    * Reminder that the lookat matrix can be defined as the matrix multiplication of these two matrices
    * | Rx Ry Rz 0 | | 1  0  0  -Px |
    * | Ux Uy Uz 0 | | 0  1  0  -Py |
    * | Dx Dy Dz 0 | | 0  0  1  -Pz |
    * | 0  0  0  1 | | 0  0  0  1   |
    * 
    * Where R is the camera's right vector,
    * U is the camera's up vector (not world up)
    * D is the camera's direction vector
    * P is the camera's position vector
    * 
    */

    glm::mat4 lookAt() {
        glm::mat4 posMat = glm::mat4(1.0f);
        posMat += glm::mat4(glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(0.0f), glm::vec4(-Pos, 0.0f));

        glm::mat4 rudMat = glm::mat4(glm::vec4(Right, 0), glm::vec4(Up, 0.0f), glm::vec4(-Front, 0), glm::vec4(0, 0, 0, 1));
        rudMat = glm::transpose(rudMat);

        return rudMat * posMat;
    }

    void updateCameraVectors() {
        //calculate new front
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        TrueFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

        //recalculate right and up with new front
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));

    }
};