#include <cassert>
#include <stb_image.h>

#include "texture_cubemap.h"

TextureCubemap::TextureCubemap(
    GLint internalFormat, int width, int height, GLenum format, GLenum dataType) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    for (uint32_t i = 0; i < 6; ++i) {
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format,
            dataType, nullptr);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureCubemap::TextureCubemap(TextureCubemap&& rhs) noexcept : Texture(std::move(rhs)) {}

void TextureCubemap::bind(int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);
}

void TextureCubemap::unbind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void TextureCubemap::generateMipmap() const {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void TextureCubemap::setParamterInt(GLenum name, int value) const {
    glTexParameteri(GL_TEXTURE_CUBE_MAP, name, value);
}

ImageTextureCubemap::ImageTextureCubemap(const std::vector<std::string>& filepaths)
    : _uris(filepaths) {
    assert(filepaths.size() == 6);
    // TODO: load six images and generate the texture cubemap
    // hint: you can refer to Texture2D(const std::string&) for image loading
    // write your code here
    // -----------------------------------------------
    // ...
    glGenTextures(1, &_handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _handle);

    int width, height, nrChannels;
    unsigned char *data;
    for (unsigned int i = 0; i < filepaths.size(); i++)
    {
        data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);
        if (data == nullptr) {
            cleanup();
            throw std::runtime_error("load" + filepaths[i] + "failure");
        }

        // choose image format
        GLenum format = GL_RGB;
        switch (nrChannels) {
        case 1: format = GL_RED;  break;
        case 3: format = GL_RGB;  break;
        case 4: format = GL_RGBA; break;
        default:
            cleanup();
            stbi_image_free(data);
            throw std::runtime_error("unsupported format");
        }

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
            0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    stbi_image_free(data);

    check();
    // -----------------------------------------------
}

ImageTextureCubemap::ImageTextureCubemap(ImageTextureCubemap&& rhs) noexcept
    : TextureCubemap(std::move(rhs)), _uris(std::move(rhs._uris)) {
    rhs._uris.clear();
}

const std::vector<std::string>& ImageTextureCubemap::getUris() const {
    return _uris;
}