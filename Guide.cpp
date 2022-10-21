#include "Guide.h"

void Guide::Initialize(Model* model)
{
	model_ = model;
}

void Guide::Update()
{

}

void Guide::Draw(ViewProjection view)
{
	model_->Draw(pos_,view);
}
