//
// Created by Admin on 06.07.2019.
//

#include "Button.h"

Button::Button(const char * texture, float x, float y, float * parameter)
: GraphicObject(texture, "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlain.fsh"){

    parameter_ = parameter;

    relativePosition.x = x;
    relativePosition.y = y;
    relativePosition.z = 0.1;
    relativePosition.height = 0.1;
    relativePosition.width = 0.2;
}

void Button::grender(float dTime) {
    //
    // Feed Projection and Model View matrices to the shaders

    GLfloat g_vertex_buffer_data[] = {
            // Позиции          // Цвета             // Текстурные координаты
            absolutePosition.x + absolutePosition.width, absolutePosition.y + absolutePosition.height, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // Верхний правый
            absolutePosition.x + absolutePosition.width, absolutePosition.y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // Нижний правый
            absolutePosition.x, absolutePosition.y, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // Нижний левый
            absolutePosition.x, absolutePosition.y + absolutePosition.height, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,    // Верхний левый
    };

//    GLuint indices[] = {  // Помните, что мы начинаем с 0!
//            0, 3, 1,   // Первый треугольник
//            3, 2, 1    // Второй треугольник
//    };

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


    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(vao_square_);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}


void Button::dragHandler(ndk_helper::Vec2 v) {
    relativePosition.x = (v.x_ - drag_from.x_)*drag_xscale + relative_position_backup.x;
    relativePosition.y = (v.y_ - drag_from.y_)*drag_yscale + relative_position_backup.y;

    *parameter_ = relativePosition.y;

    Update();
}