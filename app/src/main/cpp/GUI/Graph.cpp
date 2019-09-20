//
// Created by ibelikov on 17.07.19.
//

#include "Graph.h"

void BaseGraph::GDraw() {

    fillGLBuffer();

    glBindVertexArray(vao_);

    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * buffer_size, g_vertex_buffer_data, GL_DYNAMIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*) nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void BaseGraph::GRender(float dTime) {
    glBindVertexArray(vao_);
    glDrawArrays(GL_LINE_STRIP, 0, buffer_size);
    glBindVertexArray(0);
}

void TimeGraph::fillGLBuffer() {
    for (int i = 0; i < buffer_size; i++) {
        g_vertex_buffer_data[i*2] = globalPosition.x + i*globalPosition.width/(float)buffer_size;
        g_vertex_buffer_data[i*2+1] = globalPosition.y + (1 - buffer[r])*globalPosition.height;
        r++;
        if(r>=buffer_size) r = 0;
    };
}

void TimeGraph::update(float sample) {
    changed = true;

    if (sample > 1) sample = 1;
    if (sample < -1) sample = -1;

    buffer[r] = sample/2 + 0.5;
    r = (r + 1)%buffer_size;
}

void XYGraph::fillGLBuffer() {
    for (int i = 0; i < buffer_size; i++) {
        g_vertex_buffer_data[i*2] = globalPosition.x + buffer[2*r]*globalPosition.width;
        g_vertex_buffer_data[i*2+1] = globalPosition.y + buffer[2*r + 1]*globalPosition.height;
        r++;
        if(r>=buffer_size) r = 0;
    };
}

void XYGraph::update(float x, float y) {
    changed = true;

    if (x > 1) x = 1;
    if (x < -1) x = -1;
    if (y > 1) y = 1;
    if (y < -1) y = -1;

    buffer[2*r] = x;
    buffer[2*r+1] = y;
    r = (r + 1)%buffer_size;
}