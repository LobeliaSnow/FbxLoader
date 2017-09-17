#include "../Common.h"
#include <fbxsdk.h>

#include "../AnimationTake/AnimationTake.h"
#include "../Cluster/Cluster.h"

namespace FL {
	Cluster::Cluster(fbxsdk::FbxCluster* cluster) :cluster(cluster) {
		TakeIndicesAndWeights();
		TakeInitMatrix();
		TakeAnimationTakes();
	}

	Cluster::~Cluster() {
		for (int i = 0; i < animationCount; i++) {
			SafeDelete(animationTakes[i]);
		}
	}

	void Cluster::TakeIndicesAndWeights() {
		indexCount = cluster->GetControlPointIndicesCount();
		indices.resize(indexCount); weights.resize(indexCount);
		int* indexArray = cluster->GetControlPointIndices();
		double* weightArray = cluster->GetControlPointWeights();
		for (int i = 0; i < indexCount; i++) {
			indices[i] = indexArray[i];
			weights[i] = static_cast<float>(weightArray[i]);
		}
	}

	void Cluster::TakeInitMatrix() {
		FbxAMatrix temp;
		cluster->GetTransformLinkMatrix(temp);
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				initMatrix.mat[row][column] = static_cast<float>(temp.GetRow(row)[column]);
			}
		}
	}

	void Cluster::TakeAnimationTakes() {
		FbxArray<FbxString*> animationNames;
		fbxsdk::FbxScene* scene = cluster->GetScene();
		scene->FillAnimStackNameArray(animationNames);
		animationCount = animationNames.Size();
		animationTakes.resize(animationCount);
		for (int i = 0; i < animationCount; i++) {
			fbxsdk::FbxTakeInfo* takeInfo = scene->GetTakeInfo(animationNames[i]->Buffer());
			animationTakes[i] = new AnimationTake(this, i, takeInfo);
		}
	}

	int Cluster::GetIndexCount() { return indexCount; }

	int Cluster::GetImpactIndex(int index) { return indices[index]; }

	float Cluster::GetWeight(int index) { return weights[index]; }

	Matrix Cluster::GetInitPoseMatrix() { return initMatrix; }

	int Cluster::GetAnimationCount() { return animationCount; }

	AnimationTake* Cluster::GetAnimationTake(int index) { return animationTakes[index]; }

}