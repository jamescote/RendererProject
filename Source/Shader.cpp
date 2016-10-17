#include "Shader.h"


Shader::Shader()
{
	m_uiVertex		= 0;
	m_uiFragment	= 0;
	m_uiProgram		= 0;
	m_uiTessCntrl	= 0;
	m_uiTessEval	= 0;
	m_bInitialized	= false;
}

// Destructor
Shader::~Shader()
{
	// Delete Program and any created Shader
	glDeleteProgram(m_uiProgram);
	glDeleteShader(m_uiVertex);
	glDeleteShader(m_uiFragment);
	glDeleteShader(m_uiTessCntrl);
	glDeleteShader(m_uiTessEval);
}

bool Shader::initializeShader(string sVSLocation, string sFSLocation)
{
	string vertexSource, fragmentSource;
	bool bError = false;

	// Don't initialize if done already
	if (!m_bInitialized)
	{
		// Load Sources
		vertexSource = LoadSource(sVSLocation, &bError);
		fragmentSource = LoadSource(sFSLocation, &bError);
		if (vertexSource.empty() || fragmentSource.empty()) return false;

		// Compile Each Shader
		m_uiVertex = CompileShader(GL_VERTEX_SHADER, vertexSource, &bError);
		m_uiFragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource, &bError);

		// Link Shader
		m_uiProgram = LinkProgram(m_uiVertex, m_uiFragment, m_uiTessCntrl, m_uiTessEval, &bError);

		// Set that it's been initialized.
		m_bInitialized = !CheckGLErrors() && !bError;
	}

	return m_bInitialized;
}

bool Shader::initializeShader(string sVSLocation, string sFSLocation,
							  string sTCLocation, string sTELocation)
{
	string vertexSource, fragmentSource;
	string tessCntrlSource, tessEvalSource;
	bool bError = false;

	// Don't initialize more than once.
	if (!m_bInitialized)
	{
		// Load from Source
		vertexSource	= LoadSource(sVSLocation, &bError);
		fragmentSource	= LoadSource(sFSLocation, &bError);
		tessCntrlSource = LoadSource(sTCLocation, &bError);
		tessEvalSource	= LoadSource(sTELocation, &bError);
		if (vertexSource.empty() || fragmentSource.empty() ||
			tessCntrlSource.empty() || tessEvalSource.empty() ) return false;

		// Compile Shaders
		m_uiVertex		= CompileShader(GL_VERTEX_SHADER, vertexSource, &bError);
		m_uiFragment	= CompileShader(GL_FRAGMENT_SHADER, fragmentSource, &bError);
		m_uiTessCntrl	= CompileShader(GL_TESS_CONTROL_SHADER, tessCntrlSource, &bError);
		m_uiTessEval	= CompileShader(GL_TESS_EVALUATION_SHADER, tessEvalSource, &bError);

		// Link Shaders
		m_uiProgram = LinkProgram(m_uiVertex, m_uiFragment, m_uiTessCntrl, m_uiTessEval, &bError);

		// Set that it's been initialized
		m_bInitialized = !CheckGLErrors() && !bError;
	}

	return m_bInitialized;
}

// reads a text file with the given name into a string
// From Boilerplate code, shouldn't need to modify this at all.
string Shader::LoadSource(const string &filename, bool* bError)
{
	string source;

	ifstream input(filename);
	if (input) {
		copy(istreambuf_iterator<char>(input),
			istreambuf_iterator<char>(),
			back_inserter(source));
		input.close();
	}
	else {
		cout << "ERROR: Could not load shader source from file "
			<< filename << endl;
		*bError |= true;
	}

	return source;
}

// creates and returns a shader object compiled from the given source
GLuint Shader::CompileShader(GLenum shaderType, const string &source, bool* bError)
{
	// allocate shader object name
	GLuint shaderObject = glCreateShader(shaderType);

	// try compiling the source as a shader of the given type
	const GLchar *source_ptr = source.c_str();
	glShaderSource(shaderObject, 1, &source_ptr, 0);
	glCompileShader(shaderObject);

	// retrieve compile status
	GLint status;
	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
		cout << "ERROR compiling shader:" << endl << endl;
		cout << source << endl;
		cout << info << endl;
		*bError |= true;
	}

	return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint Shader::LinkProgram(GLuint vertexShader, 
						   GLuint fragmentShader, 
						   GLuint controlShader, 
						   GLuint evalShader,
						   bool* bError)
{
	// allocate program object name
	GLuint programObject = glCreateProgram();

	// attach provided shader objects to this program
	if (vertexShader)   glAttachShader(programObject, vertexShader);
	if (fragmentShader) glAttachShader(programObject, fragmentShader);
	if (controlShader)  glAttachShader(programObject, controlShader);
	if (evalShader)	    glAttachShader(programObject, evalShader);

	// try linking the program with given attachments
	glLinkProgram(programObject);

	// retrieve link status
	GLint status;
	glGetProgramiv(programObject, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
		string info(length, ' ');
		glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
		cout << "ERROR linking shader program:" << endl;
		cout << info << endl;
		*bError |= true;
	}

	return programObject;
}
