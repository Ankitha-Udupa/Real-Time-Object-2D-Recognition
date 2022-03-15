## Real-Time-Object-2D-Recognition

#About/Overview:
The goal is to have the computer identify a specified set of objects placed on a white surface in a translation, scale, and rotation invariant manner.The computer recognizes single objects placed in the image and identify the object an output image. Provided a video sequence, it does the same in real time.

#Threshold the input video
We used a gaussian blur filter to preprocess the input frame. We then used RG-Chromaticity for every pixel to threshold it into foreground and background. We used two-sided thresholding. Every pixel having a value close to white, was given a value of 0 (background) and the rest was assigned 255 (foreground). This resulted in a binary image.

![IMG_1046](https://user-images.githubusercontent.com/44782426/158461881-a44244b8-9b44-45fa-b60d-310990e0cc4d.JPG)
![phone1_thresh](https://user-images.githubusercontent.com/44782426/158461897-aef0094a-1cc0-4c76-b969-09978f3e0bd3.jpg)

![I![mask1_thresh](https://user-images.githubusercontent.com/44782426/158461921-ac77178d-61cc-4b40-a379-ff7fdcdc3201.jpg)
MG_1061](https://user-images.githubusercontent.com/44782426/158461910-83b83c6c-97f8-4c3f-ba6f-01a90a8ca3b1.JPG)

**Operating system and IDE**

**Project Report**
