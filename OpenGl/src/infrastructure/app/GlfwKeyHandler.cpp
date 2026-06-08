#include "infra/app/GlfwKeyHandler.h"

Engine::Infra::GlfwKeyHandler::GlfwKeyHandler()
{
    static const std::unordered_map<int, Engine::Core::KeyCode> keyMap = {
        // Numbers
        {GLFW_KEY_0, Engine::Core::KeyCode::_0},
        {GLFW_KEY_1, Engine::Core::KeyCode::_1},
        {GLFW_KEY_2, Engine::Core::KeyCode::_2},
        {GLFW_KEY_3, Engine::Core::KeyCode::_3},
        {GLFW_KEY_4, Engine::Core::KeyCode::_4},
        {GLFW_KEY_5, Engine::Core::KeyCode::_5},
        {GLFW_KEY_6, Engine::Core::KeyCode::_6},
        {GLFW_KEY_7, Engine::Core::KeyCode::_7},
        {GLFW_KEY_8, Engine::Core::KeyCode::_8},
        {GLFW_KEY_9, Engine::Core::KeyCode::_9},

        // Letters
        {GLFW_KEY_A, Engine::Core::KeyCode::A},
        {GLFW_KEY_B, Engine::Core::KeyCode::B},
        {GLFW_KEY_C, Engine::Core::KeyCode::C},
        {GLFW_KEY_D, Engine::Core::KeyCode::D},
        {GLFW_KEY_E, Engine::Core::KeyCode::E},
        {GLFW_KEY_F, Engine::Core::KeyCode::F},
        {GLFW_KEY_G, Engine::Core::KeyCode::G},
        {GLFW_KEY_H, Engine::Core::KeyCode::H},
        {GLFW_KEY_I, Engine::Core::KeyCode::I},
        {GLFW_KEY_J, Engine::Core::KeyCode::J},
        {GLFW_KEY_K, Engine::Core::KeyCode::K},
        {GLFW_KEY_L, Engine::Core::KeyCode::L},
        {GLFW_KEY_M, Engine::Core::KeyCode::M},
        {GLFW_KEY_N, Engine::Core::KeyCode::N},
        {GLFW_KEY_O, Engine::Core::KeyCode::O},
        {GLFW_KEY_P, Engine::Core::KeyCode::P},
        {GLFW_KEY_Q, Engine::Core::KeyCode::Q},
        {GLFW_KEY_R, Engine::Core::KeyCode::R},
        {GLFW_KEY_S, Engine::Core::KeyCode::S},
        {GLFW_KEY_T, Engine::Core::KeyCode::T},
        {GLFW_KEY_U, Engine::Core::KeyCode::U},
        {GLFW_KEY_V, Engine::Core::KeyCode::V},
        {GLFW_KEY_W, Engine::Core::KeyCode::W},
        {GLFW_KEY_X, Engine::Core::KeyCode::X},
        {GLFW_KEY_Y, Engine::Core::KeyCode::Y},
        {GLFW_KEY_Z, Engine::Core::KeyCode::Z},

        // Function keys
        //{GLFW_KEY_F1,  Engine::Core::KeyCode::F1},
        //{GLFW_KEY_F2,  Engine::Core::KeyCode::F2},
        //{GLFW_KEY_F3,  Engine::Core::KeyCode::F3},
        //{GLFW_KEY_F4,  Engine::Core::KeyCode::F4},
        //{GLFW_KEY_F5,  Engine::Core::KeyCode::F5},
        //{GLFW_KEY_F6,  Engine::Core::KeyCode::F6},
        //{GLFW_KEY_F7,  Engine::Core::KeyCode::F7},
        //{GLFW_KEY_F8,  Engine::Core::KeyCode::F8},
        //{GLFW_KEY_F9,  Engine::Core::KeyCode::F9},
        //{GLFW_KEY_F10, Engine::Core::KeyCode::F10},
        //{GLFW_KEY_F11, Engine::Core::KeyCode::F11},
        //{GLFW_KEY_F12, Engine::Core::KeyCode::F12},

        // Arrow keys
        {GLFW_KEY_UP,    Engine::Core::KeyCode::Up},
        {GLFW_KEY_DOWN,  Engine::Core::KeyCode::Down},
        {GLFW_KEY_LEFT,  Engine::Core::KeyCode::Left},
        {GLFW_KEY_RIGHT, Engine::Core::KeyCode::Right},

        // Control keys
        {GLFW_KEY_SPACE,      Engine::Core::KeyCode::Space},
        {GLFW_KEY_ENTER,      Engine::Core::KeyCode::Enter},
        {GLFW_KEY_ESCAPE,     Engine::Core::KeyCode::Escape},
        {GLFW_KEY_TAB,        Engine::Core::KeyCode::Tab},
        {GLFW_KEY_BACKSPACE,  Engine::Core::KeyCode::Backspace},
       //{GLFW_KEY_DELETE,     Engine::Core::KeyCode::Delete},
       //{GLFW_KEY_INSERT,     Engine::Core::KeyCode::Insert},
       //{GLFW_KEY_HOME,       Engine::Core::KeyCode::Home},
       //{GLFW_KEY_END,        Engine::Core::KeyCode::End},
       //{GLFW_KEY_PAGE_UP,    Engine::Core::KeyCode::PageUp},
       //{GLFW_KEY_PAGE_DOWN,  Engine::Core::KeyCode::PageDown},

        // Modifiers
        {GLFW_KEY_LEFT_SHIFT,    Engine::Core::KeyCode::LeftShift},
        {GLFW_KEY_RIGHT_SHIFT,   Engine::Core::KeyCode::RightShift},
        {GLFW_KEY_LEFT_CONTROL,  Engine::Core::KeyCode::LeftCtrl},
        {GLFW_KEY_RIGHT_CONTROL, Engine::Core::KeyCode::RightCtrl},
        {GLFW_KEY_LEFT_ALT,      Engine::Core::KeyCode::LeftAlt},
        {GLFW_KEY_RIGHT_ALT,     Engine::Core::KeyCode::RightAlt},
    };
	glfwKeyToKeyCode = keyMap;
}

Engine::Core::KeyCode Engine::Infra::GlfwKeyHandler::getInput(int glfwKey)
{
    auto it = glfwKeyToKeyCode.find(glfwKey);
    if (it != glfwKeyToKeyCode.end()) {
        return it->second;
    }
    return Engine::Core::KeyCode::None;
}

Engine::Core::KeyAction Engine::Infra::GlfwKeyHandler::getAction(int glfwAction)
{
    switch (glfwAction)
    {
    case GLFW_PRESS: return Core::KeyAction::Down; break;
    case GLFW_RELEASE: return Core::KeyAction::Up; break;
    case GLFW_REPEAT: return Core::KeyAction::Held; break;
    }
    return Core::KeyAction::Up;
}
