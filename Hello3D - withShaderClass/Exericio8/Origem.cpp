/* Hello Triangle - código adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gráfico - Jogos Digitais - Unisinos
 * Versão inicial: 7/4/2017
 * Última atualização em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 1000, HEIGHT = 1000;


bool rotateX=false, rotateY=false, rotateZ=false;
bool translateX = false, translateY = false, translateZ = false;

float scaleFactor = 1.0f;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola 3D - Artur Palagi!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);


	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		float angle = (GLfloat)glfwGetTime();

		// Rotação

		model = glm::mat4(1); 
		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			
		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		// Translação

		if (translateX)
		{
			model = glm::translate(model, glm::vec3(sin(angle), 0.0f, 0.0f));
		}
		else if (translateY)
		{
			model = glm::translate(model, glm::vec3(0.0f, sin(angle), 0.0f));
		}
		else if (translateZ)
		{
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, sin(angle)));
		}

		// Escala
		glm::mat4 scaleMatrix = glm::mat4(1.0f);
		scaleMatrix[0][0] = scaleFactor;
		scaleMatrix[1][1] = scaleFactor;
		scaleMatrix[2][2] = scaleFactor;
		model = model * scaleMatrix;

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36 * 2);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		
		glDrawArrays(GL_POINTS, 0, 36 * 2);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
		scaleFactor += 0.1;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		translateX = true;
		translateY = false;
		translateZ = false;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		translateX = false;
		translateY = true;
		translateZ = false;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		translateX = false;
		translateY = false;
		translateZ = true;
	}

	if (key == GLFW_KEY_KP_ADD && action == GLFW_PRESS)
	{
		scaleFactor += 0.1;
	}

	if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS)
	{
		scaleFactor -= 0.1;
	}
}

// Esta função está bastante harcoded - objetivo é criar os buffers que armazenam a 
// geometria de um triângulo
// Apenas atributo coordenada nos vértices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A função retorna o identificador do VAO
int setupGeometry()
{
	GLfloat vertices[] = {
		// Cubo 1 - Face Frontal
		-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 0 - Vermelho
		0.5, -0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 1 - Vermelho
		0.5, 0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 2 - Vermelho

		0.5, 0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 3 - Vermelho
		-0.5, 0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 4 - Vermelho
		-0.5, -0.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 5 - Vermelho

		// Cubo 1 - Face Traseira
		-0.5, -0.5, -0.5, 0.0, 1.0, 0.0,	// Vértice 6 - Verde
		-0.5, 0.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 7 - Verde
		0.5, 0.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 8 - Verde

		0.5, 0.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 9 - Verde
		0.5, -0.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 10 - Verde
		-0.5, -0.5, -0.5, 0.0, 1.0, 0.0,	// Vértice 11 - Verde

		// Cubo 1 - Face Esquerda
		-0.5, 0.5, -0.5, 0.0, 0.0, 1.0,		// Vértice 12 - Azul
		-0.5, 0.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 13 - Azul
		-0.5, -0.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 14 - Azul

		-0.5, -0.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 15 - Azul
		-0.5, -0.5, -0.5, 0.0, 0.0, 1.0,	// Vértice 16 - Azul
		-0.5, 0.5, -0.5, 0.0, 0.0, 1.0,		// Vértice 17 - Azul

		// Cubo 1 - Face Direita
		0.5, 0.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 18 - Amarelo
		0.5, 0.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 19 - Amarelo
		0.5, -0.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 20 - Amarelo

		0.5, -0.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 21 - Amarelo
		0.5, -0.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 22 - Amarelo
		0.5, 0.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 23 - Amarelo

		// Cubo 1 - Face Superior
		-0.5, 0.5, -0.5, 1.0, 0.0, 1.0,		// Vértice 24 - Magenta
		-0.5, 0.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 25 - Magenta
		0.5, 0.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 26 - Magenta

		0.5, 0.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 27 - Magenta
		0.5, 0.5, -0.5, 1.0, 0.0, 1.0,		// Vértice 28 - Magenta
		-0.5, 0.5, -0.5, 1.0, 0.0, 1.0,		// Vértice 29 - Magenta

		// Cubo 1 - Face Inferior
		-0.5, -0.5, -0.5, 0.0, 1.0, 1.0,	// Vértice 30 - Ciano
		0.5, -0.5, -0.5, 0.0, 1.0, 1.0,		// Vértice 31 - Ciano
		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 32 - Ciano

		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 33 - Ciano
		-0.5, -0.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 34 - Ciano
		-0.5, -0.5, -0.5, 0.0, 1.0, 1.0,	// Vértice 35 - Ciano

		// Cubo 2 - Face Frontal
		-0.5 + 1.5, -0.5 + 1.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 0 - Vermelho
		0.5 + 1.5, -0.5 + 1.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 1 - Vermelho
		0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 2 - Vermelho

		0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 3 - Vermelho
		-0.5 + 1.5, 0.5 + 1.5, 0.5 , 1.0, 0.0, 0.0,		// Vértice 4 - Vermelho
		-0.5 + 1.5, -0.5 + 1.5, 0.5, 1.0, 0.0, 0.0,		// Vértice 5 - Vermelho

		// Cubo 2 - Face Traseira
		-0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,	// Vértice 6 - Verde
		-0.5 + 1.5, 0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 7 - Verde
		0.5 + 1.5, 0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 8 - Verde

		0.5 + 1.5, 0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 9 - Verde
		0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,		// Vértice 10 - Verde
		-0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 0.0,	// Vértice 11 - Verde

		// Cubo 2 - Face Esquerda
		-0.5 + 1.5, 0.5 + 1.5, -0.5, 0.0, 0.0, 1.0,		// Vértice 12 - Azul
		-0.5 + 1.5, 0.5 + 1.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 13 - Azul
		-0.5 + 1.5, -0.5 + 1.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 14 - Azul

		-0.5 + 1.5, -0.5 + 1.5, 0.5, 0.0, 0.0, 1.0,		// Vértice 15 - Azul
		-0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 0.0, 1.0,	// Vértice 16 - Azul
		-0.5 + 1.5, 0.5 + 1.5, -0.5, 0.0, 0.0, 1.0,		// Vértice 17 - Azul

		// Cubo 2 - Face Direita
		0.5 + 1.5, 0.5 + 1.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 18 - Amarelo
		0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 19 - Amarelo
		0.5 + 1.5, -0.5 + 1.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 20 - Amarelo

		0.5 + 1.5, -0.5 + 1.5, 0.5, 1.0, 1.0, 0.0,		// Vértice 21 - Amarelo
		0.5 + 1.5, -0.5 + 1.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 22 - Amarelo
		0.5 + 1.5, 0.5 + 1.5, -0.5, 1.0, 1.0, 0.0,		// Vértice 23 - Amarelo

		// Cubo 2 - Face Superior
		-0.5 + 1.5, 0.5 + 1.5, -0.5, 1.0, 0.0, 1.0,		// Vértice 24 - Magenta
		-0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 25 - Magenta
		0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 26 - Magenta

		0.5 + 1.5, 0.5 + 1.5, 0.5, 1.0, 0.0, 1.0,		// Vértice 27 - Magenta
		0.5 + 1.5, 0.5 + 1.5, -0.5 , 1.0, 0.0, 1.0,		// Vértice 28 - Magenta
		-0.5 + 1.5, 0.5 + 1.5, -0.5, 1.0, 0.0, 1.0,		// Vértice 29 - Magenta

		// Cubo 2 - Face Inferior
		-0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 1.0,	// Vértice 30 - Ciano
		0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 1.0,		// Vértice 31 - Ciano
		0.5 + 1.5, -0.5 + 1.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 32 - Ciano

		0.5 + 1.5, -0.5 + 1.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 33 - Ciano
		-0.5 + 1.5, -0.5 + 1.5, 0.5, 0.0, 1.0, 1.0,		// Vértice 34 - Ciano
		-0.5 + 1.5, -0.5 + 1.5, -0.5, 0.0, 1.0, 1.0		// Vértice 35 - Ciano
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}