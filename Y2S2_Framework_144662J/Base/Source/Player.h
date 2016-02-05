#pragma once

#include "SceneGraph\SceneNode.h"

static float mouseSens = 200;

class Player : public CSceneNode
{
public:
	Player(void);
	virtual ~Player(void);

	enum States
	{
		Idle,
		Forward,
		Backward,
		Left,
		Right,
		ForwardLeft,
		ForwardRight,
		BackwardLeft,
		BackwardRight,
		NUM_GEOMETRY,
	};

	int state;
	bool Zoom;
	
	int id_leftHand, id_rightHand, id_leftLeg, id_rightLeg;

	// Initialise this class instance
	void Init(void);

	// Returns true if the player is on ground
	bool isOnGround(void);
	// Returns true if the player is jumping upwards
	bool isJumpUpwards(void);
	// Returns true if the player is on freefall
	bool isFreeFall(void);
	// Set the player's status to free fall mode
	void SetOnFreeFall(bool isOnFreeFall);
	// Set the player to jumping upwards
	void SetToJumpUpwards(bool isOnJumpUpwards);
	// Stop the player's movement
	void SetToStop(void);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set position z of the player
	void SetPos_z(int pos_y);
	// Set Jumpspeed of the player
	void SetJumpspeed(int jumpspeed);

	// Update Movements
	void MoveUpDown(const bool mode, const float timeDiff);
	void MoveFrontBack(const bool mode, const float timeDiff);
	void MoveLeftRight(const bool mode, const float timeDiff);

	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Get position z of the player
	int GetPos_z(void);
	// Get position of the player
	Vector3 GetPosition();
	// Get direction of the player
	Vector3 GetDirection();
	// Get up of the player
	Vector3 GetUp();
	// Get right of the player
	Vector3 GetRight();
	// Get speed of the player
	float GetSpeed();
	//get angle to rotate
	float GetRotate();

	// Update Jump Upwards
	void UpdateJumpUpwards();
	// Update FreeFall
	void UpdateFreeFall();
	// Update
	void UpdateMovement(const unsigned char key, const bool status = true);
	// Update
	void Update(double dt);

	void Pitch(double dt);
	void Yaw(double dt);

	// Constrain the position of the Hero to within the border
	void ConstrainHero(const int leftBorder, const int rightBorder,
		const int topBorder, const int bottomBorder,
		float timeDiff);

private:
	// Hero's information
	Vector3 curPosition;
	Vector3 curDirection;
	Vector3 up;
	Vector3 right;
	
	float speed;
	Vector3 vel;

	float curentRotate, rotateTo;
	float armRotation, bodyRotation, armRotation2;

	int jumpspeed;

	bool myKeys[255];

	Vector3 Boundary_TopLeft, Boundary_BottomRight;

	void UpdateAnimation(double dt);
	void UpdateRotate(double dt);
	void UpdateRotateAimming(double dt);
};