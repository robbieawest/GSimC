#version 440 core

layout(local_size_x = 4, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer tree_buffer {
	int tree[];
};

layout(std430, binding = 1) buffer positions_buffer {
	vec3 positions[];
};

layout(std430, binding = 2) buffer center_masses_buffer {
	vec3 center_masses[];
};

layout(std430, binding = 3) buffer center_masses_n_buffer {
	uint center_masses_n[];
};

shared uint occupiedTreeSize;

uniform vec3 simulationBounds = vec3(1.0, 1.0, 1.0);

vec3 origin;
vec3 quadBounds;


uint quad(vec3 pos, bool update) {

	//Updates based on quads on a cube(8 quads) based on the indexes:
	//0 = bottom left back (-x -y -z) (SWN) (South West Nadir) (origin quad)
	//1 = bottom right back (x -y -z) (SEN) (South East Nadir)
	//2 = top right back (x y -z) (NEN) (North East Nadir)
	//3 = top left back (-x y -z) (NWN) (North West Nadir)
	//4 = bottom left front (-x -y z) (SWZ) (South West Zenith)
	//5 = bottom right front (x -y z) (SEZ) (South East Zenith)
	//6 = top right front (x y z) (NEZ) (North East Zenith)
	//7 = top left front (-x y z) (NWZ) (North West Zenith)

	//These indexes then index further from the pointer of the internal/root node in the linear quadtree array structure

	bool xGreater = pos.x > (quadBounds.x + origin.x);
	bool yGreater = pos.y > (quadBounds.y + origin.y);
	bool zGreater = pos.z > (quadBounds.z + origin.z);
		

	if(update) {
		//Update globals
		quadBounds /= 2.0;
		origin.x += quadBounds.x * int(xGreater);
		origin.y += quadBounds.y * int(yGreater);
		origin.z += quadBounds.z * int(zGreater);
	}


	//Return the index

	//Got this result from solving for a, b, k in ax + kba + bx = i
	//The 0 case does not work with the result as a = -1, k = -2, b = 1 since kba = 2 therefore the result is hardcoded.
	//The case for z is just the original values but incremented by four so it is easy

	if(!(xGreater || yGreater || zGreater)) return 0;

	// ax + kba + bx = i
	return -int(xGreater) + 2 + int(yGreater) + 4 * int(zGreater);
}


void addNode(uint treeIndex, uint positionIndex) {

	uint i = treeIndex;
	bool keepIterating = true;

	while(keepIterating) {

		keepIterating = false;
		int treeEntry = tree[i];
		
		if(treeEntry == 0) {
			//Node is empty, place here
			tree[i] = -(int(positionIndex) + 1);

			//Update center of mass
			center_masses[i] = positions[positionIndex];
			center_masses_n[i] = 1;
		}
		else if(treeEntry > 0) {
			//Node is internal, points to another node

			//Update center of mass
			uint n = center_masses_n[treeIndex];
			center_masses[i] = (center_masses[i] / n) * (n / (n + 1)) + positions[positionIndex] / (n + 1);
			center_masses_n[i]++;

			//Iterate again
			uint ind = quad(positions[positionIndex], true);
			i = treeEntry + ind;
			keepIterating = true;
		}
		else {
			//Node is external, is a leaf

			//Split node
			tree[i] = int(occupiedTreeSize);
			atomicAdd(occupiedTreeSize, 8); //Increment occupiedTreeSize as we are adding 8 more nodes

			//Move down the previous node
			uint convertedPositionInd = -treeEntry - 1;
			uint prevInd = quad(positions[convertedPositionInd], false); //Converting treeEntry to the index for positions[] (treeEntry is -(ind + 1)) ind = -(treeEntry + 1))
			uint movedTotalInd = occupiedTreeSize + prevInd;
			
			tree[movedTotalInd] = treeEntry; //Set the previous node to its new position
			
			//Initialize center of mass for new node
			center_masses[movedTotalInd] = positions[convertedPositionInd];
			center_masses_n[movedTotalInd] = 1;

			//Update center of mass
			uint n = center_masses_n[i];
			center_masses[i] = (center_masses[i] / n) * (n / (n + 1)) + positions[positionIndex] / (n + 1);
			center_masses_n[i]++;

			//Iterate again
			uint ind = quad(positions[positionIndex], true);
			i = occupiedTreeSize + ind;
			keepIterating = true;
		}
	}
	
}

void main() {

	int position_index = int(gl_LocalInvocationIndex);
	origin = -simulationBounds / 2.0;
	quadBounds = -origin; //(simulationBounds / 2)

	if(tree[0] == 0) {
		tree[0] = 1;
		occupiedTreeSize = 1;
	}

	addNode(0, position_index);
}



