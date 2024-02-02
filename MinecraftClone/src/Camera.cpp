#include "Precompiled.h"
#include "Camera.h"

Matrix Camera::GetViewMatrix() const
{
    return Matrix::CreateTranslation(-mPos) * Matrix::CreateRotationY(-mYawInRadian) * Matrix::CreateRotationX(-mPitchInRadian);
}

Matrix Camera::GetProjMatrix() const
{
    return XMMatrixPerspectiveFovLH(XMConvertToRadians(mFOV), mWidth / static_cast<float>(mHeight), mNearZ, mFarZ);
}
