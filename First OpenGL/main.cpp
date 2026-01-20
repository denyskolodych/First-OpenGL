#include <glad/glad.h>
#include "shader.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);
float mixValue = 0.2;
float widthG = 600;
float heightG = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 6.0f));
float lastX = 300.0f;
float lastY = 300.0f;

glm::vec3 lightPos(3.0f, 0.0f, 2.0f);
static glm::vec3 basePos = lightPos;

int main()
{
	// Creating Matrix model, view, projection
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); // камеру ми рухаємо назад по осі Z на 3 одиниці
	//glm::mat4 projection;
	//projection = glm::perspective(glm::radians(fov), widthG / heightG, 1.0f, 100.0f); // 1 - кут огляду, 2 - співвідношення сторін, 3 - ближня відмітка, 4 - дальня відмітка

	glfwInit();
	GLFWwindow* window = glfwCreateWindow(600, 600, "My first project", NULL, NULL);
	if (window == NULL) {
		std::cout << "Error creating the window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glViewport(0, 0, 600, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);


	float vertices[] = {
		// positions 
		0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f,

		0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f, 0.0f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f, 1.0f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, 1.0f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f,

		0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, 0.0f, 0.0f,  0.0f, -1.0f,

		0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  1.0f


	};
	

	//glm::vec3 cubePositions[] = {
	//glm::vec3(0.0f,  0.0f,  0.0f),
	//glm::vec3(2.0f,  5.0f, -15.0f),
	//glm::vec3(-1.5f, -2.2f, -2.5f),
	//glm::vec3(-3.8f, -2.0f, -12.3f),
	//glm::vec3(2.4f, -0.4f, -3.5f),
	//glm::vec3(-1.7f,  3.0f, -7.5f),
	//glm::vec3(1.3f, -2.0f, -2.5f),
	//glm::vec3(1.5f,  2.0f, -2.5f),
	//glm::vec3(1.5f,  0.2f, -1.5f),
	//glm::vec3(-1.3f,  1.0f, -1.5f)
	//};
	// Масив який покладем в VBO який буде казать які вершини з нашого масиву малювати.
	unsigned int indices[] = {
		0, 1, 2, // Перший трикутник
		1, 3, 2, // Другий

		4, 5, 6,
		5, 7, 6,

		8, 9, 10,
		9, 11, 10,

		12, 13, 14,
		13, 15, 14,

		16, 17, 18,
		16, 19, 18,

		20, 21, 22,
		20, 23, 22


	};
	//unsigned int texture1;
	//unsigned int texture2;
	//glGenTextures(1, &texture1);
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//int width, height, nrChannels;
	//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else {
	//	std::cout << "Failed to load a texture! " << std::endl;
	//}
	//stbi_image_free(data);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//data = NULL;
	//stbi_set_flip_vertically_on_load(true);
	//data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	//if (data) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);

	//}
	//else {
	//	std::cout << "Failed to load a texture! " << std::endl;
	//}
	//stbi_image_free(data);
	unsigned int VBO; // Vertex Buffer Object - буфер у памяті відеокарти, після створення буфера змінна буде зберігати ID того буфера
	unsigned int VAO;
	unsigned int EBO; // Element Buffer Object - вирішує проблему дублювання вершин щоб не писати одну ту саму вершину два рази
	unsigned int lightVAO;
	// Генерація всіх буферів
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &VBO); // Функція яка створює один буфер в памяті відеокарти та призначає ID того буфера змінній VBO

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Тепер GL_ARRAY_BUFFER є VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Надсилає дані в памяті відеокарти і ми кажемо що вони не будуть змінюватися але будувать часто використовуватися
	// GL_STREAM_DRAW - якщо дані не будуть змінюватися та будуть використовуватися кілька разів
	// GL_DYNAMIC_DRAW - якщо дані будуть змінюватися та будуть часто використовуватися
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Кажемо як інтерпретувати наші дані в VBO. Перший аргумент location де він буде розташовуватися. Оскільки в шейдері я беру дані з location 0  то наші вершини повинні знаходитися на location 0
	// Другий аргумент це кількість компонентів на атрибут 3(x,y,z). 3 - тій це тип даних кожного компонента. 4 чи нормалізувати компонети(-1, 1). 5 - скільки байтів займає один атрибут. 6 - з якого індексу починаються атрибути
	glEnableVertexAttribArray(0); // дозволяє брати дані з location 0
	glEnableVertexAttribArray(1);
	/*glEnableVertexAttribArray(2);*/
    glBindVertexArray(0);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);








	Shader shader("vShader.vs", "fShader.fs");
	Shader shaderLight("vShaderLight.vs", "fShaderLight.fs");
	glEnable(GL_DEPTH_TEST);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//// Кажемо як інтерпретувати наші дані в VBO. Перший аргумент location де він буде розташовуватися. Оскільки в шейдері я беру дані з location 0  то наші вершини повинні знаходитися на location 0
	//// Другий аргумент це кількість компонентів на атрибут 3(x,y,z). 3 - тій це тип даних кожного компонента. 4 чи нормалізувати компонети(-1, 1). 5 - скільки байтів займає один атрибут. 6 - з якого індексу починаються атрибути
	//glEnableVertexAttribArray(0); // дозволяє брати дані з location 0
	//glEnableVertexAttribArray(1); // дозволяє брати дані з location 0

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // щоб малювати в режимі каркасу тобто лініями і не заповнювати серединуs

	/*shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);*/
	
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0f, 0.0f, 1.0f);
		/*float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2 + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		if (vertexColorLocation == -1) {
			printf("Error cannot find uniform vec4 ourColor\n");
			return -1;
		}
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);*/
		shader.use();
		float PI2 = (float)glm::radians(360.0f);
		float angle = fmodf((float)glfwGetTime(), PI2);
		glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
		lightPos = Rotate * glm::vec4(basePos, 1.0f);
		shader.setVec3("lightPos", glm::vec4(lightPos, 1.0f));
		shader.setVec3("viewPos", camera.Position);
		shader.setMatrix4("model", model);
		shader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMatrix4("view", view);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), widthG / heightG, 1.0f, 100.0f); // 1 - кут огляду, 2 - співвідношення сторін, 3 - ближня відмітка, 4 - дальня відмітка
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMatrix4("model", model);
		shader.setMatrix4("projection", projection);
		shader.setFloat("mixValue", mixValue);

		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/

		glBindVertexArray(VAO);
		
	    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // 1 - що малювати, 2 - кількість вершин, 3 - тип даних EBO, 4 - з якого індексу в Відеопамяті почати

		shaderLight.use();
		shaderLight.setMatrix4("view", view);
		shaderLight.setMatrix4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // робимо кубик меншим
		shaderLight.setMatrix4("model", model);
		glBindVertexArray(0);
		glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36); // 1 - що малювати, 2 - початковий індекс в Vertex Array, 3 - кількість вершин
		//glBindVertexArray(0); // скидаємо нащ VAO
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	/*widthG = width;
	heightG = height;*/
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (mixValue < 1) {
			mixValue += 0.01;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (mixValue > 0) {
			mixValue -= 0.01;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}
		float xoffset = xpos - lastX;
		float yoffset = -ypos + lastY;
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) { // коли крутим колесо мишки yoffset буде додатнім(1) чи відємним(-1) в залежності від напрямку прокрутки
	camera.ProcessMouseScroll(yoffset);
}