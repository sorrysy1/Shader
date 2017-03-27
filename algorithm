//漫反射的主要公式   
/*
*SurfaceOutput  输出的内容  包括 法线Normal 反射率Albedo 透明度Alpha
*lightDir  光源位置
*atten    衰减率
*
*return 漫反射的rgba
*/
float4 LightingLambertTest(SurfaceOutput s, float3 lightDir, float atten)
 {
  float diff = max(0, dot(s.Normal, lightDir));// 漫反射强度
  float4 c;
  c.rgb = s.Albedo * _LightColor0.rgb * (diff * atten * 2);
  c.a = s.Alpha;
  return c;
 }
 
 
 
 
  void surf (Input IN, inout SurfaceOutput o) {
   // 移动法线纹理的X
   IN.uv_BumpMap.x = IN.uv_BumpMap.x + _Speed * _Time;
   // 对法线贴图进行采样
   fixed3 nor = UnpackNormal (tex2D(_BumpMap, IN.uv_BumpMap));
   // 对主材质进行采样
   fixed4 tex = tex2D(_MainTex, IN.uv_MainTex);
   // 【将反射的图像正常显示在此对象上】
   // 获取屏幕纹理坐标信息
   float2 screenUV = IN.screenPos.xy / IN.screenPos.w;
   // 针对法线纹理做法线扭曲，_Distortion为扭曲强度
   screenUV += nor.xy * _Distortion ; 
   // 对反射纹理进行采样，通过屏幕纹理
   fixed4 ref = tex2D(_Ref, screenUV);
 
   // 【将屏幕的纹理正常显示在此对象上】，如果不需要半透明可去掉
   // 获取顶点在屏幕的坐标为纹理
   float4 screenUV2 = IN.screenPos;
   // 以下是根据平台来判断是否需要倒置Y坐标
   // D3D:scale=-1，那么screenUV2.y = -1 * screenUV2.y + screenUV2.w;【倒置后+w让值域保持为0到1】
   // OpenGL:scale=1，那么screenUV2.y = screenUV2.y;
   #if UNITY_UV_STARTS_AT_TOP
   float scale = -1.0;
   #else
   float scale = 1.0;
   #endif
   screenUV2.y = (screenUV2.y - screenUV2.w * 0.5) * scale + screenUV2.w * 0.5;
   screenUV2.xy = screenUV2.xy / screenUV2.w;
   // 针对法线纹理做的法线扭曲,_Distortion为强度
   screenUV2.xy += nor.xy * _Distortion;
   // 对屏幕纹理进行采样
   fixed4 trans = tex2D(_GrabTexture,screenUV2.xy);
  
   // 最终颜色 = 主材质颜色 * 主颜色 * 主色显示强度
   o.Albedo = tex.rgb * _Color.rgb * _BlendLevel;
   // 反射颜色 = 反射材质颜色和【屏幕纹理颜色】的插值
   o.Emission = lerp(ref.rgb,trans.rgb,_Transparency);
   // 法线值 = 法线材质颜色 * 法线强度
   o.Normal = nor.rgb * _Bumpness;
   // 发光强度 = 主纹理A
   o.Gloss = tex.a;
   // 透明度 = 主色
   o.Alpha = tex.a * _Color.a;
   // 镜面高光【高光需要针对针对非立方体，如果是平面BlinnPhong也无效】
   o.Specular = _Shininess * _SpecColor; 
  }
  ENDCG
 }//end subshader
}
