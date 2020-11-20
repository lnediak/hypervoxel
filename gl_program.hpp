#include "glad/gl.h"

#include <stdexcept>
#include <string>

struct GLProgram {

  GLuint prog;
  GLuint posLoc;
  GLuint colLoc;
  GLuint projMatLoc;
  GLuint buf;

  GLProgram() : prog(0) {}
  GLProgram(const GLProgram &) = delete;
  GLProgram(GLProgram &&other) : prog(other.prog) { other.prog = 0; }
  GLProgram &operator=(const GLProgram &) = delete;
  GLProgram &operator=(GLProgram &&other) {
    if (prog != other.prog) {
      this->~GLProgram();
      prog = other.prog;
    }
    other.prog = 0;
    return *this;
  }
  ~GLProgram() {
    if (prog) {
      glDeleteProgram(prog);
    }
  }

  void compileProgram(std::size_t lenTriangles) {
    std::string vsrc = "#version 110\n"
                       "attribute vec3 pos;"
                       "uniform mat4 projMat;"
                       "attribute vec4 col;"
                       "varying vec4 outCol;"
                       "void main() {"
                       "  gl_Position = projMat * vec4(pos, 1.0);"
                       "  outCol = col;"
                       "}";
    std::string fsrc = "#version 110\n"
                       "varying vec4 outCol;"
                       "void main() {"
                       "  gl_FragColor = outCol;"
                       "}";
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
      std::string infoLog(llen, ' ');
      glGetProgramInfoLog(pro, llen, NULL, &infoLog[0]);
      glDeleteProgram(pro);
      throw std::runtime_error("Failed to link program. Info Log:\n" + infoLog);
    }
    if (prog) {
      glDeleteProgram(prog);
    }
    prog = pro;
    glUseProgram(prog);

    posLoc = glGetAttribLocation(prog, "pos");
    colLoc = glGetAttribLocation(prog, "col");
    projMatLoc = glGetUniformLocation(prog, "projMat");

    glGenBuffers(1, &buf);
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferData(GL_ARRAY_BUFFER, lenTriangles * sizeof(float), nullptr,
                 GL_DYNAMIC_DRAW);
  }

  void setProjMat(GLfloat width2, GLfloat height2, GLfloat dist) {
    GLfloat mat[][4] = {{1 / width2, 0, 0, 0},
                        {0, 1 / height2, 0, 0},
                        {0, 0, (dist + 2) / dist, 2 * (dist + 1) / dist},
                        {0, 0, 1, 0}};
    glUniformMatrix4fv(projMatLoc, 1, GL_TRUE, &mat[0][0]);
  }

  void renderTriangles(float *triangles, float *triangles_end) {
    glBindBuffer(GL_ARRAY_BUFFER, buf);
    glBufferSubData(GL_ARRAY_BUFFER, 0,
                    (triangles_end - triangles) * sizeof(float), triangles);

    glEnableVertexAttribArray(posLoc);
    glEnableVertexAttribArray(colLoc);
    glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          nullptr);
    glVertexAttribPointer(colLoc, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float),
                          static_cast<float *>(nullptr) + 3);

    glDrawArrays(GL_TRIANGLES, 0, (triangles_end - triangles) / 7);
  }

private:
  GLuint createShader(const std::string &src, GLenum stype) const {
    const char *csrc = src.c_str();
    const GLint slen = src.length();

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
        throw std::runtime_error(std::string() +
                                 "Failed to compile shader. Source:\n" + src);
      }
      std::string infoLog(llen, ' ');
      glGetShaderInfoLog(shad, llen, NULL, &infoLog[0]);
      glDeleteShader(shad);
      throw std::runtime_error(std::string() +
                               "Failed to compile shader. Source:\n" + src +
                               "\n\nInfo Log:\n" + infoLog);
    }
    return shad;
  }
};

