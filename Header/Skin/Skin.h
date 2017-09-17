#pragma once
namespace FL {
	class Skin {
	private:
		fbxsdk::FbxSkin* skin;
		int clusterCount;
		std::vector<Cluster*> clusters;
	public:
		Skin(fbxsdk::FbxSkin* skin);
		~Skin();
		int GetClusterCount();
		Cluster* GetCluster(int index);
	};
}

#include "Skin.inl"