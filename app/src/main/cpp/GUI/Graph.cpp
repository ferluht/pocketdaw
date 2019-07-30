//
// Created by ibelikov on 17.07.19.
//

#include "Graph.h"

void SimpleGraph::draw() {

    for (int i = 0; i < buffer_size; i++) {
        g_vertex_buffer_data[i*3] = globalPosition.x + globalPosition.width/(float)buffer_size*i;
        g_vertex_buffer_data[i*3+1] = globalPosition.y + buffer[r]*globalPosition.height;
        g_vertex_buffer_data[i*3+2] = globalPosition.z;
        r++;
        if(r>=buffer_size) r = 0;
    };

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 3 * buffer_size, g_vertex_buffer_data, GL_DYNAMIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void SimpleGraph::grender(float dTime) {
    glDrawArrays(GL_LINE_STRIP, 0, buffer_size);
}

void SimpleGraph::update(float sample) {
    changed = true;
    buffer[r] = sample;
    r = (r + 1)%buffer_size;
}

void XYGraph::draw() {

    for (int i = 0; i < buffer_size; i++) {
        g_vertex_buffer_data[i*4] = globalPosition.x + buffer[2*r + 1]*globalPosition.width;
        g_vertex_buffer_data[i*4+1] = globalPosition.y + buffer[2*r]*globalPosition.height;
        g_vertex_buffer_data[i*4+2] = globalPosition.z;
        g_vertex_buffer_data[i*4+3] = (float)pow(1.f - (float)i / (float)buffer_size, 2);
        r++;
        if(r>=buffer_size) r = 0;
    };

    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_);

    glBindVertexArray(vao_);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * buffer_size, g_vertex_buffer_data, GL_DYNAMIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : brightness
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void XYGraph::grender(float dTime) {
    glDrawArrays(GL_LINE_STRIP, 0, buffer_size);
}

void XYGraph::update(float x, float y) {
    changed = true;
    buffer[2*r] = x;
    buffer[2*r+1] = y;
    r = (r + 1)%buffer_size;
}