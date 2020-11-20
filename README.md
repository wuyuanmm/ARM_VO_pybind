# ARM_VO_pybind
this project is the python version of the ARM_VO. the url is https://github.com/zanazakaryaie/ARM-VO. thanks zanazakaryaie to publish this project.

#Requirments

Opencv(3 or newer)

# How to Use
 ## 1  modify CMakeLists.txt 

include_directories("/home/YOURNAME/anaconda3/include/python3.7m")
include_directories("/home/YOURNAME/anaconda3/lib/python3.7/site-packages/numpy/core/include")

replace YOURNAME to your path.

## 2 make project 

```
cd build 
make all -j4
```

Generate .so file ,you will success.


## 3 using

```
import ARMVO

config_name="./Seq00-02_demo_2_3and4.yaml"
VO_obj=ARMVO.ARM_VO(config_name)
show_obj=ARMVO.Viewer();

'''process images'''

while i<24:

      img=cv2.imread(full_pic_name,0)
      
      if(VO_obj.initialized is False):
        VO_obj.init(roi_img)
      else:
        time_start=time.time()
        VO_obj.update(roi_img)
        time_end=time.time()
        
        prev_inliers=VO_obj.get_prev_inliers()
        curr_inliers=VO_obj.get_curr_inliers()
        
        #print(curr_inliers.shape, prev_inliers.shape)
        
        t_f=VO_obj.t_f
        R_f=VO_obj.R_f
        #show_img=np.zeros([sz[0],sz[1] ,3],dtype=np.uint8);
        
        res_img=show_obj.show(roi_img,prev_inliers,curr_inliers,10,t_f) # VO image
        show_img=show_obj.frameWithFeatures # features image 
 ```
