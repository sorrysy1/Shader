
//记录光照效果  包含一部分优化以及计算方法
//摘录自unity shader
//带纹理的光照效果


//在处理纹理图片之前需要给该纹理图片设置其需要进行高光效果的  高光通道（tex.a） a 的取值范围决定了高光效果1~0  1为最强 0 为无

Shader "ApcShader/BlinnPhongWithTex"
{
	//属性
	Properties
	{
		_Diffuse("Diffuse", Color) = (1,1,1,1)
		_Specular("Specular", Color) = (1,1,1,1)
		_SpecularScale("SpecularScale", Range(0.0, 5.0)) = 1.0
		_Gloss("Gloss", Range(0.0, 1)) = 20
		_MainTex("RGBSpecular", 2D) = "white"{}
	}

	//子着色器	
	SubShader
	{
		Pass
		{
			//定义Tags
			Tags{ "LightingMode" = "ForwardBase" }

			CGPROGRAM
			//引入头文件
			#include "Lighting.cginc"

			//定义函数
			#pragma vertex vert
			#pragma fragment frag

			//定义Properties中的变量
			fixed4 _Diffuse;
			fixed4 _Specular;
			float _Gloss;
			float _SpecularScale;
			sampler2D _MainTex;
			float4 _MainTex_ST;

			//定义结构体：应用阶段到vertex shader阶段的数据
			struct a2v
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float4 texcoord : TEXCOORD0;
			};

			//定义结构体：vertex shader阶段输出的内容
			struct v2f
			{
				float4 pos : SV_POSITION;
				float3 worldNormal : NORMAL;
				float3 worldPos : TEXCOORD0;
				float2 uv : TEXCOORD1;
			};

			//顶点shader
			v2f vert(a2v v)
			{
				v2f o;
				o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
				//法线转化到世界空间
				o.worldNormal = normalize(mul(v.normal, (float3x3)_World2Object));
				//顶点位置转化到世界空间 
				o.worldPos = mul(_Object2World, v.vertex).xyz;
				//转化uv
				o.uv = TRANSFORM_TEX(v.texcoord, _MainTex);
				return o;
			}

			//片元shader
			fixed4 frag(v2f i) : SV_Target
			{
				//环境光
				fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
				//世界空间下光线方向
				fixed3 worldLight = normalize(_WorldSpaceLightPos0.xyz);
				//需要再次normalize
				fixed3 worldNormal = normalize(i.worldNormal);
				//计算Diffuse
				fixed3 diffuse = _LightColor0.rgb * (dot(worldNormal, worldLight) * 0.5 + 0.5);
				//计算视线方向（相机位置-像素对应位置）
        //_WorldSpaceCameraPos.xyz - i.worldPos.xyz 应该放在顶点着色器中计算 优化片元着色器的性能
				fixed3 viewDir = normalize(_WorldSpaceCameraPos.xyz - i.worldPos.xyz);
				//计算半角向量（光线方向 + 视线方向，结果归一化）
				fixed3 halfDir = normalize(worldLight + viewDir);
				//计算Specular（Blinn-Phong计算的是）
				fixed3 specular = _LightColor0.rgb * _Specular.rgb * pow(saturate(dot(halfDir, worldNormal)), _Gloss);
				//纹理采样
				fixed4 tex = tex2D(_MainTex, i.uv);
				//纹理中rgb为正常颜色，a为一个高光的mask图，非高光部分a值为0，高光部分根据a的值控制高光强弱
				fixed3 color = (diffuse  + ambient + specular * tex.a * _SpecularScale) * tex.rgb;
				return fixed4(color, 1.0);
			}
				ENDCG
		}
	}
	//前面的Shader失效的话，使用默认的Diffuse
	FallBack "Diffuse"
}
