#include "obj.h"
#include "parser.h"
#include <stdio.h>
#include <assert.h>
#include "framework/mm.h"
#include "GL/gl.h"

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

float* face_get_point3D_data(List* list, int positions[3])
{
    puts("Point3D data");
    Point3D* v1 = (Point3D *)list_get_nth(list, positions[0]);
    point3D_print(v1);
    Point3D* v2 = (Point3D *)list_get_nth(list, positions[1]);
    point3D_print(v2);
    Point3D* v3 = (Point3D *)list_get_nth(list, positions[2]);
    point3D_print(v3);

    // Serializo las coordenadas de los puntos para concatenar al vertex array.
    float* point3DData = (float *)cg_malloc(sizeof(float)*3*3);
    point3DData[0] = v1->x;
    point3DData[1] = v1->y;
    point3DData[2] = v1->z;

    point3DData[3] = v2->x;
    point3DData[4] = v2->y;
    point3DData[5] = v2->z;

    point3DData[6] = v3->x;
    point3DData[7] = v3->y;
    point3DData[8] = v3->z;

    return point3DData;
}

float* face_get_point2D_data(List* list, int positions[3])
{
    puts("Point3D data");
    Point2D* v1 = (Point2D *)list_get_nth(list, positions[0]);
    point2D_print(v1);
    Point2D* v2 = (Point2D *)list_get_nth(list, positions[1]);
    point2D_print(v2);
    Point2D* v3 = (Point2D *)list_get_nth(list, positions[2]);
    point2D_print(v3);

    // Serializo las coordenadas de los puntos para concatenar al vertex array.
    float* point2DData = (float *)cg_malloc(sizeof(float)*3*2);
    point2DData[0] = v1->x;
    point2DData[1] = v1->y;

    point2DData[2] = v2->x;
    point2DData[3] = v2->y;

    point2DData[4] = v3->x;
    point2DData[5] = v3->y;

    return point2DData;
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
    // Asinga la memoria necesaria para guardar todos los vertices.
    // Hay Faces*3*3 vertices.
    vertexData = (float *)cg_malloc(sizeof(float)*facesCount*3*3);
    printf("Vertex Count: %d\n", facesCount*3*3);
    Block* currentFace = (Block *) obj->faces->head;
    Point3D* v1;
    Point3D* v2;
    Point3D* v3;
    Face* currentFaceData;
    int v1Index;
    int v2Index;
    int v3Index;
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

        i+=9; // Aumentamos el contador en 9 ya que cada face tiene 3 vertices.
        currentFace = currentFace->next;
    }
    printf("Finished loading vertex data.");

}

void stub_obj(Obj* obj)
{
	/* obj->name = "box"; */

    //VERTEXES
	Point3D v1 = point3D_new( -0.500000, -0.500000, -0.500000);
	Point3D v2 = point3D_new(  0.500000, -0.500000, -0.500000);
	Point3D v3 = point3D_new(  0.500000, -0.500000,  0.500000);
	Point3D v4 = point3D_new( -0.500000, -0.500000,  0.500000);
	Point3D v5 = point3D_new( -0.500000,  0.500000,  0.500000);
	Point3D v6 = point3D_new(  0.500000,  0.500000,  0.500000);
	Point3D v7 = point3D_new(  0.500000,  0.500000, -0.500000);
	Point3D v8 = point3D_new( -0.500000,  0.500000, -0.500000);

	list_add(obj->vertexes, &v1);
	list_add(obj->vertexes, &v2);
	list_add(obj->vertexes, &v3);
	list_add(obj->vertexes, &v4);
	list_add(obj->vertexes, &v5);
	list_add(obj->vertexes, &v6);
	list_add(obj->vertexes, &v7);
	list_add(obj->vertexes, &v8);
	assert(list_size(obj->vertexes) == 8);

    printf("Let's Cycle a list!");
    Block* current = obj->vertexes->head;
    while(current != NULL)
    {
        point3D_print((Point3D *) current->data);
        current = current->next;
    }

	printf("%s\n\n\n", "");
	printf("%s\n", "VERTEXES");
	printf("%s\n", "v1");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 0));
	printf("%s\n", "v2");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 1));
	printf("%s\n", "v3");
	point3D_print((Point3D *)list_get_nth(obj->vertexes, 2));
	printf("%s\n\n\n", "");

	//NORMALS
	Point3D vn1 = point3D_new( 1.000000,  0.000000,  0.000000);
	Point3D vn2 = point3D_new(-1.000000,  0.000000,  0.000000);
	Point3D vn3 = point3D_new( 0.000000, -1.000000,  0.000000);
	Point3D vn4 = point3D_new( 0.000000,  1.000000,  0.000000);
	Point3D vn5 = point3D_new( 0.000000,  0.000000,  -1.000000);
	Point3D vn6 = point3D_new( 0.000000,  0.000000,  1.000000);

	list_add(obj->normals, &vn1);
	list_add(obj->normals, &vn2);
	list_add(obj->normals, &vn3);
	list_add(obj->normals, &vn4);
	list_add(obj->normals, &vn5);
	list_add(obj->normals, &vn6);
	assert(list_size(obj->normals) == 6);


	printf("%s\n", "NORMALS");
	printf("%s\n", "n1");
	point3D_print((Point3D *)list_get_nth(obj->normals, 0));
	printf("%s\n\n\n", "");

	//TEXTURES
	Point2D t1 = point2D_new(0.000000, 0.000000);
	Point2D t2 = point2D_new(3.000000, 0.000000);
	Point2D t3 = point2D_new(0.000000, 3.000000);
	Point2D t4 = point2D_new(3.000000, 3.000000);

	list_add(obj->textures, &t1);
	list_add(obj->textures, &t2);
	list_add(obj->textures, &t3);
	list_add(obj->textures, &t4);
	assert(list_size(obj->textures) == 4);

	printf("%s\n", "TEXTURES");
	printf("%s\n", "t1");
	point3D_print((Point3D *)list_get_nth(obj->textures, 0));
	printf("%s\n", "t2");
	point3D_print((Point3D *)list_get_nth(obj->textures, 1));
	printf("%s\n", "t3");
	point3D_print((Point3D *)list_get_nth(obj->textures, 2));
	printf("%s\n\n\n", "");

	//FACES
	Face *f1 = face_new(3,1,3,7,1,2,6,1,1);

	list_add(obj->faces, &f1);
	assert(list_size(obj->faces) == 1);

	printf("%s\n", "f1");
	face_print((Face *)list_get_nth(obj->faces, 0));

	puts("vertex data for f1");
    vertexData = face_get_point3D_data(obj->vertexes, f1->vertexes);
    for(int i=0; i < 9; i++)
    {
    	printf("%f\n", vertexData[i]);
    }

    puts("normals data for f1");
    normalsData = face_get_point3D_data(obj->normals, f1->normals);
    for(int i=0; i < 9; i++)
    {
    	printf("%f\n", normalsData[i]);
    }

    puts("tex data for f1");
    texData = face_get_point2D_data(obj->textures, f1->textures);
    for(int i=0; i < 6; i++)
    {
    	printf("%f\n", texData[i]);
    }

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

    /* glEnableClientState(GL_NORMAL_ARRAY); */
    /* glNormalPointer(GL_FLOAT,0, normalsData); */

    /* glEnableClientState(GL_TEXTURE_COORD_ARRAY); */
    /* glTexCoordPointer(2, GL_FLOAT, 0, texData); */

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    glDisableClientState(GL_VERTEX_ARRAY);
    /* glDisableClientState(GL_NORMAL_ARRAY); */
    /* glDisableClientState(GL_TEXTURE_COORD_ARRAY); */
}

void obj_free (Obj * obj)
{
	/* cg_free(obj->name); */
	list_free(obj->vertexes);
	list_free(obj->normals);
	list_free(obj->textures);
	list_free(obj->faces);
	cg_free(obj);
}
