#include <shader.h>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

shader::shader(const char *vertex_path, const char *fragment_path)
{
    // 1. retrieve vertex/fragment shader source from filePath.
    std::string vertex_source;
    std::string fragment_source;
    std::ifstream vertex_shader_file;
    std::ifstream fragment_shader_file;
    // ensure ifstream objects can throw exceptions:
    vertex_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open file paths.
        vertex_shader_file.open(vertex_path);
        fragment_shader_file.open(fragment_path);
        std::stringstream vertex_shader_stream;
        std::stringstream fragment_shader_stream;
        // read file's buffer contents into streams.
        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();
        // close file handlers.
        vertex_shader_file.close();
        fragment_shader_file.close();
        // convert stream into string.
        vertex_source = vertex_shader_stream.str();
        fragment_source = fragment_shader_stream.str();
    } catch (std::ifstream::failure const&) {  
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    
    const char *vertex_shader_source = vertex_source.c_str();
    const char *fragment_shader_source = fragment_source.c_str();

    // 2. compile shaders.
    unsigned int vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_source, NULL);
    
    glCompileShader(vertex_shader_id);
    // ensure vertex shader compilation.
    int vertex_shader_compilation;  // indictation of compilation success.
    char vertex_shader_info_log[512];
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &vertex_shader_compilation);  // assign success to vertex_shader_compilation.
    if (!vertex_shader_compilation) {
        glGetShaderInfoLog(vertex_shader_id, 512, NULL, vertex_shader_info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << vertex_shader_info_log << std::endl;
    }

    unsigned int fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_source, NULL);
    
    glCompileShader(fragment_shader_id);
    // ensure fragment shader compilation.
    int fragment_shader_compilation;  // indication of compilation success.
    char fragment_shader_info_log[512];  // TODO: create global or generic info log?
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &fragment_shader_compilation);  // assign success to fragment_shader_compilation.
    if (!fragment_shader_compilation) {
        glGetShaderInfoLog(fragment_shader_id, 512, NULL, fragment_shader_info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << fragment_shader_info_log << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader_id);
    glAttachShader(ID, fragment_shader_id);
    
    glLinkProgram(ID);
    // ensure shader program linking success.
    int ID_linking = 0;  // indication of linking success.
    char ID_info_log[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &ID_linking);
    if (!ID_linking) {
        glGetProgramInfoLog(ID, 512, NULL, ID_info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << ID_info_log << std::endl;
    }

    // remove unecessary shaders.
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}

void shader::use()
{
    glUseProgram(ID);
}

void shader::set_bool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void shader::set_int(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::set_float(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::set_float4(const std::string &name, float value[4]) const
{
    glUniform4f(glGetUniformLocation(ID, name.c_str()), value[0], value[1], value[2], value[3]);
}
