# Razterizador.

Obligatorio realizado por Marcelo de León.
El rasterizador implementado cumple con todas las características obligatorias:
    - Rasterización de modelos OBJ (Knight configurado).
    - Malla de alambre.
    - Soporte de una luz puntual.
    - Sombreado por medio de Goraud shading.
    - Mapeo de texturas con filtrado bilineal.
    - Rotación controlable con las flechas del teclado.

El obligatorio no implementa características opcionales.

## Entorno compatible.
El obligatorio es autocontenido y compatible con Linux.
Desarrollado en la máquina virtual provista por la cátedra, utilizando el Framework de Texturas.

Para ejecutar, simplemente correr el comando ```./run.sh``` en la raíz del proyecto.

## Configuración del teclado.

_Activar/Desactivar_
m -> Wireframe
p -> Goraud shader
s -> Specular
z -> Z-buffer
b -> Backface culling
l -> Luz

Rotación del modelo utilizando las flechas del teclado.
Traslacion hacia atrás -> i
Traslacion hacia adelante -> o
