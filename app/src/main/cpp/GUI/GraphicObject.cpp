#include "GraphicObject.h"

GraphicObject::GraphicObject() :
GraphicObject("default.vsh", "default.fsh") {}

GraphicObject::GraphicObject(
        const char *vshader,
        const char *fshader) :
        GraphicObject(nullptr, vshader, fshader, nullptr) {}

GraphicObject::GraphicObject(
        const char *texture,
        const char *vshader,
        const char *fshader) :
        GraphicObject(texture, vshader, fshader, nullptr) {}

GraphicObject::GraphicObject(
        const char *texture,
        const char *vshader,
        const char *fshader,
        GraphicObject *parent) {
    this->parent = parent;
    this->vshader = vshader;
    this->fshader = fshader;
    this->texture_name = texture;
}

GraphicObject::~GraphicObject() { Unload(); }

void GraphicObject::Init_() {
    LoadShaders(&shader_param_, vshader, fshader);
    texture = ndk_helper::texture::loadBMP(texture_name);
    Init();
    update();

    for (auto const &gr : Graphics) {
        gr->Init_();
    }
}

void GraphicObject::addChildObject(GraphicObject *go) {
    go->parent = this;
    Graphics.push_back(go);
    update();
}

void GraphicObject::delChildObject(GraphicObject *go) {
    Graphics.remove(go);
    go->Unload();
    update();
}

void GraphicObject::update(){

    position = new_position;

    draw();

    for (auto const &gr : Graphics) {
        gr->update();
    }
}

void GraphicObject::Unload() {
    if (vbo_) {
        glDeleteBuffers(1, &vbo_);
        vbo_ = 0;
    }

    if (ibo_) {
        glDeleteBuffers(1, &ibo_);
        ibo_ = 0;
    }

    if (shader_param_.program_) {
        glDeleteProgram(shader_param_.program_);
        shader_param_.program_ = 0;
    }
}

void GraphicObject::grender_(float dTime) {
    grender(dTime);
    for (auto const &gr : Graphics) {
        gr->grender_(dTime);
    }
}

bool GraphicObject::LoadShaders(SHADER_PARAMS *params, const char *strVsh,
                                const char *strFsh) {
    GLuint program;
    GLuint vert_shader;
    GLuint frag_shader;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!ndk_helper::shader::CompileShader(&vert_shader, GL_VERTEX_SHADER,
                                           strVsh)) {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return false;
    }

    // Create and compile fragment shader
    if (!ndk_helper::shader::CompileShader(&frag_shader, GL_FRAGMENT_SHADER,
                                           strFsh)) {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(program);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(program, vert_shader);

    // Attach fragment shader to program
    glAttachShader(program, frag_shader);


    // Link program
    if (!ndk_helper::shader::LinkProgram(program)) {
        LOGI("Failed to link program: %d", program);

        if (vert_shader) {
            glDeleteShader(vert_shader);
            vert_shader = 0;
        }
        if (frag_shader) {
            glDeleteShader(frag_shader);
            frag_shader = 0;
        }
        if (program) {
            glDeleteProgram(program);
        }

        return false;
    }

    // Release vertex and fragment shaders
    if (vert_shader) glDeleteShader(vert_shader);
    if (frag_shader) glDeleteShader(frag_shader);

    params->program_ = program;
    return true;
}

GraphicObject * GraphicObject::findFocusObject(const ndk_helper::Vec2& v){
    for (auto const &gr : Graphics) if (gr->position.contains(v)) return gr->findFocusObject(v);
    return this;
}
