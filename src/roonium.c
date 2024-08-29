#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <malloc.h>
#include <roonmath.h>

#include "shader.vs.h"
#include "shader.fs.h"
#include "roon.h"
#include "roon_icon.h"

/* stb_image implementation? */
unsigned char *stbi_load_from_memory(unsigned char const *buffer, int len, int *x, int *y, int *channels_in_file, int desired_channels);

#ifndef UNUSED
#define UNUSED(_v) (void)(_v)
#endif

/* Vertex. */
typedef struct roonium_vertex
{
  roonium_vector3 position;
  roonium_vector3 normals;
  roonium_vector2 texture_coordinates;
} roonium_vertex;

/* Static mesh. */
typedef struct roonium_mesh
{
  GLuint vbo, vao;
  roonium_vertex *vertices;
  size_t vertices_count;
} roonium_mesh;

/* Static mesh. */
typedef struct roonium_camera3d
{
  roonium_vector3 position;
  roonium_vector3 up;
  roonium_vector3 target;
  float fov;
  float aspect;
} roonium_camera3d;

typedef struct roonium_app_settings
{
  int window_width;
  int window_height;
  const char *window_title;
  int window_target_fps;
} roonium_app_settings;

typedef struct roonium_app
{
  GLFWwindow *window;
  bool window_quit;
  int frames_count;
  int fps;
  double frames_time_last_fps;
  double frames_time_last;
  double frames_time_now;

  GLuint shader, texture;
  struct roonium_mesh mesh;
  struct roonium_camera3d camera;

  roonium_app_settings settings;
} roonium_app;

struct roonium_mesh generate_mesh_pyramid(
    const float _x,
    const float _y,
    const float _z)
{
  struct roonium_mesh mesh;
  roonium_vector3 n, va, vb, vc;
  roonium_vector2 ta, tb, tc;
  int i = 0;

  mesh.vertices_count = 18;
  mesh.vertices = malloc(
      mesh.vertices_count *
      sizeof(struct roonium_vertex));

  /* Toward. */
  {
    va.x = -_x / 2.0f;
    va.y = -_y / 2.0;
    va.z = _z / 2.0f;
    ta.x = 0.0f;
    ta.y = 0.0f;

    vb.x = _x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = _z / 2.0f;
    tb.x = 1.0f;
    tb.y = 0.0f;

    vc.x = 0.0f;
    vc.y = _y / 2.0;
    vc.z = 0.0f;
    tc.x = 0.5f;
    tc.y = 1.0f;

    n = get_normal(va, vb, vc);

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;

    i += 3;
  }

  /* Backward. */
  {
    va.x = _x / 2.0f;
    va.y = -_y / 2.0;
    va.z = -_z / 2.0f;
    ta.x = 0.0f;
    ta.y = 0.0f;
    vb.x = -_x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = -_z / 2.0f;
    tb.x = 1.0f;
    tb.y = 0.0f;
    vc.x = 0.0f;
    vc.y = _y / 2.0;
    vc.z = 0.0f;
    tc.x = 0.5f;
    tc.y = 1.0f;

    n = get_normal(va, vb, vc);

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;

    i += 3;
  }

  /* left. */
  {
    va.x = -_x / 2.0f;
    va.y = -_y / 2.0;
    va.z = -_z / 2.0f;
    ta.x = 0.0f;
    ta.y = 0.0f;
    vb.x = -_x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = _z / 2.0f;
    tb.x = 1.0f;
    tb.y = 0.0f;
    vc.x = 0.0f;
    vc.y = _y / 2.0;
    vc.z = 0.0f;
    tc.x = 0.5f;
    tc.y = 1.0f;

    n = get_normal(va, vb, vc);

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;

    i += 3;
  }

  /* Right. */
  {
    va.x = _x / 2.0f;
    va.y = -_y / 2.0;
    va.z = _z / 2.0f;
    ta.x = 0.0f;
    ta.y = 0.0f;
    vb.x = _x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = -_z / 2.0f;
    tb.x = 1.0f;
    tb.y = 0.0f;
    vc.x = 0.0f;
    vc.y = _y / 2.0;
    vc.z = 0.0f;
    tc.x = 0.5f;
    tc.y = 1.0f;

    n = get_normal(va, vb, vc);

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;

    i += 3;
  }

  /* Bottom square. */
  {
    va.x = -_x / 2.0f;
    va.y = -_y / 2.0;
    va.z = -_z / 2.0f;
    ta.x = 0.0f;
    ta.y = 1.0f;
    vb.x = _x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = -_z / 2.0f;
    tb.x = 1.0f;
    tb.y = 1.0f;
    vc.x = -_x / 2.0f;
    vc.y = -_y / 2.0;
    vc.z = _z / 2.0f;
    tc.x = 0.0f;
    tc.y = 0.0f;

    n = get_normal(va, vb, vc);

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;
    i += 3;

    va.x = -_x / 2.0f;
    va.y = -_y / 2.0;
    va.z = _z / 2.0f;
    ta.x = 1.0f;
    ta.y = 0.0f;
    vb.x = _x / 2.0f;
    vb.y = -_y / 2.0;
    vb.z = -_z / 2.0f;
    tb.x = 0.0f;
    tb.y = 1.0f;
    vc.x = _x / 2.0f;
    vc.y = -_y / 2.0;
    vc.z = _z / 2.0f;
    tc.x = 0.0f;
    tc.y = 0.0f;

    mesh.vertices[i + 0].position = va;
    mesh.vertices[i + 1].position = vb;
    mesh.vertices[i + 2].position = vc;
    mesh.vertices[i + 0].normals = n;
    mesh.vertices[i + 1].normals = n;
    mesh.vertices[i + 2].normals = n;
    mesh.vertices[i + 0].texture_coordinates = ta;
    mesh.vertices[i + 1].texture_coordinates = tb;
    mesh.vertices[i + 2].texture_coordinates = tc;
  }

  glGenVertexArrays(1, &mesh.vao);
  glBindVertexArray(mesh.vao);
  glGenBuffers(1, &mesh.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
  glBufferData(
      GL_ARRAY_BUFFER,
      mesh.vertices_count * sizeof(roonium_vertex),
      mesh.vertices,
      GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(
      0,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(roonium_vertex),
      (GLvoid *)offsetof(roonium_vertex, position));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(
      1,
      3,
      GL_FLOAT,
      GL_FALSE,
      sizeof(roonium_vertex),
      (GLvoid *)offsetof(roonium_vertex, normals));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(
      2,
      2,
      GL_FLOAT,
      GL_FALSE,
      sizeof(roonium_vertex),
      (GLvoid *)offsetof(roonium_vertex, texture_coordinates));

  return mesh;
}

void draw_mesh(
    const struct roonium_mesh _mesh)
{
  glBindVertexArray(_mesh.vao);
  glDrawArrays(
      GL_TRIANGLES,
      0,
      _mesh.vertices_count);
  glBindVertexArray(0);
}

void camera3d_get_projection(
    roonium_matrix _destination,
    const struct roonium_camera3d _camera)
{
  matrix_identity(_destination);
  matrix_perspective(
      _destination,
      _camera.fov,
      _camera.aspect,
      0.1f,
      100.0f);
}

void camera3d_get_view(
    roonium_matrix _destination,
    const struct roonium_camera3d _camera)
{
  matrix_identity(_destination);
  matrix_look_at(
      _destination,
      _camera.position,
      _camera.target,
      _camera.up);
}

GLuint load_shader_from_code(
    const char *_vs_code,
    const char *_fs_code)
{
  GLuint id, v_id, f_id;
  int program_validate = 0;
  char info[512];

  v_id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(v_id, 1, &_vs_code, NULL);
  glCompileShader(v_id);

  f_id = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(f_id, 1, &_fs_code, NULL);
  glCompileShader(f_id);

  id = glCreateProgram();
  glAttachShader(id, v_id);
  glAttachShader(id, f_id);
  glLinkProgram(id);

  glValidateProgram(id);
  glGetProgramiv(id, GL_VALIDATE_STATUS, &program_validate);

  if (!program_validate)
  {
    glGetProgramInfoLog(id, 512, NULL, info);
    printf("Shader compilation error: %s\n", info);
    glDeleteShader(v_id);
    glDeleteShader(f_id);
    glDeleteProgram(id);

    id = (GLuint)-1;

    return id;
  };

  glDeleteShader(v_id);
  glDeleteShader(f_id);

  return id;
}

GLuint load_texture_from_memory(
    const unsigned char *_data,
    const size_t _size)
{
  GLuint id;
  int w, h;
  uint8_t *data;
  data = stbi_load_from_memory(
      _data,
      _size,
      &w,
      &h,
      0,
      4);

  if (!data)
    return -1;

  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);

  free(data);

  return id;
}

void roonium_app__swap_buffers(
    struct roonium_app *_app)
{
  glfwSwapBuffers(_app->window);
  /* Set end drawing time. */
  _app->frames_time_last = _app->frames_time_now;
}

void roonium_app__begin_frame(
    struct roonium_app *_app)
{
  _app->frames_count++;
  _app->frames_time_now = glfwGetTime();
  if (_app->frames_time_now - _app->frames_time_last_fps > 1.0)
  {
    _app->frames_time_last_fps = _app->frames_time_now;
    _app->fps = _app->frames_count;
    _app->frames_count = 0;
  }

  glfwGetFramebufferSize(
      _app->window,
      &_app->settings.window_width,
      &_app->settings.window_height);

  _app->camera.aspect =
      (float)_app->settings.window_width /
      (float)_app->settings.window_height;

  glViewport(
      0,
      0,
      _app->settings.window_width,
      _app->settings.window_height);
  glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  /* Draw.*/
  glUseProgram(_app->shader);
}

int roonium_app__cull_frame(
    struct roonium_app *_app)
{
  _app->frames_time_now = glfwGetTime();

  if ((_app->frames_time_now - _app->frames_time_last) <=
      (1.0 / (double)_app->settings.window_target_fps))
  {
    return 1;
  }
  return 0;
}

static void error_callback(int _error, const char *_description)
{
  UNUSED(_error);
  fprintf(stderr, "Error: %s\n", _description);
}

int roonium_app__init(
    struct roonium_app *_app)
{
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
  {
    printf("Cannot initialize GLFW.\n");
    return 1;
  }
  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 1);

  _app->settings.window_width = 800;
  _app->settings.window_height = 600;
  _app->settings.window_title = "Roonium";
  _app->settings.window_target_fps = 60;
  _app->frames_count = 0;
  _app->frames_time_last_fps = 0.0;
  _app->frames_time_last = 0.0;
  _app->frames_time_now = 0.0;
  _app->fps = _app->settings.window_target_fps;

  _app->camera.position.x = 0.0f;
  _app->camera.position.y = 1.0f;
  _app->camera.position.z = 3.0f;
  _app->camera.target.x = 0.0f;
  _app->camera.target.y = 0.0f;
  _app->camera.target.z = 0.0f;
  _app->camera.up.x = 0.0f;
  _app->camera.up.y = 1.0f;
  _app->camera.up.z = 0.0f;
  _app->camera.fov = 45.0f;
  _app->camera.aspect = 800.0f / 600.0f;

  return 0;
}

int roonium_app__run(
    struct roonium_app *_app)
{
  char title[512];
  GLFWimage window_icon;
  roonium_matrix projection, view, model;

  _app->window = glfwCreateWindow(
      _app->settings.window_width,
      _app->settings.window_height,
      _app->settings.window_title,
      NULL,
      NULL);
  _app->window_quit = false;

  glfwMakeContextCurrent(_app->window);
  gladLoadGL();
  glEnable(GL_BLEND);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  _app->mesh = generate_mesh_pyramid(1.25f, 1.0f, 1.25f);
  _app->shader = load_shader_from_code(
      (const char *)RESOURCES_SHADER_VS,
      (const char *)RESOURCES_SHADER_FS);
  _app->texture = load_texture_from_memory(
      RESOURCES_ROON_JPG,
      RESOURCES_ROON_JPG_SIZE);

  /* Set icon */
  {
    window_icon.pixels = stbi_load_from_memory(
        RESOURCES_ROON_ICON_PNG,
        RESOURCES_ROON_ICON_PNG_SIZE,
        &window_icon.width,
        &window_icon.height,
        0,
        4);

    glfwSetWindowIcon(
        _app->window,
        1,
        &window_icon);

    window_icon.width = 0;
    window_icon.height = 0;
    free(window_icon.pixels);
  }

  while (!_app->window_quit)
  {
    /* Fixed FPS. */
    if (roonium_app__cull_frame(_app))
    {
      continue;
    }

    /* Events. */
    {
      glfwPollEvents();
      _app->window_quit |= glfwWindowShouldClose(_app->window);
      _app->window_quit |= glfwGetKey(_app->window, GLFW_KEY_ESCAPE);

      sprintf(
          title,
          "Roonium; FPS: %i",
          _app->fps);

      glfwSetWindowTitle(_app->window, title);
    }

    /* Set shader uniforms. */
    {
      camera3d_get_projection(
          projection,
          _app->camera);
      glUniformMatrix4fv(
          glGetUniformLocation(
              _app->shader,
              "u_projection"),
          1,
          GL_FALSE,
          (const float *)&projection);

      camera3d_get_view(view, _app->camera);
      glUniformMatrix4fv(
          glGetUniformLocation(_app->shader, "u_view"),
          1,
          GL_FALSE,
          (const float *)&view);

      matrix_identity(model);
      matrix_rotate_y(model, model, (float)glfwGetTime() * 3.0f);
      glUniformMatrix4fv(
          glGetUniformLocation(_app->shader, "u_model"),
          1,
          GL_FALSE,
          (const float *)&model);
    }

    /* Drawing */
    {
      roonium_app__begin_frame(_app);

      glUseProgram(_app->shader);
      glBindTexture(GL_TEXTURE_2D, _app->texture);
      draw_mesh(_app->mesh);

      roonium_app__swap_buffers(_app);
    }
  }

  return 0;
}

void roonium_app__quit(
    struct roonium_app *_app)
{

  glUseProgram(0);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDeleteProgram(_app->shader);
  glDeleteBuffers(1, &_app->mesh.vbo);
  glDeleteVertexArrays(1, &_app->mesh.vao);
  free(_app->mesh.vertices);
  glfwDestroyWindow(_app->window);
  glfwTerminate();
}

int main()
{
  struct roonium_app app;

  if (roonium_app__init(&app) ||
      roonium_app__run(&app))
    return 1;

  roonium_app__quit(&app);

  return 0;
}
