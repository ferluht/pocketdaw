//
// Created by ibelikov on 15.07.19.
//

#include "Canvas.h"

void Canvas::draw()
{
    GLfloat g_vertex_buffer_data[] = {
            position.x + position.width/2, position.y + position.height/2, 1.0f, 1.0f,
            position.x + position.width/2, position.y, 1.0f, 0.0f,
            position.x, position.y, 0.0f, 0.0f,
            position.x, position.y + position.height/2, 0.0f, 1.0f,
    };

    GLuint indices[] = {
            1, 3, 0,
            1, 2, 3
    };


    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                          (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Canvas::grender(float dTime) {
    ndk_helper::Mat4 mat_vp = eng->mat_projection_ * eng->mat_view_;

    glUseProgram(shader_param_.program_);

    glUniformMatrix4fv(shader_param_.matrix_projection_, 1, GL_FALSE,
                       mat_vp.Ptr());

    GLint MatrixID = glGetUniformLocation(shader_param_.program_, "rot");

    GLfloat rot[] = {1, 0, 0, 1};

    glUniformMatrix2fv(MatrixID, 1, GL_FALSE, rot);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}