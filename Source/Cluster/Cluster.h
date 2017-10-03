#pragma once
namespace FL {
	class Cluster {
		friend class AnimationTake;
	private:
		fbxsdk::FbxMesh* mesh;
		fbxsdk::FbxCluster* cluster;
		int indexCount;
		int animationCount;
		std::vector<int> indices;
		std::vector<float> weights;
		Matrix initMatrix;
		Matrix referenceTransformMatrix;
		std::vector<AnimationTake*> animationTakes;
	private:
		void TakeIndicesAndWeights();
		void TakeInitMatrix();
		void TakeAnimationTakes();
	public:
		Cluster(fbxsdk::FbxCluster* cluster, fbxsdk::FbxMesh* mesh);
		~Cluster();
		int GetIndexCount();
		int GetImpactIndex(int index);
		std::vector<int>& GetImpactIndices();
		float GetWeight(int index);
		std::vector<float> GetWeights();
		Matrix GetInitPoseMatrix();
		//óùò_ÇÕÇ‹ÇæâˆÇµÇ¢ÇØÇ«ïKóv
		Matrix GetReferenceTransformMatrix();
		int GetAnimationCount();
		AnimationTake* GetAnimationTake(int index);
		std::vector<AnimationTake*>& GetAnimationTakes();
	};
}

