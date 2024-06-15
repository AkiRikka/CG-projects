#include "render_flag.h"

RenderFlag::RenderFlag(const Options& options) : Application(options) {
    // create star shader
    const char* vsCode =
        "#version 330 core\n"
        "layout(location = 0) in vec2 aPosition;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPosition, 0.0f, 1.0f);\n"
        "}\n";

    const char* fsCode =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "    fragColor = vec4(1.0f, 0.870f, 0.0f, 1.0f);\n"
        "}\n";

    _starShader.reset(new GLSLProgram);
    _starShader->attachVertexShader(vsCode);
    _starShader->attachFragmentShader(fsCode);
    _starShader->link();

    // TODO: create 5 stars
    // hint: aspect_of_the_window = _windowWidth / _windowHeight
    // write your code here
    // ---------------------------------------------------------------
    // _stars[i].reset(new Star(ndc_position, rotation_in_radians, size_of_star,
    // aspect_of_the_window));
    // ---------------------------------------------------------------

    float aspect = static_cast<float>(_windowWidth) / _windowHeight;

    _stars[0].reset(new Star(glm::vec2(-0.66f, 0.5f), glm::radians(18.0f), 0.2f, aspect));
    _stars[1].reset(new Star(glm::vec2(-0.33f, 0.8f), glm::radians(210.964f), 0.066f, aspect));
    _stars[2].reset(new Star(glm::vec2(-0.2f, 0.6f), glm::radians(188.130f), 0.066f, aspect));
    _stars[3].reset(new Star(glm::vec2(-0.2f, 0.3f), glm::radians(164.0546f), 0.066f, aspect));
    _stars[4].reset(new Star(glm::vec2(-0.33f, 0.1f), glm::radians(141.3402f), 0.066f, aspect));

}

void RenderFlag::handleInput() {
    if (_input.keyboard.keyStates[GLFW_KEY_ESCAPE] != GLFW_RELEASE) {
        glfwSetWindowShouldClose(_window, true);
        return;
    }
}

void RenderFlag::renderFrame() {
    showFpsInWindowTitle();

    // we use background as the flag
    glClearColor(0.87f, 0.161f, 0.063f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    _starShader->use();
    for (int i = 0; i < 5; ++i) {
        if (_stars[i] != nullptr) {
            _stars[i]->draw();
        }
    }
}