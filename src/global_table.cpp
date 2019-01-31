#include "pch.h"
#include "global_table.h"
#include "shader_sources.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 보조로 사용될 함수의 정의를 이곳에 작성한다.

void UserData::Init_OpenGL(int width, int height) {

	tex1.data = stbi_load("comedy.png", &tex1.width, &tex1.height, &tex1.n, 4);
	tex1.tex = CreateTexture2D(tex1.width, tex1.height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tex1.data);
	stbi_image_free(tex1.data);

	//tex2.data = stbi_load("image1.png", &tex2.width, &tex2.height, &tex2.n, 4);
	//tex2.tex = CreateTexture2D(tex2.width, tex2.height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tex2.data);
	//stbi_image_free(tex2.data);
	//
	//tex3.data = stbi_load("hi.png", &tex3.width, &tex3.height, &tex3.n, 4);
	//tex3.tex = CreateTexture2D(tex3.width, tex3.height, 0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tex3.data);
	//stbi_image_free(tex3.data);


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	aspectRatio = (float)width / (float)height;
	//////pointcloud
	std::map<GLenum, const char*> pointcloud_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("pointcloud_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("pointcloud_fs") }
	};
	program_pointcloud = CreateProgram(pointcloud_shader_map);

	std::vector<VBO_info> VBO_info_pointcloud = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 0, nullptr, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_pointcloud = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_pointcloud = CreateVertexArray(VBO_info_pointcloud, attrib_info_pointcloud);


	////color
	glm::vec3 colorv[6] = {
	{-1.f, 1.f, 0.f}, {-1.f, -1.f, 0.f}, {1.f, -1.f, 0.f},
	{-1.f, 1.f, 0.f}, {1.f, -1.f, 0.f}, {1.f, 1.f, 0.f}
	};
	glm::vec2 colortc[6] = {
	{0.f, 0.f}, {0.f, 1.f}, {1.f, 1.f},
	{0.f, 0.f}, {1.f, 1.f}, {1.f, 0.f}
	};

	std::map<GLenum, const char*> color_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("color_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("color_fs") }
	};
	program_color = CreateProgram(color_shader_map);

	std::vector<VBO_info> VBO_info_color = {
		{ VBO_POS, 6, colorv, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 6, colortc, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_color = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_color = CreateVertexArray(VBO_info_color, attrib_info_color);

	//2D face boundary
	std::map<GLenum, const char*> face_boundary_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_boundary_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_boundary_fs") }
	};
	program_face_boundary = CreateProgram(face_boundary_shader_map);

	std::vector<VBO_info> VBO_info_face_boundary = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) }
	};
	std::vector<attrib_info> attrib_info_face_boundary = {
		{ 0, GL_FLOAT, 3, 0 }
	};
	VAO_face_boundary = CreateVertexArray(VBO_info_face_boundary, attrib_info_face_boundary);

	//3D face point cloud
	std::map<GLenum, const char*> face_pointcloud_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_pointcloud_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_pointcloud_fs") }
	};
	program_face_pointcloud = CreateProgram(face_pointcloud_shader_map);

	std::vector<VBO_info> VBO_info_face_pointcloud = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
	};
	std::vector<attrib_info> attrib_info_face_pointcloud = {
		{ 0, GL_FLOAT, 3, 0 },
	};
	VAO_face_pointcloud = CreateVertexArray(VBO_info_face_pointcloud, attrib_info_face_pointcloud);

	//2D face mesh
	std::map<GLenum, const char*> face_mesh2D_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh2D_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh2D_fs") }
	};
	program_face_mesh2D = CreateProgram(face_mesh2D_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh2D = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec2) },
	};
	std::vector<attrib_info> attrib_info_face_mesh2D = {
		{ 0, GL_FLOAT, 2, 0 },
	};
	VAO_face_mesh2D = CreateVertexArray(VBO_info_face_mesh2D, attrib_info_face_mesh2D);

	//3D face mesh
	std::map<GLenum, const char*> face_mesh3D_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh3D_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh3D_fs") }
	};
	program_face_mesh3D = CreateProgram(face_mesh3D_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh3D = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) }
	};
	std::vector<attrib_info> attrib_info_face_mesh3D = {
		{ 0, GL_FLOAT, 3, 0 }
	};
	VAO_face_mesh3D = CreateVertexArray(VBO_info_face_mesh3D, attrib_info_face_mesh3D);


	//3D face mesh textured
	std::map<GLenum, const char*> face_mesh3D_paint_shader_map = {
		{ GL_VERTEX_SHADER, ShaderSource::GetSource("face_mesh3D_paint_vs") },
		{ GL_FRAGMENT_SHADER, ShaderSource::GetSource("face_mesh3D_paint_fs") }
	};
	program_face_mesh3D_paint = CreateProgram(face_mesh3D_paint_shader_map);

	std::vector<VBO_info> VBO_info_face_mesh3D_paint = {
		{ VBO_POS, 0, nullptr, 0, sizeof(glm::vec3) },
		{ VBO_TEX, 0, nullptr, 0, sizeof(glm::vec2) }
	};
	std::vector<attrib_info> attrib_info_face_mesh3D_paint = {
		{ 0, GL_FLOAT, 3, 0 },
		{ 1, GL_FLOAT, 2, 0 }
	};
	VAO_face_mesh3D_paint = CreateVertexArray(VBO_info_face_mesh3D_paint, attrib_info_face_mesh3D_paint);



	//////OpenGL state
	glPointSize(3.0f);
	glEnable(GL_CULL_FACE); // 삼각형이 뒷면이 보이는 경우 그리지 않는다.
	glEnable(GL_DEPTH_TEST); // 뒷면에 가려지는 도형은 그리지 않는다.

	// 나중에 pixel unpack 연산 (pixel 데이터를 GPU로 보내는 작업)을 할 때 주석을 풀자.

	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
}

void UserData::Init_ImGui() {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 430");
}


void UserData::Init_RealSense() {
	selection = pipe.start();
	auto depth_stream = selection.get_stream(RS2_STREAM_DEPTH);
	auto color_stream = selection.get_stream(RS2_STREAM_COLOR);
	extrinsics_depth_to_color = depth_stream.get_extrinsics_to(color_stream);
	extrinsics_color_to_depth = color_stream.get_extrinsics_to(depth_stream);

	realsense_intrinsics_color = pipe.get_active_profile().get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>().get_intrinsics();
	realsense_intrinsics_depth = pipe.get_active_profile().get_stream(RS2_STREAM_DEPTH).as<rs2::video_stream_profile>().get_intrinsics();

}

void UserData::Init_Trackball(int width, int height) {
	trackball.curr.eye = glm::vec3{};
	trackball.curr.at = glm::vec3{ 0, 0, 1 };
	trackball.curr.up = glm::vec3{ 0, -1, 0 };
	trackball.curr.width = float(width) / 1000;
	trackball.curr.height = float(height) / 1000;
	trackball.curr.fovy = glm::radians<float>(46.f);
	trackball.curr.dnear = 0.001f;
	trackball.prev = trackball.home = trackball.curr;
	trackball.curr.dfar = 20.0f;
}

void UserData::Init_dlib() {
	detector = dlib::get_frontal_face_detector();
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
}

void UserData::Track_face() {
	dlib::array2d<dlib::rgb_pixel> img;
	
	cv::Mat frame1(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(frame1));
	
	if (get2DFaceFeaturePoints(img, face_features)) {
		isDetect = true;

		//안쪽으로 땡김
		face_features_fixed.clear();
		for (int i = 0; i < face_features.size(); i++) {
			face_features_fixed.push_back(glm::vec2(face_features[i].x - 15.f, face_features[i].y));
		}

		for (int i = 0; i < face_features.size(); i++) {
			if (0 <= i && i <= 5) {
				face_features_fixed[i].x += 15.f;
			}
			else if (6 <= i && i <= 10) {
				face_features_fixed[i].y -= 15.f;
			}
			else if (11 <= i && i <= 16) {
				face_features_fixed[i].x -= 15.f;
			}
		}

		face_inlier.clear();
		transform_2Dto3D(face_features_fixed, face_inlier);
		
		// 코 끝을 포함하는 평면 및 Vector를 구함
		glm::vec3 lefteyes(0.f, 0.f, 0.f);
		glm::vec3 righteyes(0.f, 0.f, 0.f);
		for (int i = 36; i <= 41; i++) {
			lefteyes += face_inlier[i];
		}
		for (int i = 42; i <= 47; i++) {
			righteyes += face_inlier[i];
		}

		lefteyes /= 6.f; righteyes /= 6.f; // 왼쪽 눈의 평균 및 오른쪽 눈의 평균을 구함

		glm::vec3 mouth(0.f, 0.f, 0.f);
		for (int i = 48; i <= 54; i++) {
			mouth += face_inlier[i];
		}
		mouth /= 7.f; // 입의 평균

		glm::vec3 ri_li = righteyes - lefteyes;
		glm::vec3 mth_li = mouth - lefteyes;
		glm::vec3 norm = glm::normalize(glm::cross(mth_li, ri_li)); // 눈에서 입까지의 평면 - 법선벡터
		glm::vec3 horiaxis = glm::normalize(ri_li);
		glm::vec3 vertaxis = glm::normalize(glm::cross(norm, horiaxis)); // 기저 벡터

		glm::vec3 nose = face_inlier[30]; //코 끝
		
		float angle_norm = glm::angle(init_norm, norm);
		float angle_vert = glm::angle(init_vert, vertaxis); 
		float angle_hori = glm::angle(init_hori, horiaxis); 

		Rn = glm::rotate(angle_norm / 2.f, glm::normalize(glm::cross(init_norm, norm)));
		Rv = glm::rotate(angle_vert / 2.f, glm::normalize(glm::cross(init_vert, vertaxis)));
		Rh = glm::rotate(angle_hori / 2.f, glm::normalize(glm::cross(init_hori, horiaxis)));

		TN = glm::translate(nose - init_nose);

		glm::mat4 translate_to_origin = glm::translate(-nose);
		glm::mat4 translate_to_world = glm::translate(nose);

		TM = translate_to_world * Rh * Rv * Rn * TN * translate_to_origin;

	}
	else {
		Console_msg.push_back("fail to detect");
		isFindFace = false;
		isDetect = false;
	}

}

void UserData::Capture_Point() {
	dlib::array2d<dlib::rgb_pixel> img;

	cv::Mat frame2(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(frame2));

	if (get2DFaceFeaturePoints(img, face_features)) {
		isDetect = true;

		//안쪽으로 땡김
		face_features_fixed.clear();
		for (int i = 0; i < face_features.size(); i++) {
			face_features_fixed.push_back(glm::vec2(face_features[i].x - 15.f, face_features[i].y));
		}

		for (int i = 0; i < face_features.size(); i++) {
			if (0 <= i && i <= 5) {
				face_features_fixed[i].x += 15.f;
			}
			else if (6 <= i && i <= 10) {
				face_features_fixed[i].y -= 15.f;
			}
			else if (11 <= i && i <= 16) {
				face_features_fixed[i].x -= 15.f;
			}
		}

		face_inlier.clear();
		transform_2Dto3D(face_features_fixed, face_inlier);
		VertexBufferData(VAO_face_pointcloud, VBO_POS, face_inlier.size(), sizeof(glm::vec3), face_inlier.data(), GL_STREAM_DRAW);

		// 코 끝을 포함하는 평면 및 Vector를 구함
		glm::vec3 lefteyes(0.f, 0.f, 0.f);
		glm::vec3 righteyes(0.f, 0.f, 0.f);
		for (int i = 36; i <= 41; i++) {
			lefteyes += face_inlier[i];
		}
		for (int i = 42; i <= 47; i++) {
			righteyes += face_inlier[i];
		}

		lefteyes /= 6.f; righteyes /= 6.f; // 왼쪽 눈의 평균 및 오른쪽 눈의 평균을 구함

		glm::vec3 mouth(0.f, 0.f, 0.f);
		for (int i = 48; i <= 54; i++) {
			mouth += face_inlier[i];
		}
		mouth /= 7.f; // 입의 평균

		glm::vec3 ri_li = righteyes - lefteyes;
		glm::vec3 mth_li = mouth - lefteyes;
		glm::vec3 norm = glm::normalize(glm::cross(mth_li, ri_li)); // 눈에서 입까지의 평면 - 법선벡터
		glm::vec3 horiaxis = glm::normalize(ri_li);
		glm::vec3 vertaxis = glm::normalize(glm::cross(norm, horiaxis)); // 기저 벡터

		init_norm = norm;
		init_hori = horiaxis;
		init_vert = vertaxis;

		glm::vec3 nose = face_inlier[30]; //코 끝
		
		init_nose = nose; // 법선벡터 저장
		makeFaceTextureCoordinate(face_inlier, face_tex_coord_buf);
	}
	else {
		isDetect = false;
	}

}


#include <stdlib.h>

void UserData::Update_RealSense() {
	frames = pipe.wait_for_frames();

	depth = frames.get_depth_frame();
	color = frames.get_color_frame();

	points = pc.calculate(depth);
	pc.map_to(color);


	//get depth points and texture coordinates
	depth_points.clear();
	depth_texcoords.clear();

	const rs2::vertex *vertices = points.get_vertices();
	const rs2::texture_coordinate *texcoord = points.get_texture_coordinates();

	glm::vec3 tv;
	glm::vec2 tc;

	if (isFindFace) {
		float minx = 1000.f;
		float maxx = -1000.f;
		float miny = 10000.f;
		float maxy = -1000.f;
		
		std::vector<glm::vec4> face_temp;
		face_temp.clear();
		for (int i = 0; i < face_3D_final.size(); i++)
			face_temp.push_back(glm::vec4(face_3D_final[i], 1.f));

		for (int i = 0; i < face_3D_final.size(); i++) {
			if (minx > (TM * face_temp[i]).x)
				minx = (TM * face_temp[i]).x;
			if (maxx < (TM * face_temp[i]).x)
				maxx = (TM * face_temp[i]).x;
			if (miny > (TM * face_temp[i]).y)
				miny = (TM * face_temp[i]).y;
			if (maxy < (TM * face_temp[i]).y)
				maxy = (TM * face_temp[i]).y;
		}
		for (int i = 0; i < int(points.size()); i++) {
			if (vertices[i].z == 0) continue;
			if ((maxx >= vertices[i].x && vertices[i].x >= minx ) && (maxy >= vertices[i].y && vertices[i].y >= miny)) {
				tv.x = vertices[i].x;
				tv.y = vertices[i].y;
				tv.z = vertices[i].z;
				depth_points.emplace_back(tv);

				tc.x = texcoord[i].u;
				tc.y = texcoord[i].v;
				depth_texcoords.push_back(tc);
			}
		}
	}
	else {
		for (int i = 0; i < int(points.size()); i++) {
			if (vertices[i].z == 0) continue;
			tv.x = vertices[i].x;
			tv.y = vertices[i].y;
			tv.z = vertices[i].z;
			depth_points.emplace_back(tv);

			tc.x = texcoord[i].u;
			tc.y = texcoord[i].v;
			depth_texcoords.push_back(tc);
		}
	}


	VertexBufferData(VAO_pointcloud, VBO_POS, depth_points.size(), sizeof(glm::vec3), depth_points.data(), GL_STREAM_DRAW);
	VertexBufferData(VAO_pointcloud, VBO_TEX, depth_texcoords.size(), sizeof(glm::vec2), depth_texcoords.data(), GL_STREAM_DRAW);

	//update realsense color texture
	if (realsense_tex.tex == 0) {
		glGenTextures(1, &realsense_tex.tex);
	}
	glBindTexture(GL_TEXTURE_2D, realsense_tex.tex);

	realsense_tex.width = static_cast<rs2::video_frame>(color).get_width();
	realsense_tex.height = static_cast<rs2::video_frame>(color).get_height();
	//static_cast<rs2::video_frame>(color).get_data();
	rs2_format format = static_cast<rs2::video_frame>(color).get_profile().format();
	
	switch (format)
	{
	case RS2_FORMAT_RGB8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, realsense_tex.width, realsense_tex.height, 0, GL_RGB, GL_UNSIGNED_BYTE, static_cast<rs2::video_frame>(color).get_data());
		break;
	case RS2_FORMAT_RGBA8:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, realsense_tex.width, realsense_tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, static_cast<rs2::video_frame>(color).get_data());
		break;
	default:
		printf("unsupported format: %d", format);
		throw std::runtime_error("The requested format is not suported by this demo!");
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glBindTexture(GL_TEXTURE_2D, 0);	
}

void UserData::Update_ImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	Update_ImGui_MainMenu();
	Update_ImGui_SideBar();
	Update_ImGui_Console();
	
}

void UserData::Update_dlib() {
	dlib::array2d<dlib::rgb_pixel> img;

	cv::Mat frame1(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(frame1));
	
	if (get2DFaceFeaturePoints(img, face_features)) {
		isDetect = true;

		std::vector<glm::vec3> face_features_gl;
		face_features_gl.clear();
		int w = realsense_tex.width;
		int h = realsense_tex.height;
		for (int i = 0; i < face_features.size(); i++) {
			glm::vec3 temp = { 2.f * (face_features[i].x/(float)w - 0.5f), -2.f * (face_features[i].y/(float)h - 0.5f), 0.f };
			face_features_gl.push_back(temp);
		}
		VertexBufferData(VAO_face_boundary, VBO_POS, face_features_gl.size(), sizeof(glm::vec3), face_features_gl.data(), GL_STREAM_DRAW);

		cv::Mat frame(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
		get2DMesh(frame, face_features, face_features_mesh2D);
		//opengl좌표로 변환 (color frame과 같이 그려주는 mesh)
		for (int i = 0; i < face_features_mesh2D.size(); i += 1) {
			face_features_mesh2D[i].x = (face_features_mesh2D[i].x / (float)realsense_tex.width - 0.5) * 2.f;
			face_features_mesh2D[i].y = (face_features_mesh2D[i].y / (float)realsense_tex.height - 0.5) * (-2.f);
		}

		VertexBufferData(VAO_face_mesh2D, VBO_POS, face_features_mesh2D.size(), sizeof(glm::vec2), face_features_mesh2D.data(), GL_STREAM_DRAW);

	}
	else {
		isDetect = false;
	}
}

void UserData::Update_Mesh() {
	// Read in the image.
	cv::Mat frame(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	get2DMesh(frame, face_features_fixed, face_features_mesh2D);
	transform_2Dto3D(face_features_mesh2D, face_features_mesh3D);
	VertexBufferData(VAO_face_mesh3D, VBO_POS, face_features_mesh3D.size(), sizeof(glm::vec3), face_features_mesh3D.data(), GL_STREAM_DRAW);
	VertexBufferData(VAO_face_mesh3D_paint, VBO_POS, face_features_mesh3D.size(), sizeof(glm::vec3), face_features_mesh3D.data(), GL_STREAM_DRAW);
	
	std::vector<glm::vec2> face_texcoord;
	face_texcoord.clear();
	for (int i = 0; i < face_features_mesh2D.size(); i++) {
		for (int j = 0; j < face_features_fixed.size(); j++) {
			if (face_features_mesh2D[i] == face_features_fixed[j]) {
				face_texcoord.push_back(face_tex_coord_buf[j]);
				break;
			}
		}
	}
	VertexBufferData(VAO_face_mesh3D_paint, VBO_TEX, face_texcoord.size(), sizeof(glm::vec2), face_texcoord.data(), GL_STREAM_DRAW);

	//opengl좌표로 변환 (color frame과 같이 그려주는 mesh)
	for (int i = 0; i < face_features_mesh2D.size(); i += 1) {
		face_features_mesh2D[i].x = (face_features_mesh2D[i].x / (float)realsense_tex.width - 0.5) * 2.f;
		face_features_mesh2D[i].y = (face_features_mesh2D[i].y / (float)realsense_tex.height - 0.5) * (-2.f);
	}

	VertexBufferData(VAO_face_mesh2D, VBO_POS, face_features_mesh2D.size(), sizeof(glm::vec2), face_features_mesh2D.data(), GL_STREAM_DRAW);

}


void UserData::Cleanup_OpenGL() {
	DestroyProgram(program_pointcloud);
	DestroyVertexArray(VAO_pointcloud);

	DestroyProgram(program_color);
	DestroyVertexArray(VAO_color);

	DestroyProgram(program_face_boundary);
	DestroyVertexArray(VAO_face_boundary);

	DestroyProgram(program_face_pointcloud);
	DestroyVertexArray(VAO_face_pointcloud);

	DestroyProgram(program_face_mesh2D);
	DestroyVertexArray(VAO_face_mesh2D);

	DestroyProgram(program_face_mesh3D);
	DestroyVertexArray(VAO_face_mesh3D);

	DestroyProgram(program_face_mesh3D_paint);
	DestroyVertexArray(VAO_face_mesh3D_paint);
}


void UserData::Cleanup_RealSense() {
	pipe.stop();
}

void UserData::Cleanup_ImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UserData::filterFace() {
	face_inlier_filtered.clear();
	float sumOfz = 0;
	for (int i = 0; i < face_inlier.size(); i++) {
		sumOfz += face_inlier[i].z;
	}

	float avgz = sumOfz / (float)face_inlier.size();
	
	//표준편차를 이용해서 얼굴이 아닌 points걸러냄
	float variance = 0; //분산
	for (int i = 0; i < face_inlier.size(); i++) {
		variance += pow(face_inlier[i].z - avgz, 2);
	}
	variance /= (float)face_inlier.size();
	float std_deviation = sqrt(variance); //표준편차
	
	float c = 1.f;
	float referenceDepth = avgz + c * std_deviation;
	for (int i = 0; i < face_inlier.size(); i++) {
		if (face_inlier[i].z < referenceDepth) {
			face_inlier[i].z -= 0.02f;
			face_inlier_filtered.emplace_back(face_inlier[i]);
		}
	}
}

bool UserData::filterFaceOutlier(std::vector<glm::vec3>& face, std::vector<glm::vec3>& face_inlier) {
	bool check = true;
	face_inlier.clear();
	float avgFaceLength = 0.1f; //depth가 10cm이상 차이나면 얼굴이 아닌걸로 간주한다 
	float avg = 0;
	for (int i = 0; i < face.size(); i++) {
		avg += face[i].z;
	}
	avg /= (float)face.size();
	float maxz = avg + avgFaceLength;
	float minz = avg - avgFaceLength;
	if (minz <= 0.f)
		minz = 0.01f;
	
	glm::vec3 zeroVec3(0.f, 0.f, 0.f);
	for (int i = 0; i < face.size(); i++) {
		if (maxz >= face[i].z && face[i].z >= minz) {
			face_inlier.push_back(face[i]);
		}
		else {
			face_inlier.push_back(zeroVec3);
			check = false;
		}
	}

	return check;
}

// Draw delaunay triangles
void UserData::draw_delaunay(cv::Mat& img, cv::Subdiv2D& subdiv, cv::Scalar delaunay_color)
{

	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);
	cv::Size size = img.size();
	cv::Rect rect(0, 0, size.width, size.height);

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[1] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[2] = cv::Point(cvRound(t[4]), cvRound(t[5]));

		// Draw rectangles completely inside the image.
		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2]))
		{
			line(img, pt[0], pt[1], delaunay_color, 1, cv::LINE_AA, 0);
			line(img, pt[1], pt[2], delaunay_color, 1, cv::LINE_AA, 0);
			line(img, pt[2], pt[0], delaunay_color, 1, cv::LINE_AA, 0);
		}
	}
}


void UserData::transform_2Dto3D(std::vector<glm::vec2>& point2D, std::vector<glm::vec3>& point3D) {
	point3D.clear();
	for (int i = 0; i < point2D.size(); i++) {
		float tp[3] = { 0.f, 0.f, 0.f };
		const float pix[2] = { point2D[i].x, point2D[i].y };
		rs2_deproject_pixel_to_point(tp, &realsense_intrinsics_color, pix, 2.f);
		//float pixel_distance_in_meters = static_cast<rs2::depth_frame>(depth).get_distance(face_features[i].x * dw / cw, face_features[i].y * dh / ch);

		// Apply extrinsics to the origi
		float target[3];
		rs2_transform_point_to_point(target, &extrinsics_color_to_depth, tp);
		float uv[2] = { 0, 0 };
		rs2_project_point_to_pixel(uv, &realsense_intrinsics_depth, target);

		float pixel_distance_in_meters = static_cast<rs2::depth_frame>(depth).get_distance(uv[0], uv[1]);

		float result[3];
		rs2_deproject_pixel_to_point(result, &realsense_intrinsics_depth, uv, pixel_distance_in_meters);

		glm::vec3 temp = { result[0], result[1], result[2] };
		point3D.emplace_back(temp);
	}
}

bool UserData::get2DFaceFeaturePoints(dlib::array2d<dlib::rgb_pixel>& img, std::vector<glm::vec2>& result) {
	std::vector<dlib::rectangle> dets = detector(img);
	if (dets.size() > 0) {
		//화면에 인식된 얼굴 중에서 가장 큰 얼굴을 찾는다.
		float max = 0.f;
		int fidx = 0;
		for (int i = 0; i < dets.size(); i++) {
			dlib::full_object_detection shape;
			shape = sp(img, dets[0]);

			std::vector<glm::vec2> temp;
			temp.clear();
			for (int i = 0; i < shape.num_parts(); i++) {
				glm::vec2 point = { shape.part(i).x(), shape.part(i).y() };
				temp.push_back(point);
			}
			if (max < abs(temp[0].x - temp[16].x)) {
				fidx = i;
			}
		}

		dlib::full_object_detection shape;
		shape = sp(img, dets[fidx]);
		result.clear();

		for (int i = 0; i < shape.num_parts(); i++) {
			glm::vec2 point = { shape.part(i).x(), shape.part(i).y() };
			result.push_back(point);
		}
		return true;
	}

	return false;
}

void UserData::get2DMesh(cv::Mat& img,std::vector<glm::vec2>& features, std::vector<glm::vec2>& result) {
	// Keep a copy around
	cv::Mat img_orig = img.clone();

	// Rectangle to be used with Subdiv2D
	cv::Size size = img.size();
	cv::Rect rect(0, 0, size.width, size.height);

	// Create an instance of Subdiv2D
	cv::Subdiv2D subdiv(rect);

	// Create a vector of points.
	std::vector<cv::Point2f> points;

	points.clear();

	// Get Points
	for (int i = 0; i < features.size(); i++)
		points.push_back(cv::Point2f(features[i].x, features[i].y));

	// Insert points into subdiv
	for (std::vector<cv::Point2f>::iterator it = points.begin(); it != points.end(); it++)
	{
		subdiv.insert(*it);
	}
	// Draw delaunay triangles
	//draw_delaunay(img, subdiv, delaunay_color);
	std::vector<cv::Vec6f> triangleList;
	subdiv.getTriangleList(triangleList);
	std::vector<cv::Point> pt(3);
	result.clear();

	for (size_t i = 0; i < triangleList.size(); i++)
	{
		cv::Vec6f t = triangleList[i];
		pt[0] = cv::Point(cvRound(t[0]), cvRound(t[1]));
		pt[2] = cv::Point(cvRound(t[2]), cvRound(t[3]));
		pt[1] = cv::Point(cvRound(t[4]), cvRound(t[5]));

		glm::vec2 temp[3];
		for (int j = 0; j < 3; j += 1) {
			temp[j].x = pt[j].x;
			temp[j].y = pt[j].y;
			result.push_back(temp[j]);
		}
	}

}

void UserData::FindFirstFace(){
	dlib::array2d<dlib::rgb_pixel> img;
	cv::Mat frame(cv::Size(realsense_tex.width, realsense_tex.height), CV_8UC3, (void*)color.get_data(), cv::Mat::AUTO_STEP);
	dlib::assign_image(img, dlib::cv_image<dlib::bgr_pixel>(frame));

	std::vector<glm::vec2> face;
	face.clear();

	face_3D_final.clear();

	if (get2DFaceFeaturePoints(img, face)) {
		
		//인식된 가장 화면에 크게 찍히는 얼굴을 찾아서 리턴한다
		//그 얼굴이 정면모습에 가까울때 시작
		//카메라 정면을 보고있지 않으면 다시 얼굴탐색
		float epsilon = 3.f;
		if (abs(glm::distance(face[0], face[36]) - glm::distance(face[16], face[45])) > epsilon) {
			return;
		}

		//얼굴이 카메라와 수평하지 않으면 다시 얼굴탐색
		float epsilon2 = 0.1f;
		if (abs((face[16].y - face[0].y) / (face[16].x - face[0].x)) >= epsilon2) {
			return;
		}
		
		//찾은 얼굴을 안쪽으로 땡긴다
		for (int i = 0; i < face.size(); i++) {
			face[i] = (glm::vec2(face[i].x - 15.f, face[i].y));
		}

		for (int i = 0; i < face.size(); i++) {
			if (0 <= i && i <= 5) {
				face[i].x += 15.f;
			}
			else if (6 <= i && i <= 10) {
				face[i].y -= 15.f;
			}
			else if (11 <= i && i <= 16) {
				face[i].x -= 15.f;
			}
		}

		std::vector<glm::vec2> face_mesh2D;
		std::vector<glm::vec3> face_mesh3D;
		face_mesh2D.clear();
		face_mesh3D.clear();
		get2DMesh(frame, face, face_mesh2D);

		face_idx.clear();
		//mesh로 만든 점이 원래 특징점에서 어디에 해당하는 점인지 탐색해놈
		for (int i = 0; i < face_mesh2D.size(); i++) {
			for (int j = 0; j < face.size(); j++) {
				if(face[j] == face_mesh2D[i]){
					face_idx.push_back(j);
					break;
				}
			}
		}
		//mesh2D에 들어있는 점들을 3D로 deprojection한다
		transform_2Dto3D(face_mesh2D, face_mesh3D);
		
		
		//deprojection한 후 분명히 날아간 점들이 존재한다.
		face_mesh3D_final.clear();
		filterFaceOutlier(face_mesh3D, face_mesh3D_final);

		//face_3Dmesh에서 특징점 68개의 좌표만 따로 뽑아서 저장
		face_3D_final.resize(68);
		for (int i = 0; i < face_mesh3D_final.size(); i++) {
			face_3D_final[face_idx[i]] = face_mesh3D_final[i];
		}

		//얼굴 반쪽을 찾았으면 다머지 반쪽은 대칭해서 채우고 tracking시작
		bool fl = true;
		bool fr = true;
		glm::vec3 zeroVec3(0.f, 0.f, 0.f);
		for (int i = 0; i <= 8; i++) {
			if (face_3D_final[i] == zeroVec3) {
				fl = false;
				break;
			}
		}
		for (int i = 8; i <= 16; i++) {
			if (face_3D_final[i] == zeroVec3) {
				fr = false;
				break;
			}
		}

		//만약 반쪽을 못찾았으면 다시 얼굴탐색
		if (!fl && !fr) {
			return;
		}
		else { //반쪽은 찾았으므로 나머지 반쪽을 대칭해서 넣음
			if (fl) {
				for (int i = 0; i <= 7; i++) {
					face_3D_final[16 - i].x = face_3D_final[30].x + (abs(face_3D_final[30].x - face_3D_final[i].x));
					face_3D_final[16 - i].y = face_3D_final[30].y + (face_3D_final[i].y - face_3D_final[30].y);
					face_3D_final[16 - i].z = face_3D_final[i].z;
				}
			}
			else if (fr) {
				for (int i = 0; i <= 7; i++) {
					face_3D_final[i].x = face_3D_final[30].x - (abs(face_3D_final[30].x - face_3D_final[16 - i].x));
					face_3D_final[i].y = face_3D_final[30].y + (face_3D_final[16 - i].y - face_3D_final[30].y);
					face_3D_final[i].z = face_3D_final[16 - i].z;
				}
			}
		}

		//눈, 코중심, 입 눈썹을 멀쩡하게 찾았는지 검사
		bool nm = true;
		for (int i = 17; i <= 67; i++) {
			if (face_3D_final[i] == zeroVec3) {
				nm = false;
				break;
			}
		}
		if (face_3D_final[30] == zeroVec3)
			nm = false;
		
		//눈, 코중심, 입이 멀쩡하면 눈,입의 점들을 이용해서 좌표축을 세운 후 tracking 시작
		if (nm) {
			Console_msg.push_back("detect face");
			//for (int i = 0; i < face_3D_final.size(); i++)
			//	printf("%dth : %f, %f, %f\n", i, face_3D_final[i].x, face_3D_final[i].y, face_3D_final[i].z);

			glm::vec3 lefteye(0.f, 0.f, 0.f);
			glm::vec3 righteye(0.f, 0.f, 0.f);
			glm::vec3 mouth(0.f, 0.f, 0.f);
		
			for (int i = 36; i <= 41; i++) { //lefteye
				lefteye += face_3D_final[i];
			}
			lefteye /= 6.f;

			for (int i = 42; i <= 47; i++) { //righteye
				righteye += face_3D_final[i];
			}
			righteye /= 6.f;

			for (int i = 48; i <= 54; i++) { // mouth
				mouth += face_3D_final[i];
			}
			mouth /= 7.f;

			glm::vec3 ri_li = righteye - lefteye;
			glm::vec3 mth_li = mouth - lefteye;
			glm::vec3 norm = glm::normalize(glm::cross(mth_li, ri_li)); // 눈에서 입까지의 평면 - 법선벡터
			glm::vec3 horiaxis = glm::normalize(ri_li);
			glm::vec3 vertaxis = glm::normalize(glm::cross(norm, horiaxis)); // 기저 벡터

			init_norm = norm;
			init_hori = horiaxis;
			init_vert = vertaxis;

			glm::vec3 nose = face_3D_final[30]; //코 끝

			init_nose = nose;

			for (int i = 0; i < face_mesh3D_final.size(); i++) {
				face_mesh3D_final[i] = face_3D_final[face_idx[i]];
			}
			std::vector<glm::vec2> texcoord;
			texcoord.clear();
			makeFaceTextureCoordinate(face_3D_final, texcoord);
			std::vector<glm::vec2> face_texcoord;
			face_texcoord.clear();
			for (int i = 0; i < face_mesh3D_final.size(); i++) {
				face_mesh3D_final[i] = face_3D_final[face_idx[i]];
				face_texcoord.push_back(texcoord[face_idx[i]]);
			}

			VertexBufferData(VAO_face_mesh3D, VBO_POS, face_mesh3D_final.size(), sizeof(glm::vec3), face_mesh3D_final.data(), GL_STREAM_DRAW);
			VertexBufferData(VAO_face_mesh3D_paint, VBO_POS, face_mesh3D_final.size(), sizeof(glm::vec3), face_mesh3D_final.data(), GL_STREAM_DRAW);

			VertexBufferData(VAO_face_mesh3D_paint, VBO_TEX, face_texcoord.size(), sizeof(glm::vec2), face_texcoord.data(), GL_STREAM_DRAW);

			isFindFace = true;
		}
		//만약 눈, 코중심, 입의 점들중 날아간 것이 존재하면 FindFirstFace를 다시 수행한다
		else {
			isFindFace = false;
		}
	}
	else {
		isFindFace = false;
	}
}

void UserData::makeFaceTextureCoordinate(std::vector<glm::vec3>& face, std::vector<glm::vec2>& texcoord) {
	std::vector<glm::vec3> face_plane;
	face_plane.clear();
	//코 끝을 지나는 평면상으로 얼굴의 모든 특징점을 올린다
	//평면의 Normal Vector 를 축으로 하여 코 -> 각 특징점의 Vector를 회전
	//printf("Face Inlier\n");
	for (int i = 0; i < face.size(); i++) {
		if (i == 30) {
			face_plane.push_back(face[30]);
			continue;
		}

		glm::vec3 a = face[i] - face[30]; //코 -> 특징점 Vector
		float b_size = abs(glm::dot(init_norm, a)); //음수로 나오는 문제 -> 절대값 취해서 양수로 바꿔줌.
		//printf("b_size: %f\n", b_size);
		glm::vec3 b = b_size * init_norm; //평면의 Normal Vector와 같은 방향이면서, 평면과 특징점 사이의 거리만큼의 크기를 갖는 Vector
		glm::vec3 c = glm::normalize(a + b); //a Vector를 평면상에 정사영시킴
		glm::vec3 d = glm::length(a) * c; // a Vector의 길이를 곱해줌
		glm::vec3 od = face[30] + d; // 카메라에서 평면상의 점 까지의 Vector
		face_plane.push_back(od);

	}
	texcoord.clear();
	float xmax = -10000.f;
	float ymax = -10000.f;
	float xmin = 10000.f;
	float ymin = 10000.f;;

	//코 끝의 좌표는 (1, 1)로 확인됨
	//Depth 정보(?)를 얻어오지 못하는 경우 Nan으로 표시됨.
	for (int i = 0; i < face_plane.size(); i++) {
		glm::vec3 v = face_plane[i] - init_nose;
		glm::vec3 vn = glm::normalize(glm::cross(init_hori, v));
		float theta = glm::acos(glm::dot(v, init_hori) / glm::length(v));
		float xcoord = 0.f;
		float ycoord = 0.f;

		if (i == 30) {
			xcoord = 0.f;
			ycoord = 0.f;
		}
		else if (glm::acos(glm::dot(vn, init_norm)) <= glm::pi<float>() / 2.f) {
			xcoord = glm::length(v) * glm::cos(theta);
			ycoord = -glm::length(v) * glm::sin(theta);
		}
		else {
			xcoord = glm::length(v) * glm::cos(theta);
			ycoord = glm::length(v) * glm::sin(theta);
		}


		if (xmax < xcoord) {
			xmax = xcoord;
		}
		if (ymax < ycoord) {
			ymax = ycoord;
		}
		if (xmin > xcoord) {
			xmin = xcoord;
		}
		if (ymin > ycoord) {
			ymin = ycoord;
		}
		texcoord.push_back(glm::vec2(xcoord, ycoord));
	}

	for (int i = 0; i < texcoord.size(); i++) {
		texcoord[i].x = (texcoord[i].x - xmin) / (xmax + glm::abs(xmin));
		texcoord[i].y = (texcoord[i].y - ymin) / (ymax + glm::abs(ymin));
	}
}

//////GUI
void UserData::Update_ImGui_MainMenu() {
	const ImVec2 padding(width, height/50.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);
	ImGui::BeginMainMenuBar();
	MainMenu_size = ImGui::GetWindowSize();
	ImGui::Text("Welcome to 3D face painting");
	ImGui::EndMainMenuBar();
	ImGui::PopStyleVar();
}

void UserData::Update_ImGui_SideBar() {
	//ImGui::PushStyleVar();
	SideBar_size.x = (float)width / 8.f;
	SideBar_size.y = float(height) - MainMenu_size.y;
	const ImVec2 windowPos(0.f, MainMenu_size.y);
	const ImVec2 windowSize(SideBar_size);
	ImGui::SetWindowPos("SideBar", windowPos);
	ImGui::SetWindowSize("SideBar", windowSize);
	ImGui::Begin("SideBar");
	
	if(ImGui::Button("Capture")){
		Capture_Point();
		if (isDetect) {
			Update_Mesh();
			isTrack = !isTrack;
			Console_msg.push_back("detection success");
		}
		else {
			Console_msg.push_back("fail to detect face");
		}
	}

	if (ImGui::TreeNode("Color")) {
		if(ImGui::Checkbox("Color", &isColor)) {
			if (isColor) {
				Console_msg.push_back("Start color frame");
			}
			else {
				Console_msg.push_back("Stop color frame");
			}
		}
		
		if (ImGui::Checkbox("Feature points", &isColor_FeaturePoints)) {
			if (isColor_FeaturePoints) {
				Console_msg.push_back("Start color frame feature points");
			}
			else {
				Console_msg.push_back("Stop color frame feature points");
			}
		}
		if (ImGui::Checkbox("Mesh", &isColor_Mesh)) {
			if (isColor_Mesh) {
				Console_msg.push_back("Start color frame mesh");
			}
			else {
				Console_msg.push_back("Stop color frame mesh");
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Depth")) {
		if (ImGui::Checkbox("Pointcloud", &isDepth_PointCloud)) {
			if (isDepth_PointCloud) {
 				Console_msg.push_back("Start depth frame pointcloud");
			}
			else {
				Console_msg.push_back("Stop depth frame pointcloud");
			}
		}
		if (ImGui::Checkbox("Feature points", &isDepth_FeaturePoints)) {
			if (isDepth_FeaturePoints) {
				Console_msg.push_back("Start depth frame feature points");
			}
			else {
				Console_msg.push_back("Stop depth frame feature points");
			}
		}
		if (ImGui::Checkbox("Mesh", &isDepth_Mesh)) {
			if (isDepth_Mesh) {
				Console_msg.push_back("Start depth frame mesh");
			}
			else {
				Console_msg.push_back("Stop depth frame mesh");
			}
		}
		if (ImGui::TreeNode("Painting")) {
			if (ImGui::Checkbox("texture1", &isDepth_Paint)) { //클릭되면 true 리턴
				
			}
		}
	}
	ImGui::End();
	//ImGui::PopStyleVar();
}

void UserData::Update_ImGui_Console() {
	//ImGui::PushStyleVar();
	Console_size.x = (float)width - SideBar_size.x;
	Console_size.y = (float)height*0.2f;

	const ImVec2 windowPos(SideBar_size.x, (float)height*0.8f);
	const ImVec2 windowSize(Console_size);
	
	ImGui::SetWindowPos("Console", windowPos);
	ImGui::SetWindowSize("Console", windowSize);
	ImGui::Begin("Console");

	for (int i = 0; i < Console_msg.size(); i++) {
		ImGui::Text(Console_msg[i].data());
	}
	ImGui::End();
	//ImGui::PopStyleVar();
}

