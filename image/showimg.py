#!/usr/bin/python

import cv2, sys
img = cv2.imread(sys.argv[1])
cv2.imshow("show",img)
cv2.waitKey(3000)
