sampler2D tex0 : register(s0); // �������� ��� pixel shader

// �������� ������� pixel shader'�
float4 pixel_shader_main(float2 tc:TEXCOORD0) : COLOR                
{
	return tex2D(tex0, tc);
}
