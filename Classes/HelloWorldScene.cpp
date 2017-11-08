#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    auto moveMove1 = MoveBy::create(1, Vec2(500,500));
    auto moveMove2 = moveMove1->reverse();
    layer->runAction(RepeatForever::create(Sequence::create(moveMove1,moveMove2, NULL)));
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
  
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

  
    //
//    /////////////////////////////
//    // 2. add a menu item with "X" image, which is clicked to quit the program
//    //    you may modify it.
//
//    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
//    
//	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));
//
//    // create menu, it's an autorelease object
//    auto menu = Menu::create(closeItem, NULL);
//    menu->setPosition(Vec2::ZERO);
//    this->addChild(menu, 1);
//
//    /////////////////////////////
//    // 3. add your codes below...
//
//    // add a label shows "Hello World"
//    // create and initialize a label
//    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);
//
//    // add "HelloWorld" splash screen"
//    auto sprite = Sprite::create("res/HelloWorld.png");
//
//    // position the sprite on the center of the screen
//    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
//
//    // add the sprite as a child to this layer
//    this->addChild(sprite, 0);
    
    
//    //create my own program
//    auto program = new GLProgram;
//    program->initWithFilenames("res/myVertextShader.vert", "res/myFragmentShader.frag");
//    program->link();
//    //set uniform locations
//    program->updateUniforms();
//    
//    //创建和绑定vao
//    uint32_t vao;
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    
//    //创建和绑定vbo
//    uint32_t vertexVBO;
//    glGenBuffers(1, &vertexVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
//    
//    
//    auto size = Director::getInstance()->getWinSize();
//    float vertercies[] = { 0,0,   //第一个点的坐标
//        size.width, 0,   //第二个点的坐标
//        size.width* 0.2f, size.height * 0.5f};  //第三个点的坐标
//    
//    float color[] = { 0, 1,0, 1,  1,0,0, 1, 0, 0, 1, 1};
//    
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
//    //获取vertex attribute "a_position"的入口点
//    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
//    //打开入a_position入口点
//    glEnableVertexAttribArray(positionLocation);
//    //传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
//    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//    
//    //set for color
//    uint32_t colorVBO;
//    glGenBuffers(1, &colorVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
//    
//    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
//    glEnableVertexAttribArray(colorLocation);
//    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//    
//    //for safty
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
//    auto fileUtiles = FileUtils::getInstance();
//    auto fragmentFilePath = fileUtiles->fullPathForFilename("res/ccShader_Avg_Black_White.frag");
//    auto fragSource = fileUtiles->getStringFromFile(fragmentFilePath);
//    
//    auto vertFilePath = fileUtiles->fullPathForFilename("res/ccShader_Avg_Black_White.vert");
//    auto vertSource = fileUtiles->getStringFromFile(vertFilePath);
//
//    
//  //  std::string vertSource = ccPositionTextureColor_vert;
//    auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(),fragSource.c_str());
//    auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);
//    sprite->setGLProgramState(glprogramstate);
    
    
//    this->setGLProgram(program);
    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(-1);
    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void HelloWorld::onDraw()
{
    
////    auto glProgram = CCGLProgram::createWithFilenames("res/myVertextShader.vert", "res/myFragmentShader.frag");
////    glProgram->link();
////    glProgram->updateUniforms();
////    this->setGLProgram(glProgram);
//    
//    //获得当前HelloWorld的shader
//    auto glProgram = getGLProgram();
//    glProgram->initWithFilenames("res/myVertextShader.vert", "res/myFragmentShader.frag");
//    glProgram->link();
//    glProgram->updateUniforms();
//    
//    //使用此shader
//    glProgram->use();
//    //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
//    glProgram->setUniformsForBuiltins();
//    
//    
//    auto size = Director::getInstance()->getVisibleSize();
//    auto pos = this->getParent()->convertToWorldSpace(this->getPosition());
//    //指定将要绘制的三角形的三个顶点，分别位到屏幕左下角，右下角和正中间的顶端
//    float vertercies[] = { pos.x,pos.y,   //第一个点的坐标
//        pos.x + size.width, pos.y,   //第二个点的坐标
//        pos.x + size.width / 2, pos.y + size.height};  //第三个点的坐标
//    //指定每一个顶点的颜色，颜色值是RGBA格式的，取值范围是0-1
//    float color[] = { 0, 1,0, 1,    //第一个点的颜色，绿色
//        1,0,0, 1,  //第二个点的颜色, 红色
//        0, 0, 1, 1};  //第三个点的颜色， 蓝色
//    //激活名字为position和color的vertex attribute
//    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
//    //分别给position和color指定数据源
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
//    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
//    //绘制三角形，所谓的draw call就是指这个函数调用
//    glDrawArrays(GL_TRIANGLES, 0, 3);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();
}
