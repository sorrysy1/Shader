//shader中光照效果的呈现
//在shader中光照效果的呈现主要由三种光照效果呈现： 环境光（Ambient）、漫反射光（Diffuse）、高光（Specular）
//最终的光照就是这些光照效果的叠加  ：AmbientColor + DiffuseColor + SpecularColor

//将指定光源颜色的RGB值范围是[0-1]，然后把光源颜色和物体表面颜色相乘，得到光反射的颜色

//Intensity:强度
//V3:空间向量
//nomalLightV3 :法向量
//gDirectionalLightV3 ：入射光向量
//gDirectionalLight.Color :光照颜色  (RGB值)
/*
*环境光  环境光就是在没有太阳的情况下，你周围的光照情况，它模拟的是太阳光照射地面后，在不同的位置被反射，折射，
*最后混合在一起的一种光线，环境光没有方向，没有起点，它是一种均匀的光照效果，即使在阴影中，也有环境光存在。
*/
vec4 AmbientColor()
{
  vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity; 
  return AmbientColor;
}

/*
*漫反射  漫反射光是光照在物体表面被反射
*用公式表示就是光源的强度乘以光源和物体表面法向夹角的余弦值
*/
vec4 DiffuseColor()
{
  //计算漫反射强度由入射光与物体法向量点积获得 
  //通常的处理是把 入射光的向量 与 法线的向量  归一获得矢向量  两矢向量点积获得夹角余弦值  余弦值区间[0,1]
  //入射向量 取反向的 ： -gDirectionalLightV3
  float DiffuseIntensity = dot(normalize(-gDirectionalLightV3),normalize (nomalLightV3)); 
  DiffuseIntensity = max(0.0,DiffuseIntensity);
  vec4 DiffuseColor =vec4( gDirectionalLight.Color,1.0f) * DiffuseIntensity; 
  return DiffuseColor;
}

/*
*高光    高光是物体本身的一种属性，当光作用于物体时，物体上某个部分可能特别亮，这就是高光，高光会随着我们的视点移动而不断改变位置。
*
*'I' 是照亮物体表面的入射光
*'N' 是物体表面法向
*'R' 是入射光在照射到物体表面后的发射光，反射光和入射光是沿着法线对称的。
*'E' 是物体表面上的点指向视点的向量。
*'α' 'R' 和 'E'两个向量的夹角
*我们将用点积操作来求得α的cosine值，这将作为我们计算高光时的因子，当α大于90度时候，cosine值为负值，此时没有高光效果。
*我们用reflect函数来计算反射光R
*/
vec4 SpecularColor()
{
 vec3 R = reflect (I,  N)
   //计算高光指数
 float SpecularIntensity = dot(normalize(R),normalize (E)); 
 //材质高光强度('M')
 //P高光效果增强系数[1~？？]  p越大效果越亮
 //SpecularIntensity 是高光指数做效果的次方运算得出最后的效果系数
  SpecularIntensity = pow（SpecularIntensity,p）;
  //gMatSpecularIntensity 光强度 
  vec4 SpecularColor =vec4( gDirectionalLight.Color,1.0f) * SpecularIntensity * gMatSpecularIntensity ; 
  return SpecularColor;
}




