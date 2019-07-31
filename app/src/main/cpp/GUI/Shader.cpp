//
// Created by ibelikov on 24.07.19.
//

#include "Shader.h"

Shader::Shader() {
    program_ = 0;
    param_view_ = 0;
    param_texture_angle_ = 0;
}


Shader Shader::CreateShaderProgram(const char *vsh, const char *fsh) {
    GLuint program;
    GLuint vert_shader;
    GLuint frag_shader;

    Shader sh;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!ndk_helper::shader::CompileShader(&vert_shader, GL_VERTEX_SHADER,
                                           vsh)) {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return sh;
    }

    // Create and compile fragment shader
    if (!ndk_helper::shader::CompileShader(&frag_shader, GL_FRAGMENT_SHADER,
                                           fsh)) {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(program);
        return sh;
    }

    // Attach vertex shader to program
    glAttachShader(program, vert_shader);

    // Attach fragment shader to program
    glAttachShader(program, frag_shader);


    // Link program
    if (!ndk_helper::shader::LinkProgram(program)) {
        LOGI("Failed to link program: %d", program);

        if (vert_shader) {
            glDeleteShader(vert_shader);
            vert_shader = 0;
        }
        if (frag_shader) {
            glDeleteShader(frag_shader);
            frag_shader = 0;
        }
        if (program) {
            glDeleteProgram(program);
        }

        return sh;
    }

    sh.program_ = program;
    sh.param_view_ = glGetUniformLocation(program, "uPMatrix");
    sh.param_texture_angle_ = glGetUniformLocation(program, "angle");
    sh.param_z_ = glGetUniformLocation(program, "z");
    sh.param_color_ = glGetUniformLocation(program, "color");

    // Release vertex and fragment shaders
    if (vert_shader) glDeleteShader(vert_shader);
    if (frag_shader) glDeleteShader(frag_shader);

    return sh;
}