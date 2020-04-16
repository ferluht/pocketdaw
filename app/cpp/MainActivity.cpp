#include <AMGEngine/bx/include/bx/uint32_t.h>
#include <AMGEngine/bgfx_utils/common.h>
#include <AMGEngine/bgfx_utils/bgfx_utils.h>
#include <AMGEngine/imgui/imgui.h>
#include <AMGEngine/nanovg/nanovg_bgfx.h>
#include <AMGEngine/nanovg/nanovg.h>
#include <Synth.h>

namespace
{

    int32_t createFont(NVGcontext* _ctx, const char* _name, const char* _filePath)
    {
        uint32_t size;
        void* data = load(_filePath, &size);
        if (NULL == data)
        {
            return -1;
        }

        return nvgCreateFontMem(_ctx, _name, (uint8_t*)data, size, 0);
    }


    class MainActivity : public entry::AppI
    {
    public:
        MainActivity(const char* _name, const char* _description, const char* _url)
                : entry::AppI(_name, _description, _url)
        {
        }

        void init(int32_t _argc, const char* const* _argv, uint32_t _width, uint32_t _height) override
        {
            Args args(_argc, _argv);

            m_width  = _width;
            m_height = _height;
            m_debug  = BGFX_DEBUG_TEXT;
            m_reset  = BGFX_RESET_VSYNC;

            bgfx::Init init;
            init.type     = args.m_type;
            init.vendorId = args.m_pciId;
            init.resolution.width  = m_width;
            init.resolution.height = m_height;
            init.resolution.reset  = m_reset;
            bgfx::init(init);

            // Enable debug text.
            bgfx::setDebug(m_debug);

            // Set view 0 clear state.
            bgfx::setViewClear(0
                    , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
                    , 0x303030ff
                    , 1.0f
                    , 0
            );

            const bgfx::RendererType::Enum renderer = bgfx::getRendererType();
            float texelHalf = bgfx::RendererType::Direct3D9 == renderer ? 0.5f : 0.0f;
            bool originBottomLeft = bgfx::RendererType::OpenGL == renderer
                                    || bgfx::RendererType::OpenGLES == renderer;

            imguiCreate();

            m_nvg = nvgCreate(1, 0);

            createFont(m_nvg, "sans", "droidsans.ttf");

            GUI::GEngine::getGEngine().setDisplay(m_nvg, m_width, m_height);

            S = new Synth();
            S->GPlace({0, 0});
            S->GSetHeight(1);
            S->GSetWidth(1);

            eng = new AMGEngine(S);

            eng->audio->start();
        }

        virtual int shutdown() override
        {
            imguiDestroy();

//            m_vd.teardown();

            nvgDelete(m_nvg);

            // Shutdown bgfx.
            bgfx::shutdown();

            return 0;
        }

        bool update() override
        {


            if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
            {
                imguiBeginFrame(m_mouseState.m_mx
                        ,  m_mouseState.m_my
                        , (m_mouseState.m_buttons[entry::MouseButton::Left  ] ? IMGUI_MBUT_LEFT   : 0)
                          | (m_mouseState.m_buttons[entry::MouseButton::Right ] ? IMGUI_MBUT_RIGHT  : 0)
                          | (m_mouseState.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
                        ,  m_mouseState.m_mz
                        , uint16_t(m_width)
                        , uint16_t(m_height)
                );

                showExampleDialog(this);

                imguiEndFrame();

                eng->graphic->Render();

                return true;
            }

            return false;
        }

        entry::MouseState m_mouseState;

        NVGcontext* m_nvg;

        AMGEngine * eng;

        Synth * S;

        uint32_t m_width;
        uint32_t m_height;
        uint32_t m_debug;
        uint32_t m_reset;
    };

} // namespace

ENTRY_IMPLEMENT_MAIN(
        MainActivity
, "00-helloworld"
, "Initialization and debug text."
, "https://bkaradzic.github.io/bgfx/examples.html#helloworld"
);