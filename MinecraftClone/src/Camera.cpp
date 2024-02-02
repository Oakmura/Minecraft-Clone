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

void Camera::RotateYaw(const float deltaYaw)
{
    mYawInRadian += deltaYaw;
}

void Camera::RotatePitch(const float deltaPitch)
{
    mPitchInRadian += deltaPitch;
    mPitchInRadian = std::clamp(mPitchInRadian, XMConvertToRadians(-PITCH_MAX), XMConvertToRadians(PITCH_MAX));
}

void Camera::updateOrientation()
{

}
