#include "AppClass.h"
void Application::InitVariables(void)
{
	//Make MyMesh object
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_BLACK);
	cubePositions = vector<vector3>();
	
	cubePositions.push_back((vector3(0.0f, 0.0f, 0.0f))); //First index is the middle cube to make swapping b/t left and right easier
	cubePositions.push_back((vector3(-3.0f, 4.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, 4.0f, 0.0f)));
	cubePositions.push_back((vector3(-2.0f, 3.0f, 0.0f)));
	cubePositions.push_back((vector3(2.0f, 3.0f, 0.0f)));
	cubePositions.push_back((vector3(-3.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(-2.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(-1.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(0.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(1.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(2.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, 2.0f, 0.0f)));
	cubePositions.push_back((vector3(-4.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(-3.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(-1.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(0.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(1.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(4.0f, 1.0f, 0.0f)));
	cubePositions.push_back((vector3(-5.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(-4.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(-3.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(-2.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(-1.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(1.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(2.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(4.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(5.0f, 0.0f, 0.0f)));
	cubePositions.push_back((vector3(-5.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(-3.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(-2.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(-1.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(0.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(1.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(2.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(5.0f, -1.0f, 0.0f)));
	cubePositions.push_back((vector3(-5.0f, -2.0f, 0.0f)));
	cubePositions.push_back((vector3(-3.0f, -2.0f, 0.0f)));
	cubePositions.push_back((vector3(3.0f, -2.0f, 0.0f)));
	cubePositions.push_back((vector3(5.0f, -2.0f, 0.0f)));
	cubePositions.push_back((vector3(-2.0f, -3.0f, 0.0f)));
	cubePositions.push_back((vector3(-1.0f, -3.0f, 0.0f)));
	cubePositions.push_back((vector3(1.0f, -3.0f, 0.0f)));
	cubePositions.push_back((vector3(2.0f, -3.0f, 0.0f)));

	movementStep = vector3(0.1, 0, 0);


	//Make MyMesh object
	//m_pMesh1 = new MyMesh();
	//m_pMesh1->GenerateCube(1.0f, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Swaps directions after passing a certain distance
	if (cubePositions[0].x >= 5) 
	{
		movementStep = vector3(-0.1, 0, 0);
	}
	else if (cubePositions[0].x <= -5) 
	{
		movementStep = vector3(0.1, 0, 0);
	}

	//Renders the cubes then sets each position to translate
	for (int i = 0; i < cubePositions.size(); i++)
	{
		m_pMesh->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), glm::translate(cubePositions[i]));
		cubePositions[i] += movementStep;
	}
	
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	SafeDelete(m_pMesh1);
	//release GUI
	ShutdownGUI();
}