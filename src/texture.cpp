#include <texture.h>
#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <iostream>

texture::texture(const char *texture_path)
{
    glGenTextures(1, &ID);
    glActiveTexture(33983 + ID);  // activate the correct texture corresponding to the ID.
    glBindTexture(GL_TEXTURE_2D, ID);
    
    // texture parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // set x axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);  // set y axis to use mirrored repeating textures.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // set minifying operations to use linear interpolation of the two closest mipmaps.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // set magnifying operations to use bilinear filtering(no mipmap operations as they are not used in scaling up).

    // load texture image.
    int width, height, num_color_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *image_data = stbi_load(texture_path, &width, &height, &num_color_channels, 0);
    unsigned int image_format = 6404 + num_color_channels;  // use the correct format depending on the color channels.
    if (image_data) {
        // generate texture using image data.
        // function arguments(in reference to the texture): target, mipmap level, format(RGB[A]), width, height, 0(legacy stuff), format, datatype, image data.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, image_format, GL_UNSIGNED_BYTE, image_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "ERROR::TEXTURE_" << ID << "::IMAGE::LOAD_FAILED\n" << std::endl;
    }
    stbi_image_free(image_data);
}

void texture::bind()
{
    glActiveTexture(33983 + ID);  // activate the correct texture corresponding to the ID.
    glBindTexture(GL_TEXTURE_2D, ID);
}
