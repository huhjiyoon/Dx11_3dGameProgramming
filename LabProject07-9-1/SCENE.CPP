//-----------------------------------------------------------------------------
// File: CScene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}
extern bool lightTurn = false;      
void CScene::BuildDefaultLightsAndMaterials()
{
	m_nLights = 4;
	m_pLights = new LIGHT[m_nLights];
	::ZeroMemory(m_pLights, sizeof(LIGHT) * m_nLights);

	m_xmf4GlobalAmbient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	 
	m_pLights[0].m_bEnable = true;
	m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights[0].m_fRange = 5000.0f;
	m_pLights[0].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[0].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_pLights[0].m_xmf4Specular = XMFLOAT4(0.7f, 0.7f, 0.7f, 0.0f);
	m_pLights[0].m_xmf3Position = XMFLOAT3(0.0f, 30.0f, 30.0f);
	m_pLights[0].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pLights[0].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.05f, 0.0001f); 
	m_pLights[1].m_bEnable = true; 
	m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights[1].m_fRange = 500.0f;
	if (lightTurn) { 
		m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.9f, 0.1f, 0.1f, 1.0f);
		m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.9f, 0.4f, 0.4f, 1.0f);
		m_pLights[1].m_xmf4Specular = XMFLOAT4(0.9f, 0.3f, 0.3f, 0.0f);
	}
	else {
		m_pLights[1].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		m_pLights[1].m_xmf4Diffuse = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		m_pLights[1].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	}

	m_pLights[1].m_xmf3Position = XMFLOAT3(0.0f, 20.0f, 0.0f);
	m_pLights[1].m_xmf3Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_pLights[1].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[1].m_fFalloff = 8.0f;
	m_pLights[1].m_fPhi = (float)cos(XMConvertToRadians(40.0f));
	m_pLights[1].m_fTheta = (float)cos(XMConvertToRadians(20.0f));
	m_pLights[2].m_bEnable = true; 
	m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights[2].m_xmf4Ambient = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights[2].m_xmf4Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_pLights[2].m_xmf4Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 0.0f);
	m_pLights[2].m_xmf3Direction = XMFLOAT3(0.8f, 0.0f, 0.0f);//
	m_pLights[3].m_bEnable = true; 
	m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights[3].m_fRange = 500.0f; 
	m_pLights[3].m_xmf4Ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights[3].m_xmf4Diffuse = XMFLOAT4(0.3f, 0.7f, 0.0f, 1.0f);
	m_pLights[3].m_xmf4Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 0.0f);
	m_pLights[3].m_xmf3Position = XMFLOAT3(0.0f, 30.0f, 30.0f);
	m_pLights[3].m_xmf3Direction = XMFLOAT3(0.0f, 1.0f, 1.0f);
	m_pLights[3].m_xmf3Attenuation = XMFLOAT3(1.0f, 0.01f, 0.0001f);
	m_pLights[3].m_fFalloff = 8.0f;
	m_pLights[3].m_fPhi = (float)cos(XMConvertToRadians(90.0f));
	m_pLights[3].m_fTheta = (float)cos(XMConvertToRadians(30.0f));
}

void CScene::BuildObjects(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	m_pd3dGraphicsRootSignature = CreateGraphicsRootSignature(pd3dDevice);
	CMaterial::PrepareShaders(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature);
	BuildDefaultLightsAndMaterials();

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Car Model
	//  
	//달려오는 자동차의 수를 5개로 지정하기    
	m_nCarObjects = 5; 
	if (m_CarObjects != nullptr) 
		delete[] m_CarObjects;  
	m_CarObjects = new CGameObject*[m_nCarObjects];   
	//사용할 모델 2가지 정의 
	CGameObject *CarModel1 = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/PoliceCar.bin"); 
	CarModel1->SetPosition(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_nCarObjects; ++i) {
		CApacheObject* CarObject = NULL; 
		CarObject = new CApacheObject();

		CarObject->SetChild(CarModel1, true);
		CarObject->OnInitialize();
		
	
		CarObject->SetScale(5.5f, 6.0f, 4.5f); 
		CarObject->Rotate(0.0f, 180.0f, 0.0f); 
		m_CarObjects[i] = CarObject; 

		m_CarObjects[i]->serialNum = i; 
		m_CarObjects[i]->type = 2;  
		m_CarObjects[i]->Rail_MaxSpeed = 5.0f;    
		m_CarObjects[i]->Car_MaxSpeed = randomSpeed(dre); 
	} 
	m_CarObjects[0]->SetPosition(uidX1(dre) + 25.0f, 0.0f, uidZ(dre) + 50.0f);
	m_CarObjects[1]->SetPosition(uidX2(dre) + 25.0f, 0.0f, uidZ(dre) + 50.0f);
	m_CarObjects[2]->SetPosition(uidX3(dre) + 25.0f, 0.0f, uidZ(dre) + 50.0f);
	m_CarObjects[3]->SetPosition(uidX4(dre) + 25.0f, 0.0f, uidZ(dre) + 50.0f);
	m_CarObjects[4]->SetPosition(uidX5(dre) + 25.0f, 0.0f, uidZ(dre) + 50.0f);

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Tree Fence
	// 
	//나무 펜스를 양 옆으로 한 레일 당 20개 20개씩 배치할 예정임
	m_nTreeObjects = 40; 
	m_TreeObjects = new CGameObject * [m_nTreeObjects];
	//사용할 모델 2가지 정의 
	CGameObject* TreeModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/Tree.bin"); 
	TreeModel->SetPosition(0.0f, 0.0f, 0.0f);  
	CApacheObject* TreeObject = NULL; 
	 
	for (int i = 0; i < m_nTreeObjects; ++i) {  
		TreeObject = new CApacheObject();
		TreeObject->SetChild(TreeModel, true); 
		TreeObject->OnInitialize();
		
		TreeObject->SetScale(8.0f, 8.5f,8.0f); 
		TreeObject->Rotate(0.0f, 90.0f, 0.0f);   

		if(i< 20) 
			TreeObject->SetPosition(-120.0f, 0.0f, 50*i); 
		else
			TreeObject->SetPosition(120.0f, 0.0f, 50 * (i-20)); 
		m_TreeObjects[i] = TreeObject; 

		m_TreeObjects[i]->type = 1;
		m_TreeObjects[i]->Rail_MaxSpeed = 7.0f; 
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	// Rail
	// 
	m_nRailObjects = 10;
	m_RailObjects = new CGameObject * [m_nRailObjects]; 
	//사용할 레일 모델 정의 
	CGameObject* RailModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/wood_bridge_1.bin");
	RailModel->SetPosition(0.0f, 0.0f, 0.0f); 
	CApacheObject* RailObject = NULL; 

	for (int i = 0; i < m_nRailObjects; ++i) {
		RailObject = new CApacheObject();
		RailObject->SetChild(RailModel, true);
		RailObject->OnInitialize();

		RailObject->SetScale(114.0f, 1.5f,100.5f); 
		RailObject->Rotate(0.0f, 0.0f, 0.0f);

		RailObject->SetPosition(0.0f, 0.0f, 300 * i);
		m_RailObjects[i] = RailObject;

		m_RailObjects[i]->type = 0;
		m_RailObjects[i]->Rail_MaxSpeed = 7.0f;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 선인장 장애물
	// 
	m_nCactusObjects = 3;
	m_CactusObjects = new CGameObject * [m_nCactusObjects];
	//사용할 레일 모델 정의 
	CGameObject* CactusModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/선인장2.bin");   
	CactusModel->SetPosition(0.0f, 0.0f, 0.0f);
	CApacheObject* CactusObject = NULL;

	for (int i = 0; i < m_nCactusObjects; ++i) { 
		
		CactusObject = new CApacheObject();

		CactusObject->SetChild(CactusModel, true);
		CactusObject->OnInitialize();
		CactusObject->SetScale(15.5f, 17.0f, 15.5f);   
		m_CactusObjects[i] = CactusObject;

		m_CactusObjects[i]->serialNum = i;
		m_CactusObjects[i]->type = 3; 
		m_CactusObjects[i]->Rail_MaxSpeed = 5.0f;
	}

	m_CactusObjects[0]->SetPosition(uidX_1(dre), 0.0f, uidZ(dre)); 
	m_CactusObjects[1]->SetPosition(uidX_2(dre), 0.0f, uidZ(dre)); 
	m_CactusObjects[2]->SetPosition(uidX_3(dre), 0.0f, uidZ(dre)); 


	///////////////////////////////////////////////////////////////////////////////////////////////////
	// 아이템
	// 

	m_nItemObjects = 3;
	m_ItemObjects = new CGameObject * [m_nItemObjects];
	//사용할 아이템 모델 정의 
	CGameObject* ItemModel = CGameObject::LoadGeometryFromFile(pd3dDevice, pd3dCommandList, m_pd3dGraphicsRootSignature, "Model/woodbox.bin");
	CApacheObject* ItemObject = NULL;
	ItemModel->SetPosition(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < m_nItemObjects; ++i) {

		ItemObject = new CApacheObject();
		ItemObject->SetChild(ItemModel, true);
		ItemObject->OnInitialize();
		
		ItemObject->SetScale(7.0f, 7.0f, 7.0f); 
		m_ItemObjects[i] = ItemObject; 
		
		m_ItemObjects[i]->serialNum = i;
		m_ItemObjects[i]->type = 4;
		m_ItemObjects[i]->Rail_MaxSpeed = 5.0f;
	}
	m_ItemObjects[0]->SetPosition(uidX__I(dre), 20.0f,150.0f);
	m_ItemObjects[1]->SetPosition(uidX__I(dre), 20.0f,300.0f);
	m_ItemObjects[2]->SetPosition(uidX__I(dre), 20.0f,450.0f); 
	CreateShaderVariables(pd3dDevice, pd3dCommandList); 
}

void CScene::ReleaseObjects()
{
	if (m_pd3dGraphicsRootSignature) m_pd3dGraphicsRootSignature->Release();

	//Car
	if (m_CarObjects)
	{
		for (int i = 0; i < m_nCarObjects; i++) if (m_CarObjects[i]) m_CarObjects[i]->Release();
		delete[] m_CarObjects;
	}
	//Tree
	if (m_TreeObjects)
	{
		for (int i = 0; i < m_nTreeObjects; i++) if (m_TreeObjects[i]) m_TreeObjects[i]->Release(); 
		delete[] m_TreeObjects; 
	}

	//Rail
	if (m_RailObjects)
	{
		for (int i = 0; i < m_nRailObjects; i++) if (m_RailObjects[i]) m_RailObjects[i]->Release();
		delete[] m_RailObjects;
	}
	
	//Cactus
	if (m_CactusObjects)
	{
		for (int i = 0; i < m_nCactusObjects; i++) if (m_CactusObjects[i]) m_CactusObjects[i]->Release();
		delete[] m_CactusObjects;
	}
	
	//Item
	if (m_ItemObjects)
	{
		for (int i = 0; i < m_nItemObjects; i++) if (m_ItemObjects[i]) m_ItemObjects[i]->Release();
		delete[] m_ItemObjects;
	}

	ReleaseShaderVariables();

	if (m_pLights) delete[] m_pLights;
}

ID3D12RootSignature *CScene::CreateGraphicsRootSignature(ID3D12Device *pd3dDevice)
{
	ID3D12RootSignature *pd3dGraphicsRootSignature = NULL;

	D3D12_ROOT_PARAMETER pd3dRootParameters[3];

	pd3dRootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[0].Descriptor.ShaderRegister = 1; //Camera
	pd3dRootParameters[0].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	pd3dRootParameters[1].Constants.Num32BitValues = 32;
	pd3dRootParameters[1].Constants.ShaderRegister = 2; //GameObject
	pd3dRootParameters[1].Constants.RegisterSpace = 0;
	pd3dRootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	pd3dRootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	pd3dRootParameters[2].Descriptor.ShaderRegister = 4; //Lights
	pd3dRootParameters[2].Descriptor.RegisterSpace = 0;
	pd3dRootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	D3D12_ROOT_SIGNATURE_FLAGS d3dRootSignatureFlags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS | D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;
	D3D12_ROOT_SIGNATURE_DESC d3dRootSignatureDesc;
	::ZeroMemory(&d3dRootSignatureDesc, sizeof(D3D12_ROOT_SIGNATURE_DESC));
	d3dRootSignatureDesc.NumParameters = _countof(pd3dRootParameters);
	d3dRootSignatureDesc.pParameters = pd3dRootParameters;
	d3dRootSignatureDesc.NumStaticSamplers = 0;
	d3dRootSignatureDesc.pStaticSamplers = NULL;
	d3dRootSignatureDesc.Flags = d3dRootSignatureFlags;

	ID3DBlob *pd3dSignatureBlob = NULL;
	ID3DBlob *pd3dErrorBlob = NULL;
	D3D12SerializeRootSignature(&d3dRootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &pd3dSignatureBlob, &pd3dErrorBlob);
	pd3dDevice->CreateRootSignature(0, pd3dSignatureBlob->GetBufferPointer(), pd3dSignatureBlob->GetBufferSize(), __uuidof(ID3D12RootSignature), (void **)&pd3dGraphicsRootSignature);
	if (pd3dSignatureBlob) pd3dSignatureBlob->Release();
	if (pd3dErrorBlob) pd3dErrorBlob->Release();

	return(pd3dGraphicsRootSignature);
}

void CScene::CreateShaderVariables(ID3D12Device *pd3dDevice, ID3D12GraphicsCommandList *pd3dCommandList)
{
	UINT ncbElementBytes = ((sizeof(LIGHTS) + 255) & ~255); //256의 배수
	m_pd3dcbLights = ::CreateBufferResource(pd3dDevice, pd3dCommandList, NULL, ncbElementBytes, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL);

	m_pd3dcbLights->Map(0, NULL, (void **)&m_pcbMappedLights);
}

void CScene::UpdateShaderVariables(ID3D12GraphicsCommandList *pd3dCommandList)
{
	::memcpy(m_pcbMappedLights->m_pLights, m_pLights, sizeof(LIGHT) * m_nLights);
	::memcpy(&m_pcbMappedLights->m_xmf4GlobalAmbient, &m_xmf4GlobalAmbient, sizeof(XMFLOAT4));
	::memcpy(&m_pcbMappedLights->m_nLights, &m_nLights, sizeof(int));
}

void CScene::ReleaseShaderVariables()
{
	if (m_pd3dcbLights)
	{
		m_pd3dcbLights->Unmap(0, NULL);
		m_pd3dcbLights->Release();
	}
}

void CScene::ReleaseUploadBuffers()
{
	for (int i = 0; i < m_nCarObjects; i++) m_CarObjects[i]->ReleaseUploadBuffers(); 
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{

	return(false);
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}



void CScene::Collsion()
{
	//자동차 
	for (int i = 0; i < m_nCarObjects; ++i) {
		if (m_CarObjects[i]->mx_OOBB.Intersects(m_pPlayer->mx_OOBB)) {
			m_CarObjects[i]->isCarCollision = true;
			for (int i = 0; i < m_nTreeObjects; ++i) 
				m_TreeObjects[i]->isCarCollision = true;  //모든 나무들의 Car콜리션도 true로  
			for (int i = 0; i < m_nRailObjects; ++i) 
				m_RailObjects[i]->isCarCollision = true; //모든 레일들의 Car콜리션도 true로 
			for (int i = 0; i < m_nCactusObjects; ++i) 
				m_CactusObjects[i]->isCarCollision = true; //모든 Cactus들의 Car콜리션도 true로 
			for (int i = 0; i < m_nItemObjects; ++i) 
				m_ItemObjects[i]->isCarCollision = true; //모든 Item들의 Car콜리션도 true로 
		}
	}
	//선인장 장애물
	for (int i = 0; i < m_nCactusObjects; ++i) { 
		if (m_CactusObjects[i]->mx_OOBB.Intersects(m_pPlayer->mx_OOBB))   
			m_pPlayer->isCactusCollision = true;   
	}  

	//아이템
	for (int i = 0; i < m_nItemObjects; ++i) {
		if (m_ItemObjects[i]->mx_OOBB.Intersects(m_pPlayer->mx_OOBB)) {  
			m_ItemObjects[i]->isItemCollision = true; 
		}
	}

}
void CScene::AnimateObjects(float fTimeElapsed)
{
	BuildDefaultLightsAndMaterials(); 
	if (m_pLights)
	{
		m_pLights[1].m_xmf3Position = m_pPlayer->GetPosition();  
		m_pLights[1].m_xmf3Direction = m_pPlayer->GetLookVector();
	}

	for (int i = 0; i < m_nCarObjects; i++) m_CarObjects[i]->Animate(fTimeElapsed, NULL);
	for (int i = 0; i < m_nCactusObjects; i++)m_CactusObjects[i]->Animate(fTimeElapsed, NULL); 
	for (int i = 0; i < m_nTreeObjects; i++) m_TreeObjects[i]->Animate(fTimeElapsed, NULL); 
	for (int i = 0; i < m_nRailObjects; i++) m_RailObjects[i]->Animate(fTimeElapsed, NULL); 
	for (int i = 0; i < m_nItemObjects; i++) m_ItemObjects[i]->Animate(fTimeElapsed, NULL); 

	Collsion();

}

void CScene::Render(ID3D12GraphicsCommandList *pd3dCommandList, CCamera *pCamera)
{
	pCamera->SetViewportsAndScissorRects(pd3dCommandList);
	pd3dCommandList->SetGraphicsRootSignature(m_pd3dGraphicsRootSignature);
	pCamera->UpdateShaderVariables(pd3dCommandList);
	UpdateShaderVariables(pd3dCommandList);

	D3D12_GPU_VIRTUAL_ADDRESS d3dcbLightsGpuVirtualAddress = m_pd3dcbLights->GetGPUVirtualAddress();
	pd3dCommandList->SetGraphicsRootConstantBufferView(2, d3dcbLightsGpuVirtualAddress); //Lights
	


	//Car
	for (int i = 0; i < m_nCarObjects; i++)
	{
		if (m_CarObjects[i])
		{
			m_CarObjects[i]->UpdateTransform(NULL);
			m_CarObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
	
	//Tree Fence
	for (int i = 0; i < m_nTreeObjects; i++)
	{
		if (m_TreeObjects[i])
		{
			m_TreeObjects[i]->UpdateTransform(NULL); 
			m_TreeObjects[i]->Render(pd3dCommandList, pCamera); 
		}
	}
	//Rail
	for (int i = 0; i < m_nRailObjects; i++)
	{
		if (m_RailObjects[i])
		{
			m_RailObjects[i]->UpdateTransform(NULL);
			m_RailObjects[i]->Render(pd3dCommandList, pCamera);
		}
	}
	//Cactus
	for (int i = 0; i < m_nCactusObjects; i++)
	{
		if (m_CactusObjects[i])
		{
			m_CactusObjects[i]->UpdateTransform(NULL);  
			m_CactusObjects[i]->Render(pd3dCommandList, pCamera);  
		}
	}
	//Item
	for (int i = 0; i < m_nItemObjects; i++)
	{
		if (m_ItemObjects[i])
		{
			m_ItemObjects[i]->UpdateTransform(NULL); 
			m_ItemObjects[i]->Render(pd3dCommandList, pCamera); 
		}
	}
}

