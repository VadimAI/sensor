import numpy as np
import cv2

# copy parameters to arrays
K = np.array([[506.38298499, 0,333.18830493], [0, 510.91932237, 243.55003546], [0, 0, 1]])
d = np.array([-0.40012214, 0.01093, -0.01002547, 0.00267182, 0.57013485]) # just use first two terms (no translation)

# read one of your images
img = cv2.imread("./calibration_samples/camera005.png")
h, w = img.shape[:2]

# undistort
newcamera, roi = cv2.getOptimalNewCameraMatrix(K, d, (w,h), 1, (w,h)) 
newimg = cv2.undistort(img, K, d, None, newcamera)

cv2.imwrite("original.jpg", img)
cv2.imwrite("undistorted.jpg", newimg)