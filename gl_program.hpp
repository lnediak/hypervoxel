#include "glad/gl.h"

struct GLProgram {

  GLuint prog;

  GLProgram() : prog(0) {}

  ~GLProgram() {
    if (!prog) {
      glDeleteProgram(prog);
    }
  }

  void compileProgram(const std::string &vsrc, const std::string &fsrc) {
    GLuint vs = createShader(vsrc, GL_VERTEX_SHADER);
    GLuint fs = createShader(fsrc, GL_VERTEX_SHADER);

    GLuint pro = glCreateProgram();
    if (!pro) {
      throw std::runtime_error("Failed to create OpenGL program.");
    }
    glAttachShader(pro, vs);
    glAttachShader(pro, fs);
    glLinkProgram(pro);
    glDetachShader(pro, vs);
    glDetachShader(pro, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint success = 0;
    glGetProgramiv(pro, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
      GLint llen = 0;
      glGetProgramiv(pro, GL_INFO_LOG_LENGTH, &llen);
      if (!llen) {
        glDeleteProgram(pro);
        throw std::runtime_error("Failed to link program.");
      }
      std::string infoLog(llen);
      glGetProgramInfoLog(pro, llen, NULL, &infoLog[0]);
      glDeleteProgram(pro);
      throw std::runtime_error("Failed to link program. Info Log:\n" + infoLog);
    }
    prog = pro;
  }

private:
  GLuint createShader(const std::string &src, GLenum stype) const {
    const char *csrc = str.c_str();
    const GLint slen = str.length();

    GLuint shad = glCreateShader(stype);
    if (!shad) {
      throw std::runtime_error("Failed to create OpenGL shader.");
    }
    glShaderSource(shad, 1, &csrc, &slen);
    glCompileShader(shad);

    GLint success = 0;
    glGetShaderiv(shad, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
      GLint llen = 0;
      glGetShaderiv(shad, GL_INFO_LOG_LENGTH, &llen);
      if (!llen) {
        glDeleteShader(shad);
        throw std::runtime_error("Failed to compile shader. Source:\n" + src);
      }
      std::string infoLog(llen);
      glGetShaderInfoLog(shad, llen, NULL, &infoLog[0]);
      glDeleteShader(shad);
      throw std::runtime_error("Failed to compile shader. Source:\n" + src + "\n\nInfo Log:\n" + infoLog);
    }
    return shad;
  }
};

