#pragma once
namespace FL {
	__forceinline Skin::Skin(fbxsdk::FbxSkin* skin) :skin(skin) {
		clusterCount = skin->GetClusterCount();
		for (int i = 0; i < clusterCount; i++) {
			clusters.push_back(new Cluster(skin->GetCluster(i)));
		}
	}
	__forceinline Skin::~Skin() {
		for (int i = 0; i < clusterCount; i++) {
			delete clusters[i];
		}
	}
	__forceinline int Skin::GetClusterCount() { return clusterCount; }
	__forceinline Cluster* Skin::GetCluster(int index) { return clusters[index]; }


}