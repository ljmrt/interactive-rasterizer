#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class shader
{
public:
    unsigned int ID;  // program ID

    shader(const char *vertex_path, const char *fragment_path);

    void use();  // use/activate the shader
    // utility uniform functions
    void set_bool(const std::string &name, bool value) const;
    void set_int(const std::string &name, int value) const;
    void set_float(const std::string &name, float value) const;
    void set_float4(const std::string &name, float value[4]) const;
};

#endif  // SHADER_H
