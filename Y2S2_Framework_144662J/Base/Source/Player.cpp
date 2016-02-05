#include "Player.h"
#include "Application.h"
#include "MeshBuilder.h"

Player::Player(void)
	: jumpspeed(0)
	, speed(300)
	, state(Idle)
	, curentRotate(0)
	, rotateTo(0)
	, armRotation(0)
	, bodyRotation(0)
{
	Init();
}

Player::~Player(void)
{
}

// Initialise this class instance
void Player::Init(void)
{
	curPosition.Set(250, 250, 250);
	curDirection.Set(1, 0, 1);
	curDirection.Normalize();
	up.Set(0, 1, 0);
	right = (curDirection.Cross(up)).Normalized();
	vel.SetZero();
	// Initialise the Avatar's movement flags
	for (int i = 0; i<255; i++){
		myKeys[i] = false;
	}
	Boundary_TopLeft		= Vector3( 1000.0f, 1000.0f, 1000.0f);
	Boundary_BottomRight = Vector3(0, 0, 0);
}

// Returns true if the player is on ground
bool Player::isOnGround(void)
{
	return true;
}

// Returns true if the player is jumping upwards
bool Player::isJumpUpwards(void)
{
	return true;
}

// Returns true if the player is on freefall
bool Player::isFreeFall(void)
{
	return true;
}

// Set the player's status to free fall mode
void Player::SetOnFreeFall(bool isOnFreeFall)
{
	if (isOnFreeFall == true)
	{
		jumpspeed = 0;
	}
}

// Set the player to jumping upwards
void Player::SetToJumpUpwards(bool isOnJumpUpwards)
{
	if (isOnJumpUpwards == true)
	{
		jumpspeed = 15;
	}
}

// Set position x of the player
void Player::SetPos_x(int pos_x)
{
	curPosition.x = pos_x;
}

// Set position y of the player
void Player::SetPos_y(int pos_y)
{
	curPosition.y = pos_y;
}

// Set position z of the player
void Player::SetPos_z(int pos_z)
{
	curPosition.z = pos_z;
}

// Set Jumpspeed of the player
void Player::SetJumpspeed(int jumpspeed)
{
	this->jumpspeed = jumpspeed;
}

// Stop the player's movement
void Player::SetToStop(void)
{
	jumpspeed = 0;
}

/********************************************************************************
Hero Move Up Down
********************************************************************************/
void Player::MoveUpDown(const bool mode, const float timeDiff)
{
	speed = 300;

	Vector3 temp(0, 1, 0);
	if (mode)
	{
		vel += temp * (speed * timeDiff);
	}
	else
	{
		vel -= temp * (speed * timeDiff);
	}
}

void Player::MoveFrontBack(const bool mode, const float timeDiff)
{
	if (myKeys['a'] == true || myKeys['d'] == true)
		speed = 200;
	else
		speed = 300;

	Vector3 temp(curDirection.x, 0, curDirection.z);
	temp.Normalize();
	if (mode)
	{
		vel -= temp * (speed * timeDiff);
	}
	else
	{
		vel += temp * (speed * timeDiff);
	}
}

/********************************************************************************
Hero Move Left Right
********************************************************************************/
void Player::MoveLeftRight(const bool mode, const float timeDiff)
{
	if (myKeys['w'] == true || myKeys['s'] == true)
		speed = 200;
	else
		speed = 300;

	Vector3 temp(right.x, 0, right.z);
	temp.Normalize();

	if (mode)
	{
		vel -= temp * (speed * timeDiff);
	}
	else
	{
		vel += temp * (speed * timeDiff);
	}
}


// Get position x of the player
int Player::GetPos_x(void)
{
	return curPosition.x;
}

// Get position y of the player
int Player::GetPos_y(void)
{
	return curPosition.y;
}

// Get position y of the player
int Player::GetPos_z(void)
{
	return curPosition.z;
}

// Get position of the player
Vector3 Player::GetPosition()
{
	return curPosition;
}

// Get direction of the player
Vector3 Player::GetDirection()
{
	return curDirection;
}

// Get up of the player
Vector3 Player::GetUp()
{
	return up;
}

Vector3 Player::GetRight()
{
	return right;
}

float Player::GetSpeed()
{
	return vel.Length();
}

// Get Jumpspeed of the player
float Player::GetRotate()
{
	return curentRotate;
}

// Update Jump Upwards
void Player::UpdateJumpUpwards()
{
	curPosition.y -= jumpspeed;
	jumpspeed -= 1;
	if (jumpspeed == 0)
	{
	}
}

// Update FreeFall
void Player::UpdateFreeFall()
{
	curPosition.y += jumpspeed;
	jumpspeed += 1;
}

// Constrain the position of the Hero to within the border
void Player::ConstrainHero(const int leftBorder, const int rightBorder,
	const int topBorder, const int bottomBorder,
	float timeDiff)
{
	if (curPosition.x < leftBorder)
	{
		curPosition.x = leftBorder;
	}
	else if (curPosition.x > rightBorder)
	{
		curPosition.x = rightBorder;
	}

	if (curPosition.y < topBorder)
		curPosition.y = topBorder;
	else if (curPosition.y > bottomBorder)
		curPosition.y = bottomBorder;
}


/********************************************************************************
Update Movement
********************************************************************************/
void Player::UpdateMovement(const unsigned char key, const bool status)
{
	myKeys[key] = status;
}

/********************************************************************************
Update
********************************************************************************/
void Player::Update(double dt)
{
	// WASD movement
	if (myKeys['w'] == true)
	{
		MoveFrontBack(false, dt);
	}
	if (myKeys['s'] == true)
	{
		MoveFrontBack(true, dt);
	}
	if (myKeys['a'] == true)
	{
		MoveLeftRight(true, dt);
	}
	if (myKeys['d'] == true)
	{
		MoveLeftRight(false, dt);
	}
	if(Application::IsKeyPressed(VK_SHIFT))
	{
		MoveUpDown(true, dt);
	}
	if(Application::IsKeyPressed(VK_CONTROL))
	{
		MoveUpDown(false, dt);
	}

	if (vel.Length() > 100)
	{
		vel = vel.Normalized() * 100;
	}

	curPosition += vel * dt;
	this->ApplyTranslate(vel.x * dt, vel.y * dt, vel.z * dt);

	if (((curPosition.x > Boundary_TopLeft.x) || 
		(curPosition.y > Boundary_TopLeft.y) ||
		(curPosition.z > Boundary_TopLeft.z)) ||
		((curPosition.x < Boundary_BottomRight.x) ||
		(curPosition.y < Boundary_BottomRight.y) ||
		(curPosition.z < Boundary_BottomRight.z)) )
	{
		curPosition -= vel * dt;
		this->ApplyTranslate(-vel.x * dt, -vel.y * dt, -vel.z * dt);
	}

	if (vel.Length() > 1)
		vel -= vel.Normalized() * 200 * dt;
	else
		vel.SetZero();

	//update states
	state = Idle;
	if (myKeys['a'] == true || myKeys['d'] == true)
	{
		if (myKeys['a'] == true && myKeys['d'] == true)
		{
			state = Idle;
		}
		else if (myKeys['a'] == true)
		{
			state = Left;
		}
		else if (myKeys['d'] == true)
		{
			state = Right;
		}
	}
	if (myKeys['w'] == true || myKeys['s'] == true)
	{
		if (myKeys['w'] == true && myKeys['s'] == true)
		{
			state = Idle;
		}
		else if (myKeys['w'] == true)
		{
			state = Forward;
			if (myKeys['a'] == true)
			{
				state = ForwardLeft;
			}
			else if (myKeys['d'] == true)
			{
				state = ForwardRight;
			}
		}
		else if (myKeys['s'] == true)
		{
			state = Backward;
			if (myKeys['a'] == true)
			{
				state = BackwardLeft;
			}
			else if (myKeys['d'] == true)
			{
				state = BackwardRight;
			}
		}
	}
	if (Application::Button_Right == 1)
		Zoom = true;
	else
		Zoom = false;

	if (Application::camera_yaw != 0)
		Yaw(dt);
	if (Application::camera_pitch != 0)
		Pitch(dt);

	UpdateAnimation(dt);
}

void Player::Pitch(double dt)
{
	if (Application::camera_pitch > 0.0)
	{
		float pitch = (float)(-mouseSens * Application::camera_pitch * (float)dt);
		curDirection.Normalized();
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		curDirection = rotation * curDirection;
		up = right.Cross(curDirection).Normalized();
	}
	else if (Application::camera_pitch < 0.0)
	{
		float pitch = (float)(-mouseSens * Application::camera_pitch * (float)dt);
		curDirection.Normalized();
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		curDirection = rotation * curDirection;
		up = right.Cross(curDirection).Normalized();
	}
}

void Player::Yaw(double dt)
{
	if (Application::camera_yaw > 0.0)
	{
		float yaw = (float)(-mouseSens * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		curDirection = rotation * curDirection;
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(curDirection).Normalized();
	}
	else if (Application::camera_yaw < 0.0)
	{
		float yaw = (float)(-mouseSens * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		curDirection = rotation * curDirection;
		right = curDirection.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(curDirection).Normalized();
	}
}

void Player::UpdateAnimation(double dt)
{
	this->staticTransformation.SetToRotation(curentRotate, 0, 1, 0);
	switch (state)
	{
	case Idle:
		{
			if(armRotation != 0)
			{
				if(armRotation > 0)
				{
					armRotation -= dt * 200;
					if(armRotation < 0)
					{
						armRotation = 0;
					}
				}
				else
				{
					armRotation += dt * 200;
					if(armRotation > 0)
					{
						armRotation = 0;
					}
				}
			}
			if(bodyRotation != 0)
			{
				if(bodyRotation > 0)
				{
					bodyRotation -= dt * 200;
					if(bodyRotation < 0)
					{
						bodyRotation = 0;
					}
				}
				else
				{
					bodyRotation += dt * 200;
					if(bodyRotation > 0)
					{
						bodyRotation = 0;
					}
				}
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case Forward:
		{	
			rotateTo = Vector3(0, 0, 1).GetAngle(Vector3(curDirection.x, 0, curDirection.z));
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation < 50)
			{
				armRotation += dt * 200;
			}
			if(bodyRotation < 50)
			{
				bodyRotation += dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case Backward:
		{
			rotateTo = Vector3(0, 0, 1).GetAngle(Vector3(curDirection.x, 0, curDirection.z));
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation > -50)
			{
				armRotation -= dt * 200;
			}
			if(bodyRotation > -30)
			{
				bodyRotation -= dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case Left:
		{
			rotateTo = Vector3(0, 0, 1).GetAngle(Vector3(0,1,0).Cross(Vector3(curDirection.x, 0, curDirection.z)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation < 50)
			{
				armRotation += dt * 200;
			}
			if(bodyRotation < 50)
			{
				bodyRotation += dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case Right:
		{
			rotateTo = Vector3(0, 0, 1).GetAngle(Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0,1,0)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation < 50)
			{
				armRotation += dt * 200;
			}
			if(bodyRotation < 50)
			{
				bodyRotation += dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case ForwardLeft:
		{
			rotateTo = Vector3(0.5, 0, 0.5).GetAngle(Vector3(0,1,0).Cross(Vector3(curDirection.x, 0, curDirection.z)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation < 50)
			{
				armRotation += dt * 200;
			}
			if(bodyRotation < 50)
			{
				bodyRotation += dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case ForwardRight:
		{
			rotateTo = Vector3(-0.5, 0, 0.5).GetAngle(Vector3(curDirection.x, 0, curDirection.z).Cross(Vector3(0,1,0)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation < 50)
			{
				armRotation += dt * 200;
			}
			if(bodyRotation < 50)
			{
				bodyRotation += dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case BackwardLeft:
		{
			rotateTo = Vector3(0.5, 0, -0.5).GetAngle(Vector3(0,1,0).Cross(Vector3(curDirection.x, 0, curDirection.z)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation > -50)
			{
				armRotation -= dt * 200;
			}
			if(bodyRotation > -30)
			{
				bodyRotation -= dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;

	case BackwardRight:
		{
			rotateTo = Vector3(0.5, 0, 0.5).GetAngle(Vector3(0,1,0).Cross(Vector3(curDirection.x, 0, curDirection.z)).Normalized());
			if(curentRotate != rotateTo && !Zoom)
				UpdateRotate(dt);
			if(armRotation > -50)
			{
				armRotation -= dt * 200;
			}
			if(bodyRotation > -30)
			{
				bodyRotation -= dt * 200;
			}
			Mtx44 tempRotate, tempTranslate;
			tempRotate.SetToRotation(armRotation, 1, 0, 0);
			tempTranslate.SetToTranslation(0, 0.8, 0);
			this->GetNode(id_leftHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
			tempRotate.SetToRotation(bodyRotation, 1, 0, 0);
			this->staticTransformation = staticTransformation * tempRotate;
		}
		break;
	}
	if (Zoom)
	{
		rotateTo = Vector3(0, 0, 1).GetAngle(Vector3(curDirection.x, 0, curDirection.z));
		if(curentRotate != rotateTo)
			UpdateRotateAimming(dt);
		armRotation2 = Vector3(0, 1, 0).GetAngle(Vector3(curDirection.x, curDirection.y, curDirection.z)) - 180 - bodyRotation;
		Mtx44 tempRotate, tempTranslate;
		tempRotate.SetToRotation(armRotation2, 1, 0, 0);
		tempTranslate.SetToTranslation(0, 0.8, 0);
		this->GetNode(id_rightHand)->staticTransformation = tempTranslate * tempRotate * tempTranslate.GetInverse();
	}
}

void Player::UpdateRotate(double dt)
{
	if (rotateTo > curentRotate)
	{
		if (rotateTo - curentRotate > 180)
		{
			curentRotate -= dt * 150;	
			if (curentRotate < 0)
			{
				curentRotate = 360 - dt;
			}
		}
		else
		{
			curentRotate += dt * 150;
		}
	}
	else
	{
		if (curentRotate - rotateTo > 180)
		{
			curentRotate += dt * 150;
			if (curentRotate > 360)
			{
				curentRotate -= curentRotate;
			}
		}
		else
		{
			curentRotate -= dt * 150;
		}
	}
	if(curentRotate - rotateTo < 3 && rotateTo - curentRotate < 3)
	{
		curentRotate = rotateTo;
	}
}

void Player::UpdateRotateAimming(double dt)
{
	if (rotateTo > curentRotate)
	{
		if (rotateTo - curentRotate > 180)
		{
			curentRotate -= dt * (rotateTo - curentRotate) * 30;	
			if (curentRotate < 0)
			{
				curentRotate = 360 - dt;
			}
		}
		else
		{
			curentRotate += dt * (rotateTo - curentRotate) * 30;
		}
	}
	else
	{
		if (curentRotate - rotateTo > 180)
		{
			curentRotate += dt * (curentRotate - rotateTo) * 30;
			if (curentRotate > 360)
			{
				curentRotate -= curentRotate;
			}
		}
		else
		{
			curentRotate -= dt * (curentRotate - rotateTo) * 30;
		}
	}
	if(curentRotate - rotateTo < 0.5 && rotateTo - curentRotate < 0.5)
	{
		curentRotate = rotateTo;
	}
}
