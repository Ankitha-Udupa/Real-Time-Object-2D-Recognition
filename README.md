## Real-Time-Object-2D-Recognition

**About/Overview:**
The goal is to have the computer identify a specified set of objects placed on a white surface in a translation, scale, and rotation invariant manner.The computer recognizes single objects placed in the image and identify the object an output image. Provided a video sequence, it does the same in real time.

# Threshold the input video

We used a gaussian blur filter to preprocess the input frame. We then used RG-Chromaticity for every pixel to threshold it into foreground and background. We used two-sided thresholding. Every pixel having a value close to white, was given a value of 0 (background) and the rest was assigned 255 (foreground). This resulted in a binary image.

![IMG_1046](https://user-images.githubusercontent.com/44782426/158461881-a44244b8-9b44-45fa-b60d-310990e0cc4d.JPG)
![phone1_thresh](https://user-images.githubusercontent.com/44782426/158461897-aef0094a-1cc0-4c76-b969-09978f3e0bd3.jpg)

# Clean up the binary image

The image from the previous thresholding process had a lot of salt-and-pepper noise. Hence, we used morphological operators - growing and shrinking. After growing the frame 5 times and the subsequently shrinking it 2 times yielded the best results. Growing and shrinking both use 4-connected approach to increase and decrease the foreground respectively.

![phone1_thresh](https://user-images.githubusercontent.com/44782426/158462323-6fd5da17-a84e-4552-9cdb-bf6e6de5104c.jpg)
![phone1_gs](https://user-images.githubusercontent.com/44782426/158462330-b9b9c492-2020-47fe-903d-5392d26522a9.jpg)

# Segment the image into regions

On the cleaned up binary frame received from the previous process, we iterated through the frame to find the seed pixel. We used depth-first search to traverse neighboring pixels to find foreground pixels that haven't been assigned a region id and them recursively mapped all the foreground pixels to their respective regions. We found the largest region from a window that captures the central 80% of the frame that gave us the object along with its region id.
![img1](https://user-images.githubusercontent.com/44782426/158462568-c952f364-ba3a-4bc2-![growing](https://user-images.githubusercontent.com/44782426/158462579-c1e3c677-cf4c-4eba-a755-603f70f417b9.png)
9781-e90b90f42261.png)

# Compute features for each major region

The first step was to calculate the centroid of the given region. This was achieved by calculating moments M_PQ that helped us calculating the centroids. We used the centroids to calculate central moments, central axis angle and used them to get the oriented bounding box. These values were used to extract translation, rotation and scale invariant features like aspect ratio, percent filled and second order central moments.

![phone1_bounding](https://user-images.githubusercontent.com/44782426/158462818-fb7384ce-28b2-4abe-a7ed-5438fb53175e.jpg)

# Classify new images

We extracted the features from the target frame. We read the CSV file that contained labels and features and then calculated the scaled Euclidean distance between the target image and every single image from the database. Scaled Euclidean distance was used since the scales of features were not in a similar range. The label from the image from database that gave the least distance was chosen.We have also implemented a kNN classifier was used that calculated the Euclidean distance to between the target image and all other images in the database. The k-nearest datapoints were filtered. In this filtered list, the label with highest frequency was chosen to determine the object.

<img width="637" alt="Screen Shot 2022-03-11 at 11 53 12 PM" src="https://user-images.githubusercontent.com/44782426/158463044-ba0cc795-d986-4fa9-9ad1-8270d04ca58f.png">


# DEMO

https://user-images.githubusercontent.com/44782426/158465778-53a1ac46-d871-4872-ad8c-ebdc09d6bf3e.mov

# Operating system and IDE

MacOS and Xcode.

# Instructions for running your executables

Compile the code, create img and data folders. Run the binary. 
