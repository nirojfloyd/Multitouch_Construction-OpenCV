# Multitouch-Construction-OpenCV
• Utilized C++ within the OPENCV framework to detect and track video stream blobs.

• Successfully integrated the TUIO Framework into the project.

## Results

![image](https://github.com/nirojfloyd/Multitouch_Construction-OpenCV/assets/65107710/42d5f98a-c145-42e2-be82-0d1a634713a9)

![image](https://github.com/nirojfloyd/Multitouch_Construction-OpenCV/assets/65107710/e640cf2a-f96c-4632-b69d-3ca9c6397871)

![image](https://github.com/nirojfloyd/Multitouch_Construction-OpenCV/assets/65107710/8de4db2f-20f5-4203-8af5-c291740b9342)



## Table of Contents
- [Project Overview](#overview)
- [Technology Stack](#technology-stack)

## Overview

- Blob Detection utilizing GIMP and two screenshots from the multi-touch system.
- Blob Detection using OpenCV and a video stream from the multi-touch camera

GNU Image Manipulation Program (GIMP) software for image processing was used.Background Subtraction, High-pass filter (blur + difference image), Segmentation and Edge detection were carried out utilizingthe tools facilitated by GIMP.

In different circumstances, OpenCV was used for video processing. Respective
functions for background subtraction, high-pass filter, segmentation (binarization),
threshold,and contours detection were implemented sequentially to achieve the desi-
red results.i.e Only finger blobs were seen on the video stream

- To track the blobs detected.
This goal was achieved by implementing nearest neighbour algorithm. Foremost, geometric centres of blobs/contours were calculated, followed by the distance calculation between blobs in two consecutive frames. Unique IDs were assigned to each blobs in such a way that if a new finger has been detected, a new ID is assigned and its position is saved. Meanwhile, if a previously detected finger changed its position is updated. Inactive touches were removed from the data structure.

- To integrate the TUIO framework into our own project.

Source files comprising of OSC pack and TUIO were imported into the project.
At first, blob coordinates were normalized using TUIO Server. The tracked blobs were transmitted via network protocol utilizing TUIO events. TUIO data were fetched everytime the TouchEvents were detected. The validity of sent events using TUIO testclients. i.e. TuioDump and TuioDemo.

## Technology Stack

- C++
- OpenCV
- Microsoft Visual Studio as IDE
- GIMP
    
- Version Control
  - Git



## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute it as needed.

---
