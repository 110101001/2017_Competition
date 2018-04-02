import cv2
import numpy as np
import math as ms
import serial as usart
import time

com=usart.Serial("/dev/ttyUSB0",38400)

lower_back=np.array([0,0,0])
uper_back=np.array([90,90,90])

lower_area=np.array([70,110,70])
uper_area=np.array([140,240,140])


#lower_flat=np.array([140,140,140])
#uper_flat=np.array([255,255,255])

AREA=np.zeros((9,3),np.uint16)

cap=cv2.VideoCapture(0)
while cap.read()==None:
	cap=cv2.VideoCapture(0)
cap.set(3,320)
cap.set(4,320)
temp=np.zeros((600,600,1),np.uint8)
cv2.circle(temp,(300,300),50,255,-1)
ret,temp=cv2.threshold(temp,240,255,cv2.THRESH_BINARY)
arr,hie=cv2.findContours(temp,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

temp=np.zeros((600,600,1),np.uint8)
cv2.rectangle(temp,(300,300),(350,350),255,-1)
ret,temp=cv2.threshold(temp,100,255,cv2.THRESH_BINARY)
boxline,hie=cv2.findContours(temp,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
cxlast,cylast=0,0
fail_count=0

#ret,img=cap.read()
#img=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
#img=cv2.GaussianBlur(img,(1,1),0)
#cv2.imshow('box0',img)
#contour,hie=cv2.findContours(img,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
#for i in range(0,len(contour)):
#    length=cv2.arcLength(contour[i],True)
#    if length<2800 and length>1900:
#    	M=cv2.moments(contour[i])
#        (ox,oy),(w,h),theta=cv2.minAreaRect(contour[i])
#        #ox=int(ox)-int(w * ms.cos(theta)-h * ms.sin(theta))
#        #oy=int(oy)-int(w * ms.sin(theta)-h * ms.cos(theta))
#        ox=int(ox)
#        oy=int(oy)
#        if w/h>0.8 and w/h<1.2:    
#            ret,img=cap.read()
#            cv2.line(img,(ox,oy),(ox+int(40 * ms.cos(theta)),oy+int(40 * ms.sin(theta))),(255,255,255),2)
#            cv2.circle(img,(ox,oy),5,(255,255,255),2)
#            cv2.drawContours(img,contour,i,(255,255,255),2)
#    cv2.imshow('box',img)
ox,oy=0,0
arc=0
flat_count=8
start_flag=0
while True:
	ret,img=cap.read()
	img=cv2.resize(img,(240,240),interpolation = cv2.INTER_CUBIC)
	#img=cv2.GaussianBlur(img,(3,3),0)
	if flat_count==8:
		#lower_back=np.array([0,0,0])
		#uper_back=np.array([90,90,90])
		#box=cv2.inRange(img,lower_flat,uper_flat)
		box=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
		#box=cv2.adaptiveThreshold(box,255,cv2.ADAPTIVE_THRESH_MEAN_C,cv2.THRESH_BINARY,9,2)
		if start_flag==0:
			ret,box=cv2.threshold(box,60,255,cv2.THRESH_BINARY)
			start_flag=1
		else:
			ret,box=cv2.threshold(box,130,255,cv2.THRESH_BINARY)
	        #cv2.imshow("box",box)
	        contour,hie=cv2.findContours(box,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
	        minindex=0
	        minlength=5000
       		for i in range(0,len(contour)):
	            length=cv2.arcLength(contour[i],True)
	            if length>450 and length<650:
			ret=cv2.matchShapes(boxline[0],contour[i],cv2.cv.CV_CONTOURS_MATCH_I1,0.0)
                	if ret<0.6:
                 	   #if minlength>length:
                 	   minlength=length
                 	   minindex=i
                #cv2.drawContours(img,contour,i,(255,255,255),2)
        #Rect=cv2.minAreaRect(contour[minindex])
        #(temp,temp),(temp,temp),arc=Rect
        #Rect_points=cv2.cv.BoxPoints(Rect)
        #ox,oy=Rect_points[0]
        #ox=int(ox)
        #oy=int(oy)
      		eps=0.1 * cv2.arcLength(contour[minindex],True)
        	approx=cv2.approxPolyDP(contour[minindex],eps,True)
       	 	#cv2.drawContours(img,contour,minindex,(255,255,255),2)
        	pts1=np.zeros((4,2),np.float32)
	
        	if len(approx)==4:
      	     		for i in range(0,4):
#              		cv2.circle(img,(approx[i][0][0],approx[i][0][1]),5,(0,255,0),2)
                		pts1[i][0]=approx[i][0][0]
                		pts1[i][1]=approx[i][0][1]
		#cv2.imshow("test",img)
        	pts2=np.float32([[0,0],[0,240],[240,240],[240,0]])
       		TM=cv2.getPerspectiveTransform(pts1,pts2)
		flat_count=0
	else:
		flat_count+=1
        img=cv2.warpPerspective(img,TM,(240,240))
#	if flat_count==1:
#		area=cv2.inRange(img,lower_area,uper_area)
#		#cv2.imshow("Area",area)
#		contour,hie=cv2.findContours(area,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
#		for i in range(0,len(contour)):
#			M=cv2.moments(contour[i])
#			if M['m00']!=0:
#				cx=int(M['m10']/M['m00'])
#				cy=int(M['m01']/M['m00'])
#				#if cx<300 and cy <300:
#				 
#				if AREA[int(cx/80)+int(cy/80-1)*3][2]==0:
#					AREA[int(cx/80)+int(cy/80-1)*3][0]=cx
#					AREA[int(cx/80)+int(cy/80-1)*3][1]=cy
#					AREA[int(cx/80)+int(cy/80-1)*3][2]=1
#				else:
#					err0,err1=0,0
#					if cy>80:
#						err0 += ms.fabs(AREA[int(cx/80)+int(cy/80-1)*3][0]-AREA[(int(cx/80)+int(cy/80-1)*3)%3][0])
#						err1 += ms.fabs(cx-AREA[(int(cx/80)+int(cy/80-1)*3)%3][0])
#					else:
#						err0 += ms.fabs(AREA[int(cx/80)+int(cy/80-1)*3][0]-AREA[(int(cx/80)+int(cy/80-1)*3)+6][0])
#						err1 += ms.fabs(cx-AREA[(int(cx/80)+int(cy/80-1)*3)+6][0])
#						
#					if cx>80:
#						err0 += ms.fabs(AREA[int(cx/80)+int(cy/80-1)*3][1]-AREA[3 * int(cy/80)][1])
#						err1 += ms.fabs(cy-AREA[int(cy/80) * 3][1])
#					else:
#						err0 += ms.fabs(AREA[int(cx/80)+int(cy/80-1)*3][1]-AREA[int(cy/80-1) * 3 + 2][1])
#						err1 += ms.fabs(cx-AREA[int(cy/80) * 3+2][1])
#					if err0>err1:
#						AREA[int(cx/80)+int(cy/80-1)*3][0]=cx
#						AREA[int(cx/80)+int(cy/80-1)*3][1]=cy
#		for i in range(0,9):
#              		#cv2.circle(img,(AREA[i][0],AREA[i][1]),5,(0,255,0),2)
#			AREA[i][2]=0
#			com.write("A%d %dB%d %d\r\n"%(AREA[i][0],AREA[i][1],(AREA[i][0]+AREA[i][1]) % 10,i))
        #cv2.circle(img,(int(Rect_points[0][0]),int(Rect_points[0][1])),5,(0,255,0),1)
        #cv2.circle(img,(int(Rect_points[1][0]),int(Rect_points[1][1])),5,(0,255,0),1)
        #cv2.circle(img,(int(Rect_points[2][0]),int(Rect_points[2][1])),5,(0,255,0),1)
        #cv2.circle(img,(int(Rect_points[3][0]),int(Rect_points[3][1])),5,(0,255,0),1)
	mask=cv2.inRange(img,lower_back,uper_back)
	#cv2.imshow("mask",mask)
	#img=cv2.cvtColor(img,cv2.COLOR_HSV2BGR)
	contour,hie=cv2.findContours(mask,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)
	state=0
	for i in range(0,len(contour)):
		length=cv2.arcLength(contour[i],True)
		if length>10 and length<80:
			ret=cv2.matchShapes(arr[0],contour[i],cv2.cv.CV_CONTOURS_MATCH_I1,0.0)
			if 0.3>ret:
				M=cv2.moments(contour[i])
				if M['m00']!=0:
					cx=int(M['m10']/M['m00'])
					cy=int(M['m01']/M['m00'])
					if (cxlast==0 and cylast==0) or (cx-cxlast)*(cx-cxlast)+(cy-cylast)*(cy-cylast)<8000:
						#cv2.circle(img,(cx,cy),5,(255,255,255),2)
						#cv2.line(img,(cx+15,cy+15),(cx+8,cy+15),(0,0,255),1)
						#cv2.line(img,(cx+15,cy+15),(cx+15,cy+8),(0,0,255),1)
						#cv2.line(img,(cx-15,cy+15),(cx-8,cy+15),(0,0,255),1)
						#cv2.line(img,(cx-15,cy+15),(cx-15,cy+8),(0,0,255),1)
						#cv2.line(img,(cx+15,cy-15),(cx+8,cy-15),(0,0,255),1)
						#cv2.line(img,(cx+15,cy-15),(cx+15,cy-8),(0,0,255),1)
						#cv2.line(img,(cx-15,cy-15),(cx-8,cy-15),(0,0,255),1)
						#cv2.line(img,(cx-15,cy-15),(cx-15,cy-8),(0,0,255),1)
						#cv2.putText(img,"%d,%d"%(cx,cy),(cx+15,cy-15),cv2.FONT_HERSHEY_SIMPLEX,1,(255,0,0),1)
						#cv2.putText(img,"%d,%d"%(int(length),int(1000*ret)),(cx+15,cy+15),cv2.FONT_HERSHEY_SIMPLEX,1,(255,0,0),1)
						com.write("B%d %dA%d\r\n"%(cx,cy,(cx+cy) % 10))
                                                cv2.circle(img,(cxlast,cylast),10,(0,255,0),2)
                                                cv2.line(img,(cx,cy),(2 * cx-cxlast,2 * cy-cylast),(255,255,255),2)
						cxlast=cx
						cylast=cy
						#print cx,cy
						state=1
						fail_count=0
	if state==0:
		fail_count+=1
	if fail_count==3:
		cxlast=0
		cylast=0
		fail_count=0
	#print com.read(2)
	#cv2.imshow("image",img)
	if cv2.waitKey(1)&0xFF == ord('q'):
		break
cv2.destroyAllWindows()
cap.release()
