#include "WorldTransform.h"
#include "MathUtility.h"

void WorldTransform::UpdateMatrix()
{
	//�q
	 //�ϊ��s��
	 //�X�P�[��
	Matrix4 matScale;
	SetScaleMatrix(matScale, scale_);

	//��]
	//��]�s��
	Matrix4 matRot, matRotX, matRotY, matRotZ;
	SetRotationMatrix(matRotX, rotation_.x, 'x');
	SetRotationMatrix(matRotY, rotation_.y, 'y');
	SetRotationMatrix(matRotZ, rotation_.z, 'z');
	matRot = matRotZ * matRotX * matRotY;

	//���s�ړ�
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	SetTranslationMatrix(matTrans, translation_);
	matWorld_ = normal;

	if (parent_ == nullptr)//world���̂��e��������i�e�����Ȃ��j
	{
		Matrix4xMatrix4(matWorld_,
			matScale * matRot * matTrans);
	}
	else//world���̂��q��������i�e������j
	{
		//matworld�ɓ����
		Matrix4xMatrix4(matWorld_,
			matScale * matRot * matTrans * parent_->matWorld_);
	}

	TransferMatrix();
}
