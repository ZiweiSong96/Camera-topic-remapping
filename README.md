# Camera Topic Remapping

This repo is designed for remapping the camera topic provided by AWSIM simulator to /sensing/camera related topics (image raw and camera info), so that camera_lidar_fusion perception mode can be launched in Autoware.Universe.


## Idea & Noticing

Note that this node only change the name of topics to meet the Autoware launch files requirements.

In Autoware.Universe, the yolo node which designed for providing 2D rois outputs cannot be launched correctly, so that the yolo node should be launched individually.


## 20240206

In classical ADS model, the frequency of cameras for 2D perception is 30 hz, so a ros2 node that cache and publish the old camera data from AWSIM to increase the pubilsh requency is developed.