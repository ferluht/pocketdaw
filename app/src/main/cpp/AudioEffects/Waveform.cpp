//
// Created by ibelikov on 11.07.19.
//

#include "Waveform.h"
#include "../GUI/Text.h"
#include "../GUI/Encoder.h"

Waveform::Waveform(float n, float x, float y) : AudioEffect("", "Shaders/BasicV.vsh", "Shaders/BasicF.fsh"){

    relativePosition.x = x;
    relativePosition.y = y;
    relativePosition.z = 0.1;
    relativePosition.height = 1;
    relativePosition.width = 1;
    window = n;

    addChildObject(new Text("Fonts/Roboto-Regular.ttf", L"waveform\n", -0.2, 0.1));
    addChildObject(new Encoder("Textures/encoder.bmp", 0.2f, 0.2f, &window));

    for (int i = 0; i < BUFFER_SIZE; i ++) buffer[i] = 0;
}

void Waveform::grender(float dTime) {
    //
    // Feed Projection and Model View matrices to the shaders


    const float CAM_X = 0.f;
    const float CAM_Y = 0.f;
    const float CAM_Z = 700.f;

    mat_view_ = ndk_helper::Mat4::LookAt(ndk_helper::Vec3(CAM_X, CAM_Y, CAM_Z),
                                         ndk_helper::Vec3(0.f, 0.f, 0.f),
                                         ndk_helper::Vec3(0.f, 1.f, 0.f));

    ndk_helper::Mat4 mat_vp = mat_projection_ * mat_view_;

    glUseProgram(shader_param_.program_);

    glUniformMatrix4fv(shader_param_.matrix_projection_, 1, GL_FALSE,
                       mat_vp.Ptr());

//    GLint MatrixID = glGetUniformLocation(shader_param_.program_, "rot");
//
//    GLfloat rot[] = {(float)cos(0.1), -(float)sin(0.1), (float)sin(0.1), (float)cos(0.1)};
//
//    glUniformMatrix2fv(MatrixID, 1, GL_FALSE, rot);

    static GLfloat g_vertex_buffer_data[BUFFER_SIZE*3];


    for (int i = 0; i < BUFFER_SIZE; i++) {
        g_vertex_buffer_data[i*3] = absolutePosition.x + buffer[r]*500 + 0.7f*absolutePosition.width;
        g_vertex_buffer_data[i*3+1] = absolutePosition.y + absolutePosition.height/(float)BUFFER_SIZE*i;
        g_vertex_buffer_data[i*3+2] = absolutePosition.z;
        r++;
        if(r>=BUFFER_SIZE) r = 0;
    };

    // Generate 1 buffer, put the resulting identifier in vertexbuffer
    glGenBuffers(1, &vertexbuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);


    // 1st attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
//    glDrawArrays(GL_LINE_STRIP, 0, BUFFER_SIZE);
    for(int i = 0; i < BUFFER_SIZE - 1; i++) {
        glDrawArrays(GL_LINES, i, 2);
    }

    glDisableVertexAttribArray(0);
}

float Waveform::apply(float sample)
{
    if (ai < window*BUFFER_SIZE){
        accumulator += sample;
        ai ++;
    } else {
        ai = 0;
        buffer[r] = (accumulator + prev_acc) / 2 / window / BUFFER_SIZE;
        prev_acc = accumulator;
        accumulator = 0;
        r++;
        if(r >= BUFFER_SIZE) r = 0;
    }
    return sample;
}

void Waveform::dragHandler(ndk_helper::Vec2 v) {
    relativePosition.x = (v.x_ - drag_from.x_)*drag_xscale + relative_position_backup.x;
    relativePosition.y = (v.y_ - drag_from.y_)*drag_yscale + relative_position_backup.y;

    Update();
}

void Waveform::dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) {
    drag_from = v;
    relative_position_backup = relativePosition;
    drag_xscale = xscale;
    drag_yscale = yscale;
}

void Waveform::dragEnd() {}