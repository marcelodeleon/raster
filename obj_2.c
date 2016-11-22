#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "GL/gl.h"
#include "obj.h"

// globals
FILE *stream;
Obj *gameobject[MAX_GAME_OBJECTS];
int numobjects = 0,objectsInFile = 0;
float * vertices;
float * normales;
float * texturas;

void creteArrayVertices(Obj * obj);
void creteArrayNormales(Obj * obj);
void creteArrayTexturas(Obj * obj);
char **strsplit(const char* str, const char* delim, size_t* numtokens);

char **strsplit(const char* str, const char* delim, size_t* numtokens) {
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char*));
    char *token, *rest = s;

    while ((token = strsep(&rest, delim)) != NULL) {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
    }

    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char*));
    }
    *numtokens = tokens_used;
    free(s);
    return tokens;
}


void obj_CountObjects()
{
    char s[100];
    char objectname[100];
    char *tok;

    objectsInFile = 0;
    while ((fgets(s,100,stream)) != NULL)
    {
        if (s[0] == 'g')
        {
            if (strlen(s) > 2) {
                tok = strtok(s," ");
                tok = strtok(NULL," ");
                if (strcmp(objectname,tok) != 0){
                    strcpy(objectname,tok);
                    objectname[strlen(tok)-1] = 0;
                    objectsInFile++;
                }
            }
        }
    }
    if (objectsInFile == 0)
        objectsInFile = 1;
    numobjects += objectsInFile;
}

int ObjFileQue(int whichobject,char whatdata)
{
    char s[100],objectname[100];
    char *tok;
    int numobj = 0;
    int linecount = 0,i;

    rewind(stream);
    while ((fgets(s,100,stream)) != NULL)
    {
        linecount++;
        if (s[0] == 'g' || objectsInFile == 1)
        {
            if (strlen(s) > 2)
            {
                tok = strtok(s," ");
                tok = strtok(NULL," ");
                if (strcmp(objectname,tok) != 0 || objectsInFile == 1)
                {
                    strcpy(objectname,tok);
                    objectname[strlen(tok)-1] = 0;
                    numobj++;
                    if ((numobj-1) == whichobject)
                    {
                        if (objectsInFile == 1)
                        {
                            rewind(stream);
                            linecount = 0;
                        }
                        while ((fgets(s,100,stream)) != NULL)
                        {
                            if (s[0] == whatdata)
                            {
                                rewind(stream);
                                for (i=0;i<linecount;i++)
                                    fgets(s,100,stream);
                                return 1;
                            }
                            linecount++;
                        }
                    }
                }
            }

        }
    }
    return 0;
}

void LoadFaces(int theobject)
{
    char s[100];
    int numsides,vert[4];
    char *tok;
    char temp[100];
    int i,facecount = 0;
    FACE *newface;

    if(ObjFileQue(theobject,'f')){
        while ((fgets(s,100,stream)) != NULL && s[0] == 'f')
        {
            numsides = 4;
            tok = strtok(s," ");
            i = 0;
            while ((tok = strtok(NULL," ")) != NULL)
            {
                strcpy(temp,tok);
                temp[strcspn(temp,"/")] = 0;
                vert[i] = (int)strtol(temp,NULL,10);
                i++;
            }
            newface = (FACE *)malloc(sizeof(FACE));
            gameobject[theobject + (numobjects - objectsInFile)]->faces[facecount] = newface;
            gameobject[theobject + (numobjects - objectsInFile)]->faces[facecount]->numsides = numsides;
            for(i=0;i<4;i++)
                gameobject[theobject + (numobjects - objectsInFile)]->faces[facecount]->vert[i] = vert[i];
            facecount++;
        }
        gameobject[theobject + (numobjects - objectsInFile)]->numfaces = facecount;
    }
}

void LoadFaces2(int theobject)
{
    char s[100];
    int vert[4], norm[4], text[4];
    char *tok, *tok2;
    char temp[100];
    int i,j,facecount = 0;
    FACE2 *newface;

    if(ObjFileQue(theobject,'f')){
        while ((fgets(s,100,stream)) != NULL && s[0] == 'f')
        {
            tok = strtok(s," ");
            i = 0;
            while ((tok = strtok(NULL," ")) != NULL)
            {
                char **tokens;
                size_t numtokens;
                tokens = strsplit(tok, "/", &numtokens);

//                printf("%s ", tokens[0]);
//                printf("%s ", tokens[1]);
//                printf("%s \n", tokens[2]);

                if(i==0) {
                    newface = (FACE2 *) malloc(sizeof(FACE2));
                    gameobject[theobject + (numobjects - objectsInFile)]->faces2[facecount] = newface;
                }
                gameobject[theobject + (numobjects - objectsInFile)]->faces2[facecount]->vertices[i] = (int)strtol(tokens[0],NULL,10);
                gameobject[theobject + (numobjects - objectsInFile)]->faces2[facecount]->normales[i] = (int)strtol(tokens[1],NULL,10);
                gameobject[theobject + (numobjects - objectsInFile)]->faces2[facecount]->texturas[i] = (int)strtol(tokens[2],NULL,10);

                free(tokens[0]);
                free(tokens[1]);
                free(tokens[2]);
                free(tokens);

                i++;
            }
            facecount++;
        }
        gameobject[theobject + (numobjects - objectsInFile)]->numfaces2 = facecount;
        printf("Debug: numero de caras faces v2: %d \n",facecount);
    }
}

void LoadVertices(int theobject)
{
    char s[100];
    char *tok;
    char temp[100];
    float coor[3];
    int vertcount = 0,i;
    VERTEX *newvert;

    if (ObjFileQue(theobject,'v')){
        while ((fgets(s,100,stream)) != NULL && (s[0] == 'v' && s[1] == ' '))
        {
            tok = strtok(s," ");
            for (i=0;i<3;i++)
            {
                tok = strtok(NULL," ");
                strcpy(temp,tok);
                temp[strcspn(temp," ")] = 0;
                coor[i] = (float)atof(temp);
            }
            newvert = (VERTEX *)malloc(sizeof(VERTEX));
            gameobject[theobject + (numobjects - objectsInFile)]->vertices[vertcount] = newvert;
            gameobject[theobject + (numobjects - objectsInFile)]->vertices[vertcount]->x = coor[0];
            gameobject[theobject + (numobjects - objectsInFile)]->vertices[vertcount]->y = coor[1];
            gameobject[theobject + (numobjects - objectsInFile)]->vertices[vertcount]->z = coor[2];
            vertcount++;
        }
        gameobject[theobject + (numobjects - objectsInFile)]->numverts = vertcount;
    }
}

void LoadNormales(int theobject)
{
    char s[100];
    char *tok;
    char temp[100];
    float coor[3];
    int normalCount = 0,i;
    VERTEX *newvert;

    if (ObjFileQue(theobject,'n')){
        while ((fgets(s,100,stream)) != NULL && (s[0] == 'n' && s[1] == ' '))
        {
            tok = strtok(s," ");
            for (i=0;i<3;i++)
            {
                tok = strtok(NULL," ");
                strcpy(temp,tok);
                temp[strcspn(temp," ")] = 0;
                coor[i] = (float)atof(temp);
            }
            newvert = (VERTEX *)malloc(sizeof(VERTEX));
            gameobject[theobject + (numobjects - objectsInFile)]->normales[normalCount] = newvert;
            gameobject[theobject + (numobjects - objectsInFile)]->normales[normalCount]->x = coor[0];
            gameobject[theobject + (numobjects - objectsInFile)]->normales[normalCount]->y = coor[1];
            gameobject[theobject + (numobjects - objectsInFile)]->normales[normalCount]->z = coor[2];
            normalCount++;
        }
        gameobject[theobject + (numobjects - objectsInFile)]->numNormales = normalCount;
    }
}

void LoadTexturas(int theobject)
{
    char s[100];
    char *tok;
    char temp[100];
    float coor[2];
    int textCount = 0,i;
    VERTEX2 *newvert;

    if (ObjFileQue(theobject,'t')){
        while ((fgets(s,100,stream)) != NULL && (s[0] == 't' && s[1] == ' '))
        {
            tok = strtok(s," ");
            for (i=0;i<2;i++)
            {
                tok = strtok(NULL," ");
                strcpy(temp,tok);
                temp[strcspn(temp," ")] = 0;
                coor[i] = (float)atof(temp);
            }
            newvert = (VERTEX2 *)malloc(sizeof(VERTEX2));
            gameobject[theobject + (numobjects - objectsInFile)]->texturas[textCount] = newvert;
            gameobject[theobject + (numobjects - objectsInFile)]->texturas[textCount]->x = coor[0];
            gameobject[theobject + (numobjects - objectsInFile)]->texturas[textCount]->y = coor[1];
            textCount++;
        }
        gameobject[theobject + (numobjects - objectsInFile)]->numTexturas = textCount;
    }
}


Obj* obj_load(char *filename)
{
    // open and read obj file
    int i;
    Obj *obj;

    if((stream = fopen( filename, "r+t" )) != NULL )
    {
        obj_CountObjects();
        for (i=numobjects - objectsInFile; i<numobjects; i++)
        {
            obj = (Obj *)malloc(sizeof(Obj));
            obj->numfaces=0;
            obj->numfaces2=0;
            obj->numNormales=0;
            obj->numverts=0;
            obj->numTexturas=0;

            gameobject[i] = obj;
            LoadVertices(i - (numobjects - objectsInFile));
            LoadNormales(i - (numobjects - objectsInFile));
            LoadTexturas(i - (numobjects - objectsInFile));
            LoadFaces2(i - (numobjects - objectsInFile));
            creteArrayVertices(gameobject[i]);
            creteArrayNormales(gameobject[i]);
            creteArrayTexturas(gameobject[i]);
        }
        fclose( stream );
    }
    return obj;
}

void obj_render(Obj * obj)
{
    int i;
    glBegin(GL_TRIANGLES);
    for(i=0; i< obj->numfaces; i++) {
        glVertex3f(
            obj->vertices[obj->faces[i]->vert[0] -1]->x,
            obj->vertices[obj->faces[i]->vert[0] -1]->y,
            obj->vertices[obj->faces[i]->vert[0] -1]->z
        );
        glVertex3f(
            obj->vertices[obj->faces[i]->vert[1] -1]->x,
            obj->vertices[obj->faces[i]->vert[1] -1]->y,
            obj->vertices[obj->faces[i]->vert[1] -1]->z
        );
        glVertex3f(
            obj->vertices[obj->faces[i]->vert[2] -1]->x,
            obj->vertices[obj->faces[i]->vert[2] -1]->y,
            obj->vertices[obj->faces[i]->vert[2] -1]->z
        );
    }
    glEnd();
}

void creteArrayVertices(Obj * obj)
{
    printf("Debug: numero de vertices %d \n", obj->numverts);
    int i;
    int verticesArraySize = obj->numfaces2*3*3;
    vertices = (float *) malloc(sizeof(float) * verticesArraySize);

    for (i = 0; i < obj->numfaces2 ; ++i) {
        vertices[9*i + 0] = obj->vertices[obj->faces2[i]->vertices[0] -1 ]->x;
        vertices[9*i + 1] = obj->vertices[obj->faces2[i]->vertices[0] -1 ]->y;
        vertices[9*i + 2] = obj->vertices[obj->faces2[i]->vertices[0] -1 ]->z;

        vertices[9*i + 3] = obj->vertices[obj->faces2[i]->vertices[1] -1 ]->x;
        vertices[9*i + 4] = obj->vertices[obj->faces2[i]->vertices[1] -1 ]->y;
        vertices[9*i + 5] = obj->vertices[obj->faces2[i]->vertices[1] -1 ]->z;

        vertices[9*i + 6] = obj->vertices[obj->faces2[i]->vertices[2] -1 ]->x;
        vertices[9*i + 7] = obj->vertices[obj->faces2[i]->vertices[2] -1 ]->y;
        vertices[9*i + 8] = obj->vertices[obj->faces2[i]->vertices[2] -1 ]->z;
    }
}

void creteArrayNormales(Obj * obj)
{
    printf("Debug: Numero de normales %d \n", obj->numNormales);
    int i;
    int normalesArraySize = obj->numfaces2*3*3;  //5706
    normales = (float *) malloc(sizeof(float) * normalesArraySize);

    for (i = 0; i < obj->numfaces2 ; ++i) {
        normales[9*i + 0] = obj->normales[obj->faces2[i]->normales[0] -1 ]->x;
        normales[9*i + 1] = obj->normales[obj->faces2[i]->normales[0] -1 ]->y;
        normales[9*i + 2] = obj->normales[obj->faces2[i]->normales[0] -1 ]->z;

        normales[9*i + 3] = obj->normales[obj->faces2[i]->normales[1] -1 ]->x;
        normales[9*i + 4] = obj->normales[obj->faces2[i]->normales[1] -1 ]->y;
        normales[9*i + 5] = obj->normales[obj->faces2[i]->normales[1] -1 ]->z;

        normales[9*i + 6] = obj->normales[obj->faces2[i]->normales[2] -1 ]->x;
        normales[9*i + 7] = obj->normales[obj->faces2[i]->normales[2] -1 ]->y;
        normales[9*i + 8] = obj->normales[obj->faces2[i]->normales[2] -1 ]->z; //5705
    }
}

void creteArrayTexturas(Obj * obj)
{
    printf("Numero de texturas %d \n", obj->numTexturas);
    printf("Numero de numfaces2 %d \n", obj->numfaces2);
    int i;
    int texturasArraySize = obj->numfaces2*3*2;
    printf("numero texturasArraySize %d \n", texturasArraySize); //634*3*2 = 3804
    texturas = (float *) malloc(sizeof(float) * texturasArraySize);

    for (i = 0; i < obj->numfaces2; ++i) {
        if(i<528) {
            texturas[(6 * i) + 0] = obj->texturas[obj->faces2[i]->texturas[0] - 1]->x;
            texturas[(6 * i) + 1] = obj->texturas[obj->faces2[i]->texturas[0] - 1]->y;

            texturas[(6 * i) + 2] = obj->texturas[obj->faces2[i]->texturas[1] - 1]->x;
            texturas[(6 * i) + 3] = obj->texturas[obj->faces2[i]->texturas[1] - 1]->y;

            texturas[(6 * i) + 4] = obj->texturas[obj->faces2[i]->texturas[2] - 1]->x;
            texturas[(6 * i) + 5] = obj->texturas[obj->faces2[i]->texturas[2] - 1]->y;  //3803
        }
    }
}

void obj_render2(Obj * obj)
{
    // activate and specify pointer to vertex array
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertices);

    glEnableClientState(GL_NORMAL_ARRAY);
    glNormalPointer(GL_FLOAT,0, normales);

    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texturas);

    // draw
    glDrawArrays(GL_TRIANGLES, 0, obj->numfaces2 * 3);

    // deactivate vertex arrays after drawing
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


void obj_free(Obj * obj) {
    int i;

    for (i = 0; i < obj->numfaces2; ++i) {
        free(obj->faces2[i]);
    }

    for (i = 0; i < obj->numfaces; ++i) {
        free(obj->faces[i]);
    }

    for (i = 0; i < obj->numverts; ++i) {
        free(obj->vertices[i]);
    }

    for (i = 0; i < obj->numTexturas; ++i) {
        free(obj->texturas[i]);
    }

    free(vertices);
    free(normales);
    free(texturas);

    free(obj);

}
