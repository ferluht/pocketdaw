//
// Created by ibelikov on 15.07.19.
//

#include "Canvas.h"

void Canvas::init()
{
    if ((width == -1) || (height == -1)) {
        int32_t viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        height = viewport[2];
        width = viewport[3];
    }
}

void Canvas::draw()
{
    GLfloat g_vertex_buffer_data[] = {
            x + width/2, y + height/2, 1.0f, 1.0f,
            x + width/2, y, 1.0f, 0.0f,
            x, y, 0.0f, 0.0f,
            x, y + height/2, 0.0f, 1.0f,
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
                          (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Canvas::grender(float dTime) {

    glBindVertexArray(vao_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}