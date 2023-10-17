//-----------------------------------------------------------------------------
// File: Scene.h
//-----------------------------------------------------------------------------

#pragma once

#include "Shader.h"
#include "Player.h"
#include "Object.h"

#define MAX_LIGHTS			16 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

struct LIGHT
{
	XMFLOAT4				m_xmf4Ambient;
	XMFLOAT4				m_xmf4Diffuse;
	XMFLOAT4				m_xmf4Specular;
	XMFLOAT3				m_xmf3Position;
	float 					m_fFalloff;
	XMFLOAT3				m_xmf3Direction;
	float 					m_fTheta; //cos(m_fTheta)
	XMFLOAT3				m_xmf3Attenuation;
	float					m_fPhi; //cos(m_fPhi)
	bool					m_bEnable;
	int						m_nType;
	float					m_fRange;
	float					padding;
};

struct LIGHTS
{
	LIGHT					m_pLights[MAX_LIGHTS];
	XMFLOAT4				m_xmf4GlobalAmbient;
	int						m_nLights;
};

class CScene
{
public:
    CScene();
    ~CScene();

	bool OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	bool OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList);
	virtual void ReleaseShaderVariables();

	void BuildDefaultLightsAndMaterials();
	void BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList);
	void ReleaseObjects();
	void Collsion(); 
	ID3D12RootSignature *CreateGraphicsRootSignature(ID3D12Device *pd3dDevice);
	ID3D12RootSignature *GetGraphicsRootSignature() { return(m_pd3dGraphicsRootSignature); }

	bool ProcessInput(UCHAR *pKeysBuffer);
    void AnimateObjects(float fTimeElapsed);
    void Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera=NULL);

	void ReleaseUploadBuffers();

	CPlayer						*m_pPlayer = NULL; 

public:
	ID3D12RootSignature			*m_pd3dGraphicsRootSignature = NULL;

	
	LIGHT						*m_pLights = NULL;
	int							m_nLights = 0;

	XMFLOAT4					m_xmf4GlobalAmbient;

	ID3D12Resource				*m_pd3dcbLights = NULL;
	LIGHTS						*m_pcbMappedLights = NULL;

	float						m_fElapsedTime = 0.0f;


	//Car
	CGameObject**				m_CarObjects = NULL;
	int							m_nCarObjects = 0; 

	//Tree Fence
	CGameObject**				m_TreeObjects = NULL; 
	int							m_nTreeObjects = 0;

	//Rail
	CGameObject**				m_RailObjects = NULL;   
	int							m_nRailObjects = 0; 

	//Cactus handicap
	CGameObject**				m_CactusObjects = NULL; 
	int							m_nCactusObjects = 0;  

	//Item 
	CGameObject**				m_ItemObjects = NULL;
	int							m_nItemObjects = 0;
	
	

	random_device rd;
	default_random_engine dre{ rd() };
	uniform_int_distribution<int> uidX1{ -110,-75 };
	uniform_int_distribution<int> uidX2{ -70,-33 };
	uniform_int_distribution<int> uidX3{ -30, 3 };
	uniform_int_distribution<int> uidX4{ 5,45 };
	uniform_int_distribution<int> uidX5{ 50, 75 };

	uniform_int_distribution<int> uidZ{ 400,1300 };
	uniform_int_distribution<int> randomSpeed{ 1,4 };


	uniform_int_distribution<int> uidX_1{-50,-40};
	uniform_int_distribution<int> uidX_2{ 0,10 };
	uniform_int_distribution<int> uidX_3{ 20,50};

	uniform_int_distribution<int> uidX__I{ -60, 60};



};
