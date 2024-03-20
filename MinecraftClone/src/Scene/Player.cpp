#include "Precompiled.h"

#include "Player.h"
#include "Utils/ChunkUtils.h"
#include "Core/InputManager.h"

Player::Player(Camera* playerCamera)
    : mPlayerCamera(playerCamera)
    , mVelocityY(0.0f)
{
}

void Player::HandleInput()
{
    InputManager& inputManager = InputManager::GetInstance();

    if (inputManager.IsPressed(eInputButton::MOUSE_L))
    {
        mBlockHandler.SetBlock();
    }
    else if (inputManager.IsPressed(eInputButton::MOUSE_R))
    {
        mBlockHandler.SwitchInteractionMode();
    }

#ifdef _DEBUG

    if (inputManager.IsPressed(eInputButton::F))
    {
        inputManager.ToggleInputLock();

        ::ShowCursor(inputManager.IsInputLock());
    }

#endif // _DEBUG
}

void Player::Update(World& world, const float dt)
{
    InputManager& inputManager = InputManager::GetInstance();
    if (inputManager.IsInputLock())
    {
        return;
    }

    // mVelocityY -= def::g_GRAVITY * dt; #TODO implement gravity

    const IntVector2D& mouseRelativeChange = inputManager.GetMouseRelativeChange();
    mPlayerCamera->RotateYaw(mouseRelativeChange.mX * def::g_MOUSE_SENSITIVITY);
    mPlayerCamera->RotatePitch(mouseRelativeChange.mY * def::g_MOUSE_SENSITIVITY);

    const float yaw = mPlayerCamera->GetYawInRadian();
    const float pitch = mPlayerCamera->GetPitchInRadian();

    const float sinY = sin(yaw);
    const float cosY = cos(yaw);
    const float sinP = sin(pitch);
    const float cosP = cos(pitch);

    SimpleMath::Vector3& forward = mPlayerCamera->GetForward();
    SimpleMath::Vector3& right = mPlayerCamera->GetRight();
    SimpleMath::Vector3& up = mPlayerCamera->GetUp();

    forward = SimpleMath::Vector3(sinY * cosP, -sinP, cosY * cosP);
    right = SimpleMath::Vector3(cosY, 0.f, -sinY);
    up = SimpleMath::Vector3(sinY * sinP, cosP, cosY * sinP);

    const float deltaPosition = def::g_PLAYER_SPEED * dt;
    SimpleMath::Vector3& position = mPlayerCamera->GetEyePos();

    if (inputManager.IsPressed(eInputButton::W) || inputManager.IsPressing(eInputButton::W))
    {
        position += forward * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::S) || inputManager.IsPressing(eInputButton::S))
    {
        position -= forward * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::D) || inputManager.IsPressing(eInputButton::D))
    {
        position += right * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::A) || inputManager.IsPressing(eInputButton::A))
    {
        position -= right * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::E) || inputManager.IsPressing(eInputButton::E))
    {
        position += up * deltaPosition;
    }

    if (inputManager.IsPressed(eInputButton::Q) || inputManager.IsPressing(eInputButton::Q))
    {
        position -= up * deltaPosition;
    }

    // handleCollision(); #TODO implement physics

    mBlockHandler.Update(*this);
    mBlockMarker.Update(mBlockHandler);
}

void Player::Render()
{
    mBlockMarker.Render(mBlockHandler);
}

void Player::handleCollision()
{
    SimpleMath::Vector3& position = mPlayerCamera->GetEyePos();

    // Collision Detection
    // broad phase
    int xMin = static_cast<int>(std::floor(position.x - def::g_PLAYER_RADIUS)), xMax = static_cast<int>(std::ceil(position.x + def::g_PLAYER_RADIUS));
    int zMin = static_cast<int>(std::floor(position.z - def::g_PLAYER_RADIUS)), zMax = static_cast<int>(std::ceil(position.z + def::g_PLAYER_RADIUS));
    int yMin = static_cast<int>(std::floor(position.y - def::g_PLAYER_HEIGHT)), yMax = static_cast<int>(std::ceil(position.y));

    std::vector<IntVector3D> blocks;
    for (int x = xMin; x <= xMax; ++x)
    {
        for (int y = yMin; y <= yMax; ++y)
        {
            for (int z = zMin; z <= zMax; ++z)
            {
                BlockInfo blockInfo;
                IntVector3D blockWorldPos = { x, y, z };
                if (ChunkUtils::GetBlockInfo(&blockInfo, blockWorldPos))
                {
                    blocks.push_back(blockWorldPos);
                }
            }
        }
    }

    struct CollisionInfo
    {
        IntVector3D BlockWorldPos;
        SimpleMath::Vector3 ContactPoint;
        SimpleMath::Vector3 Normal;
        float Overlap;
    };

    // narrow phase
    const float playerHeightHalf = def::g_PLAYER_HEIGHT / 2.0f;
    std::vector<CollisionInfo> collisions;
    collisions.reserve(blocks.size());
    for (const IntVector3D& block : blocks)
    {
        // find closest point
        const float closestX = max((float)block.mX, min(block.mX + 1.0f, position.x));
        const float closestY = max((float)block.mY, min(block.mY + 1.0f, position.y - playerHeightHalf));
        const float closestZ = max((float)block.mZ, min(block.mZ + 1.0f, position.z));

        // find whether the closest point is inside player's bounding cylinder
        const float dx = closestX - position.x;
        const float dy = closestY - position.y - playerHeightHalf;
        const float dz = closestZ - position.z;
        const float rSquare = dx * dx + dz * dz;

        if (abs(dy) < playerHeightHalf && rSquare < def::g_PLAYER_RADIUS * def::g_PLAYER_RADIUS) // if within bounds, closest point becomes contact point
        {
            const float overlapY = playerHeightHalf - abs(dy);
            const float overlapXZ = def::g_PLAYER_RADIUS - sqrt(rSquare);

            float overlap;
            SimpleMath::Vector3 normal;
            if (overlapY < overlapXZ)
            {
                normal = { 0, -(float)BasicUtils::GetSign(dy), 0 };
                overlap = overlapY;
            }
            else
            {
                normal = SimpleMath::Vector3(-dx, 0, -dz);
                normal.Normalize();
                overlap = overlapXZ;
            }

            collisions.push_back({ block, {closestX, closestY, closestZ}, normal, overlap });
            // BlockInfo blockInfo;
            // if (ChunkUtils::GetBlockInfo(&blockInfo, block))
            //     LOG_WARN("block type : {0}", static_cast<int>(blockInfo.BlockType));
        }
    }

    // resolve collisions
    // need sorting by overlap inc
    for (const CollisionInfo& collision : collisions)
    {
        // adjust player position
        SimpleMath::Vector3 deltaPosition = collision.Normal;
        deltaPosition *= collision.Overlap;
        position += deltaPosition;

        // negate player velocity
        // float magnitude = up.Dot(collision.Normal);
        if (collision.Normal.y == 1.0f)
        {
            mVelocityY = 0.0f;
        }
    }

    // position += up * mVelocityY * dt;
}
