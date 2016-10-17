#include "CmdHandler.h"

/**********\
* DEFINES *
\**********/
#define INPUT_SIZE 128

/*************\
 * CONSTANTS *
\*************/
const float LIGHT_MOVE_FACTOR = 0.05f;

/* Initialize Statics */
const char CmdHandler::cCommands[NUM_CMDS][CMD_SIZE] = { "-help",
														 "b-val",
														 "y-val",
														 "alpha",
														 "beta",
														 "rc-val",
														 "gc-val",
														 "bc-val",
														 "color" };

CmdHandler* CmdHandler::m_pInstance = NULL;

CmdHandler::CmdHandler(GLFWwindow *rWindow) : InputHandler()
{
	// Initializing Base Class
	m_pGPXMngr = GraphicsManager::getInstance(rWindow);
	m_pEnvMngr = EnvironmentManager::getInstance();
}

// Get a copy of CmdHandler that doesn't have any initialized
//   Input associated with it.
CmdHandler* CmdHandler::getInstance(GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new CmdHandler(rWindow);

	return m_pInstance;
}

// Get a copy of CmdHandler with an associated Input.
CmdHandler* CmdHandler::getInstance(const char* c_Input, int iInputSize, GLFWwindow *rWindow)
{
	if (NULL == m_pInstance)
		m_pInstance = new CmdHandler(c_Input, iInputSize, rWindow);
	else
		m_pInstance->load_Input(c_Input, iInputSize);

	return m_pInstance;
}

// Destructor
CmdHandler::~CmdHandler()
{
	delete m_pInstance;
	m_pGPXMngr = NULL;
	m_pEnvMngr = NULL;
}

// Process initial input, acts as though key word is first word of input.
// First word can be ignored and function will grab the first word.
void CmdHandler::process_Input( )
{
	char cInput[INPUT_SIZE] = {};
	char c_FirstWord[CMD_SIZE] = {};
	bool bEvaluating = true;

	while (bEvaluating)
	{
		cout << "Enter Commands: ";
		cin.getline(cInput, INPUT_SIZE);

		load_Input(cInput, INPUT_SIZE);
		get_Next_Word(c_FirstWord, CMD_SIZE);

		// Parse initial command
		if (!strcmp(c_FirstWord, cCommands[SET_B]))
			bEvaluating = exec_SetB();
		else if (!strcmp(c_FirstWord, cCommands[SET_Y]))
			bEvaluating = exec_SetY();
		else if (!strcmp(c_FirstWord, cCommands[SET_ALPHA]))
			bEvaluating = exec_SetAlpha();
		else if (!strcmp(c_FirstWord, cCommands[SET_BETA]))
			bEvaluating = exec_SetBeta();
		else if (!strcmp(c_FirstWord, cCommands[SET_RC]))
			bEvaluating = exec_SetRGBVal(GraphicsManager::RED);
		else if (!strcmp(c_FirstWord, cCommands[SET_GC]))
			bEvaluating = exec_SetRGBVal(GraphicsManager::GREEN);
		else if (!strcmp(c_FirstWord, cCommands[SET_BC]))
			bEvaluating = exec_SetRGBVal(GraphicsManager::BLUE);
		else if (!strcmp(c_FirstWord, cCommands[HELP]))
			outputHelpList();
		else if (!strcmp(c_FirstWord, cCommands[SET_COLOR]))
			bEvaluating = exec_SetColor();
		else
			cout << "Unknown Command: \"" << c_FirstWord << ".\"" << endl;
	}
}

// Outputs the Help Dialog for the command reader.
void CmdHandler::outputHelpList()
{
	cout << "\nVariable Names:\n"
		 << "\t- \"b-val\"\n"
		 << "\t\t-- sets the b-value of the gooch shader\n"
		 << "\t\t-- takes in a floating-point vlaue between 0.0 and 1.0\n"
		 << "\t- \"y-val\"\n"
		 << "\t\t-- sets the y-value of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"alpha\"\n"
		 << "\t\t-- sets the alpha value of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"beta\"\n"
		 << "\t\t-- sets the beta value of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"rc-val\"\n"
		 << "\t\t-- sets the red color of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"gc-val\"\n"
		 << "\t\t-- sets the green color of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"bc-val\"\n"
		 << "\t\t-- sets the blue color of the gooch shader\n"
		 << "\t\t-- takes in a floating-point value between 0.0 and 1.0\n"
		 << "\t- \"color\"\n"
		 << "\t\t-- sets the rgb value of the color in the gooch shader at once\n"
		 << "\t\t-- takes in 3 floating-point values between 0.0 and 1.0, seperated by spaces\n\n";
}

void CmdHandler::handleKeyBoardInput(unsigned char cKey, int iAction, int iMods)
{
	vec3 pMoveVec(0.f, 0.f, 0.f);
	switch (cKey)
	{
	case (GLFW_KEY_W):			// Move Light Forward (along Z)
		pMoveVec.z += LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_S):			// Move Light Backward (along Z)
		pMoveVec.z -= LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_A):			// Move Light Left (along X)
		pMoveVec.x -= LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_D):			// Move Light Right (along X)
		pMoveVec.x += LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_SPACE):		// Move Light Up (along Y)
		pMoveVec.y += LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_X):			// Move Light Down (along Y)
		pMoveVec.y -= LIGHT_MOVE_FACTOR;
		m_pEnvMngr->moveLight(pMoveVec);
		break;
	case(GLFW_KEY_ENTER):		// Input Handling (TODO)
		break;
	}
}

/*****************************************************************************\
 * Input Executions     													 *
\*****************************************************************************/

// Executes the b-val command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variable
bool CmdHandler::exec_SetB()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	char* p_End;
	float fTempVal;
	bool bReturnVal = false;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		fTempVal = strtof(c_Num, &p_End);
		if (checkRange(fTempVal, 0.0f, 1.0f))
			m_pGPXMngr->setBVal(fTempVal);
	}
	else
		cout << "Error reading in value: \"" << c_Num << "\".\n";

	return bReturnVal;
}

// Executes the y-val command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variable
bool CmdHandler::exec_SetY()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	char* p_End;
	float fTempVal;
	bool bReturnVal = false;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		fTempVal = strtof(c_Num, &p_End);
		if (checkRange(fTempVal, 0.0f, 1.0f))
			m_pGPXMngr->setYVal(fTempVal);
	}
	else
		cout << "Error reading in value: \"" << c_Num << "\".\n";

	return bReturnVal;
}

// Executes the alpha command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variable
bool CmdHandler::exec_SetAlpha()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	char* p_End;
	float fTempVal;
	bool bReturnVal = false;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		fTempVal = strtof(c_Num, &p_End);
		if (checkRange(fTempVal, 0.0f, 1.0f))
			m_pGPXMngr->setAlpha(fTempVal);
	}
	else
		cout << "Error reading in value: \"" << c_Num << "\".\n";

	return bReturnVal;
}

// Executes the beta command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variable
bool CmdHandler::exec_SetBeta()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	char* p_End;
	float fTempVal;
	bool bReturnVal = false;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		fTempVal = strtof(c_Num, &p_End);
		if (checkRange(fTempVal, 0.0f, 1.0f))
			m_pGPXMngr->setBeta(fTempVal);
	}
	else
		cout << "Error reading in value: \"" << c_Num << "\".\n";

	return bReturnVal;
}

// Executes the rc-val command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variable
bool CmdHandler::exec_SetRGBVal(GraphicsManager::eRGB eRGBType )
{
	char c_Num[MAX_INPUT_SIZE] = {};
	char* p_End;
	float fTempVal;
	bool bReturnVal = false;
	int iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);

	if (ERR_CODE != iErr)
	{
		fTempVal = strtof(c_Num, &p_End);
		if (checkRange(fTempVal, 0.0f, 1.0f))
			m_pGPXMngr->setRGBVal(eRGBType, fTempVal);
	}
	else
		cout << "Error reading in value: \"" << c_Num << "\".\n";

	return bReturnVal;
}

// Executes the color command:
//		Checks to ensure correct value input
//		Calls Graphics Manager to set the uniform Variables
bool CmdHandler::exec_SetColor()
{
	char c_Num[MAX_INPUT_SIZE] = {};
	float fTempVal;
	float fColorRGB[GraphicsManager::RGB_MAX];
	char* p_End;
	int iErr;
	bool bReturnVal = false;

	for (int i = 0; i < GraphicsManager::RGB_MAX; i++)
	{
		iErr = get_Next_Word(c_Num, MAX_INPUT_SIZE);
		if (ERR_CODE != iErr) // Ensure no error occurred.
		{
			// Verify Value
			fTempVal = strtof(c_Num, &p_End);
			bReturnVal |= !checkRange(fTempVal, 0.0f, 1.0f); // True if Valid Range -> set Value
			if (!bReturnVal) 
				fColorRGB[i] = fTempVal;
		}
	}

	if (!bReturnVal)
	{
		m_pGPXMngr->setRGBVal(GraphicsManager::RED, fColorRGB[GraphicsManager::RED]);
		m_pGPXMngr->setRGBVal(GraphicsManager::GREEN, fColorRGB[GraphicsManager::GREEN]);
		m_pGPXMngr->setRGBVal(GraphicsManager::BLUE, fColorRGB[GraphicsManager::BLUE]);
	}

	return bReturnVal;
}

// Checks the range of a floating point value between a given Min and Max.
// Returns true if value is in range
//         false otherwise
bool CmdHandler::checkRange(float fVal, float fMIN, float fMAX)
{
	bool bNotInRange = (fVal < fMIN || fVal > fMAX);

	if (bNotInRange)
		cout << "Error : Invalid range for RGB Value: " << fVal <<
			".  Please choose value between " << fMIN << " and " 
			<< fMAX << "." << endl;

	return !bNotInRange;
}