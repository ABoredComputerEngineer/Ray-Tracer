#define OS_LINUX_CPP
#define HANDMADE_MATH_IMPLEMENTATION
#include <stdio.h>
#include <cstdlib>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "common.h"
#include <cassert>
#include <cmath>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "HandmadeMath.h"
#include "ui_primitives.h"
#include "ui_objects.h"


typedef GLuint guint;

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

static uint ScreenWidth = 800;
static uint ScreenHeight = 600;

#define HMM_MAT4_PTR(x) ( &x.Elements[0][0] )
#define HMM_MAT4P_PTR(x) (&( x->Elements[0][0]  ))
#define ENABLE_GL_DEBUG_PRINT 1
#define MS_TO_SEC(x) ( (x) * 1.0e-3f )

static uint *Quad_elem_indices;
static uint quad_elem_buffer_index;
static uint *Line_elem_indices;

static float CubeVertices[] = {
  // front
  0.5f, 0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,

  //back
  0.5f, 0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,

  // right
  0.5f, 0.5f, 0.5f,
  0.5f, -0.5f, 0.5f,
  0.5f, -0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,

  //left
  -0.5f, 0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,

  // top
  0.5f, 0.5f, 0.5f,
  -0.5f, 0.5f, 0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,

  // bottom
  0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, 0.5f,
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
};

static float CubeNormals[] = {
  // front
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
  // back
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
  // right
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    // left
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    // top
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    // bottom
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
};



struct Camera {
  union {
    struct {
      v3 S, U, F;
    };
    v3 basis[3];
  };
  v3 P; 
  enum CameraState {
    ANIMATING = 1,
    STATIC
  };

  bool should_rotate;
  bool should_move;
  CameraState state;
  f32 duration;
  f32 elapsed;
  int dim;
  f32 dist_to_move;
  f32 dist_moved;
  f32 speed;

  f32 pitch; 
  f32 yaw;
  f32 max_pitch;
  f32 max_yaw;

  Plane plane;

  Camera ():max_pitch(80.0f),max_yaw(80.0f){}
  Camera ( const Camera & );
  Camera ( const v3& Eye, const v3& Center, const v3& Up ):
    should_rotate(false),should_move( true ),
    pitch(0.0f),yaw(0.0f),
    max_pitch(80.0f), max_yaw(80.0f) 
  {
    state = STATIC;
    F = HMM_NormalizeVec3(HMM_SubtractVec3(Center, Eye));
    S = HMM_NormalizeVec3(HMM_Cross(F, Up));
    U = HMM_Cross(S, F);
    P = Eye;
  }

  inline void rotate( f32 p, f32 y ){
    if ( !should_rotate ) return;
    pitch += p;
    yaw += y;
    yaw = CLAMP( yaw, -max_yaw, max_yaw );
    f32 cpitch = cos( HMM_RADIANS( pitch ) );
    f32 spitch = sin( HMM_RADIANS( pitch ) );

    f32 cyaw = cos( HMM_RADIANS( yaw ) );
    f32 syaw = sin( HMM_RADIANS( yaw ) );
    
    F.X = -cpitch * cyaw;
    F.Y = -syaw;
    F.Z = -spitch * cyaw ;
    v3 Up = { 0.0f, 1.0f, 0.0f };
    S = HMM_NormalizeVec3(HMM_Cross(F, Up));
    U = HMM_Cross(S, F);

  }

  m4 transform( ) const {
    m4 Result;
    Result.Elements[0][0] = S.X;
    Result.Elements[0][1] = U.X;
    Result.Elements[0][2] = -F.X;
    Result.Elements[0][3] = 0.0f;

    Result.Elements[1][0] = S.Y;
    Result.Elements[1][1] = U.Y;
    Result.Elements[1][2] = -F.Y;
    Result.Elements[1][3] = 0.0f;

    Result.Elements[2][0] = S.Z;
    Result.Elements[2][1] = U.Z;
    Result.Elements[2][2] = -F.Z;
    Result.Elements[2][3] = 0.0f;

    Result.Elements[3][0] = -HMM_DotVec3(S, P);
    Result.Elements[3][1] = -HMM_DotVec3(U, P);
    Result.Elements[3][2] = HMM_DotVec3(F, P);
    Result.Elements[3][3] = 1.0f;

    return (Result);
  }
  
  void move_towards( float t ){
    if ( should_move ) P = P + t * F;
  }
  void move_right( float t ){
    if ( should_move ) P = P + t * S;
  }
  void move_up( float t ){
    if ( should_move ) P = P + t * U;
  }

  void update( float dt ){
    switch ( state ){
      case ANIMATING:
      {
#if 0
        float dist = dt * (float)dist_to_move/duration;
        P += dist * direction;
        elapsed += dt;
        if ( elapsed >= duration ) state = STATIC;
#else
        float dist = dt * speed;
        P += dist * basis[dim];
#endif
        break;
      }
      default:
        break;
    }
  }


  inline void start_animate( int dir,f32 dist, f32 time ){
    if ( !should_move ) { state = STATIC;  return; }
    state = ANIMATING;
    elapsed = 0;
    dim = dir;
    dist_to_move = 1.0f;
    dist_moved = 0.0f;
    speed = dist/MS_TO_SEC( time );
  }


  inline void toggle_move( ){ should_move = !should_move; }

  inline void toggle_rotate(){ should_rotate = !should_rotate; }

  void print( ){
    fprintf( stdout, "Camera Info::\nFront: " );
    print_v3( F );
    fprintf( stdout, "\nRight: " );
    print_v3( S );
    fprintf( stdout, "\nUp: " );
    print_v3( U );
    fprintf( stdout, "\nPoint: " );
    print_v3( P );
    fprintf( stdout, "\n" );
  }

  bool hit_plane(
      const Ray &ray,
      v3 &point )
  {
    float d = HMM_DotVec3( ray.direction, F );
    if ( abs( d ) < TOLERANCE )
      return false;
    v3 temp = P - ray.start;
    float t = HMM_DotVec3( temp, F )/d ;
    point = ray.point_at( t );
    return true;
  }
};

struct GLWindow {
};

enum Keys {
    KB_KEY_A = 0,
    KB_KEY_B,
    KB_KEY_C,
    KB_KEY_D,
    KB_KEY_E,
    KB_KEY_F,
    KB_KEY_G,
    KB_KEY_H,
    KB_KEY_I,
    KB_KEY_J,
    KB_KEY_K,
    KB_KEY_L,
    KB_KEY_M,
    KB_KEY_N,
    KB_KEY_O,
    KB_KEY_P,
    KB_KEY_Q,
    KB_KEY_R,
    KB_KEY_S,
    KB_KEY_T,
    KB_KEY_U,
    KB_KEY_V,
    KB_KEY_W,
    KB_KEY_X,
    KB_KEY_Y,
    KB_KEY_Z,
    KB_KEY_ESCAPE
};

enum EventType {
    MOUSE_RBUTTON_CLICK = 1,
    MOUSE_LBUTTON_CLICK,
    MOUSE_MOVE,
    KB_PRESS_A,
    KB_PRESS_B,
    KB_PRESS_C,
    KB_PRESS_D,
    KB_PRESS_E,
    KB_PRESS_F,
    KB_PRESS_G,
    KB_PRESS_H,
    KB_PRESS_I,
    KB_PRESS_J,
    KB_PRESS_K,
    KB_PRESS_L,
    KB_PRESS_M,
    KB_PRESS_N,
    KB_PRESS_O,
    KB_PRESS_P,
    KB_PRESS_Q,
    KB_PRESS_R,
    KB_PRESS_S,
    KB_PRESS_T,
    KB_PRESS_U,
    KB_PRESS_V,
    KB_PRESS_W,
    KB_PRESS_X,
    KB_PRESS_Y,
    KB_PRESS_Z,
    KB_REPEAT_A,
    KB_REPEAT_B,
    KB_REPEAT_C,
    KB_REPEAT_D,
    KB_REPEAT_E,
    KB_REPEAT_F,
    KB_REPEAT_G,
    KB_REPEAT_H,
    KB_REPEAT_I,
    KB_REPEAT_J,
    KB_REPEAT_K,
    KB_REPEAT_L,
    KB_REPEAT_M,
    KB_REPEAT_N,
    KB_REPEAT_O,
    KB_REPEAT_P,
    KB_REPEAT_Q,
    KB_REPEAT_R,
    KB_REPEAT_S,
    KB_REPEAT_T,
    KB_REPEAT_U,
    KB_REPEAT_V,
    KB_REPEAT_W,
    KB_REPEAT_X,
    KB_REPEAT_Y,
    KB_REPEAT_Z,
    KB_RELEASE_A,
    KB_RELEASE_B,
    KB_RELEASE_C,
    KB_RELEASE_D,
    KB_RELEASE_E,
    KB_RELEASE_F,
    KB_RELEASE_G,
    KB_RELEASE_H,
    KB_RELEASE_I,
    KB_RELEASE_J,
    KB_RELEASE_K,
    KB_RELEASE_L,
    KB_RELEASE_M,
    KB_RELEASE_N,
    KB_RELEASE_O,
    KB_RELEASE_P,
    KB_RELEASE_Q,
    KB_RELEASE_R,
    KB_RELEASE_S,
    KB_RELEASE_T,
    KB_RELEASE_U,
    KB_RELEASE_V,
    KB_RELEASE_W,
    KB_RELEASE_X,
    KB_RELEASE_Y,
    KB_RELEASE_Z,
};
struct Event {
  EventType type;
  int mods;
  union {
    struct {
      int button;
    };
    struct {
      int scancode;
    };
    struct {
      f32 xp,yp;
    };
  };
};

inline Event create_mouse_event( EventType t, int m ){
  Event e;
  e.type = t;
  e.mods = m;
  return e;
}
inline Event create_mouse_event( EventType t,f32 xp, f32 yp ){
  Event e;
  e.type = t;
  e.xp = xp;
  e.yp = yp;
  return e;
}
inline Event create_keyboard_event( EventType t, int s, int m ){
  Event e;
  e.type = t;
  e.scancode = s;
  e.mods = m;
  return e;
}

#define MAX_EVENTS 100

static Event Event_Queue[ MAX_EVENTS ];
static uint Event_Count = 0;

// Probably want to mutex it
void event_push_back( Event e ){
  Event_Queue[ Event_Count++ ] = e;
}


static void APIENTRY
glDebugOutput( GLenum source, GLenum type, GLuint id,
                    GLenum severity, GLsizei msgLength,
                    const GLchar* message, const void* userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    print_error("---------------"); 
    print_error("Debug message (%d): %s ",id,message);

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
          print_error( "Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
          print_error( "Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
          print_error( "Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
          print_error( "Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION:
          print_error( "Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER:
          print_error( "Source: Other"); break;
    }
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
          print_error( "Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
          print_error( "Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
          print_error( "Type: Undefined Behaviour"); break; 
        case GL_DEBUG_TYPE_PORTABILITY:
          print_error( "Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE:
          print_error( "Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER:
          print_error( "Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
          print_error( "Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP:
          print_error( "Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER:
          print_error( "Type: Other"); break;
    } 
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
          print_error( "Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM:
          print_error( "Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW:
          print_error( "Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
          print_error( "Severity: notification"); break;
    } 
    fprintf(stderr,"\n");
}


void key_callback(GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods)
{
  EventType t ;
#define KEY_PRESS_CASE(x) \
  case GLFW_KEY_##x: t = KB_PRESS_##x; break;

#define KEY_REPEAT_CASE(x) \
  case GLFW_KEY_##x: t = KB_REPEAT_##x; break;
#define KEY_RELEASE_CASE(x) \
  case GLFW_KEY_##x: t = KB_RELEASE_##x; break;
  if ( action == GLFW_PRESS ){
    switch ( key ){
      KEY_PRESS_CASE(A)
      KEY_PRESS_CASE(B)
      KEY_PRESS_CASE(C)
      KEY_PRESS_CASE(D)
      KEY_PRESS_CASE(E)
      KEY_PRESS_CASE(F)
      KEY_PRESS_CASE(G)
      KEY_PRESS_CASE(H)
      KEY_PRESS_CASE(I)
      KEY_PRESS_CASE(J)
      KEY_PRESS_CASE(K)
      KEY_PRESS_CASE(L)
      KEY_PRESS_CASE(M)
      KEY_PRESS_CASE(N)
      KEY_PRESS_CASE(O)
      KEY_PRESS_CASE(P)
      KEY_PRESS_CASE(Q)
      KEY_PRESS_CASE(R)
      KEY_PRESS_CASE(S)
      KEY_PRESS_CASE(T)
      KEY_PRESS_CASE(U)
      KEY_PRESS_CASE(V)
      KEY_PRESS_CASE(W)
      KEY_PRESS_CASE(X)
      KEY_PRESS_CASE(Y)
      KEY_PRESS_CASE(Z)

      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
      default:
        break;
    }
  } else if ( action == GLFW_REPEAT ){
    switch ( key ){
      KEY_REPEAT_CASE(A)
      KEY_REPEAT_CASE(B)
      KEY_REPEAT_CASE(C)
      KEY_REPEAT_CASE(D)
      KEY_REPEAT_CASE(E)
      KEY_REPEAT_CASE(F)
      KEY_REPEAT_CASE(G)
      KEY_REPEAT_CASE(H)
      KEY_REPEAT_CASE(I)
      KEY_REPEAT_CASE(J)
      KEY_REPEAT_CASE(K)
      KEY_REPEAT_CASE(L)
      KEY_REPEAT_CASE(M)
      KEY_REPEAT_CASE(N)
      KEY_REPEAT_CASE(O)
      KEY_REPEAT_CASE(P)
      KEY_REPEAT_CASE(Q)
      KEY_REPEAT_CASE(R)
      KEY_REPEAT_CASE(S)
      KEY_REPEAT_CASE(T)
      KEY_REPEAT_CASE(U)
      KEY_REPEAT_CASE(V)
      KEY_REPEAT_CASE(W)
      KEY_REPEAT_CASE(X)
      KEY_REPEAT_CASE(Y)
      KEY_REPEAT_CASE(Z)

      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
      default:
        break;
    }
  } else if ( action == GLFW_RELEASE ){
    switch ( key ){
      KEY_RELEASE_CASE(A)
      KEY_RELEASE_CASE(B)
      KEY_RELEASE_CASE(C)
      KEY_RELEASE_CASE(D)
      KEY_RELEASE_CASE(E)
      KEY_RELEASE_CASE(F)
      KEY_RELEASE_CASE(G)
      KEY_RELEASE_CASE(H)
      KEY_RELEASE_CASE(I)
      KEY_RELEASE_CASE(J)
      KEY_RELEASE_CASE(K)
      KEY_RELEASE_CASE(L)
      KEY_RELEASE_CASE(M)
      KEY_RELEASE_CASE(N)
      KEY_RELEASE_CASE(O)
      KEY_RELEASE_CASE(P)
      KEY_RELEASE_CASE(Q)
      KEY_RELEASE_CASE(R)
      KEY_RELEASE_CASE(S)
      KEY_RELEASE_CASE(T)
      KEY_RELEASE_CASE(U)
      KEY_RELEASE_CASE(V)
      KEY_RELEASE_CASE(W)
      KEY_RELEASE_CASE(X)
      KEY_RELEASE_CASE(Y)
      KEY_RELEASE_CASE(Z)

      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, true);
        break;
      default:
        break;
    }
  }
  event_push_back( create_keyboard_event(t, scancode, mods ) );
}

void process_keyboard_input( GLFWwindow *window, uint8 *key_map ){
  uint8 press = 0;
#define GET_KEY_STATE(key) \
    press = glfwGetKey( window, GLFW_KEY_##key );\
    key_map[KB_KEY_##key] = ( press==GLFW_RELEASE )?0:1;
  
  GET_KEY_STATE(W)
  GET_KEY_STATE(S)
  GET_KEY_STATE(A)
  GET_KEY_STATE(D)
  GET_KEY_STATE(I)
  GET_KEY_STATE(K)
}

void mouse_callback( GLFWwindow *window, double xpos, double ypos ){
  event_push_back(
      create_mouse_event( MOUSE_MOVE, (f32)xpos,(f32)ypos)
      );
  glfwSetCursorPosCallback( window, NULL );
}

void mouse_button_callback(
    GLFWwindow* window,
    int button,
    int action,
    int mods )
{
  if ( action == GLFW_PRESS ){
    switch ( button ){
      case GLFW_MOUSE_BUTTON_RIGHT:
        event_push_back( create_mouse_event( MOUSE_RBUTTON_CLICK,mods));
        break;
      case GLFW_MOUSE_BUTTON_LEFT:
        event_push_back( create_mouse_event( MOUSE_LBUTTON_CLICK, mods));
        break;
      default:
        break;
    }
  }
#if 0
  if ( action == GLFW_PRESS ){
    switch (button){
      case GLFW_MOUSE_BUTTON_RIGHT:
      {
        int viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport);
        fprintf( stderr, "View port info: %d, %d, %d, %d\n",
            viewport[0], viewport[1], viewport[2], viewport[3] );
        double cp[2];
        if ( glfwGetWindowAttrib(window, GLFW_HOVERED ) ){
          glfwGetCursorPos( window, &cp[0], &cp[1] );
          fprintf( stdout, "%f, %f\n",
                   (f32)cp[0], (f32) cp[1] );
        }
        v3 point = v3{ ( float )cp[0], (float)cp[1], 0.0f };
        v3 wp = HMM_UnProject( point, mvp, SCREEN_WIDTH, SCREEN_HEIGHT );
        fprintf( stdout, "The point in world coords is: " );
        print_v3( wp );
        fprintf( stdout, "\n" );
        break;
      }
      default:
        break;
    }
  }
#endif
}



void resizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
  ScreenHeight = height;
  ScreenWidth = width;
}

#define CLEANUP(x)  __func__##x

int compile_shader( guint *ret, const char *source, GLenum type ){
  uint shader = glCreateShader( type );
  char *buff;
  if ( read_text_file_to_buffer( source, &buff ) == -1 ){
    goto CLEANUP(1);
  }

  glShaderSource( shader, 1, &buff, NULL );
  glCompileShader( shader );

  int success;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
  
  if ( success == GL_FALSE ){
      char infoLog[512];
      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      print_error("Failed to compile Shader!\n \
                   Location:\n %s\
                   \n%s\n", source, infoLog );
      goto CLEANUP(2);
  }
  
  *ret = shader; 
  return 0;
CLEANUP(2):
  assert( shader != 0 );
  glDeleteShader( shader );
  free( buff );
CLEANUP(1):
  return -1;
}


int compile_program( guint *program, const char *vsource ,const char *fsource ){
  guint vshader, fshader, prog;
  if ( compile_shader( &vshader, vsource, GL_VERTEX_SHADER ) == -1 ){
    goto CLEANUP(1);
  }
  if ( compile_shader( &fshader, fsource, GL_FRAGMENT_SHADER) == -1 ){
    goto CLEANUP(1);
  }

  prog = glCreateProgram();
  glAttachShader( prog, vshader );
  glAttachShader( prog, fshader );
  glLinkProgram( prog );
  int success;
  glGetProgramiv( prog, GL_LINK_STATUS, &success );
  if ( success == GL_FALSE ){
      char infoLog[512];
      glGetProgramInfoLog( prog , 512, NULL, infoLog );
      print_error( "Failed to link Shader!\n\
                    %s", infoLog );
      goto CLEANUP( 2 );
  }

  *program = prog;
  glDeleteShader( vshader );
  glDeleteShader( fshader );
  return 0;

CLEANUP(2):
  glDeleteShader( vshader );
  glDeleteShader( fshader );

CLEANUP(1):
  *program = 0;
  return -1;

}


void flipImage( uint8 *data, int width, int height){
    int top = 0, bottom = height - 1;
    while ( top <= bottom ){
        uint8 *topPixel = data + top * width * 4;
        uint8 *botPixel = data + bottom* width * 4;
        for ( int i = 0; i < width * 4 ; i++ ){
            uint8 tmp = *topPixel;
            *topPixel++ = *botPixel;
            *botPixel++ = tmp;
        }
        top++;
        bottom--;
    }
}




struct LineVertexBufferData{
  v3 p;
  v3 color;
};

struct ColorVertexData{
  v3 p;
  v3 color;
  v3 n;
};

struct QuadVertexData {
  v3 p0,p1,p2,p3;
  v3 c0,c1,c2,c3;
  v3 n0,n1,n2,n3;
};

struct RenderContext {
  ColorVertexData *color_vertex_data_buff = NULL;
  const uint max_color_vertex_data = 5000;
  uint num_color_vertex = 0;
  GLenum *color_vertex_modes = NULL;
} Rc;

void add_color_vertex( const v3 &p, const v3 &color, GLenum mode ){
  ColorVertexData v = { p, color };
  Rc.color_vertex_data_buff[ Rc.num_color_vertex ] = v;
  Rc.color_vertex_modes[ Rc.num_color_vertex ] = mode;
  Rc.num_color_vertex++;
}

void draw_color_line( v3 start ,v3 end, v3 color ){
  add_color_vertex( start, color, GL_LINES );
  add_color_vertex( end, color, GL_LINES );
}

void draw_color_vertex( const m4 &mvp ){
}

struct GridProgramInfo{
  uint id;
  uint mvp_loc;
  uint corner_loc;
  uint width_loc;

  uint8 pos_id;
  uint8 direction_id;
  uint8 color_id;
};

struct SimpleColorShaderProgram {
  uint id;
  uint mvp_loc;

  uint8 pos_id;
  uint8 color_id;
  uint8 normal_id;
};




static GridProgramInfo grid_program_info; 
static SimpleColorShaderProgram simple_color_shader_info;


int create_grid_program( ){
  if ( compile_program(&grid_program_info.id,
        "./shaders/grid_shader.vert",
        "./shaders/grid_shader.frag" ) == -1 ){
    fprintf(stderr,"Unable to compile Program!\n");
    return -1;
  }
  const int &id = grid_program_info.id;
  
  glUseProgram( id );
  grid_program_info.mvp_loc = glGetUniformLocation( id,"mvp" );
  grid_program_info.corner_loc = glGetUniformLocation( id,"corner_pos" );
  grid_program_info.width_loc = glGetUniformLocation( id,"width" );

  grid_program_info.pos_id = 0;
  grid_program_info.direction_id = 1;
  grid_program_info.color_id = 2;
  return 0;
}

int create_simple_color_shader_program( ){
  if ( compile_program(&simple_color_shader_info.id,
        "./shaders/simple-color-shader.vert",
        "./shaders/simple-color-shader.frag" ) == -1 ){
    fprintf(stderr,"Unable to compile Program!\n");
    return -1;
  }
  const int &id = simple_color_shader_info.id;
  
  glUseProgram( id );
  simple_color_shader_info.mvp_loc =
          glGetUniformLocation( id,"mvp" );

  simple_color_shader_info.pos_id = 0;
  simple_color_shader_info.color_id = 1;
  simple_color_shader_info.normal_id = 2;
  glUseProgram( 0 );
  return 0;
}


void create_grid_render_data( Grid &g ){
  const v3 &color = g.color;
  glGenVertexArrays(1,&g.vao);
  glGenBuffers(1,&g.vbo);
  f32 len = 100.0f;
  glBindVertexArray( g.vao );
  glBindBuffer( GL_ARRAY_BUFFER, g.vbo );
  
  v3 vertex_data[12];

  vertex_data[0] = g.rect.corner + len * g.dir1;
  vertex_data[1] = g.dir2;
  vertex_data[2] = color;

  vertex_data[3] = g.rect.corner;
  vertex_data[4] = g.dir2;
  vertex_data[5] = color;

  vertex_data[6] = g.rect.corner;
  vertex_data[7] = g.dir1;
  vertex_data[8] = color;

  vertex_data[9] = g.rect.corner + len * g.dir2;
  vertex_data[10] = g.dir1;
  vertex_data[11] = color;

  glBufferData( GL_ARRAY_BUFFER,
                sizeof( vertex_data ),
                vertex_data, GL_STATIC_DRAW );

  GLsizei stride = 3 * sizeof(v3);
   
  glEnableVertexAttribArray( grid_program_info.pos_id );
  glEnableVertexAttribArray( grid_program_info.direction_id);
  glEnableVertexAttribArray( grid_program_info.color_id );

  glVertexAttribPointer( grid_program_info.pos_id,
                         3, GL_FLOAT, GL_FALSE,
                         stride,
                         (void *)( 0 ) );

  glVertexAttribPointer( grid_program_info.direction_id,
                         3, GL_FLOAT, GL_FALSE,
                         stride, 
                         (void *)( sizeof(v3) ) );

  glVertexAttribPointer( grid_program_info.color_id,
                         3, GL_FLOAT, GL_FALSE,
                         stride,
                         (void *)( 2 * sizeof(v3) ) );

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}



void draw_grid( const Grid &g, const m4 &mvp ){
  glUseProgram( grid_program_info.id );
 // Generate rectangle vertices with line vertices
  glUniformMatrix4fv( grid_program_info.mvp_loc,
                      1,GL_FALSE,
                      HMM_MAT4_PTR(mvp) );
  glUniform3fv( grid_program_info.corner_loc, 1,  g.rect.corner.Elements );
  glUniform1f( grid_program_info.width_loc, g.w );

  glBindVertexArray( g.vao );
  glDrawArraysInstanced( GL_LINES, 0, 12, g.nlines );

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glUseProgram( 0 );
}


void draw_cube( const Cube &cube, const m4 &vp ){
  m4 model = cube.base_transform;
  m4 mvp = vp * model;
  glUseProgram( simple_color_shader_info.id );
  glUniformMatrix4fv( simple_color_shader_info.mvp_loc,
                      1,GL_FALSE,
                      HMM_MAT4_PTR(mvp) );
  glBindVertexArray( cube.vao );
  glDrawElements( GL_TRIANGLES,
                  36,
                  GL_UNSIGNED_INT,
                  0  );
  glBindVertexArray( 0 );
  glUseProgram( 0 );
}

void cube_add_vertex_data( const Cube &cube ){
  v3 colors[24];
  for ( int i = 0; i < 24; i++ ){
    int index = i / 4;
    colors[ i ] = cube.color[ index ];
  }

  glBindVertexArray( cube.vao );
  glBindBuffer( GL_ARRAY_BUFFER, cube.vbo );
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(CubeVertices), CubeVertices );
  glBufferSubData( GL_ARRAY_BUFFER, 
                   sizeof(CubeVertices),
                   sizeof(colors),
                   colors);
  glBufferSubData( GL_ARRAY_BUFFER, 
                   sizeof(CubeVertices) + sizeof(colors),
                   sizeof(CubeNormals),
                   (void *)CubeNormals);
  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}
#if 0
void AABB_generate_vertex( const AABB &box, v3 *mem ){
  f32 xlen = box.u[0] - box.l[0];
  f32 ylen = box.u[1] - box.l[1];
  f32 zlen = box.u[2] - box.l[2];
  
  //back 
  v3 a0 = box.l;
  v3 a1 = box.l + v3{ xlen, 0, 0 };
  v3 a2 = box.l + v3{ xlen, ylen, 0 };
  v3 a3 = box.l + v3{ 0, ylen, 0 };

  //front 
  v3 a0 = box.l + v3{ 0,0,zlen};
  v3 a1 = box.l + v3{ xlen, 0, zlen };
  v3 a2 = box.l + v3{ xlen, ylen, zlen };
  v3 a3 = box.l + v3{ 0, ylen, zlen };

  //left
  v3 a0 = box.l + v3{ 0,0,0};
  v3 a1 = box.l + v3{ 0,0,zlen };
  v3 a2 = box.l + v3{ 0,ylen,zlen };
  v3 a3 = box.l + v3{ 0,ylen,0 };


  //right
  v3 a0 = box.l + v3{ xlen,0,0};
  v3 a1 = box.l + v3{ xlen,0,zlen };
  v3 a2 = box.l + v3{ xlen,ylen,zlen };
  v3 a3 = box.l + v3{ xlen,ylen,0 };

  // bottom 
  v3 a0 = box.l + v3{ 0,0,0};
  v3 a1 = box.l + v3{ xlen,0,0};
  v3 a2 = box.l + v3{ xlen,0,zlen};
  v3 a3 = box.l + v3{ 0,0,zlen };
  //top 
  v3 a0 = box.l + v3{ 0,ylen,0};
  v3 a1 = box.l + v3{ xlen,ylen,0};
  v3 a2 = box.l + v3{ xlen,ylen,zlen};
  v3 a3 = box.l + v3{ 0,ylen,zlen };

}
#endif


void create_cube_render_data( Cube &cube ){
  uint &vao = cube.vao;
  uint &vbo = cube.vbo;
  glGenVertexArrays( 1, &vao );
  glGenBuffers( 1, &vbo );

  glBindVertexArray( vao );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );

  glBufferData( GL_ARRAY_BUFFER,
                3 * sizeof(CubeVertices), 
                NULL,
                GL_STATIC_DRAW );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quad_elem_buffer_index );
  glEnableVertexAttribArray(simple_color_shader_info.pos_id );
  glVertexAttribPointer( simple_color_shader_info.pos_id,
                         3,
                         GL_FLOAT, GL_FALSE,
                         3 * sizeof( float ), (void *)0 );

  glEnableVertexAttribArray( simple_color_shader_info.color_id );
  glVertexAttribPointer( simple_color_shader_info.color_id,
                         3,
                         GL_FLOAT, GL_FALSE,
                         3 * sizeof( float ),
                         (void *)(sizeof(CubeVertices) ) );

  glEnableVertexAttribArray( simple_color_shader_info.normal_id );
  glVertexAttribPointer( simple_color_shader_info.normal_id,
                         3,
                         GL_FLOAT, GL_FALSE,
                         3 * sizeof( float ),
                         (void *)(sizeof(CubeVertices) * 2 ) );
  
  cube.vao = vao;
  cube.vbo = vbo;
  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 
  cube_add_vertex_data( cube );
}

struct ColorQuad {
  v3 p0,p1,p2,p3;
  v3 color;
  v3 n;
};

struct World {
  
  m4 perspective;

  Camera camera;

  Grid grid;
  Cube cube;
  
  Line *lines;
  ColorQuad *temp_color_quads;
  ColorQuad *perm_color_quads;

  AABB *boxes;
  
  uint color_vao, color_vbo, color_ebo;  

  v3 *color_vertex_data;
  uint *color_vertex_indices;

  uint *index_stack; // Used as stack
  GLenum *color_vertex_modes; // Used as stack
};

void world_draw_AABB(
    const World &w,
    const AABB &box,
    v3 color,
    uint *elem_index )
{
  // Sorry for this shit, but its the best i can do for now
  // TODO: find something better
  f32 xlen = box.u[0] - box.l[0];
  f32 ylen = box.u[1] - box.l[1];
  f32 zlen = box.u[2] - box.l[2];

  array_push( w.color_vertex_data, box.l);
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );

  array_push( w.color_vertex_data, box.l + v3{ xlen, 0, 0 });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );


  array_push( w.color_vertex_data, box.l + v3{ xlen, ylen, 0 });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );


  array_push( w.color_vertex_data, box.l + v3{ 0, ylen, 0 });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );

  // front
  array_push( w.color_vertex_data, box.l + v3{ 0,0,zlen});
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );


  array_push( w.color_vertex_data, box.l + v3{ xlen, 0, zlen });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );


  array_push( w.color_vertex_data, box.l + v3{ xlen, ylen, zlen });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );


  array_push( w.color_vertex_data, box.l + v3{ 0, ylen, zlen });
  array_push( w.color_vertex_data, color );
  array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );
  
  uint value = *elem_index; 
  
  array_push( w.color_vertex_indices, value );
  array_push( w.color_vertex_indices, value + 1 );

  array_push( w.color_vertex_indices, value + 1 );
  array_push( w.color_vertex_indices, value + 2 );

  array_push( w.color_vertex_indices, value + 2 );
  array_push( w.color_vertex_indices, value + 3 );

  array_push( w.color_vertex_indices, value + 3 );
  array_push( w.color_vertex_indices, value );

  array_push( w.color_vertex_indices, value );
  array_push( w.color_vertex_indices, value + 4 );

  array_push( w.color_vertex_indices, value + 4 );
  array_push( w.color_vertex_indices, value + 5 );

  array_push( w.color_vertex_indices, value + 5 );
  array_push( w.color_vertex_indices, value + 6 );

  array_push( w.color_vertex_indices, value + 6 );
  array_push( w.color_vertex_indices, value + 7 );

  array_push( w.color_vertex_indices, value + 7 );
  array_push( w.color_vertex_indices, value + 4 );

  array_push( w.color_vertex_indices, value + 3 );
  array_push( w.color_vertex_indices, value + 7 );

  array_push( w.color_vertex_indices, value + 2 );
  array_push( w.color_vertex_indices, value + 6 );

  array_push( w.color_vertex_indices, value + 1 );
  array_push( w.color_vertex_indices, value + 5 );

  *elem_index = value + 8;
  return;
}


void draw_world( const World &w ){
  
  m4 vp = w.perspective*w.camera.transform();
  draw_grid(w.grid,vp);
  draw_cube( w.cube,vp);

  // Create line vertex data for rendering

  uint value = 0;
  for ( int i = 0; i < array_length( w.temp_color_quads ); i++ ){
    const ColorQuad &quad = w.temp_color_quads[i];

    // Push the first triangle
    array_push( w.color_vertex_data, quad.p0 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    array_push( w.color_vertex_data, quad.p1 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    array_push( w.color_vertex_data, quad.p2 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );


    array_push( w.color_vertex_data, quad.p3 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    // push the indices
    array_push( w.color_vertex_indices, value );
    array_push( w.color_vertex_indices, value + 1 );
    array_push( w.color_vertex_indices, value + 2 );
    array_push( w.color_vertex_indices, value + 2 );
    array_push( w.color_vertex_indices, value + 3 );
    array_push( w.color_vertex_indices, value );

    value += 4;
  }

  for ( int i = 0; i < array_length( w.perm_color_quads ); i++ ){
    const ColorQuad &quad = w.perm_color_quads[i];

    // Push the first triangle
    array_push( w.color_vertex_data, quad.p0 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    array_push( w.color_vertex_data, quad.p1 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    array_push( w.color_vertex_data, quad.p2 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );


    array_push( w.color_vertex_data, quad.p3 );
    array_push( w.color_vertex_data, quad.color );
    array_push( w.color_vertex_data, quad.n );

    array_push( w.color_vertex_indices, value );
    array_push( w.color_vertex_indices, value + 1 );
    array_push( w.color_vertex_indices, value + 2 );
    array_push( w.color_vertex_indices, value + 2 );
    array_push( w.color_vertex_indices, value + 3 );
    array_push( w.color_vertex_indices, value );

    value += 4;

  }

  array_push( w.color_vertex_modes, (GLenum)GL_TRIANGLES );
  array_push( w.index_stack, array_length( w.color_vertex_indices ) );


  for ( int i = 0; i < array_length( w.lines ); i++ ){
    // Push the ending point

    array_push( w.color_vertex_data, w.lines[i].start );
    array_push( w.color_vertex_data, w.lines[i].color );
    array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );
    array_push( w.color_vertex_indices, value++ );
    
    // Push the ending point
    array_push( w.color_vertex_data,w.lines[i].end );
    array_push( w.color_vertex_data, w.lines[i].color );

    // Line normal, not necessary and only for debuggin purposes
    // and another shader prob cost too much
    array_push( w.color_vertex_data, v3{0.0f,0.0f,0.0f} );
    array_push( w.color_vertex_indices, value++ );

  }

  for ( uint i = 0; i < array_length( w.boxes ); i++ ){
    world_draw_AABB( w, w.boxes[i], v3{1.0f,1.0f,1.0f }, &value );
  }
  
  array_push( w.color_vertex_modes, (GLenum)GL_LINES );
  array_push( w.index_stack, array_length(w.color_vertex_indices) );

  glUseProgram( simple_color_shader_info.id );
  glBindVertexArray( w.color_vao );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, w.color_ebo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                sizeof(uint) * array_length( w.color_vertex_indices ),
                w.color_vertex_indices,
                GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, w.color_vbo );
  glBufferData( GL_ARRAY_BUFFER, 
                sizeof(v3) * array_length( w.color_vertex_data ),
                w.color_vertex_data,
                GL_STATIC_DRAW
              );

  glUniformMatrix4fv( simple_color_shader_info.mvp_loc,
                      1,GL_FALSE,
                      HMM_MAT4_PTR(vp) );
#if 1
  uint prev = 0;
  for ( int i = 0; i < array_length( w.color_vertex_modes);i++ ){
    GLenum mode = w.color_vertex_modes[i];
    uint len = w.index_stack[i];
    uint count = len - prev;
    
    glDrawElements( mode, count,
                    GL_UNSIGNED_INT, (void *)( prev * sizeof(uint) ));

    prev = len;
  }
#else
  for ( int i = 1; i < array_length( w.color_vertex_modes ); i++ ){
    if ( mode != w.color_vertex_modes[i] ){
      len = i - start; 
      glDrawElements( mode, count, GL_UNSIGNED_INT, index );
      start = i;
      mode = w.color_vertex_modes[i];
    }
  }

  if ( start < array_length( w.color_vertex_modes ) ){
    glDrawArrays( mode, start, array_length( w.color_vertex_modes ) - start );
  }
#endif
  glUseProgram( 0 );
  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
}


bool hit_world(
    const World &w,
    const Ray &r,
    f32 tmin, f32 tmax,
    HitRecord &record )
{
  f32 max = tmax;
  HitRecord temp_record;
  bool hit_anything = false;
  if ( hit_grid( w.grid, r, tmin, max, temp_record ) ){
    hit_anything = true;
    if ( temp_record.t < max ){
      max = temp_record.t;
      record = temp_record;
    }
  }

  if ( hit_cube( w.cube, r, tmin, max, temp_record ) ){
    hit_anything = true;
    if ( temp_record.t < max ){
      max = temp_record.t;
      record = temp_record;
    }
  }
  return hit_anything;
}

int main(){

  glfwInit();
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  GLFWwindow* window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT,
                       "OpenGl", NULL, NULL );
  if ( !window ){
      print_error("Unable to open window!");
  }

  glfwMakeContextCurrent( window );
  glfwSetFramebufferSizeCallback( window, resizeCallback );
  //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
  glfwSetCursorPosCallback( window, mouse_callback );
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  glfwSetKeyCallback( window, key_callback );
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      print_error("Failed to initialize GLAD");
      return -1;
  }
#if ENABLE_GL_DEBUG_PRINT
  GLint flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
  {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE,
                          GL_DONT_CARE,
                          GL_DONT_CARE,
                          0, nullptr,
                          GL_TRUE);
  } else {
    print_error("Unable to set debug context");
  }
#endif
  glEnable( GL_DEPTH_TEST );

  Quad_elem_indices= ( uint * )malloc( 6 * 1000 * sizeof( uint ) );
  uint t[] = { 0, 1, 2, 2, 3, 0 };
  uint *tmp = Quad_elem_indices;
  for ( int i = 0; i < 1000; i++ ){
    for ( int j = 0; j < 6; j++ ){
      *tmp = t[j] + 4 * i;
      tmp++;
    }
  }
  glGenBuffers(1,&quad_elem_buffer_index);
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quad_elem_buffer_index );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
                sizeof( uint ) * 100,
                Quad_elem_indices,
                GL_STATIC_DRAW
              );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  if ( create_grid_program() == -1 ){
    return -1;
  }
  if ( create_simple_color_shader_program() == -1 ){
    return -1;
  }

  // create_world
  World *world = (World *)malloc( sizeof(World) );
  World &w = *world;
  w.grid = create_grid(
            AARect::PLANE_ZX,
            AABB( v3{ -10.0f, 0.0f, -10.0f }, v3{ 10.0f, 0.0f, 10.0f } ),
            0.0f,
            0.1f,
            v3{0.0f,0.0f,1.0f} );

  w.perspective= HMM_Perspective(40,
                  (float)SCREEN_WIDTH/SCREEN_HEIGHT,
                  0.1f, 10.0f );
  w.camera = Camera( 
            v3{ 0.0f, 0.5f, 5.0f },
            v3{ 0.0f, 0.5f, -1.0f },
            v3{ 0.0f, 1.0f, 0.0f }
            );
  Camera &camera = w.camera;

  w.cube = create_cube_one_color( 0.5f, v3{-1,0.5f,-2}, v3 {0,1,0} );

  w.cube.color[Cube::FRONT] = v3{0.82f, 0.36f, 0.45f};
  w.cube.color[Cube::BACK] = v3{0.82f, 0.36f, 0.45f};
  w.cube.color[Cube::LEFT] = v3{0.32f, 0.32f, 0.86f};
  w.cube.color[Cube::RIGHT] = v3{0.32f, 0.32f, 0.86f};
  create_cube_render_data( w.cube );
  create_grid_render_data( w.grid );
  
  w.boxes = array_allocate( AABB, 10 );
  w.lines = array_allocate( Line, 10 );
  w.temp_color_quads = array_allocate( ColorQuad, 10 );
  w.perm_color_quads = array_allocate( ColorQuad, 10 );

  w.color_vertex_data = array_allocate( v3, 1000 );
  w.color_vertex_indices = array_allocate( uint, 3000 );

  w.index_stack = array_allocate( uint, 10 );
  w.color_vertex_modes = array_allocate( GLenum ,10 );
  
  glGenVertexArrays(1, &w.color_vao );
  glGenBuffers( 1, &w.color_vbo );
  glGenBuffers( 1, &w.color_ebo );

  glBindVertexArray( w.color_vao );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, w.color_ebo );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER,
                1000  * sizeof(v3),
                NULL, GL_STATIC_DRAW );

  glBindBuffer( GL_ARRAY_BUFFER, w.color_vbo );
  glBufferData( GL_ARRAY_BUFFER,
                2000 * sizeof(v3),
                NULL, GL_STATIC_DRAW );

  glEnableVertexAttribArray( simple_color_shader_info.pos_id );
  glEnableVertexAttribArray( simple_color_shader_info.color_id );
  glEnableVertexAttribArray( simple_color_shader_info.normal_id );

  glVertexAttribPointer( simple_color_shader_info.pos_id,
                         3, GL_FLOAT, GL_FALSE,
                         3 * sizeof(v3),
                         (void *)( 0 ) );

  glVertexAttribPointer( simple_color_shader_info.color_id,
                         3, GL_FLOAT, GL_FALSE,
                         3 * sizeof(v3),
                         (void *)(sizeof(v3) ) );
  glVertexAttribPointer( simple_color_shader_info.normal_id,
                         3, GL_FLOAT, GL_FALSE,
                         3 * sizeof(v3),
                         (void *)(2 * sizeof(v3) ) );

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  int viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport);
  float dt = 0;
  float current = glfwGetTime();
  m4 vp = HMM_Mat4d(1.0f);
  double cp[2];
  glfwGetCursorPos( window, &cp[0], &cp[1] );
  const float camera_sensitivity = 0.5f;

  uint8 *key_map = (uint8 *)malloc( sizeof(uint8) * 400 );
  memset( key_map, 0, 400 * sizeof(uint8) );

  while ( !glfwWindowShouldClose( window  ) ){
    float now = glfwGetTime();
    dt = now - current;
    current = now;
    process_keyboard_input( window, key_map );

    //check camera events
    for ( int i = 0; i < Event_Count; i++ ){
      switch ( Event_Queue[i].type ){
        case MOUSE_MOVE:
        {
          float dx = Event_Queue[i].xp - cp[0];
          float dy = Event_Queue[i].yp - cp[1];
          camera.rotate( camera_sensitivity*dx,
                         camera_sensitivity*dy );
          cp[0] = Event_Queue[i].xp;
          cp[1] = Event_Queue[i].yp;
          break;
        }
        case MOUSE_RBUTTON_CLICK:
        { 
          HitRecord record;
          v3 point = v3{ ( float )cp[0], (float)cp[1], 0.0f };
          v3 wp = HMM_UnProject( point, w.perspective * w.camera.transform(),
                                 ScreenWidth, ScreenHeight);
          Ray ray( camera.P, ( wp - camera.P ) );

          if ( hit_grid( w.grid, ray, 0.001f, 100.0f, record ) ){
            record.print();
            v3 p0 = grid_get_corner_point( w.grid, record.u, record.v );
            fprintf(stdout,"Corner point: ");
            print_v3( p0 );
            v3 p1 = p0 + w.grid.dir1 * w.grid.w;
            v3 p2 = p0 + w.grid.dir1 * w.grid.w + w.grid.dir2 * w.grid.w;
            v3 p3 = p0 + w.grid.dir2 * w.grid.w;
            ColorQuad quad = { p0, p1, p2, p3,
                               v3{ 0.52f,0.15f,0.93f }, // color
                               w.grid.rect.n
                             };
            array_push( w.perm_color_quads, quad );
            fprintf(stdout,"\n" );
          }
#if 0
          array_push( w.lines,
                      create_line_from_ray( ray, 10.0f, v3{0,1.0f,0.0f} ) );
#endif
          break;
        }

        case MOUSE_LBUTTON_CLICK:
          break;
#if 1
        case KB_PRESS_W: case KB_REPEAT_W:
          camera.start_animate( 2, 0.2f ,100);
          break;

        case KB_PRESS_S:case KB_REPEAT_S:
          camera.start_animate( 2, -0.2f ,100);
          break;

        case KB_PRESS_A:case KB_REPEAT_A:
          camera.start_animate( 0, -0.2f ,100);
          break;

        case KB_PRESS_D:case KB_REPEAT_D:
          camera.start_animate( 0, 0.2f ,100);
          break;

        case KB_PRESS_I:case KB_REPEAT_I:
          camera.start_animate( 1, 0.2f ,300);
          break;
        case KB_PRESS_K:case KB_REPEAT_K:
          camera.start_animate( 1, -0.2f ,300);
          break;
        case KB_PRESS_T:
          camera.toggle_move();
          break;
        case KB_PRESS_R:
          camera.toggle_rotate();
          break;
        case KB_PRESS_P:
          camera.print();
          break;
        case KB_RELEASE_W:
          if ( !( key_map[KB_KEY_S] || key_map[KB_KEY_A] ||
                  key_map[KB_KEY_D] || key_map[KB_KEY_I] ||
                  key_map[KB_KEY_K] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
        case KB_RELEASE_S:
          if ( !( key_map[KB_KEY_W] || key_map[KB_KEY_A] ||
                  key_map[KB_KEY_D] || key_map[KB_KEY_I] ||
                  key_map[KB_KEY_K] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
        case KB_RELEASE_A:
          if ( !( key_map[KB_KEY_S] || key_map[KB_KEY_W] ||
                  key_map[KB_KEY_D] || key_map[KB_KEY_I] ||
                  key_map[KB_KEY_K] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
        case KB_RELEASE_D:
          if ( !( key_map[KB_KEY_S] || key_map[KB_KEY_A] ||
                  key_map[KB_KEY_W] || key_map[KB_KEY_I] ||
                  key_map[KB_KEY_K] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
        case KB_RELEASE_I:
          if ( !( key_map[KB_KEY_S] || key_map[KB_KEY_A] ||
                  key_map[KB_KEY_D] || key_map[KB_KEY_W] ||
                  key_map[KB_KEY_K] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
        case KB_RELEASE_K:
          if ( !( key_map[KB_KEY_S] || key_map[KB_KEY_A] ||
                  key_map[KB_KEY_D] || key_map[KB_KEY_I] ||
                  key_map[KB_KEY_W] )
             )
          {
            camera.state = Camera::STATIC;
          }
          break;
#endif
        default:
          break;
      }
    }
    glfwSetCursorPosCallback( window, mouse_callback );
    camera.update( dt );
    Event_Count = 0;
    glClearColor(0.0f,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );



    if ( !camera.should_rotate ){
      glfwGetCursorPos( window, &cp[0], &cp[1] );
      HitRecord record;

      v3 point = v3{ ( float )cp[0], (float)cp[1], 0.0f };
      v3 wp = HMM_UnProject( point, w.perspective * w.camera.transform(),
          ScreenWidth, ScreenHeight );
      Ray ray( camera.P, ( wp - camera.P ) );

      if ( hit_world( w, ray, 0.001f, 100.0f, record ) ){
        if ( record.obj_type == OBJECT_GRID ){
          Grid *grid = (Grid *)record.object;
          v3 p0 = grid_get_corner_point( *grid, record.u, record.v );
          v3 p1 = p0 + grid->dir1 * grid->w;
          v3 p2 = p0 + grid->dir1 * grid->w + grid->dir2 * grid->w;
          v3 p3 = p0 + grid->dir2 * grid->w;
          ColorQuad quad = { p0, p1, p2, p3,
            v3{ 0.42f,0.65f,0.83f }, // color
            w.grid.rect.n
          };
          array_push( w.temp_color_quads, quad );
        } else if ( record.obj_type == OBJECT_CUBE ){
          Cube *c = (Cube *)record.object;
          array_push( w.boxes, c->bounds );
        }
      }
    }

    draw_world(w);
    glfwSwapBuffers(window);
    glfwPollEvents();
    array_clear( w.color_vertex_data );
    array_clear( w.color_vertex_modes );
    array_clear( w.color_vertex_indices );
    array_clear( w.index_stack );
    array_clear( w.temp_color_quads );
    array_clear( w.boxes );
  }
  glfwTerminate();
}
