#include "SpatialPartition.h"
#include "stdio.h"

/********************************************************************************
 Constructor
 ********************************************************************************/
CSpatialPartition::CSpatialPartition(void)
: theGrid(NULL)
, xSize(0)
, ySize(0)
, xGridSize(0)
, yGridSize(0)
, xNumOfGrid(0)
, yNumOfGrid(0)
{
}

/********************************************************************************
 Destructor
 ********************************************************************************/
CSpatialPartition::~CSpatialPartition(void)
{
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			theGrid[i*yNumOfGrid + j]->DeleteObjects();
		}
	}
	for (int i = 0; i < xNumOfGrid * yNumOfGrid; ++i)
		delete theGrid[i];
}

/********************************************************************************
 Initialise the spatial partition
 ********************************************************************************/
bool CSpatialPartition::Init(const int xSize, const int ySize, const int xNumOfGrid, const int yNumOfGrid)
{
	if ((xSize>0) && (ySize > 0)
		&& (xNumOfGrid > 0) && (yNumOfGrid > 0))
	{
		this->xNumOfGrid = xNumOfGrid;
		this->yNumOfGrid = yNumOfGrid;
		this->xGridSize = xSize;
		this->yGridSize = ySize;
		this->xSize = xGridSize * xNumOfGrid;
		this->ySize = yGridSize * yNumOfGrid;

		// Create an array of grids
		for (int i = 0; i < xNumOfGrid*yNumOfGrid; i++)
		{
			theGrid.push_back(new CGrid());
		}
		//theGrid = new CGrid[ xNumOfGrid*yNumOfGrid ];

		// Initialise the array of grids
		for (int i = 0; i < xNumOfGrid; i++)
		{
			for (int j = 0; j < yNumOfGrid; j++)
			{
				theGrid[i*yNumOfGrid + j]->Init(i, j, xSize, ySize);
			}
		}
		return true;
	}
	return false;
}

/********************************************************************************
 Get xNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetxNumOfGrid(void)
{
	return xNumOfGrid;
}

/********************************************************************************
 Get yNumOfGrid
 ********************************************************************************/
int CSpatialPartition::GetyNumOfGrid(void)
{
	return yNumOfGrid;
}

/********************************************************************************
 Get a particular grid
 ********************************************************************************/
CGrid CSpatialPartition::GetGrid(const int xIndex, const int yIndex)
{
	return *(theGrid[xIndex*yNumOfGrid + yIndex]);
}

/********************************************************************************
 Get a particular grid's Mesh
 ********************************************************************************/
Mesh* CSpatialPartition::GetGridMesh(const int xIndex, const int yIndex)
{
	return theGrid[xIndex*yNumOfGrid + yIndex]->theGridMesh;
}

/********************************************************************************
 Set a particular grid
 ********************************************************************************/
void CSpatialPartition::SetGridMesh(const int xIndex, const int yIndex, Mesh* theGridMesh)
{
	theGrid[xIndex*yNumOfGrid + yIndex]->SetMesh(theGridMesh);
}

/********************************************************************************
 Add a new object model
 ********************************************************************************/
void CSpatialPartition::AddObject(CSceneNode* theObject)
{
	// Get the indices of the 2 values of each position
	float tem = (int)theObject->GetTopLeft().x;
	int index_topleft_x = ((int)theObject->GetTopLeft().x / (xGridSize));
	int index_topleft_z = ((int)theObject->GetTopLeft().z / (yGridSize));
	int index_bottomright_x = ((int)theObject->GetBottomRight().x / (xGridSize));
	int index_bottomright_z = ((int)theObject->GetBottomRight().z / (yGridSize));

	// Calculate the index of each position
	int index_topleft = index_topleft_x*yNumOfGrid + index_topleft_z;
	int index_bottomright = index_bottomright_x*yNumOfGrid + index_bottomright_z;

	// Add them to each grid
	if ((index_topleft >= 0) && (index_topleft < xNumOfGrid*yNumOfGrid))
	{
		theGrid[index_topleft]->AddObject(theObject);
		theObject->currentGrid1 = index_topleft;
	}
	// if part of the object is in another grid, then add it in as well.
	if ((index_bottomright >= 0) && (index_bottomright < xNumOfGrid*yNumOfGrid))
	{
		if (index_topleft != index_bottomright)
		{
			theGrid[index_bottomright]->AddObject(theObject);
			theObject->currentGrid2 = index_bottomright;
		}
		else
			theObject->currentGrid2 = index_topleft;
	}
}

/********************************************************************************
 Calculate the squared distance from camera to a grid's centrepoint
 ********************************************************************************/
float CSpatialPartition::CalculateDistanceSquare(Vector3* theCameraPosition, const int xIndex, const int yIndex)
{
	float xDistance = (xIndex * xGridSize + (xGridSize / 2)) - theCameraPosition->x;
	float yDistance = (yIndex * yGridSize + (yGridSize / 2)) - theCameraPosition->z;

	return (float)(xDistance*xDistance + yDistance*yDistance);
}

CSceneNode* CSpatialPartition::CheckForCollision(Vector3 position)
{
	int GridIndex_x = ((int)position.x / (xGridSize));
	int GridIndex_z = ((int)position.z / (yGridSize));

	int GridIndex = GridIndex_x * yNumOfGrid + GridIndex_z;

	if (GridIndex >= 0 && GridIndex < xNumOfGrid*yNumOfGrid)
	{
		vector<CSceneNode*> theListOfObjects = theGrid[GridIndex]->GetListOfObject();
	
		//Vector3 ObjectTopLeft, ObjectNottomRight;
		for (int i = 0; i < theGrid[GridIndex]->GetListOfObject().size(); i++)
		{
			bool cc = theGrid[GridIndex]->GetListOfObject()[i]->CheckForCollision(position);

			if (cc)
			{
				return theGrid[GridIndex]->GetListOfObject()[i];
			}
		}
	}

	return NULL;
}

/********************************************************************************
Check two positions for collision with objects in any of the grids
********************************************************************************/
CSceneNode* CSpatialPartition::CheckForCollision(Vector3 position_start, Vector3 position_end)
{
	int GridIndex_x = ((int)position_start.x / (xGridSize));
	int GridIndex_z = ((int)position_start.z / (yGridSize));

	// Calculate the index of each position
	int GridIndex = GridIndex_x*yNumOfGrid + GridIndex_z;
	position_start.y = 0.0f;
	// Add them to each grid
	if ((GridIndex >= 0) && (GridIndex<xNumOfGrid*yNumOfGrid))
	{
		vector<CSceneNode*> theListOfObjects = theGrid[GridIndex]->GetListOfObject();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 0; i<(int)theListOfObjects.size(); i++)
		{
			Vector3 hits = Vector3(0, 0, 0);

			bool cc = theListOfObjects[i]->CheckForCollision(position_start, position_end, hits);

			if (cc)
			{
				return theGrid[GridIndex]->GetListOfObject()[i];
			}
		}
	}

	return false;
}

/********************************************************************************
 Render the spatial partition
 ********************************************************************************/
void CSpatialPartition::Render(Vector3* theCameraPosition)
{
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
			// Check the distance for this grid then decide the LOD to display
			if (theCameraPosition)
			{
				float distanceGrid2Camera = CalculateDistanceSquare(theCameraPosition, i, j);
				if (distanceGrid2Camera < 900)
				{
					theGrid[i*yNumOfGrid + j]->RenderObjects(0);
				}
				else if (distanceGrid2Camera < 3600)
				{
					theGrid[i*yNumOfGrid + j]->RenderObjects(1);
				}
				else
				{
					theGrid[i*yNumOfGrid + j]->RenderObjects(2);
				}
			}
			//glPopMatrix();
		}
	}

	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			// Render the Grid and its objects
			//glPushMatrix();
			theGrid[i*yNumOfGrid + j]->Render();
			//glPopMatrix();
		}
	}
}

/********************************************************************************
 Update the spatial partition
 ********************************************************************************/
void CSpatialPartition::Update(Vector3 theCameraPosition)
{
	for (int i = 0; i < xNumOfGrid; i++)
	{
		for (int j = 0; j < yNumOfGrid; j++)
		{
			// Update the Grids
			int temp = i*yNumOfGrid + j;

			for (int k = 0; k < theGrid[temp]->ListOfObjects.size(); ++k)
			{
				int index_topleft_x = ((int)theGrid[temp]->ListOfObjects[k]->GetTopLeft().x / (xGridSize));
				int index_topleft_z = ((int)theGrid[temp]->ListOfObjects[k]->GetTopLeft().z / (yGridSize));
				int index_bottomright_x = ((int)theGrid[temp]->ListOfObjects[k]->GetBottomRight().x / (xGridSize));
				int index_bottomright_z = ((int)theGrid[temp]->ListOfObjects[k]->GetBottomRight().z / (yGridSize));

				int index_topleft = index_topleft_x*yNumOfGrid + index_topleft_z;
				int index_bottomright = index_bottomright_x*yNumOfGrid + index_bottomright_z;

				if (index_topleft >= 0 && index_topleft < xNumOfGrid*yNumOfGrid && index_bottomright >= 0 && index_bottomright < xNumOfGrid*yNumOfGrid)
				{
					if (theGrid[temp]->ListOfObjects[k]->currentGrid1 != index_topleft)
					{
						theGrid[temp]->ListOfObjects[k]->currentGrid1 = index_topleft;
						if (theGrid[temp]->ListOfObjects[k]->currentGrid2 != index_topleft)
						{
							theGrid[index_topleft]->AddObject(theGrid[temp]->ListOfObjects[k]);
						}
					}
					if (theGrid[temp]->ListOfObjects[k]->currentGrid2 != index_bottomright)
					{
						theGrid[temp]->ListOfObjects[k]->currentGrid2 = index_bottomright;
						if (theGrid[temp]->ListOfObjects[k]->currentGrid1 != index_bottomright)
						{
							theGrid[index_bottomright]->AddObject(theGrid[temp]->ListOfObjects[k]);
						}
					}

					if (theGrid[temp]->ListOfObjects[k]->currentGrid1 != temp && theGrid[temp]->ListOfObjects[k]->currentGrid2 != temp)
						theGrid[temp]->RemoveObject(k);
				}
			}
		
			float distanceGrid2Camera = CalculateDistanceSquare(&theCameraPosition, i, j);
			if (distanceGrid2Camera < 10000)
			{
				theGrid[temp]->Update(CModel::RESOLUTION_HIGH);
			}
			else if (distanceGrid2Camera < 40000)
			{
				theGrid[temp]->Update(CModel::RESOLUTION_MEDIUM);
			}
			else
			{
				theGrid[temp]->Update(CModel::RESOLUTION_LOW);
			}
		}
	}
}

/********************************************************************************
 PrintSelf
 ********************************************************************************/
void CSpatialPartition::PrintSelf()
{
	cout << "* CSpatialPartition::PrintSelf() ***********************************************" << endl;
	cout << "xSize        : " << xSize << endl;
	cout << "ySize        : " << ySize << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	cout << "xNumOfGrid   : " << xNumOfGrid << endl;
	if (theGrid.size() > 0)
	{
		cout << "theGrid : OK" << endl;
		cout << "Printing out theGrid below: " << endl;
		for (int i = 0; i < xNumOfGrid; i++)
		{
			for (int j = 0; j < yNumOfGrid; j++)
			{
				theGrid[i*yNumOfGrid + j]->PrintSelf();
			}
		}
	}
	else
		cout << "theGrid : NULL" << endl;
	cout << "********************************************************************************" << endl;
}
