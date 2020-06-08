#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "upng.h"
#include "camera.h"

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t trianglesToRender[MAX_TRIANGLES_PER_MESH];
int numTrianglesToRender = 0;

mat4_t worldMatrix;
mat4_t projectionMatrix;
mat4_t viewMatrix;

bool _running = true;
int previousFrameTime = 0;
float deltaTime = 0;

bool setup()
{
  renderMethod = RENDER_WIRE;
  cullMethod = CULL_BACKFACE;

  _colorBuffer = (uint32_t *)malloc(sizeof(uint32_t) * screenWidth * screenHeight);
  _colorBufferTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);

  zBuffer = (float*) malloc(sizeof(float) * screenWidth * screenHeight);

  float fov = M_PI/3; // 180/3 or 60deg
  float aspect = (float)screenHeight / (float)screenWidth;
  float znear = 0.1;
  float zfar = 100.0;
  projectionMatrix = mat4_makePerspective(fov, aspect, znear, zfar);

  loadObjFileData("./assets/cube.obj");
  load_png_texture_data("./assets/cube.png");
}

void handleInput()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      _running = false;
      break;

    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
        _running = false;
      if (event.key.keysym.sym == SDLK_1)
        renderMethod = RENDER_WIRE_VERTEX;
      if (event.key.keysym.sym == SDLK_2)
        renderMethod = RENDER_WIRE;
      if (event.key.keysym.sym == SDLK_3)
        renderMethod = RENDER_FILL_TRIANGLE;
      if (event.key.keysym.sym == SDLK_4)
        renderMethod = RENDER_FILL_TRIANGLE_WIRE;
      if (event.key.keysym.sym == SDLK_5)
        renderMethod = RENDER_TEXTURED;
      if (event.key.keysym.sym == SDLK_6)
        renderMethod = RENDER_TEXTURED_WIRE;
      if (event.key.keysym.sym == SDLK_c)
        cullMethod = CULL_BACKFACE;
      if (event.key.keysym.sym == SDLK_v)
        cullMethod = CULL_NONE;
      if (event.key.keysym.sym == SDLK_w)
      {
        camera.forwardVelocity = vec3Mul(camera.direction, 5.0 * deltaTime);
        camera.position = vec3Add(camera.position, camera.forwardVelocity);
      }
      if (event.key.keysym.sym == SDLK_s)
      {
        camera.forwardVelocity = vec3Mul(camera.direction, 5.0 * deltaTime);
        camera.position = vec3Sub(camera.position, camera.forwardVelocity);
      }
      if (event.key.keysym.sym == SDLK_a)
        camera.yawAngle += 1.0 * deltaTime;
      if (event.key.keysym.sym == SDLK_d)
        camera.yawAngle -= 1.0 * deltaTime;
      if (event.key.keysym.sym == SDLK_q)
        camera.position.y += 3.0 * deltaTime;
      if (event.key.keysym.sym == SDLK_e)
        camera.position.y -= 3.0 * deltaTime;
        
      break;

    default:
      break;
    }
  }
}

void update()
{
  int timeToWait = FRAME_LENGHT - (SDL_GetTicks() - previousFrameTime);
  if (timeToWait > 0 && timeToWait <= FRAME_LENGHT)
  {
    SDL_Delay(timeToWait);
  }
  deltaTime = (SDL_GetTicks() - previousFrameTime) / 1000.0;
  previousFrameTime = SDL_GetTicks();

  numTrianglesToRender = 0;

  // mesh.rotation.x += 0.01 * deltaTime;
  // mesh.rotation.y += 0.01 * deltaTime;
  // mesh.rotation.z += 0.01 * deltaTime;
  
  // mesh.scale.x += 0.002 * deltaTime;
  // mesh.scale.y += 0.001 * deltaTime;

  // mesh.translation.x += 0.01 * deltaTime;
  mesh.translation.z = 10.0;

  // camera.position.x += 0.5 * deltaTime;
  // camera.position.y += 0.5 * deltaTime;

  
  vec3_t up = { 0, 1, 0 };
  vec3_t target = { 0, 0, 1};
  mat4_t cameraYawRotation = mat4_makeRotationY(camera.yawAngle);
  camera.direction = vec3_fromVec4( mat4_mulVec4(cameraYawRotation, vec4_fromVec3(target)) );
  target = vec3Add(camera.position, camera.direction);
  mat4_t viewMatrix = mat4_lookAt(camera.position, target, up);
  
  mat4_t scaleMatrix = mat4_makeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t translationMatrix = mat4_makeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
  mat4_t rotationMatrixX = mat4_makeRotationX(mesh.rotation.x);
  mat4_t rotationMatrixY = mat4_makeRotationY(mesh.rotation.y);
  mat4_t rotationMatrixZ = mat4_makeRotationZ(mesh.rotation.z);

  int numFaces = array_length(mesh.faces);
  for (int i = 0; i < numFaces; ++i)
  {
    face_t meshFace = mesh.faces[i];

    vec3_t faceVertices[3];
    faceVertices[0] = mesh.vertices[meshFace.a];
    faceVertices[1] = mesh.vertices[meshFace.b];
    faceVertices[2] = mesh.vertices[meshFace.c];

    vec4_t transformedVertices[3];
    for (int j = 0; j < 3; ++j)
    {
      vec4_t transformedVertex = vec4_fromVec3(faceVertices[j]);
      
      worldMatrix = mat4_identity();
      worldMatrix = mat4_mulMat4(scaleMatrix, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixX, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixY, worldMatrix);
      worldMatrix = mat4_mulMat4(rotationMatrixZ, worldMatrix);
      worldMatrix = mat4_mulMat4(translationMatrix, worldMatrix);      

      transformedVertex = mat4_mulVec4(worldMatrix, transformedVertex);
      transformedVertex = mat4_mulVec4(viewMatrix, transformedVertex);
      transformedVertices[j] = transformedVertex;
    }
  
    //Culling faces
    vec3_t vecA = vec3_fromVec4(transformedVertices[0]);
    vec3_t vecB = vec3_fromVec4(transformedVertices[1]);
    vec3_t vecC = vec3_fromVec4(transformedVertices[2]);

    vec3_t vecAB = vec3Sub(vecB, vecA);
    vec3_t vecAC = vec3Sub(vecC, vecA);
    vecAB = vec3Normalize(vecAB);
    vecAC = vec3Normalize(vecAC);

    vec3_t normal = vec3Cross(vecAB, vecAC);
    normal = vec3Normalize(normal);

    vec3_t origin = {0, 0, 0};
    vec3_t cameraRay = vec3Sub(vec3Sub(origin, camera.position), vecA);

    float dotNormalCamera = vec3Dot(normal, cameraRay);

    if (cullMethod == CULL_BACKFACE && dotNormalCamera < 0) continue;

    vec4_t projectedPoints[3];

    for (int j = 0; j < 3; ++j)
    {
      projectedPoints[j] = mat4_mulVec4Projection(projectionMatrix, transformedVertices[j]);
      
      //scale into the view
      projectedPoints[j].x *= (screenWidth / 2.0);
      projectedPoints[j].y *= (screenHeight / 2.0);

      //fix inverted y orientation
      projectedPoints[j].y *= -1;
      
      //translate to the middle of the screen
      projectedPoints[j].x += (screenWidth / 2.0);
      projectedPoints[j].y += (screenHeight / 2.0);
    }

    float lightIntensityFactor = -vec3Dot(normal, light.direction);

    uint32_t triangleColor = light_applyIntensity(meshFace.color, lightIntensityFactor);

    triangle_t projectedTriangle = {
        .points = {
            { projectedPoints[0].x, projectedPoints[0].y, projectedPoints[0].z, projectedPoints[0].w },
            { projectedPoints[1].x, projectedPoints[1].y, projectedPoints[1].z, projectedPoints[1].w },
            { projectedPoints[2].x, projectedPoints[2].y, projectedPoints[2].z, projectedPoints[2].w }
        },
        .color = triangleColor,
        .texCoords = {
          { meshFace.uvA.u, meshFace.uvA.v},
          { meshFace.uvB.u, meshFace.uvB.v},
          { meshFace.uvC.u, meshFace.uvC.v}
        }
    };

    if(numTrianglesToRender < MAX_TRIANGLES_PER_MESH)
    {
      trianglesToRender[numTrianglesToRender++] = projectedTriangle;
    }
  }

}

void render()
{
  SDL_RenderClear(_renderer);

  for (int i = 0; i < numTrianglesToRender; ++i)
  {
    triangle_t triangle = trianglesToRender[i];

    if (renderMethod == RENDER_FILL_TRIANGLE || renderMethod == RENDER_FILL_TRIANGLE_WIRE)
    {
      drawFilledTriangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w,
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
          triangle.color);
    }

    if(renderMethod == RENDER_TEXTURED || renderMethod == RENDER_TEXTURED_WIRE)
    {
      drawTexturedTriangle(
          triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texCoords[0].u, triangle.texCoords[0].v,
          triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texCoords[1].u, triangle.texCoords[1].v,
          triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texCoords[2].u, triangle.texCoords[2].v,
          meshTexture
      );
    }

    if (renderMethod == RENDER_WIRE || renderMethod == RENDER_WIRE_VERTEX || renderMethod == RENDER_FILL_TRIANGLE_WIRE || renderMethod == RENDER_TEXTURED_WIRE)
    {
      drawTriangle(
          triangle.points[0].x, triangle.points[0].y,
          triangle.points[1].x, triangle.points[1].y,
          triangle.points[2].x, triangle.points[2].y,
          0xffff0000);
    }

    if (renderMethod == RENDER_WIRE_VERTEX)
    {
      drawRect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xffff0000);
      drawRect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xffff0000);
      drawRect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xffff0000);
    }
  }

  renderColorBuffer();
  clearColorBuffer(0xff000000);
  clearZBuffer();
  SDL_RenderPresent(_renderer);
}

void freeResources()
{
  free(_colorBuffer);
  free(zBuffer);
  upng_free(pngTexture);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

int main(int argc, char *argv[])
{
  _running = init();

  setup();

  while (_running)
  {
    handleInput();
    update();
    render();
  }

  freeResources();
  quit();

  return 0;
}