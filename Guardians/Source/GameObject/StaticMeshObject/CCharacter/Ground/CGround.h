#pragma once

#include "GameObject/StaticMeshObject/CStaticMeshObject.h"

/*********************************************************
*	�n�ʃN���X
**/
class CGround
	: public CStaticMeshObject
{
public:
	CGround();
	virtual ~CGround();

	virtual void Update() override;

protected:

};