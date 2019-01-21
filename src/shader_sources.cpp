#include "pch.h"
#include "shader_sources.h"

std::map<std::string, const char*> ShaderSource::shader_source_map;
ShaderSourceStaticInitializer ShaderSource::static_initializer;

const char* ShaderSource::GetSource(const char* shader_name) {
	// "pointcloud_vs" 같은 이름을 주고 코드를 반환하는 함수인데
	// 이름에 오타를 내거나 존재하지 않는 이름을 입력하면 예외를 반환한다.
	if (shader_source_map.count(shader_name) == 0) {
		std::string message = "shader name[" + std::string(shader_name) + "] does not exists.";
		throw std::runtime_error(message.c_str());
	}

	return shader_source_map[shader_name];
}

// 생성자의 시그니처 부분을 매크로로 만들어 쓰면
// Visual Studio가 코드를 들여쓰기 하지 않는다.
// * 설정에 따라 다를 수 있지만, 거의 먹히는 것 같다.
#define STATIC_INITIALIZE ShaderSourceStaticInitializer::ShaderSourceStaticInitializer()
ShaderSourceStaticInitializer::ShaderSourceStaticInitializer() {
	ShaderSource::shader_source_map["pointcloud_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;

uniform mat4 MVP;
out vec2 vtexcoord;

void main() {		
	gl_Position = MVP*vec4(pos, 1);
	vtexcoord = texcoord;
} 
)";

	ShaderSource::shader_source_map["pointcloud_fs"] = R"(
#version 430

in vec2 vtexcoord;

out vec4 fragcolor;

uniform sampler2D tex;

void main() {
	fragcolor = vec4(texture(tex, vtexcoord).rgb, 1);
}
)";

	ShaderSource::shader_source_map["color_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;


out vec2 vtexcoord;

uniform float aspectRatio;

void main() {
	float x = pos.x;
	float y = pos.y;

	if(aspectRatio > 1.0) 
		x /= aspectRatio;
	else
		y *= aspectRatio;

	gl_Position = vec4(x, y, 0, 1);
	vtexcoord = texcoord;
} 
)";

	ShaderSource::shader_source_map["color_fs"] = R"(
#version 430

in vec2 vtexcoord;

out vec4 fragcolor;

uniform sampler2D tex;

void main() {
	fragcolor = vec4(texture(tex, vtexcoord).rgb, 1);
}
)";

	ShaderSource::shader_source_map["face_boundary_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;

uniform float aspectRatio;
void main() {
	float x = pos.x;
	float y = pos.y;

	if(aspectRatio > 1.0) 
		x /= aspectRatio;
	else
		y *= aspectRatio;

	gl_Position = vec4(x, y, 0, 1);
} 
)";

	ShaderSource::shader_source_map["face_boundary_fs"] = R"(
#version 430
out vec4 fragcolor;

void main() {
	fragcolor = vec4(1, 0, 0, 1);
}
)";

	ShaderSource::shader_source_map["face_pointcloud_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;

uniform mat4 MVP;

void main() {		
	gl_Position = MVP*vec4(pos, 1);
} 
)";

	ShaderSource::shader_source_map["face_pointcloud_fs"] = R"(
#version 430

out vec4 fragcolor;

void main() {
	fragcolor = vec4(1, 0, 0, 1);
	
}
)";

	ShaderSource::shader_source_map["face_mesh2D_vs"] = R"(
#version 430
layout(location = 0) in vec2 pos;


uniform float aspectRatio;

void main() {		
	float x = pos.x;
	float y = pos.y;

	if(aspectRatio > 1.0) 
		x /= aspectRatio;
	else
		y *= aspectRatio;
	gl_Position = vec4(x, y, 0, 1);
	
} 
)";

	ShaderSource::shader_source_map["face_mesh2D_fs"] = R"(
#version 430

out vec4 fragcolor;

void main() {
	fragcolor = vec4(1, 0, 0, 1);
	
}
)";
	ShaderSource::shader_source_map["face_mesh3D_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;

uniform mat4 MVP;

void main() {		
	gl_Position = MVP*vec4(pos, 1);
} 
)";


	ShaderSource::shader_source_map["face_mesh3D_fs"] = R"(
#version 430

out vec4 fragcolor;

void main() {
	fragcolor = vec4(1, 0, 0, 1);
	
}
)";


	ShaderSource::shader_source_map["face_mesh3D_paint_vs"] = R"(
#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex_coord;
out vec2 vtex_coord;

uniform mat4 MVP;

void main() {		
	gl_Position = MVP*vec4(pos, 1);
	vtex_coord = tex_coord;
} 
)";


	ShaderSource::shader_source_map["face_mesh3D_paint_fs"] = R"(
#version 430

in vec2 vtex_coord;
out vec4 fragcolor;

//uniform sampler2D tex;

void main() {
	//fragcolor = vec4(texture(tex, vtex_coord).rgb, 1);
	fragcolor = vec4(1, 0, 0, 1);
}
)";



}
