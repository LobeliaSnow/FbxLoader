#pragma once
namespace FL {
	class Cluster {
	private:
		fbxsdk::FbxCluster* cluster;
		int indexCount;
		std::vector<int> indices;
		std::vector<float> weights;
		Matrix initMatrix;
		std::vector<Matrix> matrices;
	private:
		void TakeIndicesAndWeights();
		void TakeInitMatrix();
		void TakeAnimationMatrices();
	public:
		Cluster(fbxsdk::FbxCluster* cluster);
		~Cluster();
	};
}

#include "Cluster.inl"