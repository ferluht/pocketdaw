//
// Created by ibelikov on 15.07.19.
//

#include "Canvas.h"

void Canvas::draw()
{
    GLfloat g_vertex_buffer_data[] = {
            globalPosition.x + globalPosition.height*globalPosition.ratio, globalPosition.y + globalPosition.height, 1.0f, 1.0f,
            globalPosition.x + globalPosition.height*globalPosition.ratio, globalPosition.y, 1.0f, 0.0f,
            globalPosition.x, globalPosition.y, 0.0f, 0.0f,
            globalPosition.x, globalPosition.y + globalPosition.height, 0.0f, 1.0f,
    };

    GLuint indices[] = {
            1, 3, 0,
            1, 2, 3
    };

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data,
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                          (GLvoid *) (2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Canvas::grender(float dTime) {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}