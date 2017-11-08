#ifdef GL_ES
precision mediump float;
#endif

// frag.fsh
// 片元着色器，光栅化输出的每个片元都执行一遍片段着色器，生成一个或多个（多重渲染）颜色值作为输出
// 输入: varying, 输出: gl_FragColor + gl_FragDepth

//用于在顶点着色器和片段着色器之间传递数据，因此类型必须完全一直
//uniform vec4 u_color;
uniform float u_time;
varying vec4 v_fragmentColor;
// 每一个Shader程序都有一个main函数
float r;
float g;
float b;
void main()
{
    // gl开头的变量名是系统内置的变量
  //  float f = (v_fragmentColor.r + v_fragmentColor.g + v_fragmentColor.b) / 3.0;
    
    r = fract(v_fragmentColor.r + u_time);
    g = fract(v_fragmentColor.g + u_time);
    b = fract(v_fragmentColor.b + u_time);
//    (int)(v_fragmentColor * 100) % 100 / 100.0
    
   // gl_FragColor = v_fragmentColor;
    gl_FragColor = vec4(r,g,b,1);
 //   gl_FragColor = v_fragmentColor;
 //   gl_FragColor = u_color;
  //  gl_FragColor = v_fragmentColor;
  //  gl_FragColor = vec4(f, f, f, v_fragmentColor.a);
   // gl_FragColor = v_fragmentColor;// gl_FragColor 定义最终画在屏幕上面的像素点的颜色
}
