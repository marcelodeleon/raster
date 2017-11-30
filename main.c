#include "framework/canvas.h"
#include "framework/mm.h"

#include "point3D.h"

#ifdef WIN32
#include "glew.h"
#else
#include <GL/gl.h>
#endif

#include "obj.h"
#include "shader.h"

#include <SDL/SDL_image.h>

void setup_light()
{
    glEnable(GL_LIGHTING);

    //glEnable(GL_NORMALIZE);
    float l0[] = {1.0f,1.0f,1.0f,1.0f};
    float la[] = {0.10f,0.10f,0.10f,1.0f};
    float l0p[]= {1.0f,1.0f,1.0f,1.0f};
    float ls[] = {1.0f,1.0f,1.0f,1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, la);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, l0);
    glLightfv(GL_LIGHT0, GL_POSITION, l0p);
    glLightfv(GL_LIGHT0, GL_SPECULAR, ls);
    glEnable(GL_LIGHT0);

    float cyan[] = {1.0f, 1.0f, 1.0f, 1.f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ls);
    glMateriali(GL_FRONT, GL_SHININESS, 16);
}

int main(int argc, char* argv[])
{
    // Crear una ventana de 900x900 pixels:
	int cw = 900;
	int ch = 900;

	cg_init(cw, ch, "Rasterizador - Marcelo de León");

#ifdef WIN32
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );

	GLenum err= glewInit();
	if(err!=GLEW_OK)
	{
		//problem: glewInit failed, something is seriously wrong
		printf("GLEW Error: %s\n", glewGetErrorString(err));
		return -1;
	}
#endif
    printf("GL Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
    glClearColor(0,0,1,1);
    // Actualizar la pantalla:
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glViewport(0,0,cw, ch);
    glFrustum(-1,1,-1,1,1,1000);

    // Inicializacoión de variables para el manejo de la rotación.
    float ang_y = 0.0f;
    float ang_z = 0.0f;
    float ang = 0.0f;
    float pitch = -50.0f;
    float ang_vel = 1.0f;

    // Carga de la luz.
    setup_light();

    Obj* obj = obj_load("Models/knight_texturas.obj");
	char done = 0;
	char wireframe = 0;
	char light = 0;
	char bfc = 0;
	glEnable(GL_DEPTH_TEST);
	char zbuff = 1;
    unsigned char key_pressed[1024];
    memset(key_pressed, 0, 1024);

    char use_shader = 0;
    char specular = 0;

    // Configuración de Goraud shader.
    Shader gouraud = shader_new("shaders/gouraud_vp.glsl", "shaders/gouraud_fp.glsl");
    GLint uniform_especular = shader_get_unif_loc(gouraud, "especular");
    GLint uniform_tex = shader_get_unif_loc(gouraud, "tex");

    //Cargo la imagen de disco usando SDL_image
    SDL_Surface* surface = IMG_Load("Models/knight.png");
    if (surface==NULL) { //Si falla la carga de la imagen, despliego el mensaje de error correspondiente y termino el programa.
        printf("Error: \"%s\"\n",SDL_GetError());
        return 1;
    }

    // Configuración de Textura.
    GLuint texture;
    glGenTextures(1,&texture);
    glActiveTexture(GL_TEXTURE0);
    
    // Habilito la carga para la textura recien creada
    glBindTexture(GL_TEXTURE_2D,texture);

    // Carga de datos de imagen en textura.
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 surface->w,
                 surface->h,
                 0,
                 GL_RGB,GL_UNSIGNED_BYTE,
                 surface->pixels);

    // Liberar recursos.
    SDL_FreeSurface(surface);

    // El obligatorio debe implementar mapeo de texturas con filtro bilineal (GL_LINEAR).
    // Configuración para superficies más chicas.
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);

    // Configuración para superficies más grandes.
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);


    // Configuración para el manejo de texels fuera del rango [0, 1].
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                   GL_REPEAT);

	while (!done)
	{

		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
                    key_pressed[event.key.keysym.sym] = 1;
                    if (event.key.keysym.sym == SDLK_p)
				    {
				        use_shader = !use_shader;
				        break;
				    }
				    else if (event.key.keysym.sym == SDLK_s)
				    {
				        specular = !specular;
				        break;
				    }
                    else if (event.key.keysym.sym == SDLK_z)
				    {
				        zbuff = !zbuff;
				        if(zbuff)
                            glEnable(GL_DEPTH_TEST);
                        else
                            glDisable(GL_DEPTH_TEST);
				        break;
				    }
				    else if (event.key.keysym.sym == SDLK_m)
				    {
				        wireframe = !wireframe;
				        if(wireframe)
                            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        else
                            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				        break;
				    }
                    else if (event.key.keysym.sym == SDLK_l)
                    {
                        light = !light;
                        if(light)
                            glEnable(GL_LIGHTING);
                        else
                            glDisable(GL_LIGHTING);
                        break;
                    }
				    else if (event.key.keysym.sym == SDLK_b)
				    {
				        bfc = !bfc;
				        if(bfc)
				        {
                            glEnable(GL_CULL_FACE);
                            glCullFace(GL_BACK);
                            glFrontFace(GL_CW);
				        }
                        else
                            glDisable(GL_CULL_FACE);
				        break;
				    }
					else if (event.key.keysym.sym != SDLK_ESCAPE)
						break;
				case SDL_QUIT : done = 1;break;
				case SDL_KEYUP: key_pressed[event.key.keysym.sym] = 0;

			}
		}

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Defino traslación del modelo.
        glTranslatef(0.0f, 0.0f, pitch);
        
        // Defino rotación del modelo.
        glRotatef(ang_y, 0.0f, 1.0f, 0.0f); 
        glRotatef(ang_z, 0.0f, 0.0f, 1.0f);

        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

        if(key_pressed[SDLK_RIGHT]) ang_y += ang_vel;
        if(key_pressed[SDLK_LEFT]) ang_y -= ang_vel;
        if(key_pressed[SDLK_UP]) ang_z += ang_vel;
        if(key_pressed[SDLK_DOWN]) ang_z -= ang_vel;
        if(key_pressed[SDLK_i]) pitch -= ang_vel;
        if(key_pressed[SDLK_o]) pitch += ang_vel;

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        //Informo a OpenGL que para todas las operaciones a continuación utilice las texturas 2D cargadas
        glEnable(GL_TEXTURE_2D);

        if(use_shader)
        {
            shader_use(gouraud);
            glUniform1i(uniform_especular, specular);
            //Le digo al shader que el sampler2D de nombre "tex" se corresponde con GL_TEXTURE0
            //que es donde cargué mi textura.
            glUniform1i(uniform_tex, 0);
            //Luego asocio la textura con el id "texture"
            glBindTexture(GL_TEXTURE_2D,texture);
            obj_render(obj);
            shader_stop(gouraud);
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D,texture);
            obj_render(obj);
        }

        cg_repaint();
	}


    obj_free(obj);
    shader_free(gouraud);
    glDeleteTextures(1,&texture);
	// Liberar recursos:
	cg_close();

	return 0;
}
