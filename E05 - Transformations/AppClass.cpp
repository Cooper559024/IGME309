#include "AppClass.h"
void Application::InitVariables(void)
{
	////Change this to your name and email
	m_sProgrammer = "Aidan McMurray - acm5636@rit.edu";
	vector3 v3Position(0.0f, 0.0f, 10.0f);
	vector3 v3Target = ZERO_V3;
	vector3 v3Upward = AXIS_Y;
	m_pCameraMngr->SetPositionTargetAndUpward(v3Position, v3Target, v3Upward);

	//Allocate the memory for the Meshes
	m_uMeshCount = 6;
	m_pBlackMesh = new MyMesh();
	m_pBlackMesh->GenerateCube(1.0f, C_BLACK);

	m_pBlueMesh = new MyMesh();
	m_pBlueMesh->GenerateCube(1.0f, C_BLUE);

	m_pBrownMesh = new MyMesh();
	m_pBrownMesh->GenerateCube(1.0f, C_BROWN);

	m_pRedMesh = new MyMesh();
	m_pRedMesh->GenerateCube(1.0f, C_RED);

	m_pYellowMesh = new MyMesh();
	m_pYellowMesh->GenerateCube(1.0f, C_YELLOW);

	m_pTanMesh = new MyMesh();
	m_pTanMesh->GenerateCube(1.0f, vector3(.82f, .71f, .55f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	static float fTimer = 0;
	static uint uClock = m_pSystem->GenClock();
	fTimer += static_cast<float>(m_pSystem->GetDeltaTime(uClock));

	uint i = 0;
	static float fHorizontal = -10.0f;
	static vector3 v3Position(fHorizontal, 0.0f, 0.0f);

	v3Position.x = fHorizontal + fTimer;
	matrix4 m4Position = glm::translate(vector3(-13.0f, -8.0f, -40.0f)) * glm::translate(v3Position);

	// first row
	for (uint count = 0; count < 12; count++)
	{
		if (count < 4 || count > 7)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 0.0f, 0.0f)));
		}
	}

	// second row
	for (uint count = 1; count < 11; count++)
	{
		if (count < 4 || count > 7)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 1.0f, 0.0f)));
		}
	}

	// third row
	for (uint count = 2; count < 10; count++)
	{
		if (count < 5 || count > 6)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 2.0f, 0.0f)));
		}
	}

	// fourth row
	for (uint count = 0; count < 12; ++count)
	{
		if (count < 2 || count > 9)
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 3.0f, 0.0f)));
		}
		else
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 3.0f, 0.0f)));
		}
	}

	// fifth row
	for (uint count = 0; count < 12; ++count)
	{
		if (count < 3 || count > 8)
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 4.0f, 0.0f)));
		}
		else
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 4.0f, 0.0f)));
		}
	}

	// sixth row
	for (uint count = 0; count < 12; ++count)
	{
		if (count < 2 || count > 9)
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 5.0f, 0.0f)));
		}
		else if (count == 2 || count == 9)
		{
			m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 5.0f, 0.0f)));
		}
		else if (count == 4 || count == 7)
		{
			m_pYellowMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 5.0f, 0.0f)));
		}
		else
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 5.0f, 0.0f)));
		}
	}

	// seventh row
	for (uint count = 0; count < 12; ++count)
	{
		if (count < 4 || count > 7)
		{
			m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 6.0f, 0.0f)));
		}
		else
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 6.0f, 0.0f)));
		}
	}

	// eigth row
	for (uint count = 1; count < 11; ++count)
	{
		if (count == 4 || count == 7)
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 7.0f, 0.0f)));
		}
		else
		{
			m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 7.0f, 0.0f)));
		}
	}

	// ninth row
	for (uint count = 2; count < 8; ++count)
	{
		if (count == 4)
		{
			m_pBlueMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 8.0f, 0.0f)));
		}
		else
		{
			m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 8.0f, 0.0f)));
		}
	}

	// tenth row
	for (uint count = 2; count < 10; ++count)
	{
		m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 9.0f, 0.0f)));
	}

	// eleventh row
	for (uint count = 1; count < 11; ++count)
	{
		if (count < 3)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 10.0f, 0.0f)));
		}
		else if (count < 7)
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 10.0f, 0.0f)));
		}
		else
		{
			m_pBlackMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 10.0f, 0.0f)));
		}
	}

	// twelfth row
	for (uint count = 1; count < 12; ++count)
	{
		if (count == 1 || count == 3 || count == 4)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 11.0f, 0.0f)));
		}
		else if (count == 8)
		{
			m_pBlackMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 11.0f, 0.0f)));
		}
		else
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 11.0f, 0.0f)));
		}
	}

	// thirteenth row
	for (uint count = 1; count < 11; ++count)
	{
		if (count == 1 || count == 3)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 12.0f, 0.0f)));
		}
		else if (count == 7)
		{
			m_pBlackMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 12.0f, 0.0f)));
		}
		else
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 12.0f, 0.0f)));
		}
	}

	// fourteenth row
	for (uint count = 2; count < 9; ++count)
	{
		if (count < 5)
		{
			m_pBrownMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 13.0f, 0.0f)));
		}
		else if (count == 7)
		{
			m_pBlackMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 13.0f, 0.0f)));
		}
		else
		{
			m_pTanMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 13.0f, 0.0f)));
		}
	}

	// fifteenth row
	for (uint count = 2; count < 11; ++count)
	{
		m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 14.0f, 0.0f)));
	}

	// sixteenth row
	for (uint count = 3; count < 9; ++count)
	{
		m_pRedMesh->Render(m4Projection, m4View, glm::translate(m4Position, vector3(count, 15.0f, 0.0f)));
	}

	// draw a skybox
	m_pModelMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pModelMngr->Render();

	//clear the render list
	m_pModelMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release meshes
	SafeDelete(m_pBlackMesh);
	SafeDelete(m_pBlueMesh);
	SafeDelete(m_pBrownMesh);
	SafeDelete(m_pRedMesh);
	SafeDelete(m_pYellowMesh);
	SafeDelete(m_pTanMesh);

	//release GUI
	ShutdownGUI();
}