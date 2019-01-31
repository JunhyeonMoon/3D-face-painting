
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
		Init_ImGui();
		Init_RealSense();
		Init_Trackball(width, height);
		Init_dlib();
	}

	void update(float time_elapsed) override {
		//Update_RealSense();
		//Update_ImGui();
		//Update_dlib();
		//if (isDetect && isTrack) {
		//	Track_face();
		//}

		Update_RealSense();
		Update_ImGui();
		if (isStart) {
			if (!isFindFace) {
				FindFirstFace();
			}
			else {
				Track_face();
			}
		}
		trackball.update(time_elapsed);
		VP = trackball.projection_matrix()*trackball.view_matrix();

	}

	void render(float time_elapsed) override {
		//glClearColor(1.f, 1.f, 1.f, 1.f);
		glClearColor(0.f, 0.f, 0.f, 1.f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		bool c = isColor || isColor_FeaturePoints || isColor_Mesh;
		bool d = isDepth_PointCloud || isDepth_FeaturePoints || isDepth_Mesh || isDepth_Paint;

		//if (c && d) {
		//	setLeft();
		//	render2D();
		//	setRight();
		//	render3D();
		//}
		//else if(c) {
		//	setCenter();
		//	render2D();
		//}
		//else {
		//	setCenter();
		//	render3D();
		//}

		setCenter();
		render_pointcloud();
		if (isFindFace) {
			//render_face_mesh3D();
			//if (isDepth_Paint)
			render_face_mesh3D_paint();
		}
		render_ImGui();
	}

	// 이 함수는 상속받은 함수가 아니며 새로 추가된 함수이므로 override를 붙이지 않는다.
	// 또한, 클래스 선언 내에 있기 때문에 render보다 먼저 선언될 필요가 없다.
	// 간단한 함수는 이곳에서 선언하고 내용이 복잡한 함수는 UserData (global_table.h)에서
	// 선언하여 사용하는 것이 좋다.
	void setLeft() {
		//왼쪽반
		glm::vec2 s((float)width - SideBar_size.x, (float)height - MainMenu_size.y - Console_size.y);
		s.x /= 2.f;
		glm::vec2 p(SideBar_size.x, Console_size.y);
		glViewport(p.x, p.y, s.x, s.y);
		aspectRatio = s.x / s.y;
	}
	void setRight() {
		//오른쪽반
		glm::vec2 s((float)width - SideBar_size.x, (float)height - MainMenu_size.y - Console_size.y);
		s.x /= 2.f;
		glm::vec2 p(SideBar_size.x + s.x, Console_size.y);
		glViewport(p.x, p.y, s.x, s.y);
		aspectRatio = s.x / s.y;
	}
	void setCenter() {
		//전체
		glm::vec2 s((float)width - SideBar_size.x, (float)height - MainMenu_size.y - Console_size.y);
		glm::vec2 p(SideBar_size.x, Console_size.y);
		glViewport(p.x, p.y, s.x, s.y);
		aspectRatio = s.x / s.y;
	}
	void render2D() {
		if (isColor) {
			render_color();
		}
		if (isDetect) {
			if (isColor_FeaturePoints) {
				render_face_boundary();
			}
			if (isColor_Mesh) {
				render_face_mesh2D();
			}
		}
	}
	void render3D() {
		if (isDepth_PointCloud) {
			render_pointcloud();
		}
		if (isDetect) {
			if (isDepth_FeaturePoints) {
				render_face_pointcloud();
			}
			if (isDepth_Mesh) {
				render_face_mesh3D();
			}
			if (isDepth_Paint) {
				render_face_mesh3D_paint();
			}
		}
	}


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
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_boundary);
		glBindVertexArray(VAO_face_boundary);

		Uniform1f(program_face_boundary, "aspectRatio", aspectRatio);
		glDrawArrays(GL_POINTS, 0, VAO_face_boundary.count);

		glBindVertexArray(0);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	}

	void render_face_pointcloud() {
		glDisable(GL_DEPTH_TEST);
		glPointSize(3.f);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_pointcloud);
		glBindVertexArray(VAO_face_pointcloud);

		UniformMatrix4fv(program_face_pointcloud, "MVP", 1, GL_FALSE, &VP[0][0]);
		glDrawArrays(GL_POINTS, 0, VAO_face_pointcloud.count);
		
		glBindVertexArray(0);
		glUseProgram(0);
		glPointSize(3.f);
		glEnable(GL_DEPTH_TEST);
	}

	void render_face_mesh2D() {
		//glFrontFace(GL_CW);
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_mesh2D);
		glBindVertexArray(VAO_face_mesh2D);

		//UniformMatrix4fv(program_face_mesh, "MVP", 1, GL_FALSE, &VP[0][0]);
		Uniform1f(program_color, "aspectRatio", aspectRatio);
		glDrawArrays(GL_TRIANGLES, 0, VAO_face_mesh2D.count);

		glBindVertexArray(0);
		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
		//glFrontFace(GL_CCW);
	}

	void render_face_mesh3D() {
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_mesh3D);
		glBindVertexArray(VAO_face_mesh3D);

		UniformMatrix4fv(program_face_mesh3D, "MVP", 1, GL_FALSE, &VP[0][0]);
		UniformMatrix4fv(program_face_mesh3D, "TM", 1, GL_FALSE, &TM[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, VAO_face_mesh3D.count);

		glBindVertexArray(0);
		glUseProgram(0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
	}

	void render_face_mesh3D_paint() {
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(0);
		glUseProgram(0);

		glUseProgram(program_face_mesh3D_paint);
		glBindVertexArray(VAO_face_mesh3D_paint);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1.tex);

		UniformMatrix4fv(program_face_mesh3D_paint, "MVP", 1, GL_FALSE, &VP[0][0]);
		UniformMatrix4fv(program_face_mesh3D_paint, "TM", 1, GL_FALSE, &TM[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, VAO_face_mesh3D_paint.count);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glUseProgram(0);
		glEnable(GL_DEPTH_TEST);
	}


	void render_ImGui() {
		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	void cleanup() override {
		Cleanup_RealSense();
		Cleanup_OpenGL();
		Cleanup_ImGui();
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
				isStart = !isStart;
				break;
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

