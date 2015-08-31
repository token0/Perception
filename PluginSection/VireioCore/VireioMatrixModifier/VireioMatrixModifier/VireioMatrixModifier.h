﻿/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.h> and
Class <VireioMatrixModifier> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from Vireio source code originally
authored by Chris Drain (v1.1.x 2013).

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include<stdio.h>
#include<vector>

#include"AQU_Nodus.h"
#include"Resources.h"
#include"VireioMatrixModifierDataStructures.h"

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")
#elif defined(VIREIO_D3D9)
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#endif

#include"..\..\..\Include\Vireio_GUIDs.h"

#define BYTE_PLUG_TYPE                                 1
#define	FLOAT_PLUG_TYPE                                4
#define INT_PLUG_TYPE                                  7
#define SIZE_T_PLUG_TYPE                              11
#define UINT_PLUG_TYPE                                12
#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_VOID_PLUG_TYPE                           114
#define	D3DCOLOR_PLUG_TYPE                          1004
#define D3DFORMAT_PLUG_TYPE                         1011
#define D3DPRIMITIVETYPE_PLUG_TYPE                  1021
#define D3DTRANSFORMSTATETYPE_PLUG_TYPE             1032
#define VECTOR4F_PLUG_TYPE                          1063
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_D3DRECT_PLUG_TYPE                       2024
#define PNT_IDIRECT3DPIXELSHADER9_PLUG_TYPE         2042
#define PNT_IDIRECT3DVERTEXSHADER9_PLUG_TYPE        2051
#define PNT_ID3D10BUFFER_PLUG_TYPE                  8020
#define PNT_ID3D10DEPTHSTENCILVIEW_PLUG_TYPE        8021
#define PNT_ID3D10RENDERTARGETVIEW_TYPE             8022
#define PNT_ID3D10RESOURCE_PLUG_TYPE                8023
#define PNT_ID3D10PIXELSHADER_PLUG_TYPE             8031
#define PNT_ID3D10VERTEXSHADER_PLUG_TYPE            8037
#define PNT_D3D10_BOX_PLUG_TYPE                     8059
#define PNT_D3D10_BUFFER_DESC_PLUG_TYPE             8112
#define PNT_D3D10_SUBRESOURCE_DATA_PLUG_TYPE        8119
#define PPNT_ID3D10BUFFER_PLUG_TYPE                 9020
#define PPNT_ID3D10DEPTHSTENCILVIEW_PLUG_TYPE       9021
#define PPNT_ID3D10RENDERTARGETVIEW_PLUG_TYPE       9022
#define PPNT_ID3D10PIXELSHADER_PLUG_TYPE            9031
#define PPNT_ID3D10VERTEXSHADER_PLUG_TYPE           9037
#define PNT_ID3D11DEPTHSTENCILVIEW_PLUG_TYPE       11025
#define PNT_ID3D11RENDERTARGETVIEW_TYPE            11026
#define PNT_ID3D11RESOURCE_PLUG_TYPE               11028
#define PNT_ID3D11CLASSLINKAGE_PLUG_TYPE           11040
#define PNT_ID3D11PIXELSHADER_PLUG_TYPE            11053
#define PNT_ID3D11VERTEXSHADER_PLUG_TYPE           11060
#define PNT_D3D11_BOX_PLUG_TYPE                    11094
#define PNT_D3D11_BUFFER_DESC_PLUG_TYPE            11177
#define PNT_D3D11_SUBRESOURCE_DATA_PLUG_TYPE       11189
#define PPNT_ID3D11BUFFER_PLUG_TYPE                12024
#define PPNT_ID3D11DEPTHSTENCILVIEW_PLUG_TYPE      12025
#define PPNT_ID3D11RENDERTARGETVIEW_PLUG_TYPE      12026
#define PPNT_ID3D11PIXELSHADER_PLUG_TYPE           12053
#define PPNT_ID3D11VERTEXSHADER_PLUG_TYPE          12060

#define	PROVOKING_TYPE                                 2                     /**< Provoking type is 2 - just invoker, no provoker **/
#define METHOD_REPLACEMENT                         false                     /**< This node does NOT replace the D3D call (default) **/

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#define NUMBER_OF_COMMANDERS                           4
#define NUMBER_OF_DECOMMANDERS                        43
#elif defined(VIREIO_D3D9)
#define NUMBER_OF_COMMANDERS                           0
#define NUMBER_OF_DECOMMANDERS                        12
#endif

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

/**
* Node Commander Enumeration.
***/
enum STS_Commanders
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	ppActiveConstantBuffers_DX10_VertexShader,
	ppActiveConstantBuffers_DX11_VertexShader, // active constant buffers output
	ppActiveConstantBuffers_DX10_PixelShader,
	ppActiveConstantBuffers_DX11_PixelShader, // active constant buffers output
#elif defined(VIREIO_D3D9)
#endif
};

/**
* Node Commander Enumeration.
***/
enum STS_Decommanders
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/*** D3D10 + D3D11 methods ***/
	// ID3D10Device::CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader);
	// ID3D11Device::CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);
	pShaderBytecode_VertexShader,
	BytecodeLength_VertexShader,
	pClassLinkage_VertexShader,
	ppVertexShader_DX10,
	ppVertexShader_DX11,
	// ID3D10Device::CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader);
	// ID3D11Device::CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11PixelShader **ppPixelShader);
	pShaderBytecode_PixelShader,
	BytecodeLength_PixelShader,
	pClassLinkage_PixelShader,
	ppPixelShader_DX10,
	ppPixelShader_DX11,
	pVertexShader_10, // ID3D10Device::VSSetShader(ID3D10VertexShader *pVertexShader);
	pVertexShader_11, // ID3D11DeviceContext::VSSetShader(ID3D11VertexShader *pVertexShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	pPixelShader_10, // ID3D10Device::PSSetShader(ID3D10PixelShader *pPixelShader);
	pPixelShader_11, // ID3D11DeviceContext::PSSetShader(ID3D11PixelShader *pPixelShader, ID3D11ClassInstance *const *ppClassInstances, UINT NumClassInstances);
	pDesc_DX10, // ID3D10Device::CreateBuffer(const D3D10_BUFFER_DESC *pDesc, D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer);
	pInitialData_DX10,
	ppBuffer_DX10,
	pDesc_DX11, // ID3D11Device::CreateBuffer(const D3D11_BUFFER_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, ID3D11Buffer **ppBuffer);
	pInitialData_DX11,
	ppBuffer_DX11,
	StartSlot_VertexShader, // ID3D10Device::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers);
	NumBuffers_VertexShader,
	ppConstantBuffers_DX10_VertexShader,
	ppConstantBuffers_DX11_VertexShader, // ID3D11DeviceContext::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	StartSlot_PixelShader, // ID3D10Device::PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers);
	NumBuffers_PixelShader,
	ppConstantBuffers_DX10_PixelShader,
	ppConstantBuffers_DX11_PixelShader, // ID3D11DeviceContext::PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers);
	pDstResource_DX10, // ID3D10Device::UpdateSubresource(ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
	pDstResource_DX11, // ID3D11DeviceContext::UpdateSubresource(ID3D11Resource *pDstResource, UINT DstSubresource, const D3D11_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch);
	DstSubresource, 
	pDstBox_DX10, 
	pDstBox_DX11, 
	pSrcData, 
	SrcRowPitch, 
	SrcDepthPitch,
	pDstResource_DX10_Copy, // ID3D10Device::CopyResource(ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource);
	pSrcResource_DX10_Copy,
	pDstResource_DX11_Copy, // ID3D11DeviceContext::CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource);
	pSrcResource_DX11_Copy,
	pDstResource_DX10_CopySub, // ID3D10Device::CopySubresourceRegion(ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox);
	pDstResource_DX11_CopySub, // ID3D11DeviceContext::CopySubresourceRegion(ID3D11Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource *pSrcResource, UINT SrcSubresource, const D3D11_BOX *pSrcBox);
	DstSubresource_CopySub,
	DstX,
	DstY,
	DstZ,
	pSrcResource_DX10_CopySub,
	pSrcResource_DX11_CopySub,
	SrcSubresource,
	pSrcBox_DX10,
	pSrcBox_DX11,
	// ID3D10Device::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
	// ID3D11DeviceContext::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
	// ID3D10Device::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
	// ID3D11DeviceContext::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
#elif defined(VIREIO_D3D9)
	/*** D3D9 methods ***/
	pShader_Vertex, // SetVertexShader(IDirect3DVertexShader9 *pShader);
	pShader_Pixel, // SetPixelShader(IDirect3DPixelShader9 *pShader);
	State, // SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix);
	pMatrix,
	State_Multiply, // MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	pMatrix_Multiply,
	StartRegister_VertexShader, // SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	pConstantData_VertexShader,
	Vector4fCount_VertexShader,
	StartRegister_PixelShader, // SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	pConstantData_PixelShader,
	Vector4fCount_PixelShader,
	// GetVertexShaderConstantF(UINT StartRegister,float* pData,UINT Vector4fCount);
	// GetPixelShaderConstantF(UINT StartRegister,float* pData,UINT Vector4fCount);
	// IDirect3DStateBlock::Apply();
#endif
};

/**
* Vireio Matrix Modifier (DX9/10/11).
* Vireio Perception Stereo Matrix Modification Handler.
***/
class MatrixModifier : public AQU_Nodus
{
public:
	MatrixModifier();
	virtual ~MatrixModifier();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual int             GetProvokingType() { return PROVOKING_TYPE; }
	virtual bool            GetMethodReplacement() { return METHOD_REPLACEMENT; }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

	/**
	* Return value pointer (HRESULT).
	***/
	void* m_pvReturn;

private:

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/*** MatrixModifier private methods ***/
	void UpdateConstantBuffer(ID3D11DeviceContext* pcContext, ID3D11Resource *pcDstResource, UINT dwDstSubresource, const D3D11_BOX *psDstBox, const void *pvSrcData, UINT dwSrcRowPitch, UINT dwSrcDepthPitch, UINT dwBufferIndex, UINT dwBufferSize);
#elif defined(VIREIO_D3D9)
#endif

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	/*** MatrixModifier input pointers ***/
	void** m_ppvShaderBytecode_VertexShader;
	SIZE_T* m_pnBytecodeLength_VertexShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_VertexShader;
	ID3D10VertexShader*** m_pppcVertexShader_DX10;
	ID3D11VertexShader*** m_pppcVertexShader_DX11;
	void** m_ppvShaderBytecode_PixelShader;
	SIZE_T* m_pnBytecodeLength_PixelShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_PixelShader;
	ID3D10PixelShader*** m_pppcPixelShader_DX10;
	ID3D11PixelShader*** m_pppcPixelShader_DX11;
	ID3D10VertexShader** m_ppcVertexShader_10;
	ID3D11VertexShader** m_ppcVertexShader_11;
	ID3D10PixelShader** m_ppcPixelShader_10;
	ID3D11VertexShader** m_ppcPixelShader_11;
	D3D10_BUFFER_DESC** m_ppsDesc_DX10;
	D3D10_SUBRESOURCE_DATA** m_ppsInitialData_DX10;
	ID3D10Buffer*** m_pppcBuffer_DX10;
	D3D11_BUFFER_DESC** m_ppsDesc_DX11;
	D3D11_SUBRESOURCE_DATA** m_ppsInitialData_DX11;
	ID3D11Buffer*** m_pppcBuffer_DX11;
	UINT* m_pdwStartSlot_VertexShader;
	UINT* m_pdwNumBuffers_VertexShader;
	ID3D10Buffer*** m_pppcConstantBuffers_DX10_VertexShader;
	ID3D11Buffer*** m_pppcConstantBuffers_DX11_VertexShader;
	UINT* m_pdwStartSlot_PixelShader;
	UINT* m_pdwNumBuffers_PixelShader;
	ID3D10Buffer*** m_pppcConstantBuffers_DX10_PixelShader;
	ID3D11Buffer*** m_pppcConstantBuffers_DX11_PixelShader;
	ID3D10Resource** m_ppcDstResource_DX10;
	ID3D11Resource** m_ppcDstResource_DX11;
	UINT* m_pdwDstSubresource;
	D3D10_BOX** m_ppsDstBox_DX10; 
	D3D11_BOX** m_ppsDstBox_DX11; 
	void** m_ppvSrcData; 
	UINT* m_pdwSrcRowPitch; 
	UINT* m_pdwSrcDepthPitch;
	ID3D10Resource** m_ppcDstResource_DX10_Copy;
	ID3D10Resource** m_ppcSrcResource_DX10_Copy;
	ID3D11Resource** m_ppcDstResource_DX11_Copy;
	ID3D11Resource** m_ppcSrcResource_DX11_Copy;
	ID3D10Resource** m_ppcDstResource_DX10_CopySub;
	ID3D11Resource** m_ppcDstResource_DX11_CopySub;
	UINT* m_pdwDstSubresource_CopySub;
	UINT* m_pdwDstX;
	UINT* m_pdwDstY;
	UINT* m_pdwDstZ;
	ID3D10Resource** m_ppcSrcResource_DX10_CopySub;
	ID3D11Resource** m_ppcSrcResource_DX11_CopySub;
	UINT* m_pdwSrcSubresource;
	D3D10_BOX** m_ppsSrcBox_DX10;
	D3D10_BOX** m_ppsSrcBox_DX11;

	/*** 
	* MatrixModifier output pointers 
	***/
	void* m_pvOutput[NUMBER_OF_COMMANDERS];
	/**
	* The d3d11 shader description vector.
	* Contains all enumerated shader data structures.
	***/
	std::vector<Vireio_D3D11_Shader> m_asShaders;
	/**
	* The d3d11 constant buffer vector.
	***/
	std::vector<ID3D11Buffer*> m_apcActiveConstantBuffers11;
	/**
	* The active vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcActiveVertexShader10;
		ID3D11VertexShader* m_pcActiveVertexShader11;
	};
	/**
	* Constant Buffer private data buffer.
	***/
	union
	{
		BYTE m_pchBuffer10[D3D10_REQ_CONSTANT_BUFFER_ELEMENT_COUNT];
		BYTE m_pchBuffer11[D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT];
	};

#elif defined(VIREIO_D3D9)
	/*** MatrixModifier input pointers ***/
	IDirect3DVertexShader9** m_ppcShader_Vertex;
	IDirect3DPixelShader9** m_ppcShader_Pixel;
	D3DTRANSFORMSTATETYPE* m_psState;
	D3DMATRIX** m_ppsMatrix;
	D3DTRANSFORMSTATETYPE* m_psState_Multiply;
	D3DMATRIX** m_ppsMatrix_Multiply;
	UINT* m_pdwStartRegister_VertexShader;
	float** m_ppfConstantData_VertexShader;
	UINT* m_pdwVector4fCount_VertexShader;
	UINT* m_pdwStartRegister_PixelShader;
	float** m_ppfConstantData_PixelShader;
	UINT* m_pdwVector4fCount_PixelShader;
#endif

	/*** Matrix Modifier matrices ***/
	/**
	* Projection matrix.
	***/
	D3DXMATRIX matProjection;
	/**
	* Projection inverse matrix.
	***/
	D3DXMATRIX matProjectionInv;
	/**
	* The shifted left projection matrix.
	***/
	D3DXMATRIX projectLeft;
	/**
	* The shifted right projection matrix.
	***/
	D3DXMATRIX projectRight;
	/**
	* Left matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformLeft;
	/**
	* Right matrix used to roll (if roll enabled) and shift view for ipd.
	***/
	D3DXMATRIX transformRight;
	/**
	* Left view projection matrix.
	***/
	D3DXMATRIX matViewProjLeft;
	/**
	* Right view projection matrix.
	***/
	D3DXMATRIX matViewProjRight;
	/**
	* Left view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformLeft;
	/**
	* Right view projection transform matrix.
	***/
	D3DXMATRIX matViewProjTransformRight;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	MatrixModifier* pMatrixModifier = new MatrixModifier();
	return static_cast<AQU_Nodus*>(pMatrixModifier);
}
