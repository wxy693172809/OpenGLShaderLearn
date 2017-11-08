//
//  shaderScene.hpp
//  testShader
//
//  Created by Mrlee on 2017/10/13.
//
//

#ifndef shaderScene_hpp
#define shaderScene_hpp

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class shaderScene:public cocos2d::Layer
{
public:
    shaderScene();
    virtual ~shaderScene();
    
    static Scene* createScene();
    CREATE_FUNC(shaderScene);
    virtual bool init()override;
    
    virtual void draw(cocos2d::Renderer* renderer,const cocos2d::Mat4&parentTransform,uint32_t parentFlags) override;
    
    virtual void onDraw(const Mat4 &transform, uint32_t flags);
private:
    cocos2d::CustomCommand _customCommand;
    
    GLuint _vao;
    GLuint _vertVBO;
    GLuint _colorVBO;
};

#endif /* shaderScene_hpp */
