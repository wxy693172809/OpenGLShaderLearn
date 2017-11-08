//
//  OpenGLDrawLayer.hpp
//  testShader
//
//  Created by Mrlee on 2017/10/16.
//
//

#ifndef OpenGLDrawLayer_hpp
#define OpenGLDrawLayer_hpp

#include <stdio.h>

#include "cocos2d.h"
using namespace cocos2d;

class OpenGLDrawLayer:public Layer
{
public:
    OpenGLDrawLayer();
    virtual ~OpenGLDrawLayer();

    virtual bool init() override;
    virtual void initOpenGL() = 0;
    
    virtual void draw(cocos2d::Renderer* renderer,const cocos2d::Mat4&parentTransform,uint32_t parentFlags)override;
    virtual void onDraw(const Mat4 &transform, uint32_t flags) = 0;
    
protected:
    cocos2d::CustomCommand _customCommand;
    
    GLuint _vao;
    GLuint _vertVBO;
    GLuint _colorVBO;
    GLuint _texCoordVBO;
    GLuint _textureVBO;
    
    GLuint _indexVBO;
};


class OpenGLDrawTriLayer:public OpenGLDrawLayer
{
public:
    OpenGLDrawTriLayer();
    virtual ~OpenGLDrawTriLayer();
    
    CREATE_FUNC(OpenGLDrawTriLayer);
    virtual void initOpenGL();
    virtual void onDraw(const Mat4 &transform, uint32_t flags);
    
private:
    int m_index = 0;
};

class OpenGLDrawQuadLayer:public OpenGLDrawLayer
{
public:
    OpenGLDrawQuadLayer();
    virtual ~OpenGLDrawQuadLayer();
    
    CREATE_FUNC(OpenGLDrawQuadLayer);
    virtual void initOpenGL();
    virtual void onDraw(const Mat4 &transform, uint32_t flags);
};

class OpenGLDrawCubeLayer:public OpenGLDrawLayer
{
public:
    OpenGLDrawCubeLayer();
    virtual ~OpenGLDrawCubeLayer();
    
    CREATE_FUNC(OpenGLDrawCubeLayer);
    virtual void initOpenGL();
    virtual void onDraw(const Mat4 &transform, uint32_t flags);
    
private:
    GLuint _textureVBO1;
    float m_count = 0.0f;
};

class  OpenGLDrawColorChangeLayer:public OpenGLDrawLayer
{
public:
    OpenGLDrawColorChangeLayer();
    virtual ~OpenGLDrawColorChangeLayer();
    CREATE_FUNC(OpenGLDrawColorChangeLayer);
    virtual void initOpenGL();
    virtual void onDraw(const Mat4 &transform,uint32_t flags);
private:
    float m_timeIndex = 0.0f;
};

class  OpenGLDrawTextureLayer:public OpenGLDrawLayer
{
public:
    OpenGLDrawTextureLayer();
    virtual ~OpenGLDrawTextureLayer();
    CREATE_FUNC(OpenGLDrawTextureLayer);
    virtual void initOpenGL();
    virtual void onDraw(const Mat4 &transform,uint32_t flags);
private:
    GLuint _textureVBO1;
    float m_count = 0.0f;
};

#endif /* OpenGLDrawLayer_hpp */
