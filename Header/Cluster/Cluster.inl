#pragma once
namespace FL {
	__forceinline Cluster::Cluster(fbxsdk::FbxCluster* cluster) :cluster(cluster) {
		TakeIndicesAndWeights();
		TakeInitMatrix();
		TakeAnimationMatrices();
	}
	__forceinline Cluster::~Cluster() {

	}
	__forceinline void Cluster::TakeIndicesAndWeights() {
		indexCount = cluster->GetControlPointIndicesCount();
		indices.resize(indexCount); weights.resize(indexCount);
		int* indexArray = cluster->GetControlPointIndices();
		double* weightArray = cluster->GetControlPointWeights();
		for (int i = 0; i < indexCount; i++) {
			indices[i] = indexArray[i];
			weights[i] = static_cast<float>(weightArray[i]);
		}
	}
	__forceinline void Cluster::TakeInitMatrix() {
		FbxAMatrix temp;
		cluster->GetTransformLinkMatrix(temp);
		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				initMatrix.mat[row][column] = temp.GetRow(row)[column];
			}
		}
	}
	__forceinline void Cluster::TakeAnimationMatrices() {
		FbxArray<FbxString*> animationNames;
		fbxsdk::FbxScene* scene = cluster->GetScene();
		scene->FillAnimStackNameArray(animationNames);
		for (int i = 0; i < animationNames.Size(); i++) {
			fbxsdk::FbxTakeInfo* takeInfo = scene->GetTakeInfo(animationNames[i]->Buffer());
			
		}
	}

}