#include "obj.h"
#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include "framework/mm.h"
#include "GL/gl.h"

// Arrays utilizados para dibujar los obj utilizando Vertex Arrays.
float* vertexData;
float* normalsData;
float* texData;

Face* face_new(int v1, int n1, int t1, int v2, int n2, int t2, int v3, int n3, int t3)
{
	Face *f = (Face *) cg_malloc(sizeof(Face));
	f->vertexes[0] = v1-1;
	f->vertexes[1] = v2-1;
	f->vertexes[2] = v3-1;

	f->normals[0] = n1-1;
	f->normals[1] = n2-1;
	f->normals[2] = n3-1;

	f->textures[0] = t1-1;
	f->textures[1] = t2-1;
	f->textures[2] = t3-1;

	return f;
}

void face_print(Face* f)
{
	printf("Vertexes indexes: %d/%d/%d\n", f->vertexes[0], f->vertexes[1], f->vertexes[2]);
    printf("Normals indexes: %d/%d/%d\n", f->normals[0], f->normals[1], f->normals[2]);
    printf("Textures indexes: %d/%d/%d\n", f->textures[0], f->textures[1], f->textures[2]);
}

void load_data_for_vertex_arrays(Obj *obj)
{
    int i = 0; // Contador utilizado para cargar los vertices a vertexData.
    int facesCount = list_size(obj->faces);
    // Asinga la memoria necesaria para guardar todos los vertices. Hay Faces*3*3 vertices.
    vertexData = (float *)cg_malloc(sizeof(float)*facesCount*3*3);
    // Asinga la memoria necesaria para guardar todas los normales. Hay Faces*3*3 normales.
    normalsData = (float *)cg_malloc(sizeof(float)*facesCount*3*3);

    printf("Vertex Count: %d\n", facesCount*3*3);
    Block* currentFace = (Block *) obj->faces->head;
    Point3D* v1;
    Point3D* v2;
    Point3D* v3;
    Point3D* n1;
    Point3D* n2;
    Point3D* n3;
    Face* currentFaceData;
    int v1Index;
    int v2Index;
    int v3Index;
    int n1Index;
    int n2Index;
    int n3Index;
    while(currentFace != NULL)
    {
        currentFaceData = (Face *) currentFace->data;
        
        // Carga vertices de la cara a vertexData.
        v1Index = currentFaceData->vertexes[0];
        v2Index = currentFaceData->vertexes[1];
        v3Index = currentFaceData->vertexes[2];

        v1 = (Point3D *)list_get_nth(obj->vertexes, v1Index);
        v2 = (Point3D *)list_get_nth(obj->vertexes, v2Index);
        v3 = (Point3D *)list_get_nth(obj->vertexes, v3Index);

        vertexData[i] = v1->x;
        vertexData[i+1] = v1->y;
        vertexData[i+2] = v1->z;

        vertexData[i+3] = v2->x;
        vertexData[i+4] = v2->y;
        vertexData[i+5] = v2->z;

        vertexData[i+6] = v3->x;
        vertexData[i+7] = v3->y;
        vertexData[i+8] = v3->z;

        // Carga normales a la cara normalsData.
        n1Index = currentFaceData->normals[0];
        n2Index = currentFaceData->normals[1];
        n3Index = currentFaceData->normals[2];
        
        n1 = (Point3D *)list_get_nth(obj->normals, n1Index);
        n2 = (Point3D *)list_get_nth(obj->normals, n2Index);
        n3 = (Point3D *)list_get_nth(obj->normals, n3Index);

        normalsData[i] = n1->x;
        normalsData[i+1] = n1->y;
        normalsData[i+2] = n1->z;

        normalsData[i+3] = n2->x;
        normalsData[i+4] = n2->y;
        normalsData[i+5] = n2->z;

        normalsData[i+6] = n3->x;
        normalsData[i+7] = n3->y;
        normalsData[i+8] = n3->z;

        i+=9; // Aumentamos el contador en 9 ya que cada face tiene 3 vertices.
        currentFace = currentFace->next;
    }
    printf("Finished loading vertex data.");

}

void obj_print(Obj* obj)
{
    printf("%s\n", "Vertexes:");
    Block* currentVertex = (Block *) obj->vertexes->head;
    int i = 0;
    while(currentVertex != NULL)
    {
        Point3D* vertex = (Point3D *) currentVertex->data;

        printf("%s%d\n", "Vertex ", i);
        point3D_print(vertex);
        currentVertex = currentVertex->next;
        i++;
    }
}

Obj* obj_new()
{
	Obj* obj = (Obj *)cg_malloc(sizeof(Obj));
	/* obj->name = (char *) cg_malloc(sizeof(char) * 100); */
	obj->vertexes = list_new(sizeof(Point3D));
	obj->normals = list_new(sizeof(Point3D));
	obj->textures = list_new(sizeof(Point2D));
	obj->faces = list_new(sizeof(Face));

    return obj;
}

Obj* obj_load(char *filename)
{
	printf("%s\n", "Start obj parser");

    Obj* obj = parse_obj(filename);

    printf("Vertex size is: %d\n", list_size(obj->vertexes));
    printf("Normals size is: %d\n", list_size(obj->normals));
    printf("Faces size is: %d\n", list_size(obj->faces));

    load_data_for_vertex_arrays(obj);

    return obj;
}

void obj_render(Obj * obj)
{
    int vertexCount = list_size(obj->faces)*3;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexData);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0, normalsData);

    /* glEnableClientState(GL_TEXTURE_COORD_ARRAY); */
    /* glTexCoordPointer(2, GL_FLOAT, 0, texData); */

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    /* glDisableClientState(GL_TEXTURE_COORD_ARRAY); */
}

void obj_free (Obj * obj)
{
    // Libero arrays utilizados para dibujar los obj.
    cg_free(vertexData);
    cg_free(normalsData);

	/* cg_free(obj->name); */
	list_free(obj->vertexes);
	list_free(obj->normals);
	list_free(obj->textures);
	list_free(obj->faces);
	cg_free(obj);
}
