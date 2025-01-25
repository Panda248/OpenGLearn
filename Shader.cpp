#include "Shader.h"

class Shader {

public:

    Shader(char* vertexPath, char* fragmentPath) {
        std::string vertexCode, fragmentCode;
        std::ifstream vertexFile, fragmentFile;

        vertexFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);

        //Reading Filedata
        try {
            vertexFile.open(vertexPath);
            fragmentFile.open(fragmentPath);

            //read file buffer contents into stringstream
            std::stringstream vertexStream, fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();

            vertexFile.close();
            fragmentFile.close();

            //convert stream to string
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        }
        catch (std::ifstream::failure e) {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ CODE:" << e.code() << std::endl;
        }

        //string to const char*
        const char* vertexCodeChar = vertexCode.c_str();
        const char* fragmentCodeChar = fragmentCode.c_str();

        //Creating GL Shader Program

        //Compiling shaders
        unsigned int vertex, fragment;
        int success;
        char infoLog[512];

        //vertex
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vertexCodeChar, NULL);
        glCompileShader(vertex);

        //Failure Log
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILURE \n" << infoLog << std::endl;
        };

        //fragment
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fragmentCodeChar, NULL);
        glCompileShader(fragment);

        //Failure Log
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILURE \n" << infoLog << std::endl;
        };

        //Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
    }

    void use() {
        glUseProgram(ID);
    }

    void setBool(const std::string& name, bool value) const {

    }

    void setInt(const std::string& name, int value) const {

    }

    void setFloat(const std::string& name, float value) const {

    }
};
