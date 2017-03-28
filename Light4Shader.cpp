//shader中光照效果的呈现
//在shader中光照效果的呈现主要由三种光照效果呈现： 环境光（Ambient）、漫反射光（Diffuse）、镜面反射光即高光（Specular）
//最终的光照就是这些光照效果的叠加  ：AmbientColor + DiffuseColor + SpecularColor

//将指定光源颜色的RGB值范围是[0-1]，然后把光源颜色和物体表面颜色相乘，得到光反射的颜色


/*环境光  环境光就是在没有太阳的情况下，你周围的光照情况，它模拟的是太阳光照射地面后，在不同的位置被反射，折射，
*/最后混合在一起的一种光线，环境光没有方向，没有起点，它是一种均匀的光照效果，即使在阴影中，也有环境光存在。

vec4 AmbientColor = vec4(gDirectionalLight.Color, 1.0f) * gDirectionalLight.AmbientIntensity; 

/*
*/
