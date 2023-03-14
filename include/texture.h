#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

class texture
{
public:
    unsigned int ID;  // texture ID.

    texture(const char *image_path);

    void bind();  // bind/use the texture to its ID.
};

#endif  // TEXTURE_H
