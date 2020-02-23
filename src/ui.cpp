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
#define USE_KEY_CALLBACK 1
#include <stb_image.h>
#include "HandmadeMath.h"

void print_v3( const v3 &v ){
  fprintf( stdout,"%f, %f, %f",v.X,v.Y,v.Z );
}

typedef GLuint guint;
typedef unsigned int uint;
typedef float f32;

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

#define HMM_MAT4_PTR(x) ( &x.Elements[0][0] )
#define HMM_MAT4P_PTR(x) (&( x->Elements[0][0]  ))
#define ENABLE_GL_DEBUG_PRINT 1
#define MS_TO_SEC(x) ( (x) * 1.0e-3f )
static uint *Quad_elem_indices;
static uint quad_elem_buffer_index;
static uint *Line_elem_indices;

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


  CameraState state;
  f32 duration;
  f32 elapsed;
  int dim;
  f32 dist_to_move;
  f32 dist_moved;
  f32 speed;

  f32 pitch; 
  f32 yaw;
  const f32 max_pitch;
  const f32 max_yaw;
  Camera ():max_pitch(80.0f),max_yaw(80.0f){}
  Camera ( const v3& Eye, const v3& Center, const v3& Up ):
    pitch(0.0f),yaw(0.0f),max_pitch(80.0f), max_yaw(80.0f) 
  {
    state = STATIC;
    F = HMM_NormalizeVec3(HMM_SubtractVec3(Center, Eye));
    S = HMM_NormalizeVec3(HMM_Cross(F, Up));
    U = HMM_Cross(S, F);
    P = Eye;

    duration = MS_TO_SEC( 700 );
    speed = 1.0f/MS_TO_SEC(500);
  }

  inline void rotate( f32 p, f32 y ){
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

  m4 transform( ){
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
  
  void move_towards( float t ){ P = P + t * F; }
  void move_right( float t ){ P = P + t * S; }
  void move_up( float t ){ P = P + t * U;}

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
        elapsed += dt;
        if ( elapsed >= duration ) state = STATIC;
#endif
        break;
      }
      default:
        break;
    }
  }

  inline void start_animate( int dir,f32 dist, f32 time ){
    state = ANIMATING;
    elapsed = 0;
    dim = dir;
    dist_to_move = 1.0f;
    dist_moved = 0.0f;
    duration = MS_TO_SEC( time );
    speed = dist/MS_TO_SEC( time );
  }

  inline void continue_animate( f32 time ){
    state = ANIMATING;
    dist_moved = 0.0f;
    elapsed = 0;
    duration = MS_TO_SEC( time );
  }
};

struct GLWindow {
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
  }
  event_push_back( create_keyboard_event(t, scancode, mods ) );
}
#if 0
void processInput(GLFWwindow *window)
{
  if ( IS_KEY_PRESS( ESCAPE ) ){
    glfwSetWindowShouldClose(window, true);
  } else if ( IS_KEY_PRESS( W ) ){
    // w moves us  TOWARDS the direction the camera is facing
  } else if ( IS_KEY_PRESS( S ) ){
    // S moves us AWAY from the direction the camera is facing
  } else if ( IS_KEY_PRESS( A ) ){
  } else if ( IS_KEY_PRESS( D ) ){
  } else if ( IS_KEY_PRESS( I ) ){
  } else if ( IS_KEY_PRESS( K ) ){
  } else if ( IS_KEY_PRESS( Z ) ){
  } else if ( IS_KEY_PRESS( X ) ){
  }
}
#endif

void mouse_callback( GLFWwindow *window, double xpos, double ypos ){
  event_push_back(
      create_mouse_event( MOUSE_MOVE, (f32)xpos,(f32)ypos)
      );
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



struct Line {
  v3 start, end,color;
  f32 width;
  v3 p[4];
};

struct LineVertexBufferData{
  v3 p;
  v3 color;
};

Line create_line( v3 start, v3 end, f32 w, v3 color ){
  Line l = { start, end, w };
  v3 dir = { 1.0f, 0.0f, 0.0f }; 
  l.p[0] = start - (w/2) * dir;
  l.p[1] = start + (w/2) * dir;
  l.p[2] = end + (w/2) * dir;
  l.p[3] = end - (w/2) * dir;
  l.color = color;
  return l;
}

struct GridProgramInfo{
  uint id;
  uint mvp_loc;
  uint dir_loc;
  uint8 pos_id;
  uint8 color_id;
  uint8 tfr_id;
};

struct Grid {
  Line l1, l2;

  LineVertexBufferData *data;
  uint line_buff_len;
  uint line_buff_cap;

  v3 dir1, dir2;  

  uint vao, vbo, ebo;
};


static GridProgramInfo grid_program_info; 


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
  grid_program_info.dir_loc= glGetUniformLocation( id,"direction" );
  grid_program_info.pos_id = 0;
  grid_program_info.color_id = 1;
  grid_program_info.tfr_id = 2;
  return 0;
}

Grid create_grid_xz( ){
  Grid g;
  g.l1 = create_line(
             v3{ 0.0f, 0.0f, 10.0f },
             v3{ 0.0f, 0.0f, -10.0f },
             0.01f,
             v3{1.0f,0.0f,0.0f}
             );
  g.dir1 = v3{ 1.0f, 0.0f, 0.0f };
  g.l2 = create_line(
             v3{ -100.0f, 0.0f, 0.0f },
             v3{ 100.0f, 0.0f, 0.0f },
             0.01f,
             v3{1.0f,0.0f,0.0f}
             );
  g.dir2 = v3{ 0.0f, 0.0f, 1.0f };
  g.line_buff_len = 0;
  g.line_buff_cap = 200;
  g.data = ( LineVertexBufferData * )malloc(
            sizeof( LineVertexBufferData ) * g.line_buff_cap );

  glGenVertexArrays(1,&g.vao);
  glGenBuffers(1,&g.vbo);

  glBindVertexArray( g.vao );
  glBindBuffer( GL_ARRAY_BUFFER, g.vbo );
  glBufferData( GL_ARRAY_BUFFER,
                g.line_buff_cap * sizeof(LineVertexBufferData),
                NULL, GL_STATIC_DRAW );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, quad_elem_buffer_index );
   
  glEnableVertexAttribArray( grid_program_info.pos_id );
  glEnableVertexAttribArray( grid_program_info.color_id );

  glVertexAttribPointer( 0,
                         3, GL_FLOAT, GL_FALSE,
                         sizeof(LineVertexBufferData),
                         (void *)( 0 ) );

  glVertexAttribPointer( 1,
                         3, GL_FLOAT, GL_FALSE,
                         sizeof(LineVertexBufferData),
                         (void *)( 3 * sizeof(f32) ) );

  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  return g;
}

void draw_grid( const Grid &g, const m4 &mvp ){
  glUseProgram( grid_program_info.id );
 // Generate rectangle vertices with line vertices
  glUniformMatrix4fv( grid_program_info.mvp_loc,
                      1,GL_FALSE,
                      HMM_MAT4_PTR(mvp) );
  glUniform3fv( grid_program_info.dir_loc, 1,  &g.dir1.Elements[0] );
  const Line &l1 = g.l1;
  g.data[0] = LineVertexBufferData{ l1.start, l1.color };
  g.data[1] = LineVertexBufferData{ l1.end, l1.color };
  glBindVertexArray( g.vao );
  glBindBuffer( GL_ARRAY_BUFFER, g.vbo );
  glBufferData( GL_ARRAY_BUFFER, 
                sizeof( LineVertexBufferData ) * 2,
                (void *)g.data,
                GL_STATIC_DRAW
              );
  glDrawElementsInstanced( GL_LINES,
                           2, GL_UNSIGNED_INT,
                           0,
                           100);
  g.data[0] = LineVertexBufferData{ g.l2.start, g.l2.color };
  g.data[1] = LineVertexBufferData{ g.l2.end, g.l2.color };
  glUniform3fv( grid_program_info.dir_loc, 1,  &g.dir2.Elements[0] );
  glBindBuffer( GL_ARRAY_BUFFER, g.vbo );
  glBufferData( GL_ARRAY_BUFFER, 
                sizeof( LineVertexBufferData ) * 2,
                (void *)g.data,
                GL_STATIC_DRAW
              );
  glDrawElementsInstanced( GL_LINES,
                           2, GL_UNSIGNED_INT,
                           0,
                           100);
  glBindVertexArray( 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 );
  glUseProgram( 0 );
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
                sizeof( uint ) * 18,
                Quad_elem_indices,
                GL_STATIC_DRAW
              );
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  if ( create_grid_program() == -1 ){
    return -1;
  }
  Grid g = create_grid_xz();
  m4 projection = HMM_Perspective(40,
                  (float)SCREEN_WIDTH/SCREEN_HEIGHT,
                  0.1f, 10.0f );
  Camera camera( 
            v3{ 0.0f, 0.5f, 5.0f },
            v3{ 0.0f, 0.5f, -1.0f },
            v3{ 0.0f, 1.0f, 0.0f }
            );
  int viewport[4];
  glGetIntegerv( GL_VIEWPORT, viewport);
  float dt = 0;
  float current = glfwGetTime();
  m4 mvp = HMM_Mat4d(1.0f);

  double cp[2];
  glfwGetCursorPos( window, &cp[0], &cp[1] );
  const float camera_sensitivity = 0.5f;
  while ( !glfwWindowShouldClose( window  ) ){
    float now = glfwGetTime();
    dt = now - current;
    current = now;
//    processInput( window );
 
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
          fprintf( stdout, "The event is %d\n", Event_Queue[i].type );
          if ( glfwGetWindowAttrib(window, GLFW_HOVERED ) ){
            glfwGetCursorPos( window, &cp[0], &cp[1] );
            fprintf( stdout, "%f, %f\n",
                     (f32)cp[0], (f32) cp[1] );
          }
          f32 depth_comp = 0.0f;
          glReadPixels( (f32)cp[0],(f32)cp[1],
              1, 1,
              GL_DEPTH_COMPONENT, GL_FLOAT, &depth_comp);
          v3 point = v3{ ( float )cp[0], (float)cp[1], depth_comp };
          v3 wp = HMM_UnProject( point, mvp,
                                 SCREEN_WIDTH, SCREEN_HEIGHT );
          fprintf( stdout, "The point in world coords is: " );
          print_v3( wp );
          fprintf( stdout, "\n" );
          break;
        }

        case MOUSE_LBUTTON_CLICK:
          break;
#if 1
        case KB_PRESS_W:
          camera.start_animate( 2, 1.0f ,500);
          break;

        case KB_PRESS_S:
          camera.start_animate( 2, -1.0f ,500);
          break;

        case KB_PRESS_A:
          camera.start_animate( 0, -1.0f ,500);
          break;

        case KB_PRESS_D:
          camera.start_animate( 0, 1.0f ,500);
          break;

        case KB_PRESS_I:
          camera.start_animate( 1, 0.5f ,500);
          break;
        case KB_PRESS_K:
          camera.start_animate( 1, -0.5f ,500);
          break;
#endif
        case KB_REPEAT_W:
            camera.continue_animate( 100 );
          break;

        case KB_REPEAT_S:
            camera.continue_animate( 100 );
          break;

        case KB_REPEAT_A:
            camera.continue_animate( 100 );
          break;

        case KB_REPEAT_D:
            camera.continue_animate( 100 );
          break;
        case KB_REPEAT_I:
            camera.continue_animate( 100 );
          break;
        case KB_REPEAT_K:
            camera.continue_animate( 100 );
          break;
        default:
          break;
      }
    }
    camera.update( dt );
    mvp = projection * camera.transform();
    Event_Count = 0;
    glClearColor(0.0f,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    draw_grid( g, mvp );


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}
