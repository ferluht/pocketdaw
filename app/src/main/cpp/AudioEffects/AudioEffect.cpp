//
// Created by Admin on 12.06.2019.
//

#include "AudioEffect.h"

AudioEffect::AudioEffect() :
GraphicObject("Textures/effect_canvas.bmp", "Shaders/VS_ShaderPlain.vsh", "Shaders/ShaderPlainRect.fsh")
{

}

AudioEffect::AudioEffect(const char * tex, const char * vsh, const char * fsh) :
GraphicObject(tex, vsh, fsh)
{

}

void AudioEffect::grender(float dTime) {

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

float AudioEffect::apply(float sample)
{
    return sample;
}