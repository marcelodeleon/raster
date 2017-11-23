#include "obj.h"
#include "face.h"
#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include "framework/mm.h"
#include "GL/gl.h"

void load_data_for_vertex_arrays(Obj *obj)
{
    Point3D* v1;
    Point3D* v2;
    Point3D* v3;
    Point3D* n1;
    Point3D* n2;
    Point3D* n3;
    Point2D* t1;
    Point2D* t2;
    Point2D* t3;
    int v1Index;
    int v2Index;
    int v3Index;
    int n1Index;
    int n2Index;
    int n3Index;
    int t1Index;
    int t2Index;
    int t3Index;
    int i = 0; // Contador utilizado para cargar los vertices a vertexData y normales a normalsData.
    int texIndex = 0; // Contador utilizado para cargar las texturas a texData.
    Face* currentFaceData;
    Block* currentFace = (Block *) obj->faces->head;
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

        obj->vertexArray->vertexData[i] = v1->x;
        obj->vertexArray->vertexData[i+1] = v1->y;
        obj->vertexArray->vertexData[i+2] = v1->z;

        obj->vertexArray->vertexData[i+3] = v2->x;
        obj->vertexArray->vertexData[i+4] = v2->y;
        obj->vertexArray->vertexData[i+5] = v2->z;

        obj->vertexArray->vertexData[i+6] = v3->x;
        obj->vertexArray->vertexData[i+7] = v3->y;
        obj->vertexArray->vertexData[i+8] = v3->z;

        // Carga normales de la cara a normalsData.
        n1Index = currentFaceData->normals[0];
        n2Index = currentFaceData->normals[1];
        n3Index = currentFaceData->normals[2];

        n1 = (Point3D *)list_get_nth(obj->normals, n1Index);
        n2 = (Point3D *)list_get_nth(obj->normals, n2Index);
        n3 = (Point3D *)list_get_nth(obj->normals, n3Index);

        obj->vertexArray->normalsData[i] = n1->x;
        obj->vertexArray->normalsData[i+1] = n1->y;
        obj->vertexArray->normalsData[i+2] = n1->z;

        obj->vertexArray->normalsData[i+3] = n2->x;
        obj->vertexArray->normalsData[i+4] = n2->y;
        obj->vertexArray->normalsData[i+5] = n2->z;

        obj->vertexArray->normalsData[i+6] = n3->x;
        obj->vertexArray->normalsData[i+7] = n3->y;
        obj->vertexArray->normalsData[i+8] = n3->z;

        // Carga texturas de la cara a texData.
        t1Index = currentFaceData->textures[0];
        t2Index = currentFaceData->textures[1];
        t3Index = currentFaceData->textures[2];

        t1 = (Point2D *)list_get_nth(obj->textures, t1Index);
        t2 = (Point2D *)list_get_nth(obj->textures, t2Index);
        t3 = (Point2D *)list_get_nth(obj->textures, t3Index);

        obj->vertexArray->texData[texIndex] = t1->x;
        obj->vertexArray->texData[texIndex+1] = t1->y;

        obj->vertexArray->texData[texIndex+2] = t2->x;
        obj->vertexArray->texData[texIndex+3] = t2->y;

        obj->vertexArray->texData[texIndex+4] = t3->x;
        obj->vertexArray->texData[texIndex+5] = t3->y;

        i+=9; // Aumentamos el contador en 9 ya que cada face tiene 3 vertices con 3 coordenadas cada uno.
        // Aumentamos el contador en 6, cada face tiene 3 vertices con 2 coordenadas de textura cada uno.
        texIndex+=6;
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

    // Hasta este momento no se sabia la cuenta de las faces.
    // A partir de ahora puedo cargar la estructura VertexArray.
    obj->vertexArray = vertex_array_new(list_size(obj->faces));
    load_data_for_vertex_arrays(obj);

    return obj;
}

void obj_render(Obj *obj)
{
    int vertexCount = list_size(obj->faces)*3;
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, obj->vertexArray->vertexData);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT, 0, obj->vertexArray->normalsData);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, obj->vertexArray->texData);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void obj_free (Obj * obj)
{
	/* cg_free(obj->name); */
	list_free(obj->vertexes);
	list_free(obj->normals);
	list_free(obj->textures);
	list_free(obj->faces);
	vertex_array_free(obj->vertexArray);
	cg_free(obj);
}
