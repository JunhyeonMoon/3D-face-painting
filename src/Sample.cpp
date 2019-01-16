﻿
#include "pch.h"
#include "global_table.h"

struct App : public UserData {
	App() {}
	// override 키워드는 이 함수가 부모 클래스에서 선언한 virtual 함수를 재정의하겠다는 것을 의미한다.
	// AppBase 클래스에서 선언하지 않는 함수를 override하려 하거나,
	// 상속받은 가상함수의 이름이나 타입이 달라질 경우 override 키워드에서 컴파일 에러가 발생한다.
	void userInit(int width, int height) override {
		this->width = width;
		this->height = height;

		Init_OpenGL(width, height);
		Init_RealSense();
		Init_Trackball(width, height);
		Init_OpenCV();
	}

	void update(float time_elapsed) override {
		Update_RealSense();
		Update_OpenCV();

		if (isDetect) { //얼굴을 찾았으면
			float tw = (float)face.width / (float)realsense_tex.width;
			float th = (float)face.height / (float)realsense_tex.height;

			//texture좌표 4개점
			glm::vec2 lu = { (float)face.x / (float)realsense_tex.width, (float)face.y / (float)realsense_tex.height };
			glm::vec2 ll = { lu.x, lu.y + th };
			glm::vec2 ru = { lu.x + tw, lu.y };
			glm::vec2 rl = { lu.x + tw, lu.y + th };

			//위의 texture좌표를 OpenGL좌표로 변환
			glm::vec3 olu = { 2 * (lu.x - 0.5), -2 * (lu.y - 0.5), 0.f };
			glm::vec3 oll = { 2 * (ll.x - 0.5), -2 * (ll.y - 0.5), 0.f };
			glm::vec3 oru = { 2 * (ru.x - 0.5), -2 * (ru.y - 0.5), 0.f };
			glm::vec3 orl = { 2 * (rl.x - 0.5), -2 * (rl.y - 0.5), 0.f };

			glm::vec3 facetv[8] = {
				olu, oll, 
				oll, orl,
				orl, oru,
				oru, olu
			};

			VertexBufferData(VAO_face_boundary, VBO_POS, std::size(facetv), sizeof(glm::vec3), facetv, GL_STREAM_DRAW);
			

			//OpenCV에서 얼굴을 사각형으로 찾아줬다
			//찾아준 사각형 안의 2D좌표들을 3D좌표로 deprojection한다
			face_inlier.clear();
			int dw = static_cast<rs2::depth_frame>(depth).get_width();
			int dh = static_cast<rs2::depth_frame>(depth).get_height();
			int cw = static_cast<rs2::video_frame>(color).get_width();
			int ch = static_cast<rs2::video_frame>(color).get_height();

			int featureUnit = 1;
			for (int y_coordinate = face.y; y_coordinate < face.y + face.height; y_coordinate += featureUnit) {
				for (int x_coordinate = face.x; x_coordinate < face.x + face.width; x_coordinate += featureUnit) {
					float pixel_distance_in_meters = static_cast<rs2::depth_frame>(depth).get_distance(x_coordinate * dw/cw, y_coordinate * dh/ch);
					float tp[3] = { 0.f, 0.f, 0.f };
					const float pix[2] = { x_coordinate, y_coordinate };
					rs2_deproject_pixel_to_point(tp, &realsense_intrinsics, pix, pixel_distance_in_meters);
					glm::vec3 temp = { tp[0], tp[1], tp[2] };
					face_inlier.emplace_back(temp);
				}
			}

			filterFace();
			//VertexBufferData(VAO_test, VBO_POS, face_inlier.size(), sizeof(glm::vec3), face_inlier.data(), GL_STREAM_DRAW);
			VertexBufferData(VAO_face_pointcloud, VBO_POS, face_inlier_filtered.size(), sizeof(glm::vec3), face_inlier_filtered.data(), GL_STREAM_DRAW);
		}

		trackball.update(time_elapsed);
		VP = trackball.projection_matrix()*trackball.view_matrix();

	}

	void render(float time_elapsed) override {
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (showFace) {
			render_face_pointcloud();
		}
		else {
			if (!isColor) {
				if (isDetect) {
					render_face_pointcloud();
				}
				render_pointcloud();
			}
			else {
				if (isDetect) { //찾은 얼굴이 있으면
					render_face_boundary();
				}
				render_color();
			}
		}
	}

	// 이 함수는 상속받은 함수가 아니며 새로 추가된 함수이므로 override를 붙이지 않는다.
	// 또한, 클래스 선언 내에 있기 때문에 render보다 먼저 선언될 필요가 없다.
	// 간단한 함수는 이곳에서 선언하고 내용이 복잡한 함수는 UserData (global_table.h)에서
	// 선언하여 사용하는 것이 좋다.
	void render_pointcloud() {
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_pointcloud);
		glBindVertexArray(VAO_pointcloud);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, realsense_tex.tex);

		UniformMatrix4fv(program_pointcloud, "MVP", 1, GL_FALSE, &VP[0][0]);
		glDrawArrays(GL_POINTS, 0, VAO_pointcloud.count);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void render_color() {
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_color);
		glBindVertexArray(VAO_color);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, realsense_tex.tex);
		
		Uniform1f(program_color, "aspectRatio", aspectRatio);
		glDrawArrays(GL_TRIANGLES, 0, VAO_color.count);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void render_face_boundary() {
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_boundary);
		glBindVertexArray(VAO_face_boundary);

		Uniform1f(program_face_boundary, "aspectRatio", aspectRatio);
		glDrawArrays(GL_LINES, 0, VAO_face_boundary.count);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void render_face_pointcloud() {
		glPointSize(1.f);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_pointcloud);
		glBindVertexArray(VAO_face_pointcloud);

		UniformMatrix4fv(program_face_pointcloud, "MVP", 1, GL_FALSE, &VP[0][0]);
		glDrawArrays(GL_POINTS, 0, VAO_face_pointcloud.count);

		glBindVertexArray(0);
		glUseProgram(0);
		glPointSize(3.f);
	}

	void cleanup() override {
		Cleanup_RealSense();
		Cleanup_OpenGL();
	}

	void onWindowSize(GLFWwindow* window, int width, int height) override {
		glViewport(0, 0, width, height);
		this->width = width;
		this->height = height;
		aspectRatio = (float)width / (float)height;
	}

	void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) override {

		if (action == GLFW_PRESS) {
			switch (key) {
			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;

			case GLFW_KEY_0:
			case GLFW_KEY_1:
			case GLFW_KEY_2:
			case GLFW_KEY_3:
			case GLFW_KEY_4:
			case GLFW_KEY_5:
			case GLFW_KEY_6:
			case GLFW_KEY_7:
			case GLFW_KEY_8:
			case GLFW_KEY_9:
				printf("%d key is pressed.\n", key - GLFW_KEY_0);
				break;

			case GLFW_KEY_SPACE:
				showFace = !showFace;
				printf("space key is pressed.\n");
				break;

			case GLFW_KEY_A:
			case GLFW_KEY_B:
			case GLFW_KEY_C: isColor = !isColor; break;
			case GLFW_KEY_D:
				//case GLFW_KEY_E:
			case GLFW_KEY_F:
			case GLFW_KEY_G:
			case GLFW_KEY_H:
			case GLFW_KEY_I:
			case GLFW_KEY_J:
			case GLFW_KEY_K:
			case GLFW_KEY_L:
			case GLFW_KEY_M:
			case GLFW_KEY_N:
			case GLFW_KEY_O:
			case GLFW_KEY_P:
			case GLFW_KEY_S:
			case GLFW_KEY_T:
			case GLFW_KEY_U:
			case GLFW_KEY_V:
				//case GLFW_KEY_W:
			case GLFW_KEY_X:
			case GLFW_KEY_Y:
			case GLFW_KEY_Z:
				printf("%c key is pressed.\n", static_cast<char>(key - GLFW_KEY_A + 'A'));
				break;

			case GLFW_KEY_Q: behavior = camera::Trackball::Behavior::ROTATING; break;
			case GLFW_KEY_W: behavior = camera::Trackball::Behavior::PANNING; break;
			case GLFW_KEY_E: behavior = camera::Trackball::Behavior::ZOOMING; break;
			case GLFW_KEY_R: trackball.reset(); break;
			case GLFW_KEY_UP:
			case GLFW_KEY_RIGHT:
			case GLFW_KEY_LEFT:
			case GLFW_KEY_F1:
			case GLFW_KEY_F2:
				break;
			}
		}
	}

	void onCursorPos(GLFWwindow* window, double x, double y) override {
		trackball.motion(float(x) / width, float(y) / height);
	}

	void onMouseButton(GLFWwindow* window, int button, int action, int mods) override {
		double dx, dy; glfwGetCursorPos(window, &dx, &dy);
		float x = float(dx / width);
		float y = float(dy / height);
		float fx, fy;
		//마우스 커서 클릭위치 좌표를 실제 윈도우 좌표로 조정
		fx = 2 * (x - 0.5); fy = -2 * (y - 0.5);
		switch (button) {
		case GLFW_MOUSE_BUTTON_LEFT:
			if (action == GLFW_PRESS) {
				trackball.mouse(x, y, behavior);
			}
			else {
				if (action == GLFW_RELEASE) {
					trackball.mouse(x, y, camera::Trackball::Behavior::NOTHING);
				}
			}
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			if (action == GLFW_PRESS) {
			}
			break;
		}
	}

	void onScroll(GLFWwindow* window, double xoffset, double yoffset) override {

	}
};

int main(int argc, const char** argv)
{
	App app;
	
	try {
		app.Init(1024, 768, "Sample program");
		app.Run();
	}
	catch (std::runtime_error& e) {
		fprintf(stderr, "%s\n", e.what());
	}
	catch (std::exception& e) {
		fprintf(stderr, "%s\n", e.what());
	}

	return 0;
}

