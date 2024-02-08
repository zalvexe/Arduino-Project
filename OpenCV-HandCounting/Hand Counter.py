import cv2
import mediapipe as mp
import time
import HandTrackingModule as htm 

pTime = 0
cap = cv2.VideoCapture(0)
detector = htm.HandDetector()
tipID = [4, 8, 12, 16, 20]

while True:
    success, image = cap.read()
    img = cv2.flip(image,1)
    img = detector.FindHands(img)
    posList = detector.HandIDPosition(img)
     #---- checking the open finger ---#
    if len(posList) != 0:
        finger = []
        if(posList[tipID[0]][1] > posList[tipID[0]-1][1]):
                finger.append(0)
        else:
             finger.append(1)

        for id in range(1,5):
            if posList[tipID[id]][2] < posList[tipID[id]-1][2]: #taking the y position ( 2 = y position), the id = 8 is in higher position than 6, so index is open                    
                finger.append(1)
            else:
                finger.append(0)
    
    # ---- end ---#
        
    # ----- count how many finger ---- #
        cntFinger = finger.count(1)
        cv2.putText(img, str(cntFinger), (45,375), cv2.FONT_HERSHEY_PLAIN, 3, (255,0,255),3)
    # ---end --- #

    #fps
    cTime = time.time()
    fps = 1/(cTime-pTime)
    pTime = cTime
    cv2.putText(img,f'FPS : {int(fps)}', (400,70), cv2.FONT_HERSHEY_PLAIN,3, (0,255,0),3)
    #---- end fps ----#


    cv2.imshow("Camera", img)
    cv2.waitKey(1)


