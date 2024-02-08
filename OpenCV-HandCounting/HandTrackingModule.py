import cv2
import mediapipe as mp 
import time

class HandDetector():
    def __init__(self, mode=False, maxHand=3, complexity = 1, detectionCon=0.5, trackCon=0.5):
        self.mode = mode
        self.maxHand = maxHand
        self.complexity = complexity
        self.detectionCon = detectionCon
        self.trackCon = trackCon

        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(self.mode,self.maxHand,self.complexity, self.detectionCon,self.trackCon)
        self.mpDraw = mp.solutions.drawing_utils 

    def FindHands(self,img,draw = True):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB) 
        self.res = self.hands.process(imgRGB)

        if self.res.multi_hand_landmarks: 
            for EachHand in self.res.multi_hand_landmarks:
                if draw: 
                    self.mpDraw.draw_landmarks(img, EachHand, self.mpHands.HAND_CONNECTIONS) 

        return img
                
    def HandIDPosition(self, img, handNo = 0):
        lmList = []
        if self.res.multi_hand_landmarks:
            for myHand in self.res.multi_hand_landmarks:
                for id,lm in enumerate(myHand.landmark):                   
                    h,w,c = img.shape
                    cx,cy = int(lm.x*w), int(lm.y*h) 
                    lmList.append([id,cx,cy])
        
        return lmList
    
    def DrawCircle(self,img,handID=0, draw = True):
        if self.res.multi_hand_landmarks:
            for EachHand in self.res.multi_hand_landmarks:
                for id,lm in enumerate(EachHand.landmark):
                    h,w,c = img.shape
                    cx,cy = int(lm.x*w), int(lm.y*h)

                    if draw and id == handID:
                        cv2.circle(img, (cx,cy), 25, (255, 0,0))
        return img
