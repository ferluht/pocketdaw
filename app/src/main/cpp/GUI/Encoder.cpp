//
// Created by Admin on 06.07.2019.
//

#include "Encoder.h"

Encoder::Encoder(const char * texture, float x, float y, float * parameter)
: GraphicObject(texture, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh"){

    parameter_ = parameter;

    relativePosition.x = x;
    relativePosition.y = y;
    relativePosition.z = 0.1;
    relativePosition.height = 0.1;
    relativePosition.width = 0.1;
    angle = 0;
}

void Encoder::grender(float dTime) {
    //
    // Feed Projection and Model View matrices to the shaders
    GLfloat g_vertex_buffer_data[] = {
            // Позиции          // Цвета             // Текстурные координаты
            absolutePosition.x + absolutePosition.width, absolutePosition.y + absolutePosition.width, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Верхний правый
            absolutePosition.x + absolutePosition.width, absolutePosition.y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Нижний правый
            absolutePosition.x, absolutePosition.y, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Нижний левый
            absolutePosition.x, absolutePosition.y + absolutePosition.width, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,    // Верхний левый
    };

    GLuint indices[] = {  // Помните, что мы начинаем с 0!
            1, 3, 0,   // Первый треугольник
            1, 2, 3    // Второй треугольник
    };


    glGenBuffers(1, &vbo_);
    glGenVertexArrays(1, &vao_square_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_square_);

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


    GLint MatrixID = glGetUniformLocation(shader_param_.program_, "rot");

//    GLfloat rot_PI[] = {cos(-(float)M_PI_2), -sin(-(float)M_PI_2), sin(-(float)M_PI_2), cos(-(float)M_PI_2)};
    GLfloat rot[] = {cos(angle), -sin(angle), sin(angle), cos(angle)};

    glUniformMatrix2fv(MatrixID, 1, GL_FALSE, rot);

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao_square_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}


void Encoder::dragHandler(ndk_helper::Vec2 v) {
//    relativePosition.x = (v.x_ - drag_from.x_)*drag_xscale + relative_position_backup.x;
//    relativePosition.y = (v.y_ - drag_from.y_)*drag_yscale + relative_position_backup.y;

    angle = old_angle + (drag_from.y_ - v.y_)/100;

    if (angle < -0.75f*(float)M_PI) {
        angle = -0.75f*(float)M_PI;
    }

    if (angle > 0.75f*(float)M_PI) {
        angle = 0.75f*(float)M_PI;
    }

    float param = (angle/0.75f/(float)M_PI + 1.f) / 2.f;
    if (param < 0.1) param = 0.1;
    if (param > 0.9) param = 0.9;

    *parameter_ = param;

    Update();
}

void Encoder::dragBegin(ndk_helper::Vec2 v, float xscale, float yscale) {
    drag_from = v;
    old_angle = angle;
}