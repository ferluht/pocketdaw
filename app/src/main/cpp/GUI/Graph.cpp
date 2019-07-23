//
// Created by ibelikov on 17.07.19.
//

#include "Graph.h"

void SimpleGraph::draw() {

    for (int i = 0; i < buffer_size; i++) {
        g_vertex_buffer_data[i*3] = x + (buffer[r] + 1)*0.5f*width;
        g_vertex_buffer_data[i*3+1] = y + height/(float)buffer_size*i;
        g_vertex_buffer_data[i*3+2] = z;
        r++;
        if(r>=buffer_size) r = 0;
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vbo_);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);

    // 1st attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*) nullptr     // array buffer offset
    );
    glEnableVertexAttribArray(0);
}

void SimpleGraph::grender(float dTime) {

//    glDrawArrays(GL_LINE_STRIP, 0, BUFFER_SIZE);
    for(int i = 0; i < buffer_size - 1; i++) {
        glDrawArrays(GL_LINES, i, 2);
    }
}