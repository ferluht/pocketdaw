#include "GraphicObject.h"

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
    // Load shader
    this->vshader = vshader;
    this->fshader = fshader;
    this->texture_name = texture;
}

GraphicObject::~GraphicObject() { Unload(); }

void GraphicObject::Init() {
    // Settings
    glFrontFace(GL_CCW);
    LoadShaders(&shader_param_, vshader, fshader);
    texture = ndk_helper::texture::loadBMP(texture_name);
    Update();

    for (auto const &gr : Graphics) {
        gr->Init();
    }

}

void GraphicObject::addChildObject(GraphicObject *go) {
    go->parent = this;
    Graphics.push_back(go);
    Update();
}

void GraphicObject::Update() {
    // Init Projection matrices
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    mat_projection_ = ndk_helper::Mat4::Ortho2D(0, 0, viewport[2], -viewport[3]);

    if (parent == nullptr) {
        relativePosition.x = 0;//-viewport[2] / 100.f;
        relativePosition.y = 0;//-viewport[3] / 100.f;
        relativePosition.z = 0;
        relativePosition.width = viewport[2];// 2 * viewport[2] / 100.f;
        relativePosition.height = viewport[3];// 2 * viewport[3] / 100.f;
    }

    absolutePosition = relativePosition;
    if (parent) {
        absolutePosition.x =
                parent->absolutePosition.x + parent->absolutePosition.width * relativePosition.x;
        absolutePosition.y =
                parent->absolutePosition.y + parent->absolutePosition.height * relativePosition.y;
        absolutePosition.z = parent->absolutePosition.z + relativePosition.z;
        absolutePosition.height = relativePosition.height * parent->absolutePosition.height;
        absolutePosition.width = relativePosition.width * parent->absolutePosition.width;
    }

    for (auto const &gr : Graphics) {
        gr->Update();
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
    GLuint vert_shader, frag_shader;

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

    // Get uniform locations
    params->matrix_projection_ = glGetUniformLocation(program, "uPMatrix");

    // Release vertex and fragment shaders
    if (vert_shader) glDeleteShader(vert_shader);
    if (frag_shader) glDeleteShader(frag_shader);

    params->program_ = program;
    return true;
}


GraphicObject * GraphicObject::findDragHandler(ndk_helper::Vec2 v, float xscale, float yscale){
    for (auto const &gr : Graphics) {
        if ((gr->absolutePosition.x < v.x_)
        && (gr->absolutePosition.x + gr->absolutePosition.width > v.x_)
        && (gr->absolutePosition.y < v.y_)
        && (gr->absolutePosition.y + gr->absolutePosition.height > v.y_)){
            return gr->findDragHandler(v, xscale/absolutePosition.width, yscale/absolutePosition.height);
        }
    }

    dragBegin(v, xscale, yscale);
    return this;
}
